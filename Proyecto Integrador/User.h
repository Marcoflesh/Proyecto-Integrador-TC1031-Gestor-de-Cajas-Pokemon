/*
 * Proyecto Gestor de Cajas Pokémon — Clase User
 * Marco Antonio Hernández Roque
 * A01709802
 * 19/11/2026
 * Materia: TC1031 Programación de Estructuras de Datos y Algoritmos 
    Fundamentales
 * En este archivo se define la clase User, encargada de gestionar los 
    perfiles de jugadores registrados en el sistema, generar el nombre 
    normalizado de su archivo de guardado y mantener actualizado el 
    catálogo en Usuarios.txt.
 */

#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

using namespace std;

/**
 * User representa un perfil de entrenador dentro del sistema PC.
 * Asocia el nombre del jugador con su archivo individual de guardado 
    (.txt) y ofrece métodos para leer los perfiles válidos y registrar 
    nuevos usuarios.
 */
class User {
private:
    string nombre, archivo;
    /**
     * crear_archivos normaliza el nombre del usuario para formar su 
        archivo .txt.
     *
     * Convierte la primera letra a mayúscula y el resto a minúsculas, 
        luego concatena la extensión ".txt".
     *
     * @param const string& nombre del jugador
     * @return string nombre del archivo generado, o cadena vacía si no
        hay nombre
     */
    // Complejidad: O(n)
    string crear_archivos(const string&);

public:
    User(const string& nombre = "");

    string getUser();
    string getArchivo();

    /**
     * cargar_usuarios lee Usuarios.txt y devuelve los jugadores válidos.
     *
     * Recorre cada línea de Usuarios.txt, comprueba que exista el 
        archivo individual del jugador y verifica que inicie con la 
        cabecera "CAJAS:".
     *
     * @param
     * @return vector<User> lista de perfiles válidos encontrados
     */
    // Complejidad: O(n)
    vector<User> cargar_usuarios();
    /**
     * registrar_usuario añade el perfil actual al archivo Usuarios.txt.
     *
     * Copia los usuarios existentes a un archivo temporal verificando 
        que no haya duplicados, agrega el nuevo nombre si no existía y 
        reemplaza el archivo original mediante remove() y rename().
     *
     * @param
     * @return
     */
    // Complejidad: O(n)
    void registrar_usuario();
};

#endif