#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include "Grafo.h" 
#include "Arista.h"

class CreadorGrafo 
{
private:
    std::unordered_map<std::string, int> mapeo_str_a_int;
    std::unordered_map<int, std::string> mapeo_int_a_str;
    int next_vertex_id = 0; // Contador para asignar IDs numéricos

    int obtener_id(const std::string& nombre_vertice) {
        if (mapeo_str_a_int.find(nombre_vertice) == mapeo_str_a_int.end()) {
            // Es un vértice nuevo, asignar el siguiente ID.
            int id_asignado = next_vertex_id++;
            mapeo_str_a_int[nombre_vertice] = id_asignado;
            mapeo_int_a_str[id_asignado] = nombre_vertice;
            return id_asignado;
        }
        // El vértice ya existe, retornar su ID asignado.
        return mapeo_str_a_int[nombre_vertice];
    }

public:
    // Los atributos 'mapeo_str_a_int' y 'mapeo_int_a_str' deben ser accesibles
    // por el SolucionadorKarger para mostrar el resultado final.
    const std::unordered_map<int, std::string>& get_mapeo_int_a_str() const {
        return mapeo_int_a_str;
    }

    Grafo cargar_y_construir(const std::string& ruta_archivo) {
        std::ifstream archivo(ruta_archivo);
        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + ruta_archivo);
        }

        // 1. Estructuras temporales para construir el Grafo
        std::vector<Arista> todas_las_aristas_temp;

        // 2. Leer línea por línea
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::stringstream ss(linea);
            std::string origen_str;
            std::string destino_str;

            // Asumo un formato simple: V_origen V_destino (para cada arista)
            // Esto deberá adaptarse a tu formato de archivo exacto
            while (ss >> origen_str >> destino_str) {
                // Si la línea contiene un par (origen, destino), procesar:

                // 2.1 Obtener los IDs numéricos, creando nuevos si es necesario.
                int id_origen = obtener_id(origen_str);
                int id_destino = obtener_id(destino_str);

                // 2.2 Ignorar auto-ciclos que puedan estar en el archivo de entrada (aunque Karger se ocupa de esto)
                if (id_origen != id_destino) {
                    // 2.3 Guardar la arista con IDs numéricos
                    todas_las_aristas_temp.push_back(Arista(id_origen, id_destino));
                }
            }
        }
        archivo.close();

        // 3. Crear el objeto Grafo y llenar sus estructuras internas
        Grafo grafo_inicial(next_vertex_id); // Asumiendo un constructor en Grafo que recibe num_vertices

        for (const auto& arista : todas_las_aristas_temp) {
            // Llenar la lista de adyacencia (adj) y la lista de aristas activas (todas_las_aristas)

            // Lógica para llenar adj:
            grafo_inicial.adj[arista.origen]().push_back(arista.destino);
            grafo_inicial.adj[arista.destino].push_back(arista.origen); // Es no dirigido

            // Llenar todas_las_aristas_activos
            grafo_inicial.todas_las_aristas.push_back(arista);

            // Asegurar que los vértices estén en vertices_activos
            grafo_inicial.vertices_activos.insert(arista.origen);
            grafo_inicial.vertices_activos.insert(arista.destino);
        }

        return grafo_inicial;
    }
};