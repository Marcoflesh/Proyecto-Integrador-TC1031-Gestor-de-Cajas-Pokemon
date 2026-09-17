#include "PC.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
        if (cajas[i] == nueva) return;
    }
    cajas.push_back(nueva);
}

void PC::cargar_csv(){
    ifstream archivo("Por_definir.txt");
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
        inventario.push_back(nuevo);
        agregar_cajas(caja);
    }

    archivo.close();
    cout << inventario.size() << " Pokemon cargados correctamente" << endl;
}

void PC::guardar_csv() {
    ofstream temporal("Por_definir_temp.txt");

    temporal << "CAJAS:";
    for (int i = 0; i < cajas.size(); i++) {
        temporal << cajas[i];
        if (i < (int)cajas.size() - 1) temporal << ",";
    }
    temporal << endl;

    for(int i = 0; i < inventario.size(); i++) {
        PokeCapturado& p = inventario[i];
            temporal << p.getID() << "," << p.getNombre() << "," << p.getTipo(1)
            << "," << p.getTipo(2) << "," << p.getHP() << "," << p.getMote()
            << "," << p.getLv() << "," << p.getCaja() << endl;
    }
    temporal.close();
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

    vector<PokeCapturado> ordena = merge_sort(esta_caja);

    inventario = otra_caja;
    for (int i = 0; i < ordena.size(); i++){inventario.push_back(ordena[i]);}

    cout << "Se ordenó por No. de la Pokedex." << endl;
}

void PC::mostrar_caja() {
    cout << "\n==================================================\n";
    cout << "                 SISTEMA PC DE " << upConvert(nombre) << endl;
    cout << "==================================================\n";
    cout << " [ ESTAS EN LA CAJA: \"" << caja_actual << "\" ]" << endl;
    cout << " ------------------------------------------------\n";

    bool pokemon = false;
    for (int i = 0; i < inventario.size(); i++) {
        if (inventario[i].getCaja() == caja_actual) {
            cout << " - " << inventario[i].getMote();
            if (inventario[i].getMote() != inventario[i].getNombre()) {
                cout << " (" << inventario[i].getNombre() << ")";
            }
            cout << " [#" << inventario[i].getID() << " | Nv. "
            << inventario[i].getLv() << "]" << endl;
            pokemon = true;
        }
    }
    if (!pokemon) {
        cout << "No hay Pokemon en esta caja" << endl;
    }
    cout << "------------------------------------------------\n";
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
    for (int i = 0; i < cajas.size(); i++) {
        if (cajas[i] == caja_actual) {
            int siguiente = (i + 1) % cajas.size();
            caja_actual = cajas[siguiente];
            return;
        }
    }
    caja_actual = cajas[0];
}

void PC::caja_anterior() {
    for (int i = 0; i < cajas.size(); i++) {
        if (cajas[i] == caja_actual) {
            int anterior = (i - 1 + cajas.size()) % cajas.size();
            caja_actual = cajas[anterior];
            return;
        }
    }
    caja_actual = cajas[0];
}

//https://stackoverflow.com/questions/32929977/method-that-converts-string-to-upper-case