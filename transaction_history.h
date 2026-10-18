
#ifndef TRANSACTION_HISTORY_H
#define TRANSACTION_HISTORY_H

#include "account.h"

#define MAX_TRANSACTIONS 1000

typedef struct Transaction
{
    int transactionID;
    int accountNumber;
    char type[20];
    float amount;
    char status[20];
    struct Transaction *next;
} Transaction;

/* Transaction history */
void recordTransaction(int accountNumber,
                       const char type[],
                       float amount);

void displayTransactionHistory(void);

/* Stack operations */
void pushTransaction(Transaction *transaction);
Transaction *popTransaction(void);

/* Transaction reversal */
void reverseLatestTransaction(void);

/* File handling */
void saveTransactionHistory(void);
void loadTransactionHistory(void);
void saveAccountRecords(void);
void loadAccountRecords(void);

/* Memory cleanup */
void freeTransactionHistory(void);

#endif
