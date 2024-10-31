#include "SingletonBase.h"

SingletonBase* SingletonBase::instance = nullptr;
pthread_mutex_t SingletonBase::mutex = PTHREAD_MUTEX_INITIALIZER;

SingletonBase* SingletonBase::getInstance() {
    // Double-checked locking
    if (instance == nullptr) { //check if the instance exist
        pthread_mutex_lock(&mutex);
        if (instance == nullptr) {
            instance = new SingletonBase();  
        }
        pthread_mutex_unlock(&mutex);
    }
    return instance;
}
