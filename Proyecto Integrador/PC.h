#ifndef PC_H
#define PC_H

#include <vector>
#include <string>
#include "Pokemon.h"

using namespace std;

class PC {
private:
    vector<PokeCapturado> inventario;
    string caja_actual, nombre;

    void copy_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
        int, int);
    void merge_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
    int, int, int);
    void merge_split(vector<PokeCapturado>& a, vector<PokeCapturado>& b,
    int, int);
    
public:
    PC(const string& nombre = "");

    string getCajaActual();
    string getNombre();

    void setCajaActual(string&);
    void setNombre(string&);
    
    void cargar_csv();
    void guardar_csv();

    char* upConvert(string&);

    void mostrar_caja();
    void seleccionar_nombre(const string&);
    void mover_poke_caja(const string&, const string&);
    void agregar_pokemon(PokeCapturado&);
    void liberar_pokemon(const string&);

    vector<PokeCapturado> merge_sort(const vector<PokeCapturado>&);
    void ordenar_caja();
};

#endif