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

	HashTable<std::string> * hTableString = new HashTable<string>(8,2);
	hTableString->setColType(1);
	hTableString->add("Andre");
	hTableString->add("Felipe");
	hTableString->add("Silva");
	hTableString->add("Arthur");
	hTableString->add("Kayla");
	hTableString->remove(1);
	hTableString->add("Nidalee");
	hTableString->add("Yami");
	hTableString->set("Andre");
	hTableString->printall();

	return 0;
}
