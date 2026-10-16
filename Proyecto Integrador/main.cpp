#include "PC.h"
#include "User.h"

#include <iostream>
#include <string>
#include <vector>
//#include <Windows.h>

using namespace std;

void menu(PC&, const string&);
void menu_info(PokeCapturado&);
void menu_pokemon(Pokedex&);

int main() {

    /* 
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_NUMERIC, "C");
    */

    bool close = false;
    do {
        cout << "\n==============================" << endl;
        cout << "        SISTEMA PC" << endl;
        cout << "==============================" << endl;
        cout << "[1] Cargar jugador" << endl;
        cout << "[2] Crear nuevo jugador" << endl;
        cout << "[3] Salir" << endl;
        cout << "Opcion: ";
        
        int opcion;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                User player;
                vector<User> users = player.cargar_usuarios();

                cout << "\n Jugadores disponibles:" << endl;
                for(int i = 0; i < users.size(); i++) {
                    cout << "[" << i  << "] " << users[i].getUser() << endl;
                }
                int choice;
                cout << "Selecciona el jugador: ";
                cin >> choice;

                if (users.empty()) {break;}
                if(choice < 0 || choice >= users.size()) {break;}

                User usuario = users[choice];
                PC jugador(usuario.getUser());

                jugador.cargar_csv(usuario.getArchivo());

                if (!jugador.getCajas().empty()) {
                    menu(jugador, usuario.getArchivo());
                } else {
                    cout << "No existe ese usuario" 
                        << upConvert(usuario.getUser()) << endl; 
                }
                break;
            }

            case 2: {
                string player;

                cout << "Nombre del nuevo jugador: ";
                cin >> player;

                User usuario(player);
                PC new_player(usuario.getUser());

                new_player.crear_jugador(usuario.getArchivo());

                if (!new_player.getCajas().empty()) {
                    usuario.registrar_usuario();

                    cout << "Jugador creado" << endl;
                    menu(new_player, usuario.getArchivo());
                } else {
                    cout << "No se pudo crear el jugador" << endl;
                }
                break;
            }

            case 3:
                close = true;
                break;
        }
    } while(!close);

    cout << "Hasta luego" << endl;

    return 0;
}

void menu(PC& pc, const string& archivo) {
    Pokedex pokedex;
    bool close = false;

    do {
        pc.mostrar_caja();

        cout << "\n[A] Caja anterior" << endl;
        cout << "[D] Siguiente caja" << endl;
        cout << "[X] Ordenar esta caja por Pokedex" << endl;
        cout << "[N] Agregar nuevo Pokemon" << endl;
        cout << "[L] Liberar Pokemon" << endl;
        cout << "[C] Crear nueva caja" << endl;
        cout << "[M] Mover Pokemon a otra caja" << endl;
        cout << "[B] Buscar Pokemon" << endl;
        cout << "[P] Consultar Pokedex" << endl;
        cout << "[R] Renombrar caja actual" << endl;
        cout << "[G] Guardar y cerrar sesion" << endl;
        cout << "Opcion: ";

        char opcion;
        cin >> opcion;
        opcion = toupper(opcion);

        switch (opcion) {
            case 'A':
                pc.caja_anterior();
                break;

            case 'D':
                pc.siguiente_caja();
                break;

            case 'X':
                pc.ordenar_caja();
                break;

            case 'N': {
                string nombre;
                cout << "Nombre del Pokemon: ";
                cin.ignore();
                getline(cin, nombre);

                Pokemon* info = pokedex.buscar_pokemon(nombre);
                if (info == NULL) {break;}

                string mote;
                cout << "Mote (escribe 'Ninguno' para usar el nombre): ";
                getline(cin, mote);
                if (upConvert(mote) == "NINGUNO" || mote.empty()) {
                    mote = info->getNombre();
                }

                int hp, lv;
                do {
                    cout << "HP: ";
                    cin >> hp;
                    if (hp <= 0) {
                        cout << "El HP no puede ser menor a 0." << endl;
                    }
                } while (hp <= 0);

                do {
                    cout << "Nivel: ";
                    cin >> lv;
                    if (lv <= 0 || lv > 100) {
                        cout << "El nivel debe estar entre 1 y 100." << endl;
                    }
                } while (lv <= 0 || lv > 100);

                PokeCapturado nuevo(info->getID(), info->getNombre(),
                    info->getTipo(1), info->getTipo(2), hp, mote, lv,
                    pc.getCajaActual());
                
                pc.agregar_pokemon(nuevo);
                break;
            }

            case 'L': {
                string nombre;
                cout << "¿Qué Pokemon deseas liberar? ";
                cin.ignore();
                getline(cin, nombre);

                pc.liberar_pokemon(nombre);
                break;
            }

            case 'C': {
                string nueva_caja;

                cout << "Nombre de la nueva Caja: ";
                cin.ignore();
                getline(cin, nueva_caja);

                pc.crear_caja(nueva_caja);
                break;
            }

            case 'M': {
                vector<string> cajas = pc.getCajas();

                if (cajas.size() < 2) {break;}

                string nombre; 
                int choice;

                cout << "Nombre del pokemon que deseas mover: ";
                cin.ignore();
                getline(cin, nombre);

                cout << "Cajas disponibles:" << endl;

                for(int i = 0; i < cajas.size(); i++) {
                    cout << "[" << i + 1 << "]" << cajas[i];
                    if(cajas[i] == pc.getCajaActual()) {cout << " (actual)";}
                    cout << endl;
                }
                cout << "Selecciona la nueva caja: ";
                cin >> choice;

                if (choice < 1 || choice > cajas.size()) {break;}

                pc.mover_poke_caja(nombre, cajas[choice - 1]);
                break;
            }

            case 'B': {
                string nombre;

                cout << "Nombre o mote del pokemon: ";
                cin.ignore();
                getline(cin, nombre);

                vector<PokeCapturado> encontrados = pc.buscar_pokemon(nombre);
                if (encontrados.empty()) {break;}
                
                int opcion = 0;

                cout << "Selecciona un Pokemón: ";
                cin >> opcion;

                if (opcion < 0 || opcion >= encontrados.size()) {break;}

                menu_info(encontrados[opcion]);
                break;
            }

            case 'P': {
                menu_pokemon(pokedex);
                break;
            }

            case 'R': {
                string nombre;

                cout << "Nuevo nombre para " << pc.getCajaActual() << ": ";
                cin.ignore();
                getline(cin, nombre);

                pc.renombrar_caja(nombre);
                break;
            }

            case 'G':
                pc.guardar_csv(archivo);
                cout << "Cambios guardados correctamente" << endl;
                close = true;
                break;
        }
    } 
    while (!close);
}

void menu_info(PokeCapturado& pokemon) {   

    
    pokemon.mostrar_info();

    string opcion;

    do {
        cout << "[S] Regresar a la caja" << endl;
        cout << "Opción: "; 
        cin >> opcion;

        if (upConvert(opcion) != "S") {
            cout << "Opción no válida" << endl;
        }
    } while (upConvert(opcion) != "S");
}

void menu_pokemon(Pokedex& pokedex) {
    bool regresar = false;

    do {
        cout << "\n==============================" << endl;
        cout << "           POKEDEX" << endl;
        cout << "==============================" << endl;
        cout << "[1] Buscar Pokemon" << endl;
        cout << "[2] Ver todos los Pokemon (A-Z)" << endl;
        cout << "[3] Regresar al PC" << endl;
        cout << "Opcion: ";

        int opcion;
        cin >> opcion;

        switch(opcion) {
            case 1: {
                string name;
                cout << "Nombre del Pokemon ";
                cin.ignore();
                getline(cin, name);

                Pokemon* poke = pokedex.buscar_pokemon(name);

                if (poke != 0) {
                    poke->mostrar_info();
                } else {
                    cout << "No se encontró a " << name << endl;
                }
                break;
            }
            case 2: {
                vector<Pokemon> lista = pokedex.visit();
                cout << "\n==============================================" << endl;
                cout << "       LISTA DE POKEDEX (ORDEN A-Z)" << endl;
                cout << "==============================================" << endl;

                for (int i = 0; i < lista.size(); i++) {
                    cout << "#" << lista[i].getID() << " " << lista[i].getNombre()
                        << " | Tipo: " << lista[i].getTipo() << endl; 
                }
                cout << "Total Pokemon: " << lista.size() << endl;
                break;
            }
            case 3: {
                regresar = true;
                break;
            }
        }
    } while (!regresar);
}