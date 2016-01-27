#include <iostream>   // plik nagłówkowy zawierający standardowe wejście i wyjście
#include <vector>     // -||- udostępniający nam klasę vector<>
#include <algorithm>  // -||- funkcję sort()
#include <stack>      // -||- klasę stack<>
using namespace std;  // używamy przestrzeni nazw std::

// klasa reprezentująca krawędź grafu
class Edge {
public:
    string v1; // wierzchołek 1
    string v2; // wierzchołek 2
    double weight; // waga

    // przeciążamy operator <, aby móc sortować obiekty typu Edge za pomocą funkcji sort() oraz aby sortować je względem wag (pola weight)
    bool operator<(const Edge& e) const {
        return weight < e.weight;
    }

    // przeciążamy też resztę operatorów porównania
    // nie jest to konieczne w naszym przypadku,
    // ale warto to zrobić żeby uczynić kod bardziej użytecznym na przyszłość i ustrzec się dziwnych błędów

    bool operator>(const Edge& e) const {
        return weight > e.weight;
    }

    bool operator<=(const Edge& e) const {
        return weight <= e.weight;
    }

    bool operator>=(const Edge& e) const {
        return weight >= e.weight;
    }
};

// klasa reprezentująca graf
class Graph {
public:
    vector<string> vertices; // vector zawierający wierzchołki
    vector<Edge> edges; // vector zawierający krawędzie

    // dodawanie krawędzi e do grafu
    void addEdge(Edge e) {
        // poniższy fragment kodu dba o dodawanie wierzchołków, które nie zostały dodane wcześniej
        if (containsVertex(e.v1) == false) { // jeśli graf nie zawiera wierzchołka e.v1
            vertices.push_back(e.v1); // dodajemy e.v1 do vectora wierzchołków grafu
        }
        if (containsVertex(e.v2) == false) { // analogicznie j.w.
            vertices.push_back(e.v2);
        }

        // dodanie krawędzi e do vectora edges
        edges.push_back(e);
    }

    // usuwanie krawędzi e z grafu
    void removeEdge(Edge e) {
        // przeszukujemy cały vector krawędzi
        for (int i = 0; i < edges.size(); i++) {
            Edge graphEdge = edges[i]; // wyciągamy referencję do krawędzi, aby wygodniej się jej używało - zabieg czysto kosmetyczny

            // jeżeli w vectorze krawędzi grafu znajdujemy krawędź o takich samych wierzchołkach jak e
            if (e.v1 == graphEdge.v1 && e.v2 == graphEdge.v2) {
                edges.erase(edges.begin() + i); // usuwamy tę krawędź z vectora krawędzi grafu
                return; // wychodzimy z metody (usuwamy tylko jedną krawędź na raz)
            }
        }
    }

    // metoda zwraca vector, który zawiera wierzchołki sąsiadujące z wierzchołkiem v
    vector<string> getAdjacentVertices(string v) {
        vector<string> adjVertices; // inicjalizacja pustego wektora, który będzie zwrócony
        for (int i = 0; i < edges.size(); i++) { // iterujemy po wszystkich krawędziach wektora
            Edge e = edges[i];
            if (e.v1 == v ||  e.v2 == v) { // jeżeli jeden z wierzchołków badanej krawędzi jest wierzchołkiem v
                // wybieramy drugi wierzchołek (sąsiada v) i dodajemy go do vectora sąsiadów
                if (e.v1 == v) {
                    adjVertices.push_back(e.v2);
                } else {
                    adjVertices.push_back(e.v1);
                }
            }
        }
        return adjVertices; // zwracamy wektor zawierający sąsiadów v
    }

    // metoda sprawdza czy graf zawiera wierzchołek v
    bool containsVertex(string v) {
        for (int i = 0; i < vertices.size(); i++) { // sprawdzamy wszystkie wierzchołki w vectorze wierzchołków grafu
            if (vertices[i] == v) // jeśli znajdujemy wierzchołek v
                return true; // zwracamy true
        }
        return false; // po przeszukaniu całego vectora i nieznalezieniu wierzchołka v, zwracamy false
    }

    // metoda sprawdza (korzystając z algorytmu przeszukiwania wgłąb - dfs), czy graf zawiera co najmniej jeden cykl
    bool containsCycle() {
        vector<string> visitedVertices; // pomocniczy vector zawierający odwiedzone wierzchołki
        stack<string> tempStack; // pomocniczy stos wierzchołków; używany do implementacji dfs

        tempStack.push(vertices[0]); // wrzucamy pierwszy wierzchołek na stos

        while (tempStack.size() != 0) { // dopóki na stosie znajdują się jakieś wierzchołki
            string vertex = tempStack.top(); // odczytujemy wierzchołek
            tempStack.pop(); // wyrzucamy ww. wierzchołek ze stosu

            if (vertexWasVisited(vertex, visitedVertices) == false) { // jeśli wierzchołek vertex nie był wcześniej odwiedzony
                visitedVertices.push_back(vertex); // dodajemy go do vectora odwiedzonych wierzchołków

                vector<string> adjVertices = getAdjacentVertices(vertex); // znajdujemy sąsiadów wierzchołka vertex

                // wrzucamy wszystkich nieodwiedzonych wcześniej sąsiadów na pomocniczy stos tempStack
                for (int i = 0; i < adjVertices.size(); i++) {
                    string adjVertex = adjVertices[i];
                    if (vertexWasVisited(adjVertex, visitedVertices) == false) { // jeśli sąsiad nie był wcześniej odwiedzony
                        tempStack.push(adjVertex); // wrzucamy go na stos
                    }
                }
            } else { // jeśli wierzchołek vertex został wcześniej odwiedzony
                return true; // graf zawiera cykl
            }
        }

        return false; // po sprawdzeniu wszystkich wierzchołków, graf nie zawiera cyklu
    }

private:
    // metoda sprawdza czy vector odwiedzonych wierzchołków visitedVertices zawiera wierzchołek vertex
    // metoda ma modyfikator dostępu private, ponieważ jest jedynie metodą pomocniczą dla metody containsCycle()
    // i sama w sobie nie dostarcza funkcjonalności przydatnej programiście lub innym obiektom na zewnątrz klasy Graph
    bool vertexWasVisited(string vertex, vector<string> visitedVertices) {
        for (int i = 0; i < visitedVertices.size(); i++) { // dla wszystkich odwiedzonych wierzchołków
            if (visitedVertices[i] == vertex) {
                return true; // jeśli visitedVertices zawiera vertex, zwracamy true
            }
        }
        return false; // jeśli po przeiterowaniu całego vectora nie znajdujemy wierzchołka vertex, zwracamy false
    }
};

// funkcja wypisuje krawędź e
void printEdge(Edge& e) {
    cout << "(" << e.v1 << ", " << e.v2 << ", " << e.weight << ")" << endl;
}

// funkcja wypisuje graf g
void printGraph(Graph& g) {
    cout << "Wierzcholki: " << endl;
    for (int i = 0; i < g.vertices.size(); i++) {
        cout << g.vertices[i] << endl;
    }
    cout << "Krawedzie: " << endl;
    for (int i = 0; i < g.edges.size(); i++) {
        printEdge(g.edges[i]);
    }
    cout << endl;
}

// funkcja odczytuje wejście użytkownika i zapisuje je do grafu graph
void getUserInput(Graph& graph) {
    int graphEdgesCount = 0;
    cout << "Liczba krawedzi grafu: ";
    cin >> graphEdgesCount;

    for (int i = 0; i < graphEdgesCount; i++) {
        string v1 = "";
        string v2 = "";
        double weight = 0;

        cout << "Krawedz " << i + 1 << ", wierzcholek 1: ";
        cin >> v1;
        cout << "Krawedz " << i + 1 << ", wierzcholek 2: ";
        cin >> v2;
        cout << "Krawedz " << i + 1 << ", waga: ";
        cin >> weight;

        Edge e;
        e.v1 = v1;
        e.v2 = v2;
        e.weight = weight;

        graph.addEdge(e);
    }

    cout << endl;
}

// inicjalizacja grafu graph przykładowymi danymi
void initializeTestGraph(Graph& graph) {
    Edge edge;
    edge.v1 = "A";
    edge.v2 = "D";
    edge.weight = 5;
    graph.addEdge(edge);

    edge.v1 = "A";
    edge.v2 = "B";
    edge.weight = 7;
    graph.addEdge(edge);

    edge.v1 = "D";
    edge.v2 = "B";
    edge.weight = 9;
    graph.addEdge(edge);

    edge.v1 = "B";
    edge.v2 = "C";
    edge.weight = 8;
    graph.addEdge(edge);

    edge.v1 = "C";
    edge.v2 = "E";
    edge.weight = 5;
    graph.addEdge(edge);

    edge.v1 = "B";
    edge.v2 = "E";
    edge.weight = 7;
    graph.addEdge(edge);

    edge.v1 = "D";
    edge.v2 = "E";
    edge.weight = 15;
    graph.addEdge(edge);

    edge.v1 = "D";
    edge.v2 = "F";
    edge.weight = 6;
    graph.addEdge(edge);

    edge.v1 = "F";
    edge.v2 = "E";
    edge.weight = 8;
    graph.addEdge(edge);

    edge.v1 = "F";
    edge.v2 = "G";
    edge.weight = 11;
    graph.addEdge(edge);

    edge.v1 = "E";
    edge.v2 = "G";
    edge.weight = 9;
    graph.addEdge(edge);
}

// funkcja zwraca minimalne rozpięte drzewo na danym grafie (MST - minimal spanning tree)
// MST jest również grafem (klasa Graph)
Graph kruskal(Graph& graph) {
    Graph mst; // utworzenie pustego grafu

    vector<Edge> edges(graph.edges); // skopiowanie vectora krawędzi z grafu graph

    // posortowanie vectora edges względem wag krawędzi
    // używamy funkcji sort() oraz sortujemy względem wag dzięki przeciążeniu operatora < w klasie Edge
    sort(edges.begin(), edges.end());

    for (int i = 0; i < edges.size(); i++) { // iterujemy po posortowanych krawędziach edges
        Edge edge = edges[i];

        mst.addEdge(edge); // dodajemy do grafu mst badaną krawędź

        // sprawdzamy czy graf mst z dodaną krawędzią zawiera cykle
        if (mst.containsCycle()) { // jeśli zawiera (czyli graf nie jest MST), usuwamy badaną krawędź
            mst.removeEdge(edge);
        }

        // jeśli mst nie zawiera cykli, zostawiamy dodaną krawędź i przechodzimy do sprawdzania następnej
    }

    return mst; // po sprawdzeniu wszystkich krawędzi otrzymujemy gotowe mst, które zwracamy
}

int main() {
    Graph graph; // utworzenie pustego grafu, który jest grafem wejściowym

    getUserInput(graph); // pobranie wejścia użytkownika
    // initializeTestGraph(graph); // lub inicjalizacja przykładowego grafu

    cout << "Graf wejsciowy: " << endl;
    printGraph(graph);

    Graph mst = kruskal(graph); // wykonanie algorytmu Kruskala i zapisanie wyniku do zmiennej mst

    cout << "Minimalne drzewo rozpiete na danym grafie: " << endl;
    printGraph(mst);

    return 0;
}
