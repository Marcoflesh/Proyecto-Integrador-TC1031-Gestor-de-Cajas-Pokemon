#ifndef PC_H
#define PC_H

#include <vector>
#include <string>
#include "Pokemon.h"

using namespace std;

class Nodo;

class PC {
private:
    vector<PokeCapturado> inventario;
    string nombre;

    Nodo* primera_caja;
    Nodo* caja_actual;

    const int MAX_CAJA = 30;
    const int COLUMNAS = 6;
    const int FILAS = 5;
    const int ANCHO = 18;

    
    // Complejidad: O(n)
    void copy_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
        int, int);
    // Complejidad: O(n)
    void merge_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
    int, int, int);
    // Complejidad: O(n log n)
    void merge_split(vector<PokeCapturado>& a, vector<PokeCapturado>& b,
    int, int);
    // Complejidad: O(n log n)
    vector<PokeCapturado> merge_sort(const vector<PokeCapturado>&);
    // Complejidad: O(n)
    int poke_por_caja(const string&);
    // Complejidad: O(n)
    Nodo* buscar_caja(const string&);
    // Complejidad: O(n)
    bool agregar_cajas(const string&);
    // Complejidad: O(n)
    void limpiar_cajas();
    // Complejidad: O(n)
    bool cambiar_caja(const string&);
    
public:
    PC(const string& nombre = "");
    ~PC();

    string getCajaActual();
    string getNombre();
    vector<string> getCajas();

    void setCajaActual(string&);
    void setNombre(string&);

    // Complejidad: O(n)
    void crear_caja(const string&);
    // Complejidad: O(n)
    void renombrar_caja(const string&);
    // Complejidad O(1)
    void siguiente_caja();
    // Complejidad O(1)
    void caja_anterior();

    // Complejidad O(n)
    void cargar_csv(const string&);
    // Complejidad O(n)
    void guardar_csv(const string&);
    // Complejidad O(1)
    void crear_jugador(const string&);

    // Complejidad O(n)
    void mostrar_caja();
    // Complejidad O(n)
    void mover_poke_caja(const string&, const string&);
    // Complejidad O(n)
    void agregar_pokemon(PokeCapturado&);
    // Complejidad O(n)
    void liberar_pokemon(const string&);
    // Complejidad O(n)
    vector<PokeCapturado> buscar_pokemon(const string&);

    // Complejidad O(n log n)
    void ordenar_caja();
};

class Nodo {
private:
    string nombre;
    Nodo *siguiente;
    Nodo *anterior;

    Nodo(const string&);

    friend class PC;
};

#endif