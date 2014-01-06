/*
 * Main.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: andref
 */

#include <iostream>
#include <stdio.h>
#include <string>

#include "HashTable.h"

#include "Mutex.h"
#include "CondVar.h"
#include "Thread.h"

using namespace std;

int main(int argc, char **argv) {

	HashTable<int> * hash = new HashTable<int>(30,6,2);
	hash->add(1998);
	hash->add(18);
	hash->joinThreads();
	hash->add(15);
	hash->add(198);
	hash->printall();

	//HashTable<string> * hashString = new HashTable<string>(30,6);
	//char * name = "Andre";
	//hashString->_add(name);
	//hashString->printall();

	return 0;
}
