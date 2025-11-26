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

class CreadorGrafo {
private:
    std::unordered_map<std::string, int> mapeo_str_a_int;
    std::unordered_map<int, std::string> mapeo_int_a_str;
    int next_vertex_id = 0; 

    /**
     * @brief Obtiene el ID numérico de un vértice, asignándole uno nuevo si no existe.
     */
    int obtener_id(const std::string& nombre_vertice) {
        if (mapeo_str_a_int.find(nombre_vertice) == mapeo_str_a_int.end()) {
            int id_asignado = next_vertex_id++;
            mapeo_str_a_int[nombre_vertice] = id_asignado;
            mapeo_int_a_str[id_asignado] = nombre_vertice;
            return id_asignado;
        }
        return mapeo_str_a_int[nombre_vertice];
    }

public:
    /**
     * @brief Getter para el mapeo inverso (necesario para el SolucionadorKarger).
     */
    const std::unordered_map<int, std::string>& get_mapeo_int_a_str() const {
        return mapeo_int_a_str;
    }

    /**
     * @brief Lee el archivo, construye el Grafo inicial y realiza la indexación.
     */
    Grafo cargar_y_construir(const std::string& ruta_archivo) {
        std::ifstream archivo(ruta_archivo);
        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo: " + ruta_archivo);
        }

        std::vector<Arista> todas_las_aristas_temp;
        std::string linea;

        // 1. Fase de lectura, indexación y almacenamiento temporal de aristas
        while (std::getline(archivo, linea)) {
            std::stringstream ss(linea);
            std::string origen_str;
            std::string destino_str;

            // Lee pares de vértices (formato asumido: V_origen V_destino)
            while (ss >> origen_str >> destino_str) {

                int id_origen = obtener_id(origen_str);
                int id_destino = obtener_id(destino_str);

                if (id_origen != id_destino) {
                    todas_las_aristas_temp.push_back(Arista(id_origen, id_destino));
                }
            }
        }
        archivo.close();

        // 2. Crear el objeto Grafo y llenar sus estructuras internas
        // Usamos next_vertex_id para saber el número total de vértices iniciales
        Grafo grafo_inicial(next_vertex_id);

        for (const auto& arista : todas_las_aristas_temp) {
            // Usamos los GETTERS (getOrigen(), getDestino()) para acceder a los IDs
            int origen = arista.getOrigen();
            int destino = arista.getDestino();

            // Llenar la lista de adyacencia (adj)
            grafo_inicial.adj[origen].push_back(destino);
            grafo_inicial.adj[destino].push_back(origen); // Es no dirigido

            // Llenar la lista de aristas activas (todas_las_aristas)
            grafo_inicial.todas_las_aristas.push_back(arista);

            // Asegurar que los vértices estén en vertices_activos
            grafo_inicial.vertices_activos.insert(origen);
            grafo_inicial.vertices_activos.insert(destino);
        }

        return grafo_inicial;
    }
};