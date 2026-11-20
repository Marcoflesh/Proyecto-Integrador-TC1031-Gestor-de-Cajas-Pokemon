/*
 * Proyecto Gestor de Cajas Pokémon — Clase Pokemon
 * Marco Antonio Hernández Roque
 * A01709802
 * 19/11/2026
 * Materia: TC1031 Programación de Estructuras de Datos y Algoritmos 
    Fundamentales
 * En este archivo se define la clase base Pokemon, su clase hija 
    PokeCapturado, el BST de la Pokédex y la Lista Doblemente Ligada para 
    paginar las cajas y la Pokedex.
 */

#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

/**
 * upConvert convierte todos los caracteres de una cadena a mayúsculas.
 *
 * Recorre cada carácter de una copia del texto recibido y aplica 
    toupper() para permitir comparaciones sin distinguir entre 
    mayúsculas y minúsculas.
 *
 * @param const string& text cadena original a convertir
 * @return string copia de la cadena en mayúsculas
 */
string upConvert(const string&);

/**
 * Pokemon es la clase base que modela los datos generales de una 
    especie.
 * Guarda el número de Pokédex, puntos de salud (HP), nombre de la 
    especie y hasta dos tipos elementales. Sirve como padre para 
    PokeCapturado y como tipo de dato almacenado en los nodos del BST 
    de la Pokédex.
 */
class Pokemon {
protected:
    int id_dex, hp;
    string nom_especie, tipo1, tipo2;

public:
    Pokemon(int id = 0, const string nomb = "", const string& t1 = "", 
        const string& t2 = "", int health = 0);
    virtual ~Pokemon(){}

    int getID();
    int getHP();
    string getNombre() const;
    string getTipo();
    string getTipo(int);
    /**
     * mostrar_info imprime en consola los datos básicos de la especie 
        Pokémon.
     *
     * Despliega el nombre de la especie, su número de Pokédex y sus 
        tipos elementales formateados con encabezados.
     *
     * @param
     * @return
     */
    // complejidad: O(1)
    void mostrar_info();
};

/**
 * PokeCapturado hereda de Pokemon. Añade los atributos individuales de un
 * ejemplar perteneciente a un jugador: su mote personalizado, el nivel
 * actual y el nombre de la caja del PC donde está almacenado.
 */
class PokeCapturado: public Pokemon {
private:
    string mote, caja;
    int nivel;

public:
    PokeCapturado(int id = 0, const string nomb = "", const string& t1 = "", 
        const string& t2 = "", int hp = 0, const string& nick = "", int lv = 1, 
        const string& box = "Caja 1");
    virtual ~PokeCapturado(){}

    string getMote();
    int getLv();
    string getCaja();

    void setNivel(int nuevo_nivel);
    void setCaja(string nueva_caja);

    /**
     * mostrar_info imprime la ficha completa del Pokémon capturado en 
        consola.
     *
     * Despliega el mote, nombre de la especie, número de Pokédex, tipos
        elementales, HP, nivel y la caja actual en la que se encuentra.
     *
     * @param
     * @return
     */
    // Complejidad O(1)
    void mostrar_info();
};

/**
 * NodoPokedex representa un nodo individual del Árbol Binario de 
    Búsqueda de la Pokédex. Almacena un objeto Pokemon y los apuntadores 
    a sus subárboles izquierdo y derecho.
 */
class NodoPokedex {
private:
    Pokemon dato;

    NodoPokedex* left;
    NodoPokedex* right;

    NodoPokedex(const Pokemon&);

    friend class Pokedex;
};

/**
 * Pokedex implementa un Árbol Binario de Búsqueda (BST) que almacena 
    las 1,025 especies oficiales ordenadas alfabéticamente por nombre. 
 *Permite búsquedas rápidas para autocompletar ID y tipos, así como 
    recorrer todo el catálogo en orden A-Z mediante un recorrido inorder.
 */
class Pokedex {
private:
    NodoPokedex* raiz;

    /**
     * cargar_pokedex lee el archivo de catálogo y construye el 
        BST en memoria.
     *
     * Abre el archivo indicado, valida la cabecera ID,NOMBRE,TIPO1,
        TIPO2, separa los campos de cada línea e inserta cada especie 
        en el árbol.
     *
     * @param const string& ruta o nombre del archivo de la Pokédex
     * @return
     */
    // Complejidad: O(n log n)
    void cargar_pokedex(const string&);
    /**
     * insertar agrega recursivamente un Pokémon en su posición 
        alfabética del BST.
     *
     * Compara el nombre en mayúsculas contra el nodo actual 
        para descender por el subárbol izquierdo o derecho hasta crear 
        el nuevo nodo hoja.
     *
     * @param NodoPokedex* nodo actual, const Pokemon& especie a insertar
     * @return NodoPokedex* apuntador a la raíz del subárbol actualizado
     */
    // Complejidad: O(log n)
    NodoPokedex* insertar(NodoPokedex*, const Pokemon&);
    /**
     * buscar_nodo localiza recursivamente una especie por nombre 
        dentro del BST.
     *
     * Compara el nombre buscado en mayúsculas contra el nodo actual y
     * desciende por izquierda o derecha hasta encontrarlo o llegar a nulo.
     *
     * @param NodoPokedex* nodo actual, const string& nombre de la especie
     * @return NodoPokedex* apuntador al nodo encontrado, o 0 si no existe
     */
    // Complejidad: O(log n) 
    NodoPokedex* buscar_nodo(NodoPokedex*, const string&);
    /**
     * inorder recorre el árbol en orden alfabético (izquierda, raíz, 
        derecha).
     *
     * Visita cada nodo del BST y agrega su objeto Pokemon al vector 
        recibido para obtener el catálogo completo ordenado de la A a la Z.
     *
     * @param NodoPokedex* nodo actual, vector<Pokemon>& lista acumulada
     * @return
     */
    // Complejidad: O(n)
    void inorder(NodoPokedex*, vector<Pokemon>&);
    /**
     * limpiar libera recursivamente la memoria de todos los nodos del BST.
     *
     * Recorre el árbol en postorden aplicando delete a cada nodo para 
        evitar fugas de memoria al destruir la instancia de Pokedex.
     *
     * @param NodoPokedex* nodo actual a liberar
     * @return
     */
    // Complejidad O(n)
    void limpiar(NodoPokedex*);

public:
    Pokedex(const string& archivo = "pokedex_completa.txt");
    ~Pokedex();
    /**
     * loaded verifica si el catálogo de la Pokédex se cargó 
        correctamente.
     *
     * Comprueba si el apuntador raiz es distinto de 0 (árbol no vacío).
     *
     * @param
     * @return bool true si la Pokédex tiene datos cargados, 
     * false si está vacía
     */
    // Complejidad: O(1)
    bool loaded() const;
    /**
     * buscar_pokemon busca una especie oficial en la Pokédex por nombre.
     *
     * Utiliza buscar_nodo() sobre la raíz del BST sin distinguir 
        mayúsculas ni minúsculas y devuelve el apuntador al objeto 
        Pokemon encontrado.
     *
     * @param const string& nombre de la especie a buscar
     * @return Pokemon* apuntador a la especie encontrada, 
     * o 0 si no existe
     */
    // Complejidad: O(log n), peor caso O(n)
    Pokemon* buscar_pokemon(const string&);
    /**
     * visit devuelve todos los Pokémon de la Pokédex ordenados 
        alfabéticamente.
     *
     * Crea un vector vacío, lo llena mediante el recorrido inorder() 
        desde la raíz y retorna la lista completa ordenada de la A a la Z.
     *
     * @param
     * @return vector<Pokemon> catálogo completo en orden alfabético
     */
    // Complejidad O(n)
    vector<Pokemon> visit();
};

/**
 * NodoPagina representa una página individual dentro de la lista 
    doblemente ligada de la Pokédex. Almacena su número de página, un 
    bloque de hasta 100 Pokémon y los apuntadores a la página 
    siguiente y anterior.
 */
class NodoPagina {
private:
    int n_pagina;
    vector<Pokemon> pokemons;
    NodoPagina* siguiente;
    NodoPagina* anterior;

    NodoPagina(int, const vector<Pokemon>&);

    friend class PaginaPokedex;
};

/**
 * PaginaPokedex implementa una Lista Doblemente Ligada para dividir
    y navegar la Pokédex completa por páginas de tamaño fijo (100 Pokémon).
 * Permite avanzar y retroceder circularmente.
 */
class PaginaPokedex {
private:
    NodoPagina* primera_p;
    NodoPagina* p_actual;
    int total_pag;

    /**
     * limpiar_paginas libera la memoria de todos los nodos de página 
        creados.
     *
     * Recorre la lista circular desde primera_p aplicando delete a cada
        NodoPagina y reinicia los apuntadores y el contador a cero.
     *
     * @param
     * @return
     */
    // Complejidad: O(n)
    void limpiar_paginas();

public:
    PaginaPokedex(const vector<Pokemon>&, int);
    ~PaginaPokedex();

    /**
     * next_p avanza el puntero p_actual hacia la página siguiente.
     *
     * Al ser una lista circular, avanzar desde la última página regresa
        automáticamente a la primera.
     *
     * @param
     * @return
     */
    // Comlejidad: O(1)
    void next_p();
    /**
     * previous_p retrocede el puntero p_actual hacia la página anterior.
     *
     * Al ser una lista circular, retroceder desde la primera página salta
        automáticamente a la última.
     *
     * @param
     * @return
     */
    // Complejidad: O(1)
    void previous_p();
    /**
     * mostrar_pagina imprime en consola los Pokémon de la página actual.
     *
     * Muestra el encabezado con el número de página sobre el total y 
        lista el ID, nombre y tipos de cada Pokémon contenido en el nodo 
        actual.
     *
     * @param
     * @return
     */
    // Complejidad: O(n)
    void mostrar_pagina() const;
    /**
     * empty indica si el paginador no contiene ninguna página cargada.
     *
     * @param
     * @return bool true si primera_p es 0, false en caso contrario
     */
    // Complejidad: O(1)
    bool empty() const;
};

#endif