/*
 * HashTableThreaded.h
 *
 *  Created on: Jan 2, 2014
 *      Author: andref
 */

#ifndef HASHTABLETHREADED_H_
#define HASHTABLETHREADED_H_

#include "Mutex.h"
#include "CondVar.h"
#include "Thread.h"

#include <vector>
#include <stdio.h>
#include <iostream>

class HashTableThreaded {
	std::vector<int> numeros;
public:
	static bool lock;
	HashTableThreaded(int size);
	std::vector<int> & getNumeros();
	virtual ~HashTableThreaded();
	void printall();
};

class HashTableAdd : public Thread {
	Mutex & m_mutex;
	CondVar & m_cond;
	std::vector<int> & numeros;
	int pos;
	int valor;
public:
	HashTableAdd(Mutex & mutex,CondVar & cond, std::vector<int> & num,int v,int p) : m_mutex(mutex), m_cond(cond), numeros(num), pos(p), valor(v) {}
	void *run() {
		if(HashTableThreaded::lock) m_cond.wait();

		m_mutex.lock();
		HashTableThreaded::lock = true;
		numeros[pos] = numeros[pos] + valor;
		HashTableThreaded::lock = false;
		m_mutex.unlock();
		m_cond.signal();
		return NULL;
	}
};

class HashTableDelete : public Thread {
	CondVar & m_cond;
	std::vector<int> & numeros;
	int pos;
public:
	HashTableDelete(CondVar & cond, std::vector<int> & num, int p) : m_cond(cond), numeros(num), pos(p) {}
	void *run() {
		std::cout << "Deletando" << std::endl;
		return NULL;
	}
};
#endif /* HASHTABLETHREADED_H_ */
