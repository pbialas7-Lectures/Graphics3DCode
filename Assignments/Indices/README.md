# Indices 
Termin oddania: 27.10.21 23:59 5pkt

To ćwiczenie ma za zadanie zaznajomienie Państwa z działaniem bufora indeksów. Jak Państwo pewnie zauważyli część wierchołków w buforze wierzchołków się powtarza. W przypadku dużych siatek każdy wierzchołek może powtarzać się kilka razy co prowadzi do marnotrawienia zasobów, a co za tym idzie wolniejszego działania aplikacji.  Żeby temu zaradzić można posłużyć się buforem indeksów. Idea polega na załadowaniu do bufora wierzchołków tylko niepowtarzających  się wierzchołków. Następnie w celu zdefiniowania trókątów  posługujemy  się _indeksami_ tych wierzchołków które przekazujemu właśnie w buforze indeksów.


1. Na początek nie będziemy usuwać wierzchołków, tylko dodamy bufor indeksów.  W tym celu należy zadeklarować nowy wektor `vector<std::GLushort>` który będzie zawierał indeksy. Ponieważ na razie nic nie zmieniamy w buforze wierzchołków wypełniamy go po kolei indeksami od `0` do `liczba wierzchołków -1`. 

1. Następnie  poleceniem `glGenBuffers` tworzymy nowy bufor indkesów w taki sam sposób jak tworzyliśmy bufor wierzchołków. Następnie bindujemy ten bufor poleceniem `glBindBuffer` podając za pierwszy argument `GL_ELEMENT_ARRAY_BUFFER`. Potem poleceniem `glBufferData` kopiujemy do niego dane z wektora indeksów i odbindowujemy bufor. 

1. Teraz   jeszcze raz bindujemy ten bufor, ale robimy to pomiędzy wywołaniami `glBindVertexArray(vao_)` i `glBindVertexArray(0)`. W ten sposób  zostanie to zapomiętane w Vertex Array Object (VAO) `vao_`. 

1. Teraz  w funkcji `frame` podmieniamy  funkcję rysującą na `glDrawElements` oczywiście zmieniając odpowiednio argumenty. 

1. Jeśli nasz domek się wyświetlił, to proszę usunąć powtarzające się wierzchołki z bufora wierzchołków i odpowiednie zmodyfikowanie bufora indeksów tak, aby na ekranie dalej pokazywał się __ten sam__ domek. 

1. Ile bajtów oszczędziliśmy ? Ile więcej bajtów moglibyśmy jeszcze oszczędzić ? Odpowiedź proszę zapisać w pliku README.md w folderze z zadaniem. 