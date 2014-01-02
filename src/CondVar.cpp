/*
 * CondVar.cpp
 *
 *  Created on: Dec 19, 2013
 *      Author: andref
 */

#include "CondVar.h"

CondVar::CondVar() {
	m_lock = Mutex();
	pthread_cond_init(&m_cond, NULL);
}
CondVar::CondVar(Mutex & mutex) : m_lock(mutex) {
	pthread_cond_init(&m_cond, NULL);
}

CondVar::~CondVar() {
	pthread_cond_destroy(&m_cond);
}

int CondVar::wait() {
	return pthread_cond_wait(&m_cond, &(m_lock.m_mutex));
}

int CondVar::signal() {
	return pthread_cond_signal(&m_cond);
}

int CondVar::broadcast() {
	return pthread_cond_broadcast(&m_cond);
}
