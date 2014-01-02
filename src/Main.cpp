/*
 * Main.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: andref
 */

#include <iostream>
#include <stdio.h>
#include <string>

//#include "HashTable.h"
#include "HashTableThreaded.h"

#include "Mutex.h"
#include "CondVar.h"
#include "Thread.h"

using namespace std;

int main(int argc, char **argv) {

	std::cout << "Ola" << std::endl;
	int blocos = 2;
	int pos;
	int size = 10;
	int secao;
	Mutex * aMutex = new Mutex[blocos];
	CondVar * aCond = new CondVar[blocos];
	for (int i = 0; i < blocos; i++) {
		aCond[i] = CondVar(aMutex[i]);
	}
	HashTableThreaded * htt = new HashTableThreaded(size);
	std::cout << "Antes: " << std::endl;
	htt->printall();
	pos = 0;
	secao = pos / (size / blocos);
	HashTableAdd * a1 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),5,pos);
	a1->start();
	HashTableAdd * a11 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),10,pos);
	a11->start();
	HashTableAdd * a12 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),15,pos);
	a12->start();
	HashTableAdd * a13 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),20,pos);
	a13->start();
	HashTableAdd * a14 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),25,pos);
	a14->start();

	pos = 1;
	secao = pos / (size / blocos);
	HashTableAdd * a2 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),2,pos);
	a2->start();

	pos = 2;
	secao = pos / (size / blocos);
	HashTableAdd * a3 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),3,pos);
	a3->start();

	pos = 3;
	secao = pos / (size / blocos);
	HashTableAdd * a4 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),4,pos);
	a4->start();

	pos = 4;
	secao = pos / (size / blocos);
	HashTableAdd * a5 = new HashTableAdd(aMutex[secao],aCond[secao],htt->getNumeros(),5,pos);
	a5->start();


	a11->join();
	a12->join();
	a13->join();
	a14->join();
	a2->join();
	a3->join();
	a4->join();
	a5->join();
	/**/

	a1->join();
	std::cout << "Depois: " << std::endl;
	htt->printall();


	return 0;
}
