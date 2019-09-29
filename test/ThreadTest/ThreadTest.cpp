#include <cstdio>
#include <thread>
#include <mutex>
#include "Mutex.hpp"

std::mutex _mutex;
static volatile int _idMutex = 0;
CSMutex _csMutex;
static volatile int _idCSMutex = 0;
SRWMutex _srwMutex;
static volatile int _idSRWMutex = 0;

void threadMutex(int id)
{
    int _id;
    do
    {
        _mutex.lock();
        _id = _idMutex;
        _mutex.unlock();
        std::this_thread::yield();
    } while(_id != id);

    ++_idMutex;

    printf("Thread(%d) Mutex\n", id);
}

void threadCSMutex(int id)
{
    int _id;
    do
    {
        _csMutex.lock();
        _id = _idCSMutex;
        _csMutex.unlock();
        std::this_thread::yield();
    } while(_id != id);

    ++_idCSMutex;

    printf("Thread(%d) CSMutex\n", id);
}

void threadSRWMutex(int id)
{
    int _id;
    do
    {
        _srwMutex.lock();
        _id = _idSRWMutex;
        _srwMutex.unlock();
        std::this_thread::yield();
    } while(_id != id);

    ++_idSRWMutex;

    printf("Thread(%d) SRWMutex\n", id);
}

#define THREAD_CNT 10

int main()
{
    std::thread* threads;

    {
        threads = new std::thread[THREAD_CNT];

        for(int i = 0; i < THREAD_CNT; ++i)
        {
            threads[i] = std::thread(threadMutex, i);
        }

        for(int i = 0; i < THREAD_CNT; ++i)
        {
            threads[i].join();
        }

        delete[] threads;
    }

    putc('\n', stdout);

    {
        threads = new std::thread[THREAD_CNT];

        for(int i = 0; i < THREAD_CNT; ++i)
        {
            threads[i] = std::thread(threadCSMutex, i);
        }

        for(int i = 0; i < THREAD_CNT; ++i)
        {
            threads[i].join();
        }

        delete[] threads;
    }

    putc('\n', stdout);

    {
        threads = new std::thread[THREAD_CNT];

        for(int i = 0; i < THREAD_CNT; ++i)
        {
            threads[i] = std::thread(threadSRWMutex, i);
        }

        for(int i = 0; i < THREAD_CNT; ++i)
        {
            threads[i].join();
        }

        delete[] threads;
    }

    getchar();
    return 0;
}