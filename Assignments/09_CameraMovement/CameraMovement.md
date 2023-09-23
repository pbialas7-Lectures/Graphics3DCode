# Porusznie kamerą

W poprzednim ćwiczeniu dodaliśmy możliwość "zoomowania", czyli zmiany kąta widzenia kamery za pomocą pokrętła myszy . W
tym ćwiczeniu dodamy obracanie kamery wokół centralnego punktu:

1. Za punkt centralny będzie przyjęty punkt podany jako parametr `center` w funkcji  `look_at`.
1. Ruch myszą poziomo będzie obracał kamerę wokół osi równoległej do osi y kamery (kierunek up) i przechodzącej przez
   punkt centralny.
2. Ruch myszą pionowo będzie obracał kamerę wokół osi równoległej do osi x (kierunek right) i przechodzącej przez punkt
   centralny.

To będzię długie ćwiczenie, ale możliwość dowolnego oglądania naszych modeli daje dziwną satysfakcję :) Proponuj ę
pracować małymi kroczkami, za każdym razem sprawdzając czy kod jeszcze działa. I nie iść dalej póki wszystko nie będzie
OK.

## Kamera

Zaczniemy od wprowadzenie zmian w klasie `Camera`. Zamiast przechowywać macierz widoku `V_` będziemy przechowywali
położenie kamery `position_`, punkt centralny `center_` i trzy wektory definiujące orientację `x_`, `y_` i `z_`.

Proszę dodać te pola do klasy `Camera`.

Następnie kod metody `look_at` klasy `Camera` należy zastąpić kodem ustawiającym te zmienne tak jak to było opisane na
wykładzie:

```c++
z_ = glm::normalize(eye - center);
x_ = glm::normalize(glm::cross(up, z_));
y_ = glm::normalize(glm::cross(z_, x_));

position_ = eye;
center_ = center;
```

Potrzebujemy jeszcze metod odczytujacych te wartości:

```c++
glm::vec3 x() const { return x_; }
glm::vec3 y() const { return y_; }
glm::vec3 z() const { return z_; }
glm::vec3 position() const { return position_; }
glm::vec3 center() const { return center_; }
```

Cały czas nasz kod powinien się dać skompilować i działać poprawnie. Kolejnym krokiem jest zamiana kodu w metodzie
`view` tej samej klasy. Zamiast zwracać macierz `V_`, będziemy ją generować z nowych zmiennych "na żądanie", znów tak
jak to zostało opisane na wykładzie :)

```c++
glm::mat4 view() const {
    glm::mat4 V(1.0f);
    for (int i = 0; i < 3; ++i) {
        V[i][0] = x_[i];
        V[i][1] = y_[i];
        V[i][2] = z_[i];
    }
    
    auto t = -glm::vec3{
            glm::dot(x_, position_),
            glm::dot(y_, position_),
            glm::dot(z_, position_),
    };
    V[3] = glm::vec4(t, 1.0f);
    
    return V;
    
    }
```  

Jeśli wszytsko zostało wykonane poprawnie to program pownienien dalej działać bez zmian. Możemy już usunać pole `V_
` z klasy.

Teraz zaimplementujemy metody służące do obracania kamery. Obrót kamery wokół osi w punkcie centralnym polega na

1. Obrocie połozenia kamery wokół tej osi i punktu.
2. Obrotu wszystkich trzech wektorów definiujących orientację kamery wokół tej osi.

Zaczniemy od zdefiniowania pomocniczej funkcji tworzącej macierz obrotu o kat `angle` wokół osi `axis`:

```c++
inline glm::mat3 rotation(float angle, const glm::vec3 &axis) {
    auto u = glm::normalize(axis);
    auto s = std::sin(angle);
    auto c = std::cos(angle);

    return glm::mat3(
        c + u.x * u.x * (1.0f - c),
        u.y * u.x * (1.0f - c) + u.z * s,
        u.z * u.x * (1.0f - c) - u.y * s,

        u.x *u.y*(1.0f-c)-u.z *s,
        c + u.y*u.y *(1.0f-c),
        u.z*u.y*(1.0f-c)+u.x*s,

        u.x*u.z*(1.0f -c)+ u.y*s,
        u.y*u.z*(1.0f-c)-u.x*s,
        c+u.z*u.z*(1.0f -c)
    );
}     
```

Ja tą funkcję umieściłem w osobnym pliku nagłówkowym `rotation.h` (dlatego potrzebna jest deklaracja `inline`).

Teraz możemy zaimplementować już metodę obracającą kamerę w klasie `Camera`:

```c++
void rotate_around_point(float angle, const glm::vec3 &axis, const glm::vec3 &c) {
    auto R = rotation(angle, axis);
    x_ = R * x_;
    y_ = R * y_;
    z_ = R * z_;

    auto t = position_ - c;
    t = R * t;
    position_ = c + t;

}
```

i za jej pomocą:

```c++
void rotate_around_center(float angle, const glm::vec3 &axis) {
    rotate_around_point(angle, axis, center_); 
}
```

## Kontroler

Klasa `Camera` jest już gotowa, teraz musimy dodać obsługę myszy. W tym celu stworzymy nową klasę `CameraControler
`. W ten sposób odizolujemy sposób poruszania kamerą od samej kamery i jak przyjdzie potrzeba będziemy mogli podmienić
kontroler na inny np. na FPC (First Person Camera).

W nowym pliku nagłówkowym `camera_controler.h` tworzymy klasę

```c++
class CameraControler {
public:
    CameraControler():camera_(nullptr) {}
    CameraControler(Camera* camera):camera_(camera) {}
    void set_camera(Camera *camera) { camera_ = camera; }

private:
    Camera *camera_; 
}
```

Zakładając, że odczytaliśmy już zmianę położenia myszy `dx` i `dy` dokonany obrotu kamery za pomocą metody:

```c++
void rotate_camera(float dx, float dy) {
    camera_->rotate_around_center(-scale_ * dy, camera_->x());
    camera_->rotate_around_center(-scale_ * dx, Camera->y());
    }
```

`scale_` jest kolejnym polem klasy `CameraControler`  i jest przelicznikiem pomiędzy pikselami a kątem. Jego wartość
należy dobrać doświadczalnie dla danej rozdzielczości ekranu.

Powoli zbliżamy się do końca. Teraz zaimplementujemy trzy metody odpowiedzialne za obsługę wydarzeń myszy. Przede
wszystkim chcemy, aby obracanie działało tylko jak ruszamy myszą trzymając wciśniety lewy przycisk myszy (LMB
). Dlatego dodamy do klasy kolejne pole `LMB_pressed_` typu `bool`. W konstruktorze klasy powinnismy ustawić jego
wartość na `false`. Ponieważ będzemy śledzić zmiany położenia myszy potrzebujemy dodatkowych dwy pól `x_` i `y_` typu
`float` które będa przechowywały ostatnie położenie myszy. Mając te pola obsługa ruchu myszy będzie dokonywana przez
metodę:

```c++
void mouse_moved(float x, float y) {
    if (LMB_pressed_) {
        auto dx = x - x_;
        auto dy = y - y_;
        x_ = x;
        y_ = y;

        rotate_camera(dx, dy);
    }
};
```   

Dwie kolejne metody obsłużą zdarzenia naciśnięcia i puszczenia przycisku myszy:

```c++
void LMB_pressed(float x, float y) {
    LMB_pressed_ = true;
    x_ = x;
    y_ = y;
};

void LMB_released(float x, float y) {
    LMB_pressed_ = false;
    auto dx = x - x_;
    auto dy = y - y_;

    rotate_camera(dx, dy);
};
``` 

## Obsługa zdarzeń

Na koniec musimy podpiąć metody kontrolera pod obsługę zdarzeń. W tym celu w klasie `SimpleShapeApplication` dodajemy
pole

```c++
CameraControler *controler_;
```

i metodę

```c++
void set_controler(CameraControler *controler) { controler_ = controler; }
```

a w metodzie  `init` ustawiamy kontroler i kamerę:

```c++
set_controler(new CameraControler(camera()));
```

I wreszcie dodajemy obługę zdarzeń nadpisując metody:
`void mouse_button_callback(int button, int action, int mods)` i
`void cursor_position_callback(double x, double y)` metodami

```c++
void mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }

}

void cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}
```

Powodzenia :) 
 
