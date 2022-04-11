#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define MAX_DISPATCH_NUM 30
#define MIN_THREAD_ID 0
#define MAX_THREAD_ID 9
#define BADDEST_THREAD_TIME 3750 // 2500 * 1.2 = 3750
//预定义线程
void* thread0(void*);
void* thread1(void*);
void* thread2(void*);
void* thread3(void*);
void* thread4(void*);
void* thread5(void*);
void* thread6(void*);
void* thread7(void*);
void* thread8(void*);
void* thread9(void*);
void * dispatcher(void *arg);
pthread_t tid0, tid1, tid2 ,tid3 ,tid4 ,tid5, tid6, tid7, tid8, tid9, tid10;
pthread_mutex_t mutex0 = PTHREAD_MUTEX_INITIALIZER,mutex1 = PTHREAD_MUTEX_INITIALIZER,mutex2 = PTHREAD_MUTEX_INITIALIZER,mutex3 = PTHREAD_MUTEX_INITIALIZER,mutex4 = PTHREAD_MUTEX_INITIALIZER,mutex5 = PTHREAD_MUTEX_INITIALIZER,mutex6 = PTHREAD_MUTEX_INITIALIZER,mutex7 = PTHREAD_MUTEX_INITIALIZER,mutex8 = PTHREAD_MUTEX_INITIALIZER, mutex9 = PTHREAD_MUTEX_INITIALIZER;
int dispatch_list[MAX_DISPATCH_NUM] = {};
pthread_mutex_t dis;
//用于分配CPU
char str1[] = "taskset -pc 0 ";
char str2[] = "taskset -pc 1 ";

int endFlag = 1;

int main() {
    system("trace-cmd start -p function");
    //初始化调度器信号量
    pthread_mutex_init(&dis,NULL);
    //初始化各线程信号量
    pthread_mutex_lock(&mutex0);
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    pthread_mutex_lock(&mutex3);
    pthread_mutex_lock(&mutex4);
    pthread_mutex_lock(&mutex5);
    pthread_mutex_lock(&mutex6);
    pthread_mutex_lock(&mutex7);
    pthread_mutex_lock(&mutex8);
    pthread_mutex_lock(&mutex9);
    //创建各计算线程
    pthread_create(&tid0, NULL, thread0, NULL);
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_create(&tid3, NULL, thread3, NULL);
    pthread_create(&tid4, NULL, thread4, NULL);
    pthread_create(&tid5, NULL, thread5, NULL);
    pthread_create(&tid6, NULL, thread6, NULL);
    pthread_create(&tid7, NULL, thread7, NULL);
    pthread_create(&tid8, NULL, thread8, NULL);
    pthread_create(&tid9, NULL, thread9, NULL);
    //创建调度器线程
    pthread_create(&tid10, NULL, dispatcher, dispatch_list);

    pthread_join(tid0,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    pthread_join(tid5,NULL);
    pthread_join(tid6,NULL);
    pthread_join(tid7,NULL);
    pthread_join(tid8,NULL);
    pthread_join(tid9,NULL);

    system("trace-cmd stop");
}

void * dispatcher(void *arg) {
    char str[20];
    //输出调度器信息
    sprintf(str, "%s%d", str2, syscall(SYS_gettid));
    system(str);
    printf("************************************\n");
    printf("Dispatcher PID is: %d\n", syscall(SYS_gettid));
    printf("************************************\n");
    while(endFlag){
        int listLength = 0;
        //读取调度数组
        while(1) {
            int currentNum;
            scanf("%d", &currentNum);
            //程序结束
            if(currentNum == -2) {
                endFlag = 0;
                break;
            }
            //输入结束
            if(currentNum == -1) {
                printf("Dispatcher start!\n");
                break;
            }
            //调度列表满
            if(listLength == MAX_DISPATCH_NUM) {
                printf("List is full! Dispatcher start...\n");
                break;
            }
            //输入错误
            if(currentNum > MAX_THREAD_ID || currentNum < MIN_THREAD_ID) {
                printf("Input error!\n");
                continue;
            }
            dispatch_list[listLength++] = currentNum;
        }
        if(!endFlag)
            break;
        //计算调度器最差周期
        long BADDEST_DISPATCH_TIME = BADDEST_THREAD_TIME * listLength * 1.2;
        printf("************************************\n");
        printf("Baddest dispatch time is %ld\n", BADDEST_DISPATCH_TIME);
        printf("************************************\n");
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        for(int i = 0; i < listLength; i++) {
            //从传入的数组中取出需要执行的线程编号
            pthread_mutex_lock(&dis);
            printf("************************************\n");
            printf("Dispatching... Next thread is %d\n", dispatch_list[i]);
            printf("************************************\n");
            //释放对应线程信号量
            switch (dispatch_list[i]) {
                case 0: pthread_mutex_unlock(&mutex0); break;
                case 1: pthread_mutex_unlock(&mutex1); break;
                case 2: pthread_mutex_unlock(&mutex2); break;
                case 3: pthread_mutex_unlock(&mutex3); break;
                case 4: pthread_mutex_unlock(&mutex4); break;
                case 5: pthread_mutex_unlock(&mutex5); break;
                case 6: pthread_mutex_unlock(&mutex6); break;
                case 7: pthread_mutex_unlock(&mutex7); break;
                case 8: pthread_mutex_unlock(&mutex8); break;
                case 9: pthread_mutex_unlock(&mutex9); break;
            }
        }
        //模拟sleep usleep
        while(timeUsage < BADDEST_DISPATCH_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("************************************\n");
        printf("Dispatcher time usage is: %ld\n", timeUsage);
        printf("************************************\n");
    }
    pthread_mutex_unlock(&mutex0);
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex3);
    pthread_mutex_unlock(&mutex4);
    pthread_mutex_unlock(&mutex5);
    pthread_mutex_unlock(&mutex6);
    pthread_mutex_unlock(&mutex7);
    pthread_mutex_unlock(&mutex8);
    pthread_mutex_unlock(&mutex9);
}

void* thread0(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex0);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 0\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        //模拟sleep
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread1(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex1);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 1\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread2(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex2);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 2\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread3(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex3);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 3\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread4(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex4);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 4\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread5(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex5);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 5\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread6(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex6);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 6\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread7(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex7);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 7\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread8(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex8);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 8\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}

void* thread9(void* arg) {
    char str[20];
    sprintf(str, "%s%d", str1, syscall(SYS_gettid));
    system(str);
    while(endFlag) {
        pthread_mutex_lock(&mutex9);
        if(!endFlag)
            break;
        struct timeval start, end;
        long timeUsage = 0;
        gettimeofday(&start, NULL);
        printf("Enter thread 9\n");
        printf("PID is: %d\n", syscall(SYS_gettid));
        printf("Calculating...\n");
        double calc = 0.00;
        for(int i = 0; i < 1000000; i++) {
            calc += 1.01;
        }
        printf("Done!\n");
        while(timeUsage < BADDEST_THREAD_TIME) {
            gettimeofday(&end, NULL);
            timeUsage = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        }
        printf("Time usage is: %ld\n", timeUsage);
        pthread_mutex_unlock(&dis);
    }
}