#pragma once
#include <iostream>
#include <queue>
#include <future>
#include <condition_variable>
#include <functional>
#include <vector>

using namespace std;

void taskFunc(int id, int delay);
typedef function<void()> task_type;
typedef void (*FuncType) (int*, int, int, bool, promise<void>& p);

template<class T> 
class BlockedQueue
{
public:
    void push(T& item) 
    {
        lock_guard <mutex> l(m_locker);
        m_task_queue.push(item);
        m_event_holder.notify_one();
    }
    void pop(T& item) 
    {
        unique_lock <mutex> l(m_locker);
        if (m_task_queue.empty())
            m_event_holder.wait(l, [this] {return !m_task_queue.empty(); });
        item = m_task_queue.front();
        m_task_queue.pop();
    }
    bool fast_pop(T& item) 
    {
        unique_lock <mutex> l(m_locker);
        if (m_task_queue.empty())
            return false;
        item = m_task_queue.front();
        m_task_queue.pop();
        return true;
    }
private:
    mutex m_locker;
    queue <T> m_task_queue;
    condition_variable m_event_holder;
};

class ThreadPool {
public:
    ThreadPool();
    void start();
    void stop();
    void push_task(FuncType f, int *a, int l, int r, bool t, promise<void>& p);
    void threadFunc(int qindex);
private:
    int m_thread_count;
    vector <thread> m_threads;
    vector <BlockedQueue<task_type>> m_thread_queues;
    int m_index;
};

class RequestHandler {
public:
    RequestHandler();
    ~RequestHandler();
    void pushRequest(FuncType f, int* a, int l, int r, bool t, promise<void>& p);
private:
    ThreadPool m_tpool;
};