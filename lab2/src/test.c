#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

void* addMail() {
    for (int i = 0; i < 500000; ++i) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[4];
    int i = 0;
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; ++i) {
        if (pthread_create(&th[i], NULL, &addMail, NULL)) {
            return i;
        }
    }
    for (int i = 0; i < 4; ++i) {
        if (pthread_join(th[i], NULL)) {
            return i;
        }
    }

    pthread_mutex_destroy(&mutex);
    printf("Mails = %d\n", mails);
}