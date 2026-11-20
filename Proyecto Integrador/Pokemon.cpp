#include "Pokemon.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string upConvert(const string& text) {
    string copia = text;
    for (int i = 0; i < copia.length(); i++) {
        copia[i] = toupper(copia[i]);
    }
    return copia;
}

Pokemon::Pokemon(int id, const string nomb, const string& t1, const string& t2,
int health) : id_dex(id), nom_especie(nomb), tipo1(t1), tipo2(t2), hp(health) {}

int Pokemon::getHP() {return hp;}
int Pokemon::getID() {return id_dex;}
string Pokemon::getNombre() const {return nom_especie;}
string Pokemon::getTipo() {
    if (tipo2.empty()) {
        return tipo1;
    }
    return tipo1 + " / " + tipo2;
}
string Pokemon::getTipo(int puesto) {
    if (puesto == 1) {return tipo1;}
    if (puesto == 2) {return tipo2;}
    return "";
}

void Pokemon::mostrar_info() {
    cout << "\n==================================\n";
    cout << "Info del Pokemón: " << nom_especie << endl;
    cout << "\n==================================\n";
    cout << "No. Pokedex:  #" << id_dex << endl;
    cout << "Tipos:        " << tipo1;
    if (!tipo2.empty()) {cout << " / " << tipo2;} cout << endl;
    cout << "\n==================================\n";
}

PokeCapturado::PokeCapturado(int id, const string nomb, const string& t1, 
    const string& t2, int hp, const string& nick, int lv, const string& box) 
    : Pokemon(id, nomb, t1, t2, hp), mote(nick.empty() ? nomb : nick), 
        nivel(lv), caja(box) {}

string PokeCapturado::getMote() {return mote;}
int PokeCapturado::getLv() {return nivel;}
string PokeCapturado::getCaja() {return caja;}

void PokeCapturado::setNivel(int nuevo_nivel) {nivel = nuevo_nivel;}
void PokeCapturado::setCaja(string nueva_caja) {caja = nueva_caja;}

void PokeCapturado::mostrar_info() {
    cout << "\n==================================\n";
    cout << "Info del Pokemón: " << mote << endl;
    cout << "\n==================================\n";
    cout << "Nombre:      " << nom_especie << endl;
    cout << "No. Pokedex:  #" << id_dex << endl;
    cout << "Tipos:        " << tipo1;
    if (!tipo2.empty()) {cout << " / " << tipo2;} cout << endl;
    cout << "HP:           " << hp << endl;
    cout << "Nivel:        " << nivel << endl;
    cout << "Caja actual:  " << caja << endl;
    cout << "\n==================================\n";
}

NodoPokedex::NodoPokedex(const Pokemon& pokemon) : dato(pokemon), left(0), 
    right(0) {}

void Pokedex::cargar_pokedex(const string& file) {
    ifstream archivo(file);
    string linea;

    if (!archivo.is_open()) {
        cout << "Nose pudo cargar la pokedex" << endl;
        return;
    }

    if (!getline(archivo,linea)) {
        archivo.close();
        return;
    }

    if (!linea.empty() && linea.back() == '\r') {linea.pop_back();}

    if (linea != "ID,NOMBRE,TIPO1,TIPO2") {
        archivo.close();
        return;
    }

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }
        
        stringstream datos(linea);

        string id;
        string nombre;
        string tipo1;
        string tipo2;

        getline(datos, id, ','); getline(datos, nombre, ',');
        getline(datos, tipo1, ','); getline(datos, tipo2, ',');

        if(id.empty() || nombre.empty() || tipo1.empty() || tipo2.empty()) {
            continue;
        }

        if (upConvert(tipo2) == "NINGUNO") {tipo2 = "";}

        Pokemon pokemon(stoi(id), nombre, tipo1, tipo2);

        raiz = insertar(raiz, pokemon);
    }
    archivo.close();
}

NodoPokedex* Pokedex::insertar(NodoPokedex* nodo, const Pokemon& pokemon) {
    if (nodo == 0) {return new NodoPokedex(pokemon);}

    string nuevo = upConvert(pokemon.getNombre());
    string actual = upConvert(nodo->dato.getNombre());

    if (nuevo < actual) {
        nodo->left = insertar(nodo->left, pokemon);
    }
    else if (nuevo > actual) {
        nodo->right = insertar(nodo->right, pokemon);
    }
    return nodo;
}

NodoPokedex* Pokedex::buscar_nodo(NodoPokedex* nodo, const string& nombre) {
    if (nodo == 0) return 0;

    string buscado = upConvert(nombre);
    string actual = upConvert(nodo->dato.getNombre());

    if (buscado == actual) return nodo;
    if (buscado < actual) {
        return buscar_nodo(nodo->left, nombre);
    } else if (buscado > actual) {
        return buscar_nodo(nodo->right, nombre);
    }
    return buscar_nodo(nodo->right, nombre);
}

void Pokedex::inorder(NodoPokedex* nodo, vector<Pokemon>& lista) {
    if (nodo == 0) return;

    inorder(nodo->left, lista);
    lista.push_back(nodo->dato);
    inorder(nodo->right, lista);
}

void Pokedex::limpiar(NodoPokedex* nodo) {
    if (nodo == 0) return;

    limpiar(nodo->left);
    limpiar(nodo->right);

    delete nodo;
}

Pokedex::Pokedex(const string& file) : raiz(0) {cargar_pokedex(file);}

bool Pokedex::loaded() const {
    return raiz != 0;
}

Pokedex::~Pokedex() {
    limpiar(raiz);
    raiz = 0;
}

Pokemon* Pokedex::buscar_pokemon(const string& nombre) {
    if (nombre.empty()) return 0;

    NodoPokedex* encontrado = buscar_nodo(raiz, nombre);
    if (encontrado == 0) return 0;
    return &(encontrado->dato);
}

vector<Pokemon> Pokedex::visit() {
    vector<Pokemon> lista;
    inorder(raiz, lista);
    return lista;
}

NodoPagina::NodoPagina(int num, const vector<Pokemon>& lista) : n_pagina(num),
    pokemons(lista), siguiente(0), anterior(0) {}

PaginaPokedex::PaginaPokedex(const vector<Pokemon>& lista, int por_pagina = 100) :
    primera_p(0), p_actual(0), total_pag(0) {
        if (lista.empty() || por_pagina <= 0) return;

        int n = 1;
        for (int i = 0; i < lista.size(); i += por_pagina) {
            vector<Pokemon> bloque;
            for (int j = i; j < i + por_pagina && j < lista.size(); j++) {
                bloque.push_back(lista[j]);
            }

            NodoPagina* nueva = new NodoPagina(n, bloque);
            if (primera_p == 0) {
                primera_p = nueva;
                p_actual = nueva;
                nueva->siguiente = nueva;
                nueva->anterior = nueva;
            } else {
                NodoPagina* ultima = primera_p->anterior;
                ultima->siguiente = nueva;
                nueva->anterior = ultima;
                nueva->siguiente = primera_p;
                primera_p->anterior = nueva;
            }
            n++;
            total_pag++;
        }
    }

PaginaPokedex::~PaginaPokedex() {limpiar_paginas();}

void PaginaPokedex::limpiar_paginas() {
    if (primera_p == 0) {
        p_actual = 0;
        total_pag = 0;
        return;
    }

    NodoPagina* p = primera_p->siguiente;
    while (p != primera_p) {
        NodoPagina* siguiente = p->siguiente;
        delete p;
        p = siguiente;
    }

    delete primera_p;
    primera_p = 0;
    p_actual = 0;
    total_pag = 0;
}

void PaginaPokedex::next_p() {
    if (p_actual == 0) return;
    p_actual = p_actual->siguiente;
}

void PaginaPokedex::previous_p() {
    if (p_actual == 0) return;
    p_actual = p_actual->anterior;
}

void PaginaPokedex::mostrar_pagina() const {
    if (p_actual == 0) return;

    cout << "\n==============================================" << endl;
    cout << "              POKEDEX " << p_actual->n_pagina
        << " DE " << total_pag << endl;
    cout << "==============================================" << endl;

    for (int i = 0; i < p_actual->pokemons.size(); i++) {
        cout << "#" << p_actual->pokemons[i].getID() << " "
            << p_actual->pokemons[i].getNombre()
            << " | Tipo: " << p_actual->pokemons[i].getTipo() << endl;
    }
}

bool PaginaPokedex::empty() const {return primera_p == 0;}