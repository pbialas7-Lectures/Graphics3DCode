# Resize

W tym ćwiczeniu "naprawimy" niepoprawne zachowanie aplikacji w trakcie zmieniania rozmiaru okna. Obecnie zmiana rozmiaru okna powoduje zmianę  proporcji wyświetlanego obrazu. Dzieje się tak dlatego, że proporcje okna przestają się zgadzać z proporcjami użytymi w macierzy projekcji.  Żeby to naprawić musimy po każdej zmianie rozmiaru okna zmienić macierz projekcji tak aby do nich pasowała.

Skorzystamy w tym celu z  przygotowanej przeze mnie wirtualnej metody  `virtual void Application::framebuffer_resize_callback (int w, int h) {}` która jest wołana podczas każdej zmiany rozmiarów bufora ramki z parametrami `w` i `h` określającymi nowe rozmiary bufora.  Metoda ta może zostać nadpisana w klasach pochodnych klasy `Applicationa` takich jak `SimpleShapeApp` której Państwo używacie w swoich ćwiczeniach.

Ponieważ ta metoda  będzie musiała mieć dostęp do macierzy projekcji `P` oraz jej parametrów, będziemy musieli dokonać paru "przegrupowań" i przenieść  cześć zmiennych z metody `init` na poziom klasy.

Pierwszym krokiem  będzie stworzenie pól określających macierz projekcji  w klasie  `SimpleShapeApplication`.
W pliku `app.h` w klasie `SimpleShapeApp` w części `private` dodajemy  pola:
```
float fov_;
float aspect_;
float near_;
float far_; 

glm::mat4 P_;
glm::mat4 V_; 
glm::mat4 M_; 
```

Teraz w  metodzie `init` klasy `SimpleShapeApplication` musimy  przypisać wartości nowym polom np.:
```
auto[w, h] = frame_buffer_size();
aspect_ = (float)w/h;
fov_ = glm::pi<float>()/4.0;
near_ = 0.1f;
far_ = 100.0f;
P_ = glm::perspective(fov_, aspect_, near_, far_); 
V_ = glm::lookAt(...)

M_ =  glm::mat4(1.0f);
```
i zmieniamy kod  odpowiednio, tak żeby używał tylko pól `P_`, `V_` i `M_`.


Ponieważ teraz macierz projekcji `P_` może zmieniać się od klatki do klatki kod  obliczający i przesyłający macierz PVM do szadera poprzez bufor uniform musimy przenieść do metody `frame`. W tym celu zmienną zawierającą nazwę bufora uniform odpowiedzialnego za przesyłanie macierzy `PVM` do szadera musimy też uczynić polem klasy dodając w pliku `app.h` w części `private`  deklaracji klasy `SimpleShapeApplication`
```
GLuint u_pvm_buffer_; 
```   
W metodzie `init`  odpowiednio modyfikujemy kod, tak aby zawierał odniesienia do nowego pola.

W metodzie `init` pozostawiamy kod odpowiadający za inicjalizację bufora `u_pvm_buffer_`, ale kod odpowiadający za przesyłanie danych do bufora musimy przenieść do metody `frame`.  W  tym celu w metodzie `frame` dodajemy:
```
auto PVM = P_ * V_ *M_;
glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
glBindBuffer(GL_UNIFORM_BUFFER, 0);
```

Następnie w części `public` klasy `SimpleShapeApplication` dodajemy deklarację nadpisywanej funkcji:
```
void framebuffer_resize_callback(int w, int h) override; 
```
a w pliku `app.cpp` dodajemy odpowiadającą jej definicję:
```
void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h); 
    aspect_ = (float) w / h;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
}
```    
Teraz po zmianie rozmiaru okna proporcje powinny zostać zachowane. Proporcje nie musza być zachowane _podczas_ zmiany rozmiaru.     
   
  
  