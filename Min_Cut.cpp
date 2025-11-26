// Min_Cut.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "Creador.h"

int main()
{
	Creador<int> creador;
	creador.leerArchivo("grafo.txt");
	return 0;
}