# Zoom 

W tym ćwiczeniu będziemy dalej kontynuować dodawanie elementów interakcji do naszej aplikacji poprzez dodanie  możliwości "zoomowania" za pomocą kółka przewijania myszy.


Zanim do tego przejdziemy dokonamy _refaktoryzacji_ kodu poprzez wydzielenie obsługi kamery do osobnej klasy. W tym celu tworzymy nowy plik nagłówkowy `camera.h` z zawartością:
```
#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

    void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
        V_ = glm::lookAt(eye, center, up);
    }

    void perspective(float fov, float aspect, float near, float far) {
        fov_ = fov;
        aspect_ = aspect;
        near_ = near;
        far_ = far;
    }

    void set_aspect(float aspect) {
        aspect_ = aspect;
    }

    glm::mat4 view() const { return V_; }

    glm::mat4 projection() const { return glm::perspective(fov_, aspect_, near_, far_); }
    
private:
    float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 V_;
};
```

W pliku `app.h` w klasie `SimpleShapeAplication` dodajemy pole:
```
Camera *camera_;
```
i metody
```
void set_camera(Camera *camera) { camera_ = camera; }
Camera *camera() const { return camera_; }
```

W metodzie `init`  inicjujemy  wskaźnik do kamery:
```
set_camera(new Camera);
```
Następnie modyfikujemy nasz kod tak, aby korzystał z klasy `Camera` i usuwamy niepotrzebne pola
```
float fov_;
float aspect_;
float near_;
float far_;

glm::mat4 P_;
glm::mat4 V_;
```
z klasy `SimpleShapeApplication`.


Zoomowanie będzie polegać na zmianie kąta widzenia:  zmniejszenie kąta to "zoom-in", zwiększenie to "zoom-out". Należy jednak pamiętać, że  pole widzenia ograniczone jest do przedziału (0, pi). W celu płynnego przechodzenia od jednego kónca zakresu do drugiego wykorzystamy funkcję logistyczną:
```
float logistic(float y) {
    return 1.0f/(1.0f+std::exp(-y));
}
```
i jej odwrotność:
```
float inverse_logistics(float x) {
    return std::log(x/(1.0f-x)); 
}
```

Niech `y_offset` będzie wartością otrzymaną poprzez obrót kółkiem myszy. Kąt widzenia `fov` modyfikujemy następująco:

Najpierw obliczamy
```
auto x = fov/glm::pi<float>();
```
jest to liczba z zakresu [0,1], a następnie liczymy
```
auto y = inverse_logistic(x);
```
`y` jest już liczbą z zakresu (-Inf, Inf). Do tej liczby dodajemy nasz `y_offset`
```
y+=y_offset
```
i przekształcamy z powrotem do przedziału (0,1) za pomocą funkcji logistycznej:
```
x = logistic(y); 
```
i w kóncu do przedziału (0, pi):
```
fov = x*glm::pi<float>(); 
```

Korzystając z powyższych informacji proszę zaimplementować metodę `zoom` w klasie `Camera` która odpowiednio zmienia `fov`.
```
void zoom(float y_offset) {
    /*
      Implementacja  ...
    */
}
```

Na koniec nadpisujemy implementację metody obsługującej kółko myszy
```
void scroll_callback(double xoffset, double yoffset) override {
         Application::scroll_callback(xoffset, yoffset);   
         camera()->zoom(yoffset / 30.0f);
     }
```
w klasie `SimpleShapeApplication`. Stała przeskalowywująca `30.0f` została dobrana doświadczalnie.

   