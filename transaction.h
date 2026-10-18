#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "account.h"

#define MAX_QUEUE_TRANSACTIONS 100

struct QueueTransaction
{
    int transactionId;
    char type[20];
    int accountNumber;
    int receiverAccountNumber;
    float amount;
};

int isQueueEmpty();
int isQueueFull();
void enqueue(char type[], int accountNumber, int receiverAccountNumber, float amount);
void dequeue();
void displayQueue();
float readAmount();
struct Account *getAccount(char prompt[]);
void deposit();
void withdraw();
void transfer();

#endif
