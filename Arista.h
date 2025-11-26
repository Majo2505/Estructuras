#pragma once
#include <iostream>
#include <string>
using namespace std;

template <class T>
class Arista
{
private:
	T origen;
	T destino;
public:
	Arista(T o, T d) : origen(o), destino(d) {}
	T getOrigen() const { return origen; }
	T getDestino() const { return destino; }
	void setOrigen(T o) { origen = o; }
	void setDestino(T d) { destino = d; }
};

