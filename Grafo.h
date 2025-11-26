#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm> 
#include "Arista.h"

class Grafo
{
public:
    // Estructuras de Datos
    std::unordered_map<int, std::vector<int>> adj;
    std::unordered_set<int> vertices_activos;
    std::vector<Arista> todas_las_aristas;

private:
    int contador_id_vertices; // Puede dejarse o eliminarse, no es esencial si usamos un representante.

public:
    // --- FUNCIONES FALTANTES Y CORREGIDAS ---

    // Constructor necesario para CreadorGrafo
    Grafo(int num_inicial_vertices = 0) : contador_id_vertices(num_inicial_vertices) {
        // Inicializa la semilla del generador de números aleatorios si no se ha hecho
        // srand((unsigned int)time(0)); // Mejor hacerlo una vez en main()
    }

    // Copia Profunda (CRÍTICA para Karger)
    Grafo CopiaProfunda() const {
        Grafo copia;
        copia.adj = adj;
        copia.vertices_activos = vertices_activos;
        copia.todas_las_aristas = todas_las_aristas; // Copia de Aristas (tienen valor semántico)
        copia.contador_id_vertices = contador_id_vertices;
        return copia;
    }

    // Obtener Arista Aleatoria (CRÍTICA para Karger)
    Arista obtener_arista_aleatoria() const {
        if (todas_las_aristas.empty()) {
            throw std::runtime_error("No hay aristas para contraer.");
        }
        int indice_aleatorio = rand() % todas_las_aristas.size();
        return todas_las_aristas[indice_aleatorio];
    }

    // Contar Corte Final (CRÍTICA para Karger)
    int contar_corte_final() const {
        // En este punto, solo deben quedar 2 super-vértices. 
        // El corte es el número de aristas restantes entre ellos.
        return todas_las_aristas.size();
    }

    // ---------------------------------------------------------------------
    // FUNCIONES PROPORCIONADAS (CORREGIDAS)
    // ---------------------------------------------------------------------

    void contraer(int v1, int v2) {
        // Decidimos que v1 será el vértice representante (el nuevo super-vértice).
        int representante = v1;
        int eliminado = v2;

        if (v1 == v2) return;

        // ---------------------------------------------------------------------
        // PASO CRÍTICO 1: Actualizar IDs en la Lista Global de Aristas
        // ---------------------------------------------------------------------
        for (Arista& a : todas_las_aristas) {
            // USAR SETTERS EN LUGAR DE ACCESO DIRECTO
            if (a.getOrigen() == eliminado) {
                a.setOrigen(representante);
            }
            if (a.getDestino() == eliminado) {
                a.setDestino(representante);
            }
        }

        // ---------------------------------------------------------------------
        // PASO CRÍTICO 2: Fusionar Listas de Adyacencia y Actualizar Vecinos
        // ---------------------------------------------------------------------
        // Recorrer los vecinos del vértice a ser eliminado (v2)
        // Usamos .count() o .find() para verificar si la llave existe antes de usar .at()
        if (adj.count(eliminado)) {
            for (int vecino : adj.at(eliminado)) {
                // 1.1: Mover la arista a la lista del representante (si no es un auto-ciclo)
                if (vecino == representante) continue;

                // Mover el ID del vecino a la lista del representante
                adj[representante].push_back(vecino);

                // 1.2: Actualizar la lista de adyacencia del vecino 'vecino'. 
                // Reemplazar todas las referencias al ID 'eliminado' por 'representante'.
                if (adj.count(vecino)) {
                    auto& lista_vecino = adj.at(vecino);
                    for (int& v_id : lista_vecino) {
                        if (v_id == eliminado) {
                            v_id = representante;
                        }
                    }
                }
            }
        }

        // --- 3. Limpieza de Estructuras del Vértice Eliminado ---
        adj.erase(eliminado);
        vertices_activos.erase(eliminado);

        // --- 4. Eliminar Auto-Ciclos ---
        eliminar_auto_ciclos(representante);
    }


    void eliminar_auto_ciclos(int v) {
        if (!adj.count(v)) return; // Si el vértice no existe, no hay nada que hacer.

        // --- 1. Limpiar la Lista de Adyacencia del Vértice (v) ---
        auto& lista_vecinos = adj.at(v);
        lista_vecinos.erase(
            std::remove(lista_vecinos.begin(), lista_vecinos.end(), v),
            lista_vecinos.end()
        );

        // --- 2. Limpiar la Lista Global de Aristas (todas_las_aristas) ---
        // Elimina cualquier arista donde origen == destino (ya que fueron actualizados en contraer)
        todas_las_aristas.erase(
            std::remove_if(todas_las_aristas.begin(), todas_las_aristas.end(),
                [](const Arista& a) {
                    // USAR GETTERS
                    return (a.getOrigen() == a.getDestino());
                }),
            todas_las_aristas.end()
        );
    }
};