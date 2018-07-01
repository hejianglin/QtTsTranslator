#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <QQueue>
#include <QMutexLocker>

template <class T>
class TaskQueue
{
public:


private:
    QMutex m_cMutext;
    QQueue<T> m_queueTask;
};

#endif // TASKQUEUE_H
