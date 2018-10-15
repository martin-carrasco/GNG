template <class CustomTrait>
Kruskal<CustomTrait>::Kruskal(Graph<CustomTrait> graph) : graph(graph) {

}

template <class CustomTrait>
auto  Kruskal<CustomTrait>::generate_minimum_spanning_tree() {
   // vector<Edge*> tree;

    return this->graph.EdgesList;

    /*
    int size_arestas = arestas.size();
    // ordena as arestas pelo menor peso
    sort(arestas.begin(), arestas.end());
    // aloca memória para criar "V" subconjuntos
    int * subset = new int[V];
    // inicializa todos os subconjuntos como conjuntos de um único elemento
    memset(subset, -1, sizeof(int) * V);
    for(int i = 0; i < size_arestas; i++)
    {
        int v1 = buscar(subset, arestas[i].obterVertice1());
        int v2 = buscar(subset, arestas[i].obterVertice2());
        if(v1 != v2)
        {
            // se forem diferentes é porque NÃO forma ciclo, insere no vetor
            arvore.push_back(arestas[i]);
            unir(subset, v1, v2); // faz a união
        }
    }
    int size_arvore = arvore.size();
    // mostra as arestas selecionadas com seus respectivos pesos
    for(int i = 0; i < size_arvore; i++)
    {
        char v1 = 'A' + arvore[i].obterVertice1();
        char v2 = 'A' + arvore[i].obterVertice2();
        cout << "(" << v1 << ", " << v2 << ") = " << arvore[i].obterPeso() << endl;
    }*/
}