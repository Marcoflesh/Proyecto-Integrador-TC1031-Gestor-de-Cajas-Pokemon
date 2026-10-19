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

NodoPokedex::NodoPokedex(const Pokemon& pokemon) : dato(pokemon), left(nullptr), 
    right(nullptr) {}

void Pokedex::cargar_pokedex(const string& file) {
    ifstream archivo(file);
    string linea;

    if (!archivo.is_open()) {return;}

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
    if (nodo == nullptr) {return new NodoPokedex(pokemon);}

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
    if (nodo == nullptr) return nullptr;

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
    if (nodo == nullptr) return;

    limpiar(nodo->left);
    limpiar(nodo->right);

    delete nodo;
}

Pokedex::Pokedex(const string& file) : raiz(0) {cargar_pokedex(file);}

Pokedex::~Pokedex() {
    limpiar(raiz);
    raiz = nullptr;
}

Pokemon* Pokedex::buscar_pokemon(const string& nombre) {
    if (nombre.empty()) return 0;

    NodoPokedex* encontrado = buscar_nodo(raiz, nombre);
    if (encontrado == nullptr) return nullptr;
    return &(encontrado->dato);
}

vector<Pokemon> Pokedex::visit() {
    vector<Pokemon> lista;
    inorder(raiz, lista);
    return lista;
}
