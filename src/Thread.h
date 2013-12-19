#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "CondVar.h"

class Thread : public CondVar {
	CondVar m_cond;
public:
    Thread();
    virtual ~Thread();

    int start();
    int join();
    int detach();
    pthread_t self();

    virtual void *run() = 0;
private:
    pthread_t   m_tid;
    int         m_running;
    int         m_detached;

};

#endif // THREAD_H
