#include "PC.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

string upConvert(const string& text) {
    string copia = text;
    for (int i = 0; i < copia.length(); i++) {
        copia[i] = toupper(copia[i]);
    }
    return copia;
}

Nodo::Nodo(const string& nombre) : nombre(nombre), siguiente(nullptr) {}

PC::PC(const string& nombre) : nombre(nombre), 
    primera_caja(nullptr), caja_actual(nullptr) {}

PC::~PC() {limpiar_cajas();}

string PC::getCajaActual() {
    if (caja_actual == nullptr) {return "";}
    return caja_actual->nombre;
}
string PC::getNombre() {return nombre;}
vector<string> PC::getCajas() {
    vector<string> nombres;

    if (primera_caja == nullptr) {
        return nombres;
    }
    Nodo* p = primera_caja;
    do {
        nombres.push_back(p->nombre);
        p = p->siguiente;
    } while (p != primera_caja);

    return nombres;
}

void PC::setCajaActual(string& nueva_caja) {cambiar_caja(nueva_caja);}
void PC::setNombre(string& nuevo_nombre) {nombre = nuevo_nombre;}

Nodo* PC::buscar_caja(const string& nombre) {
    if (primera_caja == nullptr) {return nullptr;}

    string busqueda = upConvert(nombre);
    Nodo* p = primera_caja;
    
    do {
        if (upConvert(p->nombre) == busqueda) {
            return p;
        }
        p = p->siguiente;
    } while(p != primera_caja);

    return nullptr;
}

void PC::limpiar_cajas() {
    if (primera_caja == nullptr) {
        caja_actual = nullptr;
        return;
    }
    Nodo* p = primera_caja->siguiente;

    while (p != primera_caja) {
        Nodo* siguiente = p->siguiente;
        delete p;
        p = siguiente;
    }
    delete primera_caja;
    primera_caja = nullptr;
    caja_actual = nullptr;
}

void PC::agregar_cajas(const string& nueva){
    if(nueva.empty() || buscar_caja(nueva) != nullptr) {return;}

    Nodo* nuevo = new Nodo(nueva);

    if (primera_caja == nullptr) {
        primera_caja = nuevo;
        caja_actual  = nuevo;
        nuevo->siguiente = nuevo;
        return;
    }
    Nodo* p = primera_caja;

    while (p->siguiente != primera_caja){p = p->siguiente;}
    p->siguiente = nuevo;
    nuevo->siguiente = primera_caja;
}

void PC::crear_caja(const string& nueva) {
    if(nueva.empty()) {return;}

    for (unsigned int i = 0; i < nueva.length(); i++) {
        if (nueva[i] == ',') {return;}
    }

    if(buscar_caja(nueva) != nullptr) {return;}
    
    agregar_cajas(nueva);
    caja_actual = buscar_caja(nueva);
}

void PC::renombrar_caja(const string& nombre) {
    if(caja_actual == nullptr) {return;}

    if (nombre.empty()) {return;}

    for (unsigned int i = 0; i < nombre.length(); i++) {
        if (nombre[i] == ',') {return;}
    }

    if (nombre == caja_actual->nombre) {return;}

    Nodo* caja_existente = buscar_caja(nombre);

    if (caja_existente != nullptr && caja_existente != caja_actual) {return;}

    string previo = caja_actual->nombre;
    int poke_actualizado = 0;

    for (unsigned int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == previo) {
            inventario[i].setCaja(nombre);
            poke_actualizado++;
        }
    }
    caja_actual->nombre = nombre;
}

int PC::poke_por_caja(const string& nombre) {
    int cantidad = 0;
    int i = 0;

    while (i < inventario.size()) {
        if (inventario[i].getCaja() == nombre) {
            cantidad++;
        }
        i++;
    }
    return cantidad;
}

void PC::cargar_csv(const string& file){
    inventario.clear();
    limpiar_cajas();

    ifstream archivo(file);
    string linea;

    if (!archivo.is_open()) return;

    bool primer_linea = true;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        if (primer_linea) {
            primer_linea = false;
            if (linea.rfind("CAJAS:", 0) == 0) {
                stringstream cs(linea.substr(6));
                string nombre_caja;
                while (getline(cs, nombre_caja, ',')) {
                    agregar_cajas(nombre_caja);
                }
                continue;
            }
        }

        stringstream ss(linea);
        string id, nomb, t1, t2, hp, mote, lv, caja;

        getline(ss, id, ','); getline(ss, nomb, ','); getline(ss, t1, ',');
        getline(ss, t2, ','); getline(ss, hp, ','); getline(ss, mote, ',');
        getline(ss, lv, ','); getline(ss, caja, ',');

        PokeCapturado nuevo(stoi(id), nomb, t1, t2, stoi(hp), mote, stoi(lv), 
        caja);
        agregar_cajas(caja);

        if (poke_por_caja(caja) < MAX_CAJA) {inventario.push_back(nuevo);} 
        else {cout << "Espacio en caja lleno" << endl;}
    }

    archivo.close();
    caja_actual = primera_caja;

    cout << inventario.size() << " Pokemon cargados correctamente" << endl;
}

void PC::guardar_csv(const string& file) {
    string temp = file + ".temp";

    ifstream entrada(file);
    ofstream temporal(temp);

    if (!temporal.is_open()) {return;}

    temporal << "CAJAS:";

    if (primera_caja != nullptr) {
        Nodo* p = primera_caja;

        do {
            temporal << p->nombre;
            p = p->siguiente;

            if (p != primera_caja) {temporal << ",";}
        } while (p != primera_caja);
    }
    temporal << endl;

    for(int i = 0; i < inventario.size(); i++) {
        PokeCapturado& p = inventario[i];
            temporal << p.getID() << "," << p.getNombre() << "," << p.getTipo(1)
            << "," << p.getTipo(2) << "," << p.getHP() << "," << p.getMote()
            << "," << p.getLv() << "," << p.getCaja() << endl;
    }
    entrada.close();
    temporal.close();

    remove(file.data());
    rename(temp.data(), file.data());
}

void PC::crear_jugador(const string& file) {
    ifstream entrada(file);

    if (entrada.is_open()) {
        entrada.close();
        return;
    }

    inventario.clear();
    limpiar_cajas();

    agregar_cajas("Caja 1");
    agregar_cajas("Caja 2");
    caja_actual = primera_caja;

    ofstream archivo(file);

    if (!archivo.is_open()) {return;}

    archivo << "CAJAS:Caja 1,Caja 2" << endl;
    archivo.close();
}

void PC::copy_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
    int low, int high) {
    for (int i = low; i <= high; i++) {
        a[i] = b[i];
    }
}

void PC::merge_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b,
    int low, int mid, int high) {

    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= high) {
        if (a[i].getID() <= a[j].getID()) {
            b[k] = a[i];
            i++;
        } else {
            b[k] = a[j];
            j++;
        }
        k++;
    }

    if (i > mid) {
        while (j <= high) {
            b[k++] = a[j];
            j++;
        }
    } else {
        while (i <= mid) {
            b[k++] = a[i];
            i++;
        }
    }
}

void PC::merge_split(vector<PokeCapturado>& a, vector<PokeCapturado>& b,
    int low, int high) {
    
    if ((high - low) < 1) {
        return;
    }

    int mid = low + (high - low) / 2;

    merge_split(a, b, low, mid);
    merge_split(a, b, mid + 1, high);
    merge_array(a, b, low, mid, high);
    copy_array(a, b, low, high);
}

vector<PokeCapturado> PC::merge_sort(const vector<PokeCapturado>& source) {
    if (source.empty()) return source;

    vector<PokeCapturado> v(source);
    vector<PokeCapturado> tmp(v.size());

    merge_split(v, tmp, 0, v.size() - 1);
    return v;
}

void PC::ordenar_caja() {
    string actual = getCajaActual();

    if (actual.empty()) {return;}

    vector<PokeCapturado> esta_caja;
    vector<PokeCapturado> otra_caja;

    for(int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == actual) {
            esta_caja.push_back(inventario[i]);
        } else {
            otra_caja.push_back(inventario[i]);
        }
    }

    if (esta_caja.empty()) {return;}

    esta_caja = merge_sort(esta_caja);
    inventario = otra_caja;

    for (int i = 0; i < esta_caja.size(); i++){
        inventario.push_back(esta_caja[i]);
    }

    cout << "Se ordenó por No. de la Pokedex." << endl;
}

void PC::mostrar_caja() {
    string actual = getCajaActual();

    if (actual.empty()) {return;}

    vector<unsigned int> pokemon_caja;

    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == actual) {
            pokemon_caja.push_back(i);
        }
    }

    cout << "\n============================================================";
    cout << "============================================================\n";

    cout << "                    PC DE " << upConvert(nombre) << endl;

    cout << "                 CAJA ACTUAL: " << upConvert(actual) << endl;

    cout << "============================================================";
    cout << "============================================================\n";

    string borde = "+";

    for (int i = 0; i < COLUMNAS; i++) {borde += string(ANCHO, '-') + "+";}

    cout << borde << endl;

    for(int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            unsigned int posicion = i * COLUMNAS + j;
            string contenido = "";
            if (posicion < pokemon_caja.size()) {
                PokeCapturado& pokemon = inventario[pokemon_caja[posicion]];

                contenido = " " + to_string(posicion + 1) + ". " +
                pokemon.getMote();

                if (contenido.length() > ANCHO) {
                    contenido = contenido.substr(0, ANCHO);
                }
            }
            cout << "|" << left << setw(ANCHO) << contenido;
        }
        cout << "|" << endl;
        for (int k = 0; k < COLUMNAS; k++) {
            unsigned int posicion = i * COLUMNAS + k;
            
            string contenido = "";

            if (posicion < pokemon_caja.size()) {
                PokeCapturado& pokemon = inventario[pokemon_caja[posicion]];

                contenido = " #" + to_string(pokemon.getID()) + " Lv. "
                    + to_string(pokemon.getLv());
            }
            cout << "|" << left << setw(ANCHO) << contenido;
        }
        cout << "|" << endl;
        cout << borde << endl;
    }
}

void PC::seleccionar_nombre(const string& nombre) {
    string actual = getCajaActual();
    string busqueda = upConvert(nombre);

    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == actual && (
            upConvert(inventario[i].getMote()) == busqueda || 
            upConvert(inventario[i].getNombre()) == busqueda)) {
                inventario[i].mostrar_info();
                return;
        }
    }
    cout << "No se encontró ningún " << nombre << " En esta caja" << endl;
}

void PC::mover_poke_caja(const string& nombre, const string& name_destino) {
    Nodo* destino = buscar_caja(name_destino);

    if (destino == nullptr) {return;}

    if (destino == caja_actual) {return;}

    if (poke_por_caja(destino->nombre) >= MAX_CAJA) {return;}

    string busqueda = upConvert(nombre);
    string actual = getCajaActual();

    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == actual && 
        (upConvert(inventario[i].getMote()) == upConvert(nombre) || 
        upConvert(inventario[i].getNombre()) == upConvert(nombre))) {
            inventario[i].setCaja(destino->nombre);
            
            string tag = inventario[i].getMote();
            if (inventario[i].getMote() != inventario[i].getNombre()) {
                tag += " (" + inventario[i].getNombre() + ")";
            }
            cout << tag << " fue transferido a " << destino->nombre << endl;
            return;
        }
    }
    cout << "No se encontró a " << nombre << " En la caja actual" << endl;
}

void PC::agregar_pokemon(PokeCapturado& nuevo) {
    string name_caja = nuevo.getCaja();

    if (poke_por_caja(name_caja) >= MAX_CAJA) {
        cout << "No se puede agregar " << nuevo.getMote() 
            << " la caja está llena" << endl;
            return;
    }

    agregar_cajas(name_caja);
    inventario.push_back(nuevo);
}

void PC::liberar_pokemon(const string& nombre) {
    string actual = getCajaActual();
    string busqueda = upConvert(nombre);

    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == actual && 
        (upConvert(inventario[i].getMote()) == busqueda ||
            upConvert(inventario[i].getNombre()) == busqueda)) {

                string tag = inventario[i].getMote();
                if (inventario[i].getMote() != inventario[i].getNombre()) {
                    tag += " (" + inventario[i].getNombre() + ")";
                }
                cout << "Se ha liberado a " << tag << endl;
                inventario.erase(inventario.begin() + i);
                return;
            } 
    }
    cout << "No se encontró a " << nombre << " en esta caja" << endl;
}

bool PC::cambiar_caja(const string& nombre) {
    Nodo* found = buscar_caja(nombre);

    if (found == nullptr) {return false;}

    caja_actual = found; 
    return true;
}

void PC::siguiente_caja() {
    if (primera_caja == nullptr) return;

    if (caja_actual == nullptr) {
        caja_actual = primera_caja;
        return;
    }

    caja_actual = caja_actual->siguiente;
}

void PC::caja_anterior() {
    if (primera_caja == nullptr) {return;}

    if (caja_actual == nullptr) {
        caja_actual = primera_caja;
        return;
    }

    Nodo* p = primera_caja;

    while (p->siguiente != caja_actual) {p = p->siguiente;}

    caja_actual = p;
}


//https://stackoverflow.com/questions/32929977/method-that-converts-string-to-upper-case