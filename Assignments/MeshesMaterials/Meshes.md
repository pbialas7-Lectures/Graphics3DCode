# Meshes

Proszę sciągnąć najnowszy kod z mojego repozytorium. W katalogu `src/Engine` powinny się pojawić pliki `Mesh.h`
i `Mesh.cpp` w których zdefiniowana jest klasa `Mesh`. W katalogu `Application` powiny się pojawić pliki `RegisteredObject.h` i `RegisteredObject.cpp` oraz zmienić się plik `CMakeLists.txt`

Jeśli
wszystko poszło dobrze to pliki w tej kartotece powinny zostać skompilowane do biblioteki `
Engine`. Bibliotekę można podłączyć w pliku `CMakeLists.txt` poleceniem `
target_link_libraries`. Jeśli stworzyli Państwo nowy katalog `
Meshes` i przekopiowali Państwo do niego zadanie `CameraMovement` to w pliku `
CMakeLists.txt` w tym katalogu należy dodać po poleceniu `add_executable` polecenie

```cmake
target_link_libraries(${PROJECT_NAME} PUBLIC Engine)
```

W g


Zanim jeszcze przejdziemy dalej, to umieścimy pliki zawierające obsługę kamery i kontrolera kamery też w kartotece `Engine`
. Tym sam nie bedziemy ich kopiować za każdym razem na nowo. Jeśli Państwo stworzyli kartotekę `Mesh` poprzez
przekopiowanie kartoteki `CameraMovement` to proszę przenieść pliki  `Camera.*`, `CameraControler.*` i `rotation.h` do
kartoteki `Engine`. Należy odpowiednio zmodyfikować plik `CMakeLists.txt` w tej kartotece.

Umieścimy też klasy `Camera` i `CameraControler` w pzestrzeni nazw `xe`. W tym celu należy umieścić deficje tych klas w
plikach `*.h` wewnątrz

```c++
namespace xe {
    
    
}
```

a następnie odpowiednio zmodyfikować odwołania do tych klas i ich metod.

## Klasa `Mesh`

Klasa `Mesh` odpowiada jednemu `Vertex Array Object` (VAO)  zawierającemu jeden bufor wierzchołów i jeden bufor
indeksów. Każdy obiekt tej klasy __musi__ posiadać jeden lub więcej obiektów klasy `SubMesh`. Klasa `SubMesh` odpowiada
jednemu poleceniu `gLDrawElements` i zawiera początek i koniec zakresu indeksów przekazywanych do tego polecenia.
Podsiatki dodajemy za pomocą metody  `add_submesh`.

Żeby skorzystać z instancji klasy `Mesh` musimy zapewnić dostęp do nich zarówno w metodzie `init`
klasy `SimpleShapeApplication` jak i w metodzie `frame`. W tym celu w tej klasie (`SimpleShapeApplication) proszę dodać
pole

```c++
std::vector<Mesh*> meshes_; 
```

i odpowiednią metodę dodającą siatki

```c++
void add_submesh(Mesh *mesh) {
    meshes_.push_back(mesh);
}
```

Następnie w metodzie `init` musimy stworzyć siatkę odpowiadającą piramidce. Najpierw tworzymy nową siatkę poleceniem

```c++
auto pyramid = new Mesh; 
```

potem wykorzystując metody tej klasy alokujemy pamieć dla bufora wierzchołków (`allocate_vertex_buffer`), załadować
dane (`load_vertices`) i ustalić sposób odczytu tych danych (`vertex_attrib_pointer`). I podobnie dla bufora indeksów.

Na koniec w metodzie `frame` musimy dodać wywołanie metody `draw` na każdej siatce z tablicy (wektora) `messhes_`

```c++
 for (auto m: meshes_)
m->draw();
```
