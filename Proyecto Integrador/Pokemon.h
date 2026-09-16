#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <iostream>

using namespace std;

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
    string getNombre();
    string getTipo();
    string getTipo(int);
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

#endif