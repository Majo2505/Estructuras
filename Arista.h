#pragma once
#include <iostream>

class Arista
{
public:
    int origen; // Hacemos públicos por simplicidad en Karger
    int destino; // Hacemos públicos por simplicidad en Karger

public:
    // Constructores
    Arista(int o, int d) : origen(o), destino(d) {}
    Arista() : origen(-1), destino(-1) {}

    // ------------------------------------
    // GETTERS (Funciones de Acceso)
    // ------------------------------------
    int getOrigen() const {
        return origen;
    }

    int getDestino() const {
        return destino;
    }

    // ------------------------------------
    // SETTERS (Necesarios para la Contracción)
    // ------------------------------------
    void setOrigen(int nuevo_origen) {
        origen = nuevo_origen;
    }

    void setDestino(int nuevo_destino) {
        destino = nuevo_destino;
    }

    // -------------------------------
    // OPERADOR DE IGUALDAD (Correcto)
    // ------------------------------------
    bool operator==(const Arista& otra) const
    {
        // El orden no importa: (O, D) es igual a (D, O)
        // Nota: Los miembros ahora son públicos, pero los getters siguen siendo válidos.
        return (origen == otra.origen && destino == otra.destino) ||
            (origen == otra.destino && destino == otra.origen);
    }
};