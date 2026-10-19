#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

using namespace std;

class User {
private:
    string nombre, archivo;
    // Complejidad: O(n)
    string crear_archivos(const string&);

public:
    User(const string& nombre = "");

    string getUser();
    string getArchivo();

    // Complejidad: O(n)
    vector<User> cargar_usuarios();
    // Complejidad: O(n)
    void registrar_usuario();
};

#endif