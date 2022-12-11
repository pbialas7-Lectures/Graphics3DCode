# Materials

Nastepnym krokiem jest dodanie materiałów. W tym celu w katalogu `src/Engine` znajdują się trzy pliki `Material.h`, `ColorMaterial.h` i 
`ColorMaterial.cpp` w których zaimplementowana jest bazowa klasa `Material` i klasa `ColorMaterial`. Dodane zostały również
odpowiednie kody szaderów oraz pliki `utils.h` i `utils.cpp`.

Klasa `Material` jest klasą bazową dla wszystkich materiałów. Posiada abstrakcyjną wirtualną metodę `bind()` którą
należy wywołać przed rysowaniem podsiatki która ma być pokryta danym materiałem. Ta metoda musi być zaimplementowana w
podklasach klasy `Material`. Celem metody `bind()` jest załadowanie odpowiedniego dla danego materiału programu, a
następnie przesłanie do niego koniecznych danych.

Klasa `ColorMaterial` implementuje chyba najprostszy możliwy materiał polegający na kolorowaniu podsiatki jednym
kolorem. Kody szaderów są praktycznie identyczne z tymi które Państwo napisali na potrzeby zadania `Uniforms`. Różnica
jest taka, że zamiast modyfikować kolor, dane z bufora uniform są wykorzystywane do przypisania koloru danemu pikselowi.
Metoda `init()` z klasy `ColorMaterial` kompiluje te shadery i tworzy program, który jest zapisany w zmiennej `shader_`.
Zarówno metoda `init` jak i zmienna `shader_` są statyczne. Oznacza to, że są związane z klasą, a nie obiektem. Jest tak
ponieważ chcemy, aby wszystkie obiekty typu `ColorMaterial` miały ten sam program. Podobnie chcemy, aby wszystkie
obiekty tej klasy dzieliły ten sam bufor uniform służący do przekazywania koloru do szadera. Ten bufor jest również
tworzony w metodzie `init()`. Ta metoda musi być więc wywołana raz(!) zanim użyjemy obiektów `ColorMaterial` po raz
pierwszy. Ponieważ metoda ta nie jest związana z żadnym obiektem wywołujemy ją poleceniem

```c++
xe::ColorMaterial::init();
```

które najlepiej dodać na początku metody  `init()` klasy `SimpleShapeApplication`.

Klasa `ColorMaterial` posiada (niestatyczne) pole

```c++
glm::vec4 color_; 
```

w którym każdy obiekt tej klasy będzie przechowywał kolor danego materiału. Pole to ustawiane jest w konstruktorze
klasy.

Metoda `bind()` zaimplementowana w tej klasie ładuje program, podpina bufor uniform i przesyła do niego kolor.

Żeby zacząć korzystać z materiałów należy trochę zmodyfikować klasę `Mesh`. Po pierwsze będzie potrzebna tablica

```c++
std::vector<Material*> materials_;
```

przechowująca wskaźniki `Material*` odpowiadające kolejnym podsiatkom w tablicy `submeshes_`. Po dodaniu tablicy należy
zmodyfikować odpowiednio metodę `add_submesh` tak, żeby móc podać do niej wskaźnik na materiał dla danej podsiatki. W
przypadku nie podania materiału powinien być ustawiany wskaźnik `nullptr`.

```c++
void add_submesh(Gluint start, GLuint end, Material* mat) } {...}
void add_submesh(Gluint start, GLuint end) } {add_submesh(start, end, nullptr);}
```

Teraz w metodzie `draw` klasy `Mesh` należy przed wydaniem polecenia `glDrawElements` wykonać polecenie `bind` na wskaźniku do
materiału danej podsiatki. Oczywiście należy najpierw sprawdzić czy dany wskaźnik nie jest równy `nullptr`. Tak samo po
wydaniu tego polecenia należy wywołać metodę `unbind`.

Teraz proszę wykorzystać materiały do pokolorowania piramidy w ten sam sposób, w jaki już była pokolorowana. W tym celu
należy podzielić piramidkę na pięć podsiatek i do każdej dodać odpowiedni materiał. Jak to się uda, to proszę usunąć
informację o kolorze z bufora wierzchołków.   