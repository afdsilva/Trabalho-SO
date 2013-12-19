/*
 * Mutex.h
 *
 *  Created on: Dec 19, 2013
 *      Author: andref
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>

class Mutex {
	friend class CondVar;
	pthread_mutex_t m_mutex;
public:
	Mutex();
	virtual ~Mutex();
	int lock();
	int trylock();
	int unlock();
};

#endif /* MUTEX_H_ */
