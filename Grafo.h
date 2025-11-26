#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "Arista.h" 

using namespace std;
class Grafo 
{
public:
    // Estructuras de Datos
    unordered_map<int, std::vector<int>> adj;
    unordered_set<int> vertices_activos;
    vector<Arista> todas_las_aristas;

private:
    int contador_id_vertices;

public:

    Grafo(int num_inicial_vertices) : contador_id_vertices(num_inicial_vertices) {

        std::srand(std::time(0));
    }

    // Constructor por defecto (necesario para algunas operaciones)
    Grafo() : contador_id_vertices(0) {}



    Grafo CopiaProfunda() const {
        // Usar el constructor de copia
        Grafo nueva_copia = *this;

        // C++ maneja la copia profunda de vector, unordered_map, y unordered_set por defecto
        // ya que contienen tipos primitivos (int) o tipos que tienen un operador de asignación
        // de copia definido (Arista, vector, unordered_map, unordered_set).
        return nueva_copia;
    }


    // --------------------------------------------------------------------------------
    // FUNCIONES DEL ALGORITMO (IMPLEMENTACIÓN DE LAS PRIMITIVAS)
    // --------------------------------------------------------------------------------

    Arista obtener_arista_aleatoria() {
        if (todas_las_aristas.empty()) {
            throw std::runtime_error("El grafo no tiene aristas activas.");
        }

        size_t tamano = todas_las_aristas.size();
        // Usamos rand() % tamano para obtener un índice aleatorio.
        int indice_azar = std::rand() % tamano;

        return todas_las_aristas[indice_azar];
    }

 
    void contraer(int v1, int v2) {
        // Decidimos que v1 será el vértice representante (el nuevo super-vértice).
        int representante = v1;
        int eliminado = v2;

        // Evitar contracciones de un vértice consigo mismo
        if (v1 == v2) return;

        // --- 1. Fusionar Listas de Adyacencia y Actualizar Vecinos ---
        // Recorrer los vecinos del vértice a ser eliminado (v2)
        for (int vecino : adj[eliminado]) {
            if (vecino == representante) continue; // Si es el representante, la arista se convertirá en auto-ciclo

            // 1.1: Mover la arista a la lista del representante
            adj[representante].push_back(vecino);

            // 1.2: Actualizar la lista de adyacencia del vecino 'vecino'. 
            // Esto asegura que 'vecino' ya no apunte a 'eliminado' sino a 'representante'.
            auto& lista_vecino = adj[vecino];
            for (int& v_id : lista_vecino) {
                if (v_id == eliminado) {
                    v_id = representante; // Reemplazar la referencia
                }
            }
        }

        // --- 2. Limpieza de Estructuras del Vértice Eliminado ---
        adj.erase(eliminado);
        vertices_activos.erase(eliminado);

        // --- 3. Eliminar Auto-Ciclos y Actualizar todas_las_aristas ---
        // Es crucial que esta función actualice tanto adj[representante] como todas_las_aristas
        eliminar_auto_ciclos(representante);
    }


    /**
     * @brief Elimina aristas que conectan un vértice consigo mismo (auto-ciclos).
     * @details Se debe limpiar tanto la lista de adyacencia (adj) como la lista de aristas
     * global (todas_las_aristas).
     */
    void eliminar_auto_ciclos(int v) {
        // --- 1. Limpiar la Lista de Adyacencia del Vértice (v) ---
        // Usamos erase-remove idiom para eliminar eficientemente de la lista de vecinos
        auto& lista_vecinos = adj[v];
        lista_vecinos.erase(
            std::remove(lista_vecinos.begin(), lista_vecinos.end(), v),
            lista_vecinos.end()
        );
        todas_las_aristas.erase(
            std::remove_if(todas_las_aristas.begin(), todas_las_aristas.end(),
                [v](const Arista& a) {
                    // Un auto-ciclo se forma cuando (get_origen() == get_destino()) en el nuevo super-vértice
                    // o cuando (get_origen() y get_destino()) son el mismo ID (v)
                    return (a.get_origen() == v && a.get_destino() == v);
                }),
            todas_las_aristas.end()
        );
    }

    /**
     * @brief Cuenta el número de aristas que cruzan el corte final.
     * @details Se llama cuando vertices_activos.size() == 2.
     * @return int El tamaño del corte (número de aristas).
     */
    int contar_corte_final() const {
        // Después de la última contracción, el grafo se reduce a dos super-vértices A y B.
        // Las aristas que quedan en adj[A] y adj[B] son las que conectan A y B, y su número
        // es el valor del corte.

        if (vertices_activos.size() != 2) {
            throw std::runtime_error("El conteo final solo debe ejecutarse con 2 vértices activos.");
        }

        // El tamaño de la lista de adyacencia del primer vértice restante es el número de aristas del corte.
        // No es necesario sumar adj[A].size() + adj[B].size(), ya que ambas deben ser iguales
        // y contarían el mismo conjunto de aristas.
        int v_final_id = *vertices_activos.begin();

        return adj.at(v_final_id).size();

        // Alternativamente, se puede retornar simplemente: 
        // return todas_las_aristas.size();
    }
};