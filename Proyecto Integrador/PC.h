#ifndef PC_H
#define PC_H

#include <vector>
#include <string>
#include "Pokemon.h"

using namespace std;

string upConvert(const string&);

class PC {
private:
    vector<PokeCapturado> inventario;
    vector<string> cajas;
    string caja_actual, nombre;
    const int MAX_CAJA = 30;
    const int COLUMNAS = 6;
    const int FILAS = 5;
    const int ANCHO = 18;


    void copy_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
        int, int);
    void merge_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
    int, int, int);
    void merge_split(vector<PokeCapturado>& a, vector<PokeCapturado>& b,
    int, int);
    int poke_por_caja(const string&);
    
public:
    PC(const string& nombre = "");

    string getCajaActual();
    string getNombre();
    vector<string> getCajas();

    void setCajaActual(string&);
    void setNombre(string&);

    void agregar_cajas(const string&);
    void crear_caja(const string&);
    bool cambiar_caja(const string&);
    void siguiente_caja();
    void caja_anterior();

    void cargar_csv(const string&);
    void guardar_csv(const string&);
    void crear_jugador(const string&);

    void mostrar_caja();
    void seleccionar_nombre(const string&);
    void mover_poke_caja(const string&, const string&);
    void agregar_pokemon(PokeCapturado&);
    void liberar_pokemon(const string&);

    vector<PokeCapturado> merge_sort(const vector<PokeCapturado>&);
    void ordenar_caja();
};

#endif