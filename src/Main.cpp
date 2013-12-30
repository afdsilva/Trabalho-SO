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
using namespace std;

int main(int argc, char **argv) {

	HashTable<int> * hTable = new HashTable<int>(8,2);
	HashTable<float> * hTableFloat = new HashTable<float>(8,2);
	HashTable<std::string> * hTableString = new HashTable<string>(8,2);
	hTable->add(1);
	hTableFloat->add(1.1);
	hTableString->add("Andre");

	//hashObject mObject(1,1);
	//hashObject mObject2(1,1);
	//if (mObject == mObject2)
	//	std::cout << "Igual" << std::endl;
	return 0;
}
