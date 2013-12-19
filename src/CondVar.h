/*
 * CondVar.h
 *
 *  Created on: Dec 19, 2013
 *      Author: andref
 */

#ifndef CONDVAR_H_
#define CONDVAR_H_

#include "Mutex.h"

class CondVar {
	pthread_cond_t m_cond;
	Mutex m_lock;

public:
	CondVar();
	virtual ~CondVar();

	int wait();
	int signal();
	int broadcast();
};

#endif /* CONDVAR_H_ */
