# Proyecto Integrador TC1031 "Gestor de Cajas Pokemon"

### Hecho por Marco Antonio Hernández Roque - A01709802

## Descripción
Este es un proyecto en C++ en el cual el usuario tiene su equipo Pokémon y puede almacenarlo dentro de un sistema de cajas similar al de los videojuegos de las consolas de Nintendo. El programa permite gestionar perfiles de jugadores, navegar entre cajas de almacenamiento, consultar una Pokédex completa, ordenar los Pokémon almacenados, realizar búsquedas por nombre o mote y guardar los cambios en archivos de texto.

## Problema que resuelve
Dentro de los juegos de Pokémon, un jugador puede capturar cientos de criaturas a lo largo de su partida. Cuando el inventario crece, resulta difícil localizar a un Pokémon específico, organizar las cajas por número de registro, consultar los tipos elementales de cada especie o transferir ejemplares de un contenedor a otro sin perder sus datos. Este sistema resuelve ese problema organizando el almacenamiento en estructuras de datos que permiten navegar entre cajas en tiempo constante, validar las especies contra una Pokédex oficial y mantener los datos persistentes entre sesiones.

## Decisiones sobre las tecnologías y estructuras implementadas
Para el desarrollo del proyecto se tomaron las siguientes decisiones técnicas:

1. **Lista Doblemente Ligada Circular para las cajas (`Nodo` en `PC`):** En los videojuegos de Pokémon, al avanzar más allá de la última caja el jugador regresa automáticamente a la primera, y al retroeder desde la primera pasa a la última. Se eligió una lista doblemente ligada circular porque modela este comportamiento de forma natural: cada nodo apunta a su caja `siguiente` y `anterior`, logrando que el cambio de caja (`[A]` y `[D]`) ocurra en tiempo constante $O(1)$ sin importar cuántas cajas existan.
2. **Árbol Binario de Búsqueda (BST) para la Pokédex (`Pokedex` y `NodoPokedex`):** Al agregar un nuevo Pokémon al PC, pedirle al usuario que escriba manualmente el ID y los tipos elementales generaba errores de captura e inconsistencias. Se implementó un BST que carga las 1,025 especies desde `pokedex_completa.txt` ordenadas alfabéticamente por nombre. Esto permite buscar cualquier especie en O(log n) en promedio para autocompletar su ID y tipos oficiales, además de permitir listar toda la Pokédex en orden alfabético (A-Z) mediante un recorrido `inorder`.
3. **Vector dinámico (`vector<PokeCapturado>`) para el inventario:** Se utilizó un `vector` para almacenar los Pokémon capturados del jugador porque facilita el acceso indexado en O(1) al dibujar la cuadrícula de 6x5 y permite aplicar el algoritmo de ordenamiento `Merge Sort` sobre arreglos pegados.
4. **Persistencia con archivos CSV/TXT y archivos temporales:** Se eligió el formato separado por comas en texto plano porque es ligero y fácil de inspeccionar en cualquier editor. Para la escritura se implementó un esquema de archivo temporal (`.temp`) seguido de `remove()` y `rename()`, evitando que el archivo original se corrompa si el programa se interrumpe a mitad del guardado. Para la generación de estos archivos se utilizó la ayuda de IA generativa para poder realizar los documentos en el formato requerido de una manera más simple y uniforme. Los archivos generados con IA son `Ash.txt`, `Red.txt` y `pokedex_completa.txt`. 

## Estructuras de datos utilizadas y análisis de complejidad

### 1. Lista Doblemente Ligada Circular (`Nodo` y `PC`)
Cada caja de almacenamiento está representada por un objeto de la clase `Nodo` con su nombre y dos punteros (`siguiente` y `anterior`). La clase `PC` mantiene los punteros `primera_caja` y `caja_actual`.
* **Navegar a caja siguiente / anterior (`siguiente_caja`, `caja_anterior`):**
  * Nivel de complejidad: O(1), ya que solo mueve el puntero `caja_actual` al nodo de a lado.
* **Buscar una caja por nombre (`buscar_caja`):**
  * Mejor caso: O(1) si la caja buscada es `primera_caja`.
  * Caso promedio y peor caso: O(n), ya que recorre los enlaces de la lista circular hasta dar una vuelta completa.
* **Insertar una caja (`agregar_cajas`, `crear_caja`):**
  * Nivel de complejidad: O(n). Debido a que primero se ejecuta `buscar_caja` para verificar que no exista una caja con el mismo nombre.
* **Liberar memoria de cajas (`limpiar_cajas`):**
  * Nivel de complejidad: O(n), recorriendo y aplicando `delete` a cada nodo de la lista.

### 2. Árbol Binario de Búsqueda (`Pokedex` y `NodoPokedex`)
Almacena el catálogo de los Pokémon. Cada `NodoPokedex` contiene un objeto `Pokemon` y los punteros `left` y `right`. Sea x el número total de Pokemon en la Pokédex (x = 1025):
* **Insertar Pokemon (`insertar`):**
  * Nivel de complejidad: O(log n) en un árbol balanceado.
* **Buscar Pokemon (`buscar_nodo`, `buscar_pokemon`):**
  * Mejor caso: O(1) si la especie buscada se encuentra en la raíz del árbol.
  * Casos promedio y peor: O(log n) descartando la mitad del subárbol en cada comparación.
* **Recorrido alfabético (`inorder`, `visit`):**
  * Nivel de complejidad: O(n), ya que visita cada nodo del árbol exactamente una vez siguiendo el orden izquierda, raíz, derecha.
* **Liberar memoria del árbol (`limpiar`, `~Pokedex`):**
  * Nivel de complejidad: O(n), eliminando todos los nodos mediante un recorrido post-order recursivo.

### 3. Arreglo dinámico (`vector<PokeCapturado>` y `vector<User>`)
Se utiliza para gestionar el inventario de Pokémon capturados del jugador y la lista de usuarios registrados.
* **Agregar Pokémon al inventario (`agregar_pokemon`):**
  * Nivel de complejidad: O(n), debido a que cuenta cuántos Pokémon hay en la caja destino con `poke_por_caja`, verifica la caja e inserta al final del vector con `push_back`.
* **Buscar, mover, renombrar caja o liberar Pokémon (`buscar_pokemon`, `mover_poke_caja`, `renombrar_caja`, `liberar_pokemon`):**
  * Nivel de complejidad: O(n), porque realizan recorridos lineales sobre los n Pokémon del inventario y en el caso de `erase`, se desplazan los elementos restantes del vector.

## Algoritmo de ordenamiento
Se implementó el algoritmo **Merge Sort** dentro de la clase `PC` para ordenar los Pokémon de la caja actual de forma ascendente según su número de la Pokédex (`ID`).

Se eligió Merge Sort porque es un algoritmo estable que garantiza un rendimiento predecible sin degradarse a tiempo cuadrático aunque los Pokémon estén en orden inverso o desordenados.

La complejidad de Merge Sort para x elementos en la caja actual y n Pokémon en el inventario total es:
* **Nivel de complejidad:** O(n log n) en `ordenar_caja` al filtrar primero el inventario.
  
## Lectura y escritura de archivos

### Lectura de datos (`ifstream`)
1. **Usuarios (`User::cargar_usuarios`):** Abre `Usuarios.txt` con `ifstream`, lee línea por línea cada nombre de jugador y verifica que su archivo individual (por ejemplo, `Ash.txt`) exista y comience con el encabezado válido `CAJAS:`. 
2. **Inventario y Cajas (`PC::cargar_csv`):** Lee el archivo del jugador seleccionado. De la primera línea (`CAJAS:...`) extrae los nombres separados por comas usando `stringstream` y construye la lista doblemente ligada circular. De las líneas siguientes separa los 8 atributos (`ID,NOMBRE,TIPO1,TIPO2,HP,MOTE,NIVEL,CAJA`), instancia cada `PokeCapturado` y lo agrega al inventario respetando el límite de 30 Pokémon por caja. 
3. **Pokédex (`Pokedex::cargar_pokedex`):** Abre `pokedex_completa.txt`, valida que la cabecera sea `ID,NOMBRE,TIPO1,TIPO2` y lee los Pokemon para insertarlos en el BST.

### Escritura de datos (`ofstream`)
1. **Guardado de partida (`PC::guardar_csv`):** Cuando el usuario selecciona guardar y cerrar sesión (`[G]`), el programa crea un archivo temporal (`<Jugador>.txt.temp`) mediante `ofstream`. Primero recorre la lista circular desde `primera_caja` para escribir la línea `CAJAS:Caja 1,Caja 2,...`. Después recorre el vector `inventario` escribiendo los atributos de cada Pokémon separados por comas. Finalmente, elimina el archivo anterior con `remove()` y renombra el archivo temporal al nombre original con `rename()`. 
2. **Creación y registro de nuevos jugadores (`PC::crear_jugador` y `User::registrar_usuario`):** `crear_jugador` inicializa las cajas por defecto (`Caja 1` y `Caja 2`) y genera el archivo `.txt` del jugador en O(1). Por su parte, `registrar_usuario` copia los nombres actuales de `Usuarios.txt` a `Usuarios_temp.txt`, agrega al nuevo jugador si no existía y reemplaza el archivo con `remove()` y `rename()`.

## Nota para Windows

Si se está compilando en Windows, favor de quitar los comentarios de estas líneas en `main.cpp` para que la consola pueda mostrar los acentos en el programa:
```cpp
#include <Windows.h>
SetConsoleOutputCP(CP_UTF8);
SetConsoleCP(CP_UTF8);
setlocale(LC_NUMERIC, "C");
```

## Compilar

``` bash
g++ main.cpp PC.cpp Pokemon.cpp User.cpp -o main
```
Después ingresar este texto en terminal para poder ejecutar el programa y disfrutar.
``` bash
./main
```

## Referencias
- Educative. (2023). What is the iomanip library in C++? Educative Answers. https://www.educative.io/answers/iomanip-library-in-cpp
- GeeksforGeeks. (2023). File handling through C++ classes. GeeksforGeeks. https://www.geeksforgeeks.org/cpp/file-handling-c-classes/
- GeeksforGeeks. (2023). How to find index of a given element in a Vector in C++. GeeksforGeeks. https://www.geeksforgeeks.org/cpp/how-to-find-index-of-a-given-element-in-a-vector-in-cpp/
- GeeksforGeeks. (2023). How to split a string in C/C++, Python and Java? GeeksforGeeks. https://www.geeksforgeeks.org/cpp/how-to-split-string-by-delimiter-in-cpp/
- GeeksforGeeks. (2023). std::string::rfind in C++ with Examples. GeeksforGeeks. https://www.geeksforgeeks.org/cpp/stdstringrfind-in-c-with-examples/
- Stack Overflow. (2013, 11 de enero). Parse (split) a string in C++ using string delimiter (standard C++) [Publicación en foro]. Stack Overflow. https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
- Stack Overflow. (2015, 3 de octubre). Method that converts string to upper-case [Publicación en foro]. Stack Overflow. https://stackoverflow.com/questions/32929977/method-that-converts-string-to-upper-case
