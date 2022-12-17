# Textures

W tym zadaniu rozszerzymy klasę `ColorMaterial`, tak aby kolor mógł być pobierany z tekstury. W tym celu musimy dodać
do niej dodatkowe pola:

```c++
static GLint uniform_map_Kd_location_;
GLuint texture_;
GLuint texture_unit_;
```

Pole statyczne `uniform_map_Kd_location_` będzie zawierać "adres" zmiennej uniform w shaderze fragmentów
odpowiedzialnej za teksturowanie. Jest to pole `static` bo chcemy, żeby było wspólne dla wszystkich obiektów tej klasy.
Pola typu `static` wymagają dodatkowej definicji. W pliku `ColorMaterial.cpp`należy więc dodać wewnątrz przestrzeni
nazw `xe`
linijkę:

```c++
GLint  ColorMaterial::uniform_map_Kd_location_ = 0;
```

To pole będzie inicjalizowane w funkcji `init` klasy `ColorMaterial`, za pomocą polecenia `glGetUniformLocation`.

Pola `texture_` i `texture_unit_` będą zawierać "uchwyt" do tekstury oraz numer jednostki teksturującej. Należy dodać
odpowiednie "getery" i "setery" czyli metody nadające im i zwracające ich wartość, a w istniejącym konstruktorze klasy
nadać obu
wartość zero.

Możemy też dodać konstruktor, który będzie te pola ustawiał.

W metodzie `bind` sprawdzamy, czy pole `texture_` ma wartość większą od zera. Jeśli tak, to poleceniem `glUniform1i`
przesyłamy pod adres wskazany polem `uniform_map_Kd_location_` wartość z pola `texture_unit_`. Następnie ustawiamy tą
jednostkę jako aktywną poleceniem

```c++
glActiveTexture(GL_TEXTURE0 + texture_unit_); 
```

Potem podpinamy teksturę z pola `texture_` poleceniem `glBindTexture`.

Ponieważ chcemy, aby klasa `ColorMaterial` obsługiwała zarówno tekstury, jak i stałe kolory, to musimy przesłać do
shadera informację o tym, czy używamy tekstury czy nie. W tym celu w metodzie `bind` deklarujemy zmienna `use_map_Kd`
typu `GLuint` i
nadajemy jej wartość zero, jeśli nie używamy tekstury i jeden, jeśli używamy. Potem musimy wysłać tą zmienną do bufora
uniform `Color`. Tak więc dodajemy kolejne pole do bloku uniform w shaderze fragmentów `Engine/shaders/color_fs.glsl`:

```glsl
layout(std140, binding=0) uniform Color {
    vec4  Kd;
    bool use_map_Kd;
};
```

W kodzie metody `bind` dodajemy odpowiednie polecenie przesyłające zmienną `use_map_Kd` do bufora uniform. Rozmiar tej
zmiennej to `sizeof(GLuint)`, a nie `sizeof(bool)` !
Proszę też pamiętać o zwiększeniu bufora tak, żeby ta zmienna się w nim zmieściła.

Teraz musimy dodać współrzędne tekstury do bufora wierzchołków. Po poprzednim ćwiczeniu powinni mieć państwo w nim już
tylko współrzędne 5 wierzchołków. Dodanie dodatkowego atrybutu, czyli współrzędnych tekstury będzie przebiegać tak samo
jak
w ćwiczeniu `Colors`, tyle że współrzędne tekstury mają tylko dwie składowe. Wartości tych współrzędnych można odczytać
z uv mapy w pliku `uv.png`. Widać, że sam wierzchołek piramidy musi zostać powtórzony czterokrotnie z różnymi
wartościami współrzędnych tekstury.
Koniec końców powinni mieć Państwo w buforze wierzchołków 8 wierzchołków.

Koniec końców powinniśmy mieć w szaderze fragmentów zmienną

```glsl
in vec2 vertex_texcoords; 
```

Zawierającą współrzędne tekstury dla danego piksela. Teraz będziemy mieli tylko jedną podsiatkę ponieważ tekstura
pokryje wszystkie ścianki.

Klasa `ColorMaterial` odpowiada za podpięcie tektstury, ale nie za jej stworzenie i wczytanie. To dokonamy w
metodzie `init` klasy `SimpleShapeApplication`. Najpierw proszę w głownym katalogu stworzyć pod katalog `Models` i
przekopiować do niego plik `multicolor.png`. Obrazek tekstury wczytamy za pomocą polecenia `stb_image`. W tym celu w
pliku `app.cpp` dodajemy

```c++
#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"
```

na samym końcu listy includów. Obrazek wczytujemy za pomocą poleceń

```c++
stbi_set_flip_vertically_on_load(true);
GLint width, height, channels;
auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
if (!img) {
    std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
} else {
    std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
}
```

Jeśli obrazek załaduje się, to w zmiennych `height`, `width` i `channels` zostaną zapisane odpowiednie wartości.
Zmienna `channels` będzie zawierać liczbę kanałów w obrazku. Teraz trzeba stworzyć teksturę poleceniem  `glGenTeksture`,
zbindować ją, załadować do niej obrazek poleceniem `glTexImage2D` i ustawić odpowiednie parametry. Jak to się uda, to
możemy tą teksturę przekazać do obiektu `ColorMaterial`.

I na konieć zmodyfikujemy szader fragmentów. Dodamy do niego zmienna

```glsl
uniform sampler2D map_Kd;
```

W funkcji `main` sprawdzamy czy zmienna `use_map_Kd` z bloku uniform `Color` ma wartość zero. Jeśli tak to
do `vFragColor` przypisujemy wartość `Kd`. Jeśli nie to przypisujemy
`Kd*texture(map_Kd, vertex_texcoords)`.

Musimy jeszcze tylko zainicjalizować pole  `uniform_map_Kd_location` klasy `ColorMaterial`. W tym celu należy dodać kod

```c++
uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
if (uniform_map_Kd_location_ == -1) {
    spdlog::warn("Cannot get uniform {} location", "map_Kd");
}
```

w metodzie `init` tej klasy. 





