/*
 * libhash.h
 *
 *      Author: Andre Silva
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
#include "Thread.h"
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
		this->value = 0;
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
template<>
class hashObject<string> {
public:
	int key;
	string value;

	hashObject() {
		this->key = -1;
		this->value.clear();
	}
	hashObject(int key, string value) {
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

class ThreadList {
private:
	Mutex threadMutex;
	CondVar threadCond;

public:
	std::vector<pthread_t *> threads;
	int activeThreads;
	int maxThreads;

	ThreadList(int nr) {
		maxThreads = nr;
		activeThreads = 0;
		threadCond = CondVar(threadMutex);
	}
	int create(void *(*__start_routine) (void *), void * __arg) {
		int retorno = -1;
		try {

			if (activeThreads >= maxThreads) {
				//std::cout << "Max threads reached, waiting for a thread being freed" << std::endl;
				threadCond.wait();
				//std::cout << "Thread freed" << std::endl;
			} else {
				//std::cout << "creating new thread" << std::endl;
			}
			pthread_t * thread = new pthread_t();
			int retorno = pthread_create(thread, NULL, __start_routine,__arg);
			if (retorno == 0) {
				threadMutex.lock();
				activeThreads++;
				threads.push_back(thread);
				retorno = threads.size() - 1;
				threadMutex.unlock();

			} else
				throw 1;

		} catch (int e) {
			std::cout << "ERROR: create Thread" << std::endl;
			retorno = -1;
		}
		return retorno;
	}
	void freeThread() {
		if (activeThreads > 0)
			activeThreads--;
		threadCond.signal();
	}
	void join(int pos) {
		pthread_join(*threads[pos], NULL);
		//freeThread();
	}
	void joinAll() {
		for (int i = 0; i < (int) threads.size(); i++) {
			join(i);
		}
		threads.clear();
	}
	Mutex & getThreadMutex() {
		return threadMutex;
	}
	CondVar & getThreadCondVar() {
		return threadCond;
	}
};

template<class T>
class HashTable {
private:
	Mutex * aMutex;
	CondVar * aCond;
	ThreadList * threadlist;

	std::vector<hashObject<T> > table;

	int tableSize;
	int actualSize;
	int hashPrime;
	int hashBlocks;
	int hashLoad;
	int hashRecord;

	int C1;
	int C2;
	int colType;

	/**
	 * Funcao hash utilizada, simples
	 */
	int hash(T value) {
		int retorno = 0;
		if (typeid(T) == typeid(string)) {
			string tmp;
			tmp = value;
			retorno = (int) tmp[0];
		} else {
			retorno = *((int *) &value);
		}
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
	HashTable(int size, int blocks, int threads) {
		try {

			if (size % blocks != 0)
				throw 1;
			hashObject<T> nObj;
			aMutex = new Mutex[blocks];
			aCond = new CondVar[blocks];
			for (int i = 0; i < blocks; i++) {
				aCond[i] = CondVar(aMutex[i]);
			}
			threadlist = new ThreadList(threads);
			table.resize(size,nObj);
			tableSize = size;
			actualSize = 0;
			hashPrime = proxPrimo(size);
			hashBlocks = blocks;
			hashLoad = 0.75;
			hashRecord = 0;

			colType = 0;
			C1 = 1;
			C2 = 2;

		} catch (int e) {
			std::cout << "Nao foi possivel criar a tabela hash, numero de blocos e tamanho da tabela não são multiplos." << std::endl;
		}
	}
	virtual ~HashTable() {
		//join em todas threads forçando todas threads a terminarem.
		threadlist->joinAll();
		for(int i = 0; i < hashBlocks; i++) {
			//libera todos mutex e variaveis de condicao;
			aMutex[i].unlock();
			aCond[i].signal();
			//libera as variaveis
			aMutex[i].~Mutex();
			aCond[i].~CondVar();
		}
	}
	hashObject<T> & get(int pos) {
		int secao = pos / (tableSize / hashBlocks);
		aMutex[secao].lock();
		hashObject<T> & retorno = table[pos];
		aMutex[secao].unlock();
		return retorno;
	}

	bool _add(T value) {
		int pos;
		pos = hash(value);
		hashObject<T> nObj(pos,value);
		return _add(nObj);
	}
	bool _add(hashObject<T> & nObj) {
		int nPos, k, secao;
		try {
			if (actualSize >= tableSize)
				throw 2;
			nPos = nObj.key;
			if (search(nObj) == 0) {
				k = 1;
				while(collision(nPos)) {

					if (colType == 0)
						nPos = probingLinear(k,nObj.key);
					else if (colType == 1)
						nPos = probingQuadratic(k,nObj.key);
					k++;
					if (k > tableSize)
						throw 2;
				}
				secao = nPos / (tableSize / hashBlocks);
				aMutex[secao].lock();
				table[nPos] = nObj;
				aMutex[secao].unlock();
				actualSize++;
			} else {
				throw 1;
			}
			return true;

		} catch (int e) {
			if (e == 1)
				std::cout << "Registro ja existe" << std::endl;
			else if (e == 2)
				std::cout << "Tabela cheia" << std::endl;
			else
				std::cout << "Erro _add(hashObject<T> nObj)" << std::endl;
			return false;
		}
	}

	int add(T value);
	int addString(string value);
	/**
	 * Busca na tabela hash o objeto e verifica colisoes
	 * retorno 0: nao foi encontrado o objeto, valor nao encontrado
	 * retorno 1: objeto foi encontrado sem colisao, valor e chave iguais
	 * retorno 2: objeto foi encontrado com colisao, chave diferente
	 */
	int search(hashObject<T> & obj) {
		int pos = obj.key;
		int nPos = pos;
		int secao;
		hashObject<T> nObj = table.at(pos);

		if ((obj == nObj) == 1) return 1;
		for (int k = 1; k < tableSize; k++) {
			if (colType == 0) {
				//busca linear
				nPos = probingLinear(k, pos);
			} else if (colType == 1) {
				//busca quadratica
				nPos = probingQuadratic(k, pos);
			}
			secao = nPos / (tableSize / hashBlocks);
			aMutex[secao].lock();
			nObj = table[nPos];
			aMutex[secao].unlock();

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
		int secao = pos / (tableSize / hashBlocks);
		aMutex[secao].lock();
		hashObject<T> tObj = table[pos];
		aMutex[secao].unlock();
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
	bool _set(T value) {
		try {
			int pos = hash(value);
			hashObject<T> nObj(pos,value);
			if (search(nObj) == 0) {
				int secao = pos / (tableSize / hashBlocks);
				aMutex[secao].lock();
				table[pos] = nObj;
				aMutex[secao].unlock();
			} else {
				throw 1;
			}
			return true;
		} catch (int e) {
			if (e == 1)
				std::cout << "Registro ja existe" << std::endl;
			else
				std::cout << "Erro set(T value, int pos)" << std::endl;
			return false;
		}
	}
	int set(T value);
	/**
	 * Sobrepoe a posicao com um objeto vazio
	 */
	void remove(int pos) {
		int secao = pos / (tableSize / hashBlocks);
		aMutex[secao].lock();
		hashObject<T> nObj;
		table[pos] = nObj;
		actualSize--;
		aMutex[secao].unlock();
	}
	void print(int pos) {
		int secao = pos / (tableSize / hashBlocks);
		aMutex[secao].lock();
		hashObject<T> nObj = table.at(pos);
		aMutex[secao].unlock();
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

	const std::vector<hashObject<T> >& getTable() const {
		return table;
	}
	ThreadList & getThreadList() {
		return * threadlist;
	}
	void joinThread(int pos) {
		threadlist->join(pos);
	}
	void joinThreads() {
		threadlist->joinAll();
	}
};

template<class T>
class DataWrapper {
public:
	T value;
	HashTable<T> & ht;
	DataWrapper(T v, HashTable<T> & h) : value(v), ht(h) {}
};

template<class T> void * addWrapper(void * ptr);
template<class T> void * setWrapper(void * ptr);

template<class T>
int HashTable<T>::add(T value) {

	DataWrapper<T> * t = new DataWrapper<T>(value,*this);
	return threadlist->create(addWrapper<T>,(void *) t);

}

template<class T>
int HashTable<T>::addString(string value) {

	DataWrapper<T> * t = new DataWrapper<T>(value,*this);
	return threadlist->create(addWrapper<T>,(void *) t);

}

template<class T>
int HashTable<T>::set(T value) {

	DataWrapper<T> * t = new DataWrapper<T>(value,*this);
	return threadlist->create(setWrapper<T>,(void *) t);

}

template<class T>
void * addWrapper(void * ptr) {
	DataWrapper<T> dt = *((DataWrapper<T> *) ptr);
	HashTable<T> & hashTable = dt.ht;

	T value = dt.value;
	hashTable._add(value);

	ThreadList & threadlist = hashTable.getThreadList();
	threadlist.freeThread();
	return NULL;
}

template<class T>
void * setWrapper(void * ptr) {
	DataWrapper<T> dt = *((DataWrapper<T> *) ptr);
	HashTable<T> & hashTable = dt.ht;

	T value = dt.value;
	hashTable._set(value);

	ThreadList & threadlist = hashTable.getThreadList();
	threadlist.freeThread();
	return NULL;
}

#endif /* HASHTABLE_H_ */
