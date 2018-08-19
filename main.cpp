// thread 1: 发布消息到管道
// thread 2: 接收消息到队列
// thread 3: 打印消息
// 不输出更容易引发 Segmentation fault
// 因为输出占用大部分时间，使得冲突概率减小

#include <iostream>
#include <unistd.h>
#include <queue>
#include <pthread.h>

using namespace std;

queue<char> msg_pipe, que;

void* send_msg(void *) {
    char c = 'A';
    while (1) {
        msg_pipe.push(c);
        // cout << "Send: " << c << endl;
        ++c;
        if (c > 'Z')
            c = 'A';
    }
    return NULL;
}

void* rcv_msg(void *) {
    char c;
    while (1) {
        while (!msg_pipe.empty()) {
            // 使用线程撤销构造临界区
            c = msg_pipe.front();
            msg_pipe.pop();
            que.push(c);
            // cout << "Get: " << c << endl;
        }
    }
    return NULL;
}

void* print_msg(void *) {
    char c;
    while (1) {
        while (!que.empty()) {
            c = que.front();
            que.pop();
            // cout << "Print: " << c << endl;
        }
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
