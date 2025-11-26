#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <limits> // Para std::numeric_limits<int>::max()

// Asumimos que las clases Grafo, Arista y CreadorGrafo están definidas o incluidas
#include "Grafo.h" 
#include "Arista.h" 

/**
 * @brief Gestiona la estrategia del Algoritmo de Contracción de Karger:
 * la ejecución de múltiples corridas y el seguimiento del corte mínimo global.
 */
class SolucionadorKarger {
private:
    Grafo grafo_inicial;
    int num_ejecuciones;
    int corte_minimo_global;
    std::unordered_map<int, std::string> mapeo_nombres;

public:

    SolucionadorKarger(const Grafo& g_inicial,
        const std::unordered_map<int, std::string>& mapeo,
        int corridas)
        : grafo_inicial(g_inicial),
        num_ejecuciones(corridas),
        corte_minimo_global(std::numeric_limits<int>::max()), // Inicializar a infinito
        mapeo_nombres(mapeo) {
    }


    int ejecutar_corrida_unica(Grafo g_actual) {
        // Bucle principal: Mientras queden más de 2 vértices.
        while (g_actual.vertices_activos.size() > 2) {
            try {
                // 1. Elegir una arista al azar
                Arista arista_a_contraer = g_actual.obtener_arista_aleatoria();

                // 2. Contraer los vértices conectados por esa arista
                g_actual.contraer(arista_a_contraer.origen, arista_a_contraer.destino);

            }
            catch (const std::runtime_error& e) {
                // Esto no debería pasar en un grafo conexo, pero es buena práctica
                std::cerr << "Error en corrida: " << e.what() << std::endl;
                return std::numeric_limits<int>::max(); // Fallo de corrida
            }
        }

        // 3. Contar el corte final
        return g_actual.contar_corte_final();
    }


    int resolver() {
        std::cout << "Iniciando Algoritmo de Karger con " << num_ejecuciones << " ejecuciones..." << std::endl;

        for (int i = 0; i < num_ejecuciones; ++i) {

            // 1. Clonar el grafo inicial. ¡CRUCIAL!
            // Cada corrida debe comenzar con un grafo inalterado.
            Grafo grafo_actual = grafo_inicial.CopiaProfunda();

            // 2. Ejecutar la contracción y obtener el resultado
            int corte_actual = ejecutar_corrida_unica(grafo_actual);

            // 3. Comparar y actualizar el corte mínimo global
            if (corte_actual < corte_minimo_global) {
                corte_minimo_global = corte_actual;
                std::cout << "-> Nuevo corte mínimo encontrado: " << corte_minimo_global
                    << " (Corrida " << i + 1 << "/" << num_ejecuciones << ")" << std::endl;

                // Opcional: Almacenar los nombres de los super-vértices para el resultado final
                // (Los dos vértices que quedan en grafo_actual definen el corte)
            }
        }

        std::cout << "\n--- PROCESO FINALIZADO ---" << std::endl;
        std::cout << "Corte Mínimo Global encontrado después de " << num_ejecuciones << " corridas: "
            << corte_minimo_global << std::endl;

        return corte_minimo_global;
    }
};

