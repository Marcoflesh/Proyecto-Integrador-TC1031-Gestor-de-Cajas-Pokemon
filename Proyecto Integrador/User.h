#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

using namespace std;

class User {
private:
    string nombre, archivo;
    string crear_archivos(const string&);

public:
    User(const string& nombre = "");

    string getUser();
    string getArchivo();

    vector<User> cargar_usuarios();
    void registrar_usuario();
};

#endif