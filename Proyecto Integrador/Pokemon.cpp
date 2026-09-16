#include "Pokemon.h"
#include <iostream>

using namespace std;

Pokemon::Pokemon(int id, const string nomb, const string& t1, const string& t2,
int health) : id_dex(id), nom_especie(nomb), tipo1(t1), tipo2(t2), hp(health) {}

int Pokemon::getHP() {return hp;}
int Pokemon::getID() {return id_dex;}
string Pokemon::getNombre() {return nom_especie;}
string Pokemon::getTipo() {
    if (tipo2.empty()) {
        return tipo1;
    }
    return tipo1 + " / " + tipo2;
}
string Pokemon::getTipo(int puesto) {
    if (puesto == 1) {return tipo1;}
    if (puesto == 2) {return tipo2;}
    return "";
}

PokeCapturado::PokeCapturado(int id, const string nomb, const string& t1, 
    const string& t2, int hp, const string& nick, int lv, const string& box) 
    : Pokemon(id, nomb, t1, t2, hp), mote(nick.empty() ? nomb : nick), 
        nivel(lv), caja(box) {}

string PokeCapturado::getMote() {return mote;}
int PokeCapturado::getLv() {return nivel;}
string PokeCapturado::getCaja() {return caja;}

void PokeCapturado::setNivel(int nuevo_nivel) {nivel = nuevo_nivel;}
void PokeCapturado::setCaja(string nueva_caja) {caja = nueva_caja;}

void PokeCapturado::mostrar_info() {
    cout << "\n==================================\n";
    cout << "Info del Pokemón: " << mote << endl;
    cout << "\n==================================\n";
    cout << "Especie:      " << nom_especie << endl;
    cout << "No. Pokedex:  #" << id_dex << endl;
    cout << "Tipos:        " << tipo1;
    if (!tipo2.empty()) cout << " / " << tipo2 << endl;
    cout << "Nivel:        " << nivel << endl;
    cout << "Caja actual:  " << caja << endl;
    cout << "\n==================================\n";
}
