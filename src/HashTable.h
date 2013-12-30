/*
 * HashTable.h
 *
 *  Created on: Dec 18, 2013
 *      Author: andref
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <string>
#include <string.h>
#include <typeinfo>

#include "Mutex.h"
#include "CondVar.h"

using namespace std;
/**
 * Classe para tipo de dado abstrato que sera armazenada na tabela hash
 * o tipo de objeto é selecionado na criacao da tabela hash
 */
template <class O>
class hashObject {
public:
	int key;
	O value;

	hashObject() {
		this->key = -1;
		if (typeid(O) == typeid(std::string) ) {
			std::cout << "Construtor hashObect" << std::endl;
		}
	}
	hashObject(int key, O value) {
		this->key = key;
		this->value = value;
	}
	bool operator==(const hashObject & obj) {
		if (obj.key == this->key && obj.value == this->value) return true;
		return false;
	}
};

template<class T>
class HashTable {
private:
	CondVar * m_Cond;

	std::vector<hashObject<T> > * table;

	int tableSize;
	int hashPrime;
	int hashBlocks;
	int hashLoad;
	int hashRecord;

	int hash(long int value) {
		return value % tableSize;
	}
	int hash(T value) {
		int retorno = 0;
		if (typeid(value) == typeid(std::string)) {
			//retorno = std::atoi(value[0]);
			std::cout << "string :v" << std::endl;

		} else {
			//retorno = value;
		}
		std::cout << "value: " << value;
		std::cout << std::endl << "retorno: " << retorno << std::endl;
		return retorno % tableSize;
	}
public:
	/**
	 * Construtor da classe HashTable, o tipo é dado na criação da tabela
	 */
	HashTable(int size, int blocks) {
		try {
			if (size % blocks != 0)
				throw 1;
			m_Cond = new CondVar[blocks];
			table = new std::vector<hashObject<T> >[size];
			tableSize = size;
			hashPrime = size;
			hashBlocks = blocks;
			hashLoad = 0.75;
			hashRecord = 0;


		} catch (int e) {
			std::cout << "Erro HashTable" << std::endl;
		}
	}
	virtual ~HashTable() {
		delete(table);
		delete(m_Cond);
	}

	hashObject<T> get(int pos) {
		hashObject<T> retorno = new hashObject<T>;
		try {
			throw 1;
		} catch (int e) {
			std::cout << "Erro get" << std::endl;
			return NULL;
		}
		return retorno;
	}
	void add(T value) {
		try {
			hashObject<T> nObj;
			nObj.key = hash(value);
			nObj.value = value;
			this->add(nObj);

		} catch (int e) {
			std::cout << "Erro add(T value)" << std::endl;
		}
	}
	void add(hashObject<T> & value) {
		try {
			this->table->insert(value);
			throw 1;
		} catch (int e) {
			std::cout << "Erro add(hashObject<T> value)" << std::endl;
		}
	}
	bool search(T value) {
		return false;
	}
	void set(int pos);
	void remove(int pos);
	void print(int pos) {

	}
	void printall();

};

#endif /* HASHTABLE_H_ */
