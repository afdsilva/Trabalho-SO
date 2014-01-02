/*
 * HashTableThreaded.cpp
 *
 *  Created on: Jan 2, 2014
 *      Author: andref
 */

#include "HashTableThreaded.h"

bool HashTableThreaded::lock = false;

HashTableThreaded::HashTableThreaded(int size) {
	numeros.resize(size,0);
}

HashTableThreaded::~HashTableThreaded() {
	// TODO Auto-generated destructor stub
}

std::vector<int> & HashTableThreaded::getNumeros() {
	return numeros;
}

void HashTableThreaded::printall() {
	for (int i = 0; i < (int) numeros.size(); i++) {
		std::cout << "Pos: " << i << " Valor: " << numeros[i] << std::endl;
	}
}
