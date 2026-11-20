/*
 * Proyecto Gestor de Cajas Pokémon — Clases PC
 * Marco Antonio Hernández Roque
 * A01709802
 * 19/11/2026
 * Materia: TC1031 Programación de Estructuras de Datos y 
    Algoritmos Fundamentales
 * En este archivo se define la clase controladora PC y el nodo Nodo 
    de su Lista Ligada Circular de cajas. Funciona como el cerebro del
    sistema de almacenamiento el cual administra el inventario de 
    Pokémon capturados, la navegación entre cajas, el ordenamiento con 
    Merge Sort y la persistencia en archivos CSV/TXT.
 */

#ifndef PC_H
#define PC_H

#include <vector>
#include <string>
#include "Pokemon.h"

using namespace std;

class Nodo;

/**
 * PC funciona como el núcleo del sistema de almacenamiento del entrenador.
 * Combina un vector dinámico de PokeCapturado para el inventario con 
    una Lista Doblemente Ligada de nodos Nodo para representar las cajas
    en una tabla de 6x5. También implementa Merge Sort por ID de Pokédex 
    y la lectura/escritura en archivos de texto.
 */
class PC {
private:
    vector<PokeCapturado> inventario;
    string nombre;

    Nodo* primera_caja;
    Nodo* caja_actual;

    // Constantes de capacidad y formato visual de la cuadrícula
    const int MAX_CAJA = 30;
    const int COLUMNAS = 6;
    const int FILAS = 5;
    const int ANCHO = 18;

    /**
     * copy_array copia el rango [low, high] del vector auxiliar al 
        original.
     *
     * Transfiere los elementos ya ordenados desde el vector temporal 
        b hacia el vector a después de cada fase de mezcla en Merge Sort.
     *
     * @param vector<PokeCapturado>& a destino, vector<PokeCapturado>& b 
        fuente, int low, int high
     * @return
     */
    // Complejidad: O(n)
    void copy_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
        int, int);
    /**
     * merge_array combina dos mitades ordenadas de un subarreglo por ID.
     *
     * Compara secuencialmente el ID de Pokédex de cada mitad para 
        escribir el resultado ordenado en el vector auxiliar b 
        manteniendo estabilidad.
     *
     * @param vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
        int low, int mid, int high
     * @return
     */
    // Complejidad: O(n)
    void merge_array(vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
    int, int, int);
    /**
     * merge_split divide recursivamente el vector en mitades para 
        ordenarlo.
     *
     * Calcula el punto medio, invoca recursivamente sobre ambas mitades 
        y luego las combina con merge_array() y copy_array().
     *
     * @param vector<PokeCapturado>& a, vector<PokeCapturado>& b, 
        int low, int high
     * @return
     */
    // Complejidad: O(n log n)
    void merge_split(vector<PokeCapturado>& a, vector<PokeCapturado>& b,
    int, int);
    /**
     * merge_sort ordena una copia del vector recibido por número de 
        Pokédex.
     *
     * Prepara el vector temporal y ejecuta merge_split() sobre todo el 
        rango para devolver el conjunto ordenado de forma ascendente.
     *
     * @param const vector<PokeCapturado>& vector origen a ordenar
     * @return vector<PokeCapturado> nuevo vector ordenado por ID
     */
    // Complejidad: O(n log n)
    vector<PokeCapturado> merge_sort(const vector<PokeCapturado>&);
    /**
     * poke_por_caja cuenta cuántos Pokémon están guardados en una caja 
        dada.
     *
     * Recorre el vector inventario incrementando un contador por cada
        Pokémon cuya caja coincida con el nombre especificado.
     *
     * @param const string& nombre de la caja a contabilizar
     * @return int número de Pokémon almacenados en esa caja
     */
    // Complejidad: O(n)
    int poke_por_caja(const string&);
    /**
     * buscar_caja localiza el nodo de una caja por nombre en la lista 
        circular.
     *
     * Recorre los enlaces desde primera_caja comparando nombres con 
        upConvert() hasta encontrar coincidencia o completar una vuelta 
        entera.
     *
     * @param const string& nombre de la caja a buscar
     * @return Nodo* apuntador al nodo encontrado, o 0 si no existe
     */
    // Complejidad: O(n)
    Nodo* buscar_caja(const string&);
    /**
     * agregar_cajas inserta un nuevo nodo de caja al final de la lista 
        circular.
     *
     * Verifica que el nombre no esté vacío ni repetido mediante 
        buscar_caja() y actualiza los punteros siguiente y anterior para 
        mantener la circularidad.
     *
     * @param const string& nombre de la nueva caja
     * @return bool true si se insertó correctamente, 
        false si ya existía o era inválida
     */
    // Complejidad: O(n)
    bool agregar_cajas(const string&);
    /**
     * limpiar_cajas libera la memoria de todos los nodos de la 
        lista circular.
     *
     * Recorre la lista aplicando delete a cada Nodo y reinicia 
        primera_caja y caja_actual a 0 para evitar fugas de memoria.
     *
     * @param
     * @return
     */
    // Complejidad: O(n)
    void limpiar_cajas();
    
public:
    PC(const string& nombre = "");
    ~PC();

    string getCajaActual();
    string getNombre();
    vector<string> getCajas();

    void setCajaActual(string&);
    void setNombre(string&);

    /**
     * crear_caja valida y crea una nueva caja vacía en el PC.
     *
     * Rechaza nombres vacíos, con comas o solo de espacios; si el 
        nombre es válido y no existe, inserta la caja y mueve 
        caja_actual hacia ella.
     *
     * @param const string& nombre de la nueva caja
     * @return
     */
    // Complejidad: O(n)
    void crear_caja(const string&);
    /**
     * renombrar_caja cambia el nombre de la caja actual y actualiza
        sus Pokémon.
     *
     * Verifica que el nuevo nombre sea válido y no esté repetido, 
        actualiza el atributo caja de todos los Pokémon contenidos y 
        renombra el nodo.
     *
     * @param const string& nuevo nombre para la caja actual
     * @return
     */
    // Complejidad: O(n)
    void renombrar_caja(const string&);
    /**
     * siguiente_caja mueve caja_actual al nodo siguiente de la lista 
        circular.
     *
     * @param
     * @return
     */
    // Complejidad O(1)
    void siguiente_caja();
    /**  
     * caja_anterior mueve caja_actual al nodo anterior de la lista 
        circular.
     *
     * @param
     * @return
     */
    // Complejidad O(1)
    void caja_anterior();
    /**
     * cambiar_caja salta directamente a la caja indicada por nombre.
     *
     * Busca el nodo correspondiente con buscar_caja() y actualiza 
        caja_actual si la caja existe.
     *
     * @param const string& nombre de la caja destino
     * @return bool true si el cambio se realizó, false si la caja no 
        existe
     */
    // Complejidad: O(n)
    bool cambiar_caja(const string&);
    /**
     * mostrar_cajas lista en consola todas las cajas del jugador.
     *
     * Recorre la lista circular desde primera_caja mostrando cada 
        nombre y añadiendo etiquetas (full) si alcanzó 30 Pokémon y 
        (actual) si está abierta.
     *
     * @param
     * @return
     */
    // Complejidad: O(n)
    void mostrar_cajas();

    /**
     * cargar_csv lee el archivo del jugador y reconstruye sus cajas e 
        inventario.
     *
     * Procesa la cabecera CAJAS: para armar la lista circular y luego 
        lee cada línea separada por comas para instanciar los 
        PokeCapturado respetando MAX_CAJA.
     *
     * @param const string& ruta o nombre del archivo del jugador
     * @return
     */
    // Complejidad O(n)
    void cargar_csv(const string&);
    /**
     * guardar_csv respalda las cajas y el inventario usando un archivo 
        temporal.
     *
     * Escribe primero la línea CAJAS: y después cada Pokémon en un 
        archivo .temp; al finalizar elimina el original y renombra 
        el temporal.
     *
     * @param const string& ruta o nombre del archivo del jugador
     * @return
     */
    // Complejidad O(n)
    void guardar_csv(const string&);
    /**
     * crear_jugador inicializa un archivo nuevo con las cajas por 
        defecto.
     *
     * Si el archivo no existe, crea Caja 1 y Caja 2 en la lista 
        y genera el archivo de texto inicial del entrenador.
     *
     * @param const string& nombre del archivo del nuevo jugador
     * @return
     */
    // Complejidad O(1)
    void crear_jugador(const string&);

    /**
     * mostrar_caja dibuja en consola la cuadrícula 6x5 de la caja 
        actual.
     *
     * Filtra los Pokémon de caja_actual, imprime el encabezado con las 
        cajas vecinas [A] y [D] y despliega cada ranura con posición, 
        mote, ID y nivel.
     *
     * @param
     * @return
     */
    // Complejidad O(n)
    void mostrar_caja();
    /**
     * mover_poke_caja transfiere un Pokémon de la caja actual a otra 
        caja.
     *
     * Valida que la caja destino exista, sea distinta a la actual y 
        tenga espacio disponible antes de actualizar la caja del Pokémon 
        buscado.
     *
     * @param const string& nombre o mote del Pokémon, const string& 
        caja destino
     * @return
     */
    // Complejidad O(n)
    void mover_poke_caja(const string&, const string&);
    /**
     * agregar_pokemon deposita un nuevo Pokémon en el inventario del 
        jugador.
     *
     * Verifica que la caja destino no supere MAX_CAJA, asegura que la 
        caja exista, inserta el Pokémon al final del vector y confirma 
        en pantalla.
     *
     * @param PokeCapturado& objeto Pokémon a almacenar
     * @return
     */
    // Complejidad O(n)
    void agregar_pokemon(PokeCapturado&);
    /**
     * liberar_pokemon elimina un Pokémon de la caja actual tras 
        validarlo.
     *
     * Recolecta todas las coincidencias por mote o especie en 
        caja_actual; si hay más de una, muestra mote, nombre, HP y 
        nivel para elegir cuál liberar antes de borrarlo del vector 
        con erase().
     *
     * @param const string& nombre o mote del Pokémon a liberar
     * @return
     */
    // Complejidad O(n)
    void liberar_pokemon(const string&);
    /**
     * buscar_pokemon busca coincidencias exactas por mote o especie en 
        todo el PC.
     *
     * Recorre una sola vez el vector inventario usando upConvert(), 
        imprime cada coincidencia con su índice, mote, especie y caja, 
        y devuelve la lista.
     *
     * @param const string& nombre o mote a buscar
     * @return vector<PokeCapturado> vector con todos los Pokémon 
        coincidentes
     */
    // Complejidad O(n)
    vector<PokeCapturado> buscar_pokemon(const string&);

    /**
     * ordenar_caja ordena únicamente los Pokémon de la caja actual por 
        ID.
     *
     * Separa los Pokémon de caja_actual del resto del inventario, 
        aplica merge_sort() sobre la caja activa y reintegra el vector 
        resultante.
     *
     * @param
     * @return
     */
    // Complejidad O(n log n)
    void ordenar_caja();
};

/**
 * Nodo representa una caja de almacenamiento dentro de la Lista 
    Doblemente Ligada administrada por PC. Guarda el nombre de la caja 
    y los apuntadores a la caja siguiente y a la caja anterior.
 */
class Nodo {
private:
    string nombre;
    Nodo *siguiente;
    Nodo *anterior;

    Nodo(const string&);

    friend class PC;
};

#endif