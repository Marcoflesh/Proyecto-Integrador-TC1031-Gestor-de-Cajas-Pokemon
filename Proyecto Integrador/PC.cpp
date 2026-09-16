#include "PC.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

PC::PC(const string& nombre) : caja_actual("Caja 1"), nombre(nombre) {}

string PC::getCajaActual() {return caja_actual;}
string PC::getNombre() {return nombre;}

void PC::setCajaActual(string& nueva_caja) {caja_actual = nueva_caja;}
void PC::setNombre(string& nuevo_nombre) {nombre = nuevo_nombre;}

char* upConvert(string& s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        s[i] = toupper(s[i]);
    }
}

void PC::cargar_csv(){
    ifstream archivo("Por_definir.txt");
    string linea;

    if (!archivo.is_open()) return;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string id, esp, t1, t2, hp, mote, lv, caja;

        getline(ss, id, ','); getline(ss, esp, ','); getline(ss, t1, ',');
        getline(ss, t2, ','); getline(ss, hp, ','); getline(ss, mote, ',');
        getline(ss, lv, ','); getline(ss, caja, ',');

    }

    archivo.close();
    cout << inventario.size() << " Pokemon cargados correctamente" << endl;
}

void PC::guardar_csv() {
    ifstream entrada("Por_definir.txt");
    ofstream temporal("Por_definir_temp.txt");

    for(int i = 0; i < inventario.size(); i++) {
        PokeCapturado& p = inventario[i];
        if (p.getCaja() == caja_actual) {
            temporal << p.getID() << "," << p.getNombre() << "," << p.getTipo(1)
            << "," << p.getTipo(2) << "," << p.getHP() << "," << p.getMote()
            << "," << p.getLv() << "," << p.getCaja() << endl;
        }
        entrada.close();
        temporal.close();
        remove("Por_definir.txt");
        rename("Por_definir_temp.txt", "Por_definir.txt");
    }
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

    int mid = (high - low) / 2;

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


//https://stackoverflow.com/questions/32929977/method-that-converts-string-to-upper-case