#include "PC.h"
#include "User.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void menu(PC&, const string&);

int main() {
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
                    cout << "[" << i + 1 << "] " << users[i].getUser() << endl;
                }
                int choice;
                cout << "Selecciona el jugador: ";
                cin >> choice;

                if(choice < 1 || choice > users.size()) {break;}

                User usuario = users[choice - 1];
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
        cout << "[B] Buscar Pokemon en esta caja" << endl;
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
                int id, hp, lv;
                string nomb, tipo1, tipo2, mote;

                cout << "Numero de Pokedex: ";
                cin >> id;

                cout << "Nombre del Pokemon: ";
                cin >> nomb;

                cout << "Tipo principal: ";
                cin >> tipo1;

                cout << "Tipo secundario " << "(Ninguno si no tiene): ";
                cin >> tipo2;

                if (upConvert(tipo2) == "NINGUNO") {tipo2 = "";}

                cout << "HP: ";
                cin >> hp;

                cout << "Mote " << "(Ninguno para usar la especie): ";
                cin >> mote;

                if (upConvert(mote) == "NINGUNO") {mote = "";}

                cout << "Nivel: ";
                cin >> lv;

                PokeCapturado nuevo(id, nomb, tipo1, tipo2, hp, mote, lv,
                pc.getCajaActual());

                pc.agregar_pokemon(nuevo);
                break;
            }

            case 'L': {
                string nombre;
                cout << "¿Qué Pokemon deseas liberar? ";
                cin >> nombre;

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
                cin >> nombre;

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
                cin >> nombre;

                pc.seleccionar_nombre(nombre);
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