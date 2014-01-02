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
#include <cmath>

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
		//this->value = NULL;
	}
	hashObject(int key, O value) {
		this->key = key;
		this->value = value;
	}
	/**
	 * Compara dois objetos e retorna:
	 * 0: caso sejam diferentes
	 * 1: caso sejam iguais, chaves e valores
	 * 2: caso valores sejam iguais, porem chaves diferentes (quando uma funcao hash eh reaplicada a chave sera diferente)
	 * 3: caso chaves sejam iguais
	 **/
	int operator==(const hashObject & obj) {
		if (obj.key == this->key && obj.value == this->value) return 1;
		else if (obj.value == this->value) return 2;
		else if (obj.key == this->key) return 3;
		return 0;
	}
};

template<class T>
class HashTable {
private:
	CondVar * m_Cond;

	std::vector<hashObject<T> > table;

	int tableSize;
	int hashPrime;
	int hashBlocks;
	int hashLoad;
	int hashRecord;

	int C1;
	int C2;
	int colType;

	int hash(T value) {
		int retorno = 0;

		string tmp;
		tmp = value;
		retorno = (int) tmp[0];

		return retorno % tableSize;
	}
	int probingQuadratic(int k, int pos) {
		int retorno = -1;
		if (pos > -1) {
			if (k >= tableSize) return -1;
			retorno = (pos + (C1 * k) + (int) pow(C2 * k,2)) % tableSize;
		}
		return retorno;
	}
	int probingLinear(int k, int pos) {
		int retorno = (pos + (C1 * k)) % tableSize;
		return retorno;
	}
	int proxPrimo(int n) {
		int retorno = 0;
		for (int i = n; i >= 2; i--) {
			if ( n % i == 0) retorno++;
			if (retorno > 2) return proxPrimo(n-1);
		}
		return retorno;
	}
public:
	/**
	 * Construtor da classe HashTable, o tipo é dado na criação da tabela
	 */
	HashTable(int size, int blocks) {
		try {
			if (size % blocks != 0)
				throw 1;
			hashObject<T> nObj;
			//m_Cond = new CondVar[blocks];
			table.resize(size,nObj);
			tableSize = size;
			hashPrime = proxPrimo(size);
			hashBlocks = blocks;
			hashLoad = 0.75;
			hashRecord = 0;

			colType = 0;
			C1 = 1;
			C2 = 2;

		} catch (int e) {
			std::cout << "Erro HashTable" << std::endl;
		}
	}
	virtual ~HashTable() {

	}
	hashObject<T> & get(int pos) {
		return table[pos];
	}
	void add(T value) {
		int pos, nPos, k;
		try {
			pos = hash(value);
			nPos = pos;
			hashObject<T> nObj(pos,value);

			if (search(nObj) == 0) {
				k = 1;
				while(collision(nPos)) {

					if (colType == 0)
						nPos = probingLinear(k,pos);
					else if (colType == 1)
						nPos = probingQuadratic(k,pos);
					k++;
					if (k > tableSize)
						throw 2;
				}
				table[nPos] = nObj;
			} else {
				throw 1;
			}
			//table[pos] = nObj;

		} catch (int e) {
			if (e == 1)
				std::cout << "Registro ja existe" << std::endl;
			if (e == 2)
				std::cout << "Tabela cheia" << std::endl;
			else
				std::cout << "Erro add(T value)" << std::endl;
		}
	}
	/**
	 * Busca na tabela hash o objeto e verifica colisoes
	 * retorno 0: nao foi encontrado o objeto, valor nao encontrado
	 * retorno 1: objeto foi encontrado sem colisao, valor e chave iguais
	 * retorno 2: objeto foi encontrado com colisao, chave diferente
	 */
	int search(hashObject<T> & obj) {
		int pos = obj.key;
		int nPos = pos;
		hashObject<T> nObj;

		hashObject<T> tObj = table.at(pos);
		if ((obj == tObj) == 1) return 1;
		for (int k = 1; k < tableSize; k++) {
			if (colType == 0) {
				//busca linear
				nPos = probingLinear(k, pos);
			} else if (colType == 1) {
				//busca quadratica
				nPos = probingQuadratic(k, pos);
			}
			nObj = table[nPos];
			if ((nObj == obj) == 2) return 2;

		}
		return 0;
	}
	/**
	 * Verifica se posicao esta ocupada
	 * 0: posicao vazia
	 * 1: jah existe um objeto
	 * -1: erro
	 **/
	int collision(int pos) {
		int retorno = -1;
		hashObject<T> tObj = table[pos];
		if (pos > -1) {
			retorno = 0;
			if (tObj.key != -1) retorno = 1;
		}
		return retorno;
	}
	/*
	 * Seta o valor, calculando o hash para posicao
	 * SOBREPOE o valor anterior, caso exista ignorando colisões.
	 */
	void set(T value) {
		try {
			int pos = hash(value);
			hashObject<T> nObj(pos,value);
			if (search(nObj) == 0) {
				table[pos] = nObj;
			} else {
				throw 1;
			}
		} catch (int e) {
			if (e == 1)
				std::cout << "Registro ja existe" << std::endl;
			else
				std::cout << "Erro set(T value, int pos)" << std::endl;
		}

	}
	/**
	 * Sobrepoe a posicao com um objeto vazio
	 */
	void remove(int pos) {
		hashObject<T> nObj;
		table[pos] = nObj;
	}
	void print(int pos) {
		hashObject<T> nObj = table.at(pos);
		std::cout << pos << ": key = " << nObj.key << " value = " << nObj.value;
		std::cout << std::endl;
	}
	void printall() {
		for (int i = 0; i < tableSize; i++) {
			print(i);
		}
	}

	int getC1() const {
		return C1;
	}

	void setC1(int c1) {
		C1 = c1;
	}

	int getC2() const {
		return C2;
	}

	void setC2(int c2) {
		C2 = c2;
	}

	int getColType() const {
		return colType;
	}

	void setColType(int colType) {
		this->colType = colType;
	}
};

#endif /* HASHTABLE_H_ */
