#include "User.h"

#include <fstream>

using namespace std;

User::User(const string& user) : nombre(user), archivo(crear_archivos(nombre)) {}

string User::getUser() {return nombre;}
string User::getArchivo() {return archivo;}

string User::crear_archivos(const string& name) {
    string file = name;

    if (file.empty()) {
        return "";
    }

    if(file[0] >= 'a' && file[0] <= 'z') {
        file[0] = file[0] - ('a' - 'A');
    }

    for (int i = 1; i < file.length(); i++) {
        if (file[i] >= 'A' && file[i] <= 'Z') {
            file[i] = file[i] + ('a' - 'A');
        }
    }
    return file + ".txt";
}

vector<User> User::cargar_usuarios() {
    vector<User> users;
    ifstream archivo("Usuarios.txt");
    string nombre;

    while(getline(archivo, nombre)) {
        if (!nombre.empty()) {
            User usuario(nombre);

            ifstream pc(usuario.getArchivo());
            string plinea;

            getline(pc, plinea);

            if (plinea.rfind("CAJAS:", 0) == 0) {
                users.push_back(usuario);
            }
            pc.close();
        }
    }
    archivo.close();
    return users;
}

void User::registrar_usuario() {
    if (nombre.empty()) {return;}

    ifstream entrada("Usuarios.txt");
    ofstream temporal("Usuarios_temp.txt");
    string name;

    bool exists = false;

    while(getline(entrada, name)) {
        if(!name.empty()) {
            User existe(name);

            if (existe.getArchivo() == archivo) {
                exists = true;
            }
            temporal << name << endl;
        }
    }
    if(!exists) {temporal << nombre << endl;}
    entrada.close();
    temporal.close();

    remove("Usuarios.txt");
    rename("Usuarios_temp.txt", "Usuarios.txt");
}