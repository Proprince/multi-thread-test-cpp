// thread 1: 发布消息到管道
// thread 2: 接收消息到队列
// thread 3: 打印消息

#include <iostream>
#include <unistd.h>
#include <queue>
#include <pthread.h>
#include <mutex>

using namespace std;

queue<char> msg_pipe, que;
pthread_mutex_t mutex_pipe, mutex_que;

void* send_msg(void *) {
    char c = 'A';
    while (1) {
        pthread_mutex_lock(&mutex_pipe);
        msg_pipe.push(c);
        pthread_mutex_unlock(&mutex_pipe);
        // cout << "Send: " << c << endl;
        ++c;
        if (c > 'Z')
            c = 'A';
    }
    return NULL;
}

void* rcv_msg(void *) {
    char c;
    int flag;
    while (1) {
        flag = 0;
        pthread_mutex_lock(&mutex_pipe);
        while (!msg_pipe.empty()) {
            c = msg_pipe.front();
            msg_pipe.pop();
            flag = 1;
        }
        pthread_mutex_unlock(&mutex_pipe);

        if (flag) {
            pthread_mutex_lock(&mutex_que);
            que.push(c);
            pthread_mutex_unlock(&mutex_que);
            // cout << "Get: " << c << endl;
        }       
    }
    return NULL;
}

void* print_msg(void *) {
    char c;
    int flag;
    while (1) {
        flag = 0;
        pthread_mutex_lock(&mutex_que);
        while (!que.empty()) {
            c = que.front();
            que.pop();
            flag = 1;
        }
        pthread_mutex_unlock(&mutex_que);

        if (flag) {
            // cout << "Print: " << c << endl;
        }
    }
    return NULL;
}

int main(void) {
    pthread_t sender, rcver, printer;

    pthread_mutex_init(&mutex_pipe, NULL);
    pthread_mutex_init(&mutex_que, NULL);

    pthread_create(&sender, NULL, send_msg, NULL);
    pthread_create(&rcver, NULL, rcv_msg, NULL);
    pthread_create(&printer, NULL, print_msg, NULL);

    pthread_join(sender, NULL);
    pthread_join(rcver, NULL);
    pthread_join(printer, NULL);

    pthread_mutex_destroy(&mutex_pipe);
    pthread_mutex_destroy(&mutex_que);
    return 0;
}
