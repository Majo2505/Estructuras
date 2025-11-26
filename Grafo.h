#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include "Arista.h"
using namespace std;

template <class T>
class Grafo 
{
private:
    int numVertices;
    vector<Arista<int>> aristasOriginales;
    vector<string> nombresNodos;

public:
    Grafo(int n);
    void mostrarGrafo();
	vector<Arista<int>> getaristasOriginales();
    vector<string> getNombresNodos();
	int KargerMinCut();
    void alAzar(vector<Arista<int>>& aristasActuales);
    void realizarcontraccion(const Arista<T>& aristaAContraer, vector<int>& padre, int& verticesRestantes);

};

template<class T>
inline Grafo<T>::Grafo(int n)
{
}

template<class T>
inline void Grafo<T>::mostrarGrafo()
{

}

template<class T>
inline vector<Arista<int>> Grafo<T>::getaristasOriginales()
{
	return aristasOriginales;
}

template<class T>
inline vector<string> Grafo<T>::getNombresNodos()
{
    return nombresNodos;
}

template<class T>
inline int Grafo<T>::KargerMinCut()
{
        vector<Arista> aristasActuales = aristasOriginales;
        vector<int> padre(numVertices);
        for (int i = 0; i < numVertices; i++) padre[i] = i;
        int verticesRestantes = numVertices;

        while (verticesRestantes > 2) {
            Arista<T> aristaSeleccionada = seleccionarAristaAlAzar(aristasActuales);
        }
}

template<class T>
inline void Grafo<T>::alAzar(vector<Arista<int>>& aristasActuales)
{
    int i = rand() % aristasActuales.size();
    Arista<T> aristaElegida = aristasActuales[i];
    aristasActuales[i] = aristasActuales.back();
    aristasActuales.pop_back();
    return aristaElegida;
}
