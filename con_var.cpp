// thread 1: 发布消息到管道
// thread 2: 接收消息到队列
// thread 3: 打印消息

#include <iostream>
#include <unistd.h>
#include <queue>
#include <pthread.h>
#include <mutex>
#include <condition_variable>

using namespace std;

queue<char> msg_pipe, que;

void* send_msg(void *) {
    char c = 'A';
    while (1) {
        msg_pipe.push(c++);
        if (c > 'Z')
            c = 'A';
        cout << "Send: " << c << endl;
        usleep(100000);
    }
    return NULL;
}

void* rcv_msg(void *) {
    while (1) {
        while (!msg_pipe.empty()) {
            // 使用线程撤销构造临界区
            que.push(msg_pipe.front());
            msg_pipe.pop();
        }
        usleep(100);
    }
    return NULL;
}

void* print_msg(void *) {
    while (1) {
        while (!que.empty()) {
            cout << "Print: " << que.front() << endl;
            que.pop();
        }
        usleep(100);
    }
    return NULL;
}

int main(void) {
    pthread_t sender, rcver, printer;
    pthread_create(&sender, NULL, send_msg, NULL);
    pthread_create(&rcver, NULL, rcv_msg, NULL);
    pthread_create(&printer, NULL, print_msg, NULL);

    pthread_join(sender, NULL);
    pthread_join(rcver, NULL);
    pthread_join(printer, NULL);
    return 0;
}
