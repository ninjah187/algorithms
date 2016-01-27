import time

start_time = time.time()

# deklaracja klasy, która przedstawia graf
class Graph:
    vertices = [] # lista zawierająca wierzchołki; są to stringi jednoznacznie identyfikujące dany wierzchołek
    edges = [] # lista zawierająca krawędzie; są to tuple w formacie (x, u, v), gdzie x to waga, u to Iszy wierzchołek, a v to IIgi wierzchołek

    # konstruktor
    def __init__(self):
        self.vertices = [] # deklaracja nowej listy
        self.edges = [] # jw.

    # funkcja zwraca dokładną kopię danego grafu
    def copy(self):
        g = Graph() # nowy obiekt typu Graph

        # tworzenie nowych list z danymi grafu
        g.vertices = list(self.vertices)
        g.edges = list(self.edges)

        return g

    # funkcja dodaje krawędź do listy edges oraz niestniejące wcześniej wierzchołki do listy vertices
    def addEdge(self, edge):
        # dodanie do grafu wierzchołków, które wcześniej nie były do niego dodane
        if (edge[1] not in self.vertices):
            self.vertices.append(edge[1])
        if (edge[2] not in self.vertices):
            self.vertices.append(edge[2])

        # dodanie krawędzi do listy
        self.edges.append(edge)

    # funkcja zwraca sąsiadów wierzchołka v
    def getAdjacentVertices(self, v):
        adjVertices = [] # lista, która zostanie zwrócona
        for edge in self.edges:
            if (edge[1] == v or edge[2] == v): # sprawdzanie czy jeden z końców krawędzi jest wierzchołkiem v
                # (v, x) - krawędź o wierzchołkach v i x; poniższy if - dodanie wierzchołka x do listy sąsiadów v
                if (edge[1] == v):
                    adjVertices.append(edge[2])
                else: adjVertices.append(edge[1])
        return adjVertices

    # funkcja sprawdza (korzystając z algorytmu przeszukiwania wgłąb - dfs), czy dany graf zawiera cykl
    def containsCycle(self):
        visitedVertices = [] # odwiedzone wierzchołki
        stack = [] # pomocniczy stos; używany do implementacji dfs
        stack.append(self.vertices[0]) # wrzucenie pierwszego wierzchołka do stosu
        while len(stack) != 0:
            v = stack.pop() # wyciągnięcie wierzchołka
            if v not in visitedVertices: # jeżeli wierzchołek nie został odwiedzony wcześniej
                visitedVertices.append(v) # dodaj wierzchołek do odwiedzonych
                for vertex in self.getAdjacentVertices(v):
                    if (vertex not in visitedVertices):
                        stack.append(vertex) # wrzucamy na stos wszystkich sąsiadów wierzchołka, którzy nie zostali wcześniej odwiedzeni
            else: # jeżeli wierzchołek został wcześniej odwiedzony
                return True # graf zawiera cykl
        return False # po sprawdzeniu wszystkich wierzchołków, graf nie zawiera cyklu

    def dfs(self):
        visitedVertices = []
        stack = []
        stack.append(self.vertices[0])
        while len(stack) != 0:
            v = stack.pop()
            if v not in visitedVertices:
                visitedVertices.append(v)
                print(v)
                for vertex in self.getAdjacentVertices(v):
                    stack.append(vertex)

# funkcja, która wypisuje wierzchołki i krawędzie grafu
def printGraph(graph):
    print('Wierzchołki: ')
    for vertex in graph.vertices:
        print(vertex)
    print('Krawędzie: ')
    for edge in graph.edges:
        print(edge)

# funkcja zwraca minimalne rozpięte drzewo (MST) na danym grafie
# minimalne rozpięte drzewo jest również grafem (klasa Graph)
def kruskal(graph):
    mst = Graph() # utworzenie pustego grafu

    edges = list(graph.edges) # skopiowanie listy krawędzi z grafu
    edges.sort() # posortowanie skopiowanej listy względem wag

    for edge in edges: # sprawdzamy każdą krawędź:
        temp = mst.copy() # utworzenie kopii grafu MST
        temp.addEdge(edge) # dodanie do tymczasowego grafu krawędzi
        if (temp.containsCycle() == False): # sprawdzenie czy nowouzyskany graf zawiera cykle
            mst = temp # jeśli nie zawiera cykli, staje się nowym MST

    return mst # zwracamy graf MST

    # zamiast każdorazowo tworzyć kopię mst, dodawać do niego krawędź, sprawdzać czy jest cykl i odrzucać kopię (jeśli jest cykl) lub ustalać ją jako mst (jeśli nie ma cyklu)
    # można by tworzyć jedno mst, dodawać do niego krawędź, sprawdzać czy jest cykl i, jeśli jest, usuwać badaną krawędź. wynik to mst

graph = Graph(); # tworzenie pustego grafu, który będzie grafem wejściowym

graph.addEdge((5, 'A', 'D'))
graph.addEdge((7, 'A', 'B'))
graph.addEdge((9, 'D', 'B'))
graph.addEdge((8, 'B', 'C'))
graph.addEdge((5, 'C', 'E'))
graph.addEdge((7, 'B', 'E'))
graph.addEdge((15, 'D', 'E'))
graph.addEdge((6, 'D', 'F'))
graph.addEdge((8, 'F', 'E'))
graph.addEdge((11, 'F', 'G'))
graph.addEdge((9, 'E', 'G'))

# graphEdgesCount = int(input("Ilość krawędzi grafu: ")) # ilość krawędzi grafu, które wprowadzi user; rzutujemy na inta, aby móc użyć tej zmiennej w pętli for

# # zbieranie wejściowych danych
# for i in range(graphEdgesCount):
#     print("Krawędź", i + 1, ", wierzchołek 1: ")
#     vertex1 = input()
#     print("Krawędź", i + 1, ", wierzchołek 2: ")
#     vertex2 = input()
#     print("Krawędź", i + 1, ", waga: ")
#     weight = int(input())
#     edge = (weight, vertex1, vertex2) # utworzenie tupla, który reprezentuje krawędź
#     graph.addEdge(edge)
#     print()

# wypisanie wejściowego grafu
print("Wejściowy graf: ")
printGraph(graph)

print()
# rozwiązanie i wypisanie MST
print("Minimalne drzewo rozpięte na danym grafie: ")
mst = kruskal(graph)
printGraph(mst)

# graph.dfs()

print("--- %s seconds ---" % (time.time() - start_time))