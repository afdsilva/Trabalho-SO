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

/**
 * Classe dummy que servira como container para as outras classes que usam threads
 */
template <class T>
class HashTableAction : public Thread {
public:
	HashTableAction() {}
	virtual void * run() = 0;
};
/**
template <class T>
class HashTableAdd : public HashTableAction<T> {
	Mutex & m_mutex;
	CondVar & m_cond;
	std::vector<hashObject<T> > & hashtable;
	hashObject<T> & obj;
	int pos;
public:
	HashTableAdd(Mutex & mutex,CondVar & cond, std::vector<hashObject<T> > & t,hashObject<T> & o, int p) : m_mutex(mutex), m_cond(cond), hashtable(t), obj(o), pos(p) {}
	void *run() {
		std::cout << "HashTableAdd key: " << obj.key << " value: " << obj.value;
		std::cout << std::endl;
		if(lock) m_cond.wait();

		m_mutex.lock();
		lock = true;
		hashtable[pos] = obj;
		lock = false;
		m_mutex.unlock();
		m_cond.signal();		return NULL;
	}
};
**/
#endif /* HASHTABLETHREADED_H_ */
