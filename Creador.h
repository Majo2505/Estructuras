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
#include "Grafo.h"

using namespace std;
template <class T>
class Creador 
{
private:
    unordered_map<string, int> mapaNombres;
    int contadorId = 0;

public:
    Grafo<T> leerArchivo(string rutaArchivo) {
        ifstream archivo(rutaArchivo);
        string linea;
        vector<Arista<int>> listaTemporal;

        mapaNombres.clear();
        contadorId = 0;
        vector<string> listaNombresOrdenada;

        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string origenStr, vecinoStr;

            ss >> origenStr;
            if (mapaNombres.find(origenStr) == mapaNombres.end()) {
                mapaNombres[origenStr] = contadorId++;
                listaNombresOrdenada.push_back(origenStr); 
            }
            int u = mapaNombres[origenStr];

            while (ss >> vecinoStr) {
                if (mapaNombres.find(vecinoStr) == mapaNombres.end()) {
                    mapaNombres[vecinoStr] = contadorId++;
                    listaNombresOrdenada.push_back(vecinoStr); 
                }
                int v = mapaNombres[vecinoStr];
                if (u < v) {
                    listaTemporal.push_back({ u, v });
                }
            }
        }
        archivo.close();
        Grafo<int> nuevoGrafo(contadorId);
        nuevoGrafo.getaristasOriginales() = listaTemporal;
        nuevoGrafo.getNombresNodos() = listaNombresOrdenada;

        return nuevoGrafo;
    }
};
