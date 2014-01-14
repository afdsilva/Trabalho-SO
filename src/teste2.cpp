/*
 * teste2.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: andref
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

	int threads = 5;
	int tamTabela = 10;
	int blocos = 5;

	HashTable<int> * hash = new HashTable<int>(tamTabela,blocos,threads);
	hash->add(30);
	hash->add(300);
	hash->add(3000);
	hash->add(30000);
	hash->add(300000);
	hash->add(3000000);

	hash->joinThreads();

	hash->printall();

}
