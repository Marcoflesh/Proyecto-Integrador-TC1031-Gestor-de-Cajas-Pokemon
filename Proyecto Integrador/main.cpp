/*
 * Proyecto Gestor de Cajas Pokémon — Main
 * Marco Antonio Hernández Roque
 * A01709802
 * 19/11/2026
 * Materia: TC1031 Programación de Estructuras de Datos y Algoritmos 
    Fundamentales
 * Este es el archivo principal que ejecuta todo el proyecto.
 * Se encarga de iniciar el Sistema PC, cargar o crear perfiles de 
    jugadores y mostrar los menús en consola para administrar cajas, 
    buscar y liberar Pokémon, consultar la Pokédex paginada y guardar 
    los cambios en disco.
 */
#include "PC.h"
#include "User.h"

#include <iostream>
#include <string>
#include <vector>
//#include <Windows.h>

using namespace std;

// Despliega el menú principal del PC del jugador activo
void menu(PC&, const string&);
// Muestra la info de un Pokémon encontrado y permite saltar a su caja
void menu_info(PokeCapturado&, PC&);
// Despliega el submenú de consulta y navegación paginada de la Pokédex
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

/**
 * menu controla el ciclo interactivo principal del PC de un jugador.
 *
 * Carga el BST de la Pokédex, dibuja la caja actual y muestra el menú 
    de acciones en formato 3x4 para navegar cajas ([A]/[D]/[J]), 
    ordenar con Merge Sort ([X]), agregar ([N]), liberar ([L]), 
    crear/renombrar cajas ([C]/[R]), mover ([M]), buscar ([B]), 
    consultar Pokédex ([P]) y guardar cambios ([G]).
 *
 * @param PC& pc instancia del PC del jugador, const string& archivo 
    ruta de guardado
 * @return
 */
void menu(PC& pc, const string& archivo) {
    Pokedex pokedex;
    bool close = false;

    do {
        pc.mostrar_caja();

        cout << "\n  [A] Caja anterior        [D] Siguiente caja        [J] Ir a caja" << endl;
        cout << "  [B] Buscar Pokemon       [N] Agregar Pokemon       [M] Mover Pokemon" << endl;
        cout << "  [L] Liberar Pokemon      [X] Ordenar caja (ID)     [C] Crear nueva caja" << endl;
        cout << "  [R] Renombrar caja       [P] Consultar Pokedex     [G] Guardar y salir" << endl;
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

            case 'J': {
                pc.mostrar_cajas();
                string destino;
                cout << "\nNombre de la caja a que deseas ir: ";
                cin.ignore();
                getline(cin, destino);

                if (destino.empty()) break;

                if (upConvert(destino) == upConvert(pc.getCajaActual())) {
                    cout << "Estás en la caja \"" << pc.getCajaActual() << endl;
                } else if (!pc.cambiar_caja(destino)) {
                    cout << "No existe la caja \"" << destino << "\"" << endl;
                }
                break;
            }

            case 'X':
                pc.ordenar_caja();
                break;

            case 'N': {
                if (!pokedex.loaded()) {
                    cout << "La Pokedex no se cargó correctamente" << endl;
                    break;
                }

                string nombre;
                cout << "Nombre del Pokemon: ";
                cin.ignore();
                getline(cin, nombre);

                Pokemon* info = pokedex.buscar_pokemon(nombre);
                if (info == 0) {
                    cout << "No se econtró \"" << nombre << endl;
                    break;
                }

                cout << "Encontrado en Pokedex -> #" << info->getID() << " "
                    << info->getNombre() << " | Tipo: " << info->getTipo() << endl;

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

                cout << "\nSelecciona un Pokemón: ";
                cin >> opcion;

                if (opcion < 0 || opcion >= encontrados.size()) {break;}

                menu_info(encontrados[opcion], pc);
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

/**
 * menu_info muestra la ficha completa de un Pokémon seleccionado en 
    búsqueda.
 *
 * Imprime los datos del Pokémon con mostrar_info(). Si el Pokémon se 
    encuentra en una caja distinta a la actual, habilita la opción [I] 
    para saltar directamente a esa caja; en cualquier caso permite 
    regresar con [S].
 *
 * @param PokeCapturado& pokemon ejemplar seleccionado, PC& pc instancia 
    del PC
 * @return
 */
void menu_info(PokeCapturado& pokemon, PC& pc) {   

    pokemon.mostrar_info();

    bool otra_caja = (upConvert(pokemon.getCaja()) != upConvert(pc.getCajaActual()));

    string opcion;

    do {
        if (otra_caja) {
            cout << "[I] Ir a la caja \"" << pokemon.getCaja() << "\"" << endl;
        }
        cout << "[S] Regresar a la caja" << endl;
        cout << "Opción: ";
        cin >> opcion;

        if (otra_caja && upConvert(opcion) == "I") {
            pc.cambiar_caja(pokemon.getCaja());
            return;
        } else if (upConvert(opcion) != "S") {
            cout << "Opción no válida" << endl;
        }
    } while (upConvert(opcion) != "S");
}

/**
 * menu_pokemon gestiona el submenú de consulta de la Pokédex oficial.
 *
 * Permite buscar una especie individual en el BST ([1]), recorrer todo 
    el catálogo en orden alfabético A-Z paginado de 100 en 100 mediante 
    la lista circular PaginaPokedex ([2]), o regresar al PC principal 
    ([3]).
 *
 * @param Pokedex& pokedex árbol binario con el catálogo de especies
 * @return
 */
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

                if (lista.empty()) {
                    cout << "No hay datos cargados" << endl;
                    break;
                }

                PaginaPokedex pagina(lista,100);
                char nav = ' ';

                do {
                pagina.mostrar_pagina();
                cout << "\n[A] Página anterior      [D] Siguiente página" << endl;
                cout << "[S] Regresar" << endl;
                cout << "Opcion: ";
                cin >>  nav;
                nav = toupper(nav);

                if (nav == 'A') {pagina.previous_p();} 
                else if (nav == 'D') {pagina.next_p();}

                } while (nav != 'S');
                break;
            }
            case 3: {
                regresar = true;
                break;
            }
        }
    } while (!regresar);
}