# Phong  specular

## Specular

Proszę dodać teraz człon specular do szadera fragmentów. Ponadto co było potrzebne do obliczenia dyfuzji potrzebny
będzie jeszcze "view vector" czyli znormalizowany wektor w kierunku od położenia fragmentu do obserwatora. W tym celu
potrzebujemy położenie obserwatora, czyli kamery. W przestrzeni widoku jest to po prostu punkt (0,0,0). Po obliczeniu
wektora "view" liczymy pół-wektor i następnie jego iloczyn skalarny z wektorem normalnym. Jeśli ten jest większy od zera
podnosimy go do potęgi `Ns`. Jest to wielkość, która charakteryzuje materiał i na razie nie mamy jej dostępnej w
szaderze. Proszę ustawić ją ręcznie w szaderze na 500.0.

```glsl
float Ns = 500.0; 
```

Podobnie jest z kolorem odbicia `Ks`. Zwykle jest to inny kolor niż kolor diffuse `Kd` ponieważ odpowiada za niego
zupełnie inny process fizyczny. I znów nie mamy tego w materiale, więc zaczniemy od ustawienia go w szaderze na kolor
biały.

```glsl
vec3 Ks=vec3(1.0, 1.0, 1.0); 
```

Do obliczenia całkowitego światła używamy wzoru

<img src="phong.png"/> 

## Phong material

Jeśli odblaski nam działają, to teraz rozszerzymy `PhongMaterial`, tak aby uwzględnić dodatkowe cechy materiału, takie
jak kolor ambient `Ka`, kolor odblasku `Ks` i siłę odblasku `Ns`.

Zaczniemy od prostej zmiany nazw. Jak na razie
w `PhongMaterial` zdefiniowana jest tylko zmienna `color_` i odpowiadająca mu tekstura. Nazwy tych zmiennych proszę
zmienić
odpowiednio na `Kd` i `map_Kd` i przenieść te definicje do części `public` klasy. Odpowiednio poprawiamy istniejące już
setery i getery, nie zmieniając ich nazwy. To
jest potrzebne, aby poprzednie ćwiczenia nie przestały działać. Musimy też poprawić konstruktor(y) tej klasy oraz metodę

```c++
PhongMaterial::bind()
```

Nazwę zmiennej przechowującej texture
unit zmieniamy na `map_Kd_unit` i dodajemy jej deklarację `static const` i też przenosimy do części publicznej

```c++
static const GLuint map_Kd_unit = 0;
```

Robimy tak, ponieważ chcemy, aby każdy obiekt `PhongMaterial`  używał tego same texture unit dla tekstur diffuse. Znów
odpowiednio zmieniamy getery i setery oraz konstruktor(y). W szczególności
seter `void PhongMaterial::set_texture_unit(GLuint textureUnit)` powinien mieć pustą definicję, ponieważ
zmiennej `const` nie możemy przypisać żadnej wartości.

Teraz dodajmy pola `Ka`, i `Ks` typu `glm::vec4` oraz pole `Ns` typu `float` również w części public
klasy `PhongMaterial`. W szaderze
wierzchołków rozszerzamy interface block odpowiedzialny za przesyłanie koloru, tak aby uwzględnił te dodatkowe zmienne.
Dobrze jest też zmienić jego nazwę na `Material`. Następnie w metodzie `init` klasy `PhongMaterial` alokujemy dla tego
bufora odpowiednią ilość pamięci.

Zmieniamy teraz odpowiednio metodę `bind` aby przesyłała wszystko do bufora. Jeśli ustawimy zmienne w dobrej kolejności
to
możemy to zrobić jednym poleceniem `glBufferSubData`.

W pliku `mesh_loader.cpp` zmieniamy funkcję `make_phong_material`, tak aby nadać wartości tym nowym polom. Na początek
nadamy im wartości ręcznie. Pole `Ka` ustawiamy na `glm::vec4{0.23125 0.23125 0.23125}`, pole `Ks` na `glm::vec4{0.773911, 0.773911,
0.773911, 1.0}`, a `Ns` na
`200.0`. W szaderze fragmentów zmieniamy nasz kod, tak aby korzystał z tych nowych wartości. Jeśli wszystko pójdzie
dobrze, to powinniśmy zobaczyć srebrne odblaski na naszym kwadracie.

Na koniec należy zmodyfikować funkcję `make_phong_material`, tak aby nowe wartości były wczytywane z pliku `*.mtl`
wzorując się na tym, jak wczytywany jest kolor diffuse. Należy tylko wiedzieć, że
wartością `Ka`, `Ks` i `Ns` odpowiadają pola `ambient`, `specular` i `shininess` obiektu `mat`. Należy oczywiście dodać
odpowiednie deklaracje do pliku `square.mtl`.


