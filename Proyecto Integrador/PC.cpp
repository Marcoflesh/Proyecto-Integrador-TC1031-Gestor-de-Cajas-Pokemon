#include "PC.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

string upConvert(const string& text) {
    string copia = text;
    for (unsigned int i = 0; i < copia.length(); i++) {
        copia[i] = toupper(copia[i]);
    }
    return copia;
}

PC::PC(const string& nombre) : caja_actual("Caja 1"), nombre(nombre) {}

string PC::getCajaActual() {return caja_actual;}
string PC::getNombre() {return nombre;}
vector<string> PC::getCajas() {return cajas;}

void PC::setCajaActual(string& nueva_caja) {caja_actual = nueva_caja;}
void PC::setNombre(string& nuevo_nombre) {nombre = nuevo_nombre;}

void PC::agregar_cajas(const string& nueva){
    for (int i = 0; i < cajas.size(); i++) {
        if (cajas[i] == nueva) {return;}
    }
    cajas.push_back(nueva);
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
    cajas.clear();
    caja_actual = "Caja 1";

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
    if (!cajas.empty()) {caja_actual = cajas[0];}
    cout << inventario.size() << " Pokemon cargados correctamente" << endl;
}

void PC::guardar_csv(const string& file) {
    string temp = file + ".temp";

    ifstream entrada(file);
    ofstream temporal(temp);

    if (!temporal.is_open()) return;

    temporal << "CAJAS:";

    for (size_t i = 0; i < cajas.size(); i++) {
        temporal << cajas[i];
        if (i + 1 < cajas.size()) {temporal << ",";}
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
    cajas.clear();
    caja_actual = "Caja 1";

    agregar_cajas("Caja 1");
    agregar_cajas("Caja 2");

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
    vector<PokeCapturado> esta_caja;
    vector<PokeCapturado> otra_caja;

    for(int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == caja_actual) {
            esta_caja.push_back(inventario[i]);
        } else {
            otra_caja.push_back(inventario[i]);
        }
    }

    if (esta_caja.empty()) return;

    esta_caja = merge_sort(esta_caja);

    inventario = otra_caja;
    for (int i = 0; i < esta_caja.size(); i++){
        inventario.push_back(esta_caja[i]);
    }

    cout << "Se ordenó por No. de la Pokedex." << endl;
}

void PC::mostrar_caja() {
    vector<size_t> pokemon_caja;

    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == caja_actual) {
            pokemon_caja.push_back(i);
        }
    }

    cout << "\n============================================================";
    cout << "============================================================\n";

    cout << "                 SISTEMA PC DE " << upConvert(nombre) << endl;

    cout << "                 CAJA ACTUAL: " << upConvert(caja_actual) << endl;

    cout << "============================================================";
    cout << "============================================================\n";

    string borde = "+";

    for (int i = 0; i < COLUMNAS; i++) {borde += string(ANCHO, '-') + "+";}

    cout << borde << endl;

    for(int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            size_t posicion = i * COLUMNAS + j;
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
            size_t posicion = i * COLUMNAS + k;
            
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
    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == caja_actual && (
            inventario[i].getMote() == nombre || 
            inventario[i].getNombre() == nombre)) {
                inventario[i].mostrar_info();
                return;
        }
    }
    cout << "No se encontró ningún " << nombre << " En esta caja" << endl;
}

void PC::mover_poke_caja(const string& nombre, const string& caja) {
    bool existe = false;

    for (int i = 0; i < cajas.size(); i++) {
        if (cajas[i] == caja) {
            existe = true;
            break;
        }
    }

    if (!existe) {
        return;
    }

    if (caja != caja_actual && poke_por_caja(caja) >= MAX_CAJA) {
        cout << "La caja " << caja << " está llena" << endl;
        return;
    }

    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == caja_actual && 
        (inventario[i].getMote() == nombre || 
        inventario[i].getNombre() == nombre)) {
            inventario[i].setCaja(caja);
            
            string tag = inventario[i].getMote();
            if (inventario[i].getMote() != inventario[i].getNombre()) {
                tag += " (" + inventario[i].getNombre() + ")";
            }
            cout << tag << " fue transferido a " << caja << endl;
            return;
        }
    }
    cout << "No se encontró a " << nombre << " En la caja actual" << endl;
}

void PC::agregar_pokemon(PokeCapturado& nuevo) {
    if (poke_por_caja(nuevo.getCaja()) >= MAX_CAJA) {
        cout << "No se puede agregar " << nuevo.getMote() 
            << " la caja está llena" << endl;
            return;
    }

    inventario.push_back(nuevo);
    agregar_cajas(nuevo.getCaja());
}

void PC::liberar_pokemon(const string& nombre) {
    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == caja_actual && 
        (inventario[i].getMote() == nombre ||
            inventario[i].getNombre() == nombre)) {

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
    for (int i = 0; i < cajas.size(); i++) {
        if (cajas[i] == nombre) {
            caja_actual = nombre;
            return true;
        }
    }
    return false;
}

void PC::siguiente_caja() {
    if (cajas.empty()) return;

    for (size_t i = 0; i < cajas.size(); i++) {
        if (cajas[i] == caja_actual) {
            size_t siguiente = (i + 1) % cajas.size();
            caja_actual = cajas[siguiente];
            return;
        }
    }
    caja_actual = cajas[0];
}

void PC::caja_anterior() {
    if (cajas.empty()) {return;}

    for (size_t i = 0; i < cajas.size(); i++) {
        if (cajas[i] == caja_actual) {
            size_t anterior = (i + cajas.size() - 1) % cajas.size();
            caja_actual = cajas[anterior];
            return;
        }
    }
    caja_actual = cajas[0];
}


//https://stackoverflow.com/questions/32929977/method-that-converts-string-to-upper-case