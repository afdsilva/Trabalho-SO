/*
 * Main.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: Andre Silva
 */

#include <iostream>
#include <stdio.h>
#include <string>
#include <ctime>

#include "libhash.h"

#include "Mutex.h"
#include "CondVar.h"
#include "Thread.h"

using namespace std;

int main(int argc, char **argv) {
	//como parametros, deve receber o numero de threads, tamanho da tabela, numero de blocos e o total de operacoes a serem realizadas
	if (argc != 5) {
		std::cout << "USO: ./teste <threads> <tamanho tabela> <nr blocos> <total operacoes>" << std::endl;
		return -1;
	}

	int threads = atoi(argv[1]);
	int tamTabela = atoi(argv[2]);
	int blocos = atoi(argv[3]);
	int nrOperacoes = atoi(argv[4]);

	int valor;
	srand(time(NULL));
	HashTable<int> * hash = new HashTable<int>(tamTabela,blocos,threads);
	for (int i = 0; i < nrOperacoes; i++) {
		valor = rand() % 10000;
		hash->add(valor);
	}
	hash->joinThreads();

	hash->printall();

	for (int i = 0; i < nrOperacoes; i++) {
		valor = rand() % 10000;
		hash->set(valor);
	}
	hash->joinThreads();

	hash->printall();

	hash->~HashTable();

	std::cout << "Tabela hash finalizada, todas operações realizadas, saindo..." << std::endl;
	return 0;
}
