#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

string upConvert(const string&);

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
    void mostrar_info();
};

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

    void mostrar_info();
};

class NodoPokedex {
private:
    Pokemon dato;

    NodoPokedex* left;
    NodoPokedex* right;

    NodoPokedex(const Pokemon&);

    friend class Pokedex;
};

class Pokedex {
private:
    NodoPokedex* raiz;

    void cargar_pokedex(const string&);
    NodoPokedex* insertar(NodoPokedex*, const Pokemon&);
    NodoPokedex* buscar_nodo(NodoPokedex*, const string&);
    void inorder(NodoPokedex*, vector<Pokemon>&);
    void limpiar(NodoPokedex*);

public:
    Pokedex(const string& archivo = "pokedex_completa.txt");
    ~Pokedex();
    Pokemon* buscar_pokemon(const string&);
    vector<Pokemon> visit();
};

#endif