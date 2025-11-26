#pragma once
#include <iostream>


class Arista
{
private:
    int origen;
    int destino;
public:
    Arista(int o, int d) : origen(o), destino(d) {}
    Arista() : origen(-1), destino(-1) {}

    bool operator==(const Arista& otra) const
    {
        return (origen == otra.origen && destino == otra.destino) ||
            (origen == otra.destino && destino == otra.origen);
    }
};