
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transaction_history.h"

/* Linked list for transaction history */
static Transaction *historyHead = NULL;
static Transaction *historyTail = NULL;

/* Stack for recent eligible transactions */
static Transaction *undoStack[MAX_TRANSACTIONS];
static int top = -1;

/* Generate unique transaction IDs */
static int nextTransactionID = 1;

/* Check whether a transaction can be reversed */
static int isReversibleType(const char type[])
{
    return strcmp(type, "Deposit") == 0 ||
           strcmp(type, "Withdrawal") == 0;
}

/* Add a transaction to the history linked list */
static void appendHistory(Transaction *t)
{
    t->next = NULL;

    if (historyHead == NULL)
    {
        historyHead = historyTail = t;
    }
    else
    {
        historyTail->next = t;
        historyTail = t;
    }
}

/* Add a transaction to the stack */
void pushTransaction(Transaction *transaction)
{
    if (top == MAX_TRANSACTIONS - 1)
    {
        printf("Reversal stack is full.\n");
        return;
    }

    undoStack[++top] = transaction;
}

/* Remove and return the top transaction */
Transaction *popTransaction(void)
{
    if (top == -1)
    {
        return NULL;
    }

    return undoStack[top--];
}

/* Record a completed transaction */
void recordTransaction(int accountNumber,
                       const char type[],
                       float amount)
{
    if (accountNumber <= 0 || amount <= 0)
    {
        printf("Invalid transaction details.\n");
        return;
    }

    if (!isReversibleType(type) &&
        strcmp(type, "Transfer") != 0)
    {
        printf("Unsupported transaction type.\n");
        return;
    }

    Transaction *t = malloc(sizeof(Transaction));

    if (t == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    t->transactionID = nextTransactionID++;
    t->accountNumber = accountNumber;

    snprintf(t->type, sizeof(t->type), "%s", type);
    t->amount = amount;
    snprintf(t->status, sizeof(t->status), "Completed");

    appendHistory(t);

    if (isReversibleType(type))
    {
        if (top < MAX_TRANSACTIONS - 1)
        {
            pushTransaction(t);
        }
        else
        {
            printf("Warning: reversal stack is full; "
                   "this transaction cannot be reversed "
                   "using the current stack.\n");
        }
    }

    saveTransactionHistory();

    printf("Transaction recorded successfully. ID: %d\n",
           t->transactionID);
}

/* Display all transaction records */
void displayTransactionHistory(void)
{
    Transaction *current = historyHead;

    if (current == NULL)
    {
        printf("\nNo transaction history available.\n");
        return;
    }

    printf("\n========== TRANSACTION HISTORY ==========\n");

    while (current != NULL)
    {
        printf("Transaction ID : %d\n",
               current->transactionID);
        printf("Account Number : %d\n",
               current->accountNumber);
        printf("Type           : %s\n", current->type);
        printf("Amount         : %.2f\n", current->amount);
        printf("Status         : %s\n", current->status);
        printf("-----------------------------------------\n");

        current = current->next;
    }
}

/* Reverse the most recent eligible transaction */
void reverseLatestTransaction(void)
{
    if (top == -1)
    {
        printf("\nNo transaction available to reverse.\n");
        return;
    }

    Transaction *t = undoStack[top];

    if (strcmp(t->status, "Completed") != 0)
    {
        printf("Transaction has already been reversed.\n");
        return;
    }

    struct Account *account = findAccount(t->accountNumber);

    if (account == NULL)
    {
        printf("Account not found. Reversal cancelled.\n");
        return;
    }

    if (strcmp(t->type, "Deposit") == 0)
    {
        if (account->balance < t->amount)
        {
            printf("Insufficient balance to reverse deposit.\n");
            return;
        }
    }
    else if (strcmp(t->type, "Withdrawal") != 0)
    {
        printf("This transaction type cannot be reversed here.\n");
        return;
    }

    /* Allocate the reversal record before changing the balance */
    Transaction *reversal = malloc(sizeof(Transaction));

    if (reversal == NULL)
    {
        printf("Memory allocation failed. Reversal cancelled.\n");
        return;
    }

    /* Undo the original transaction */
    if (strcmp(t->type, "Deposit") == 0)
    {
        account->balance -= t->amount;
    }
    else
    {
        account->balance += t->amount;
    }

    /* Preserve the original transaction in history */
    strcpy(t->status, "Reversed");

    reversal->transactionID = nextTransactionID++;
    reversal->accountNumber = t->accountNumber;
    strcpy(reversal->type, "Reversal");
    reversal->amount = t->amount;
    strcpy(reversal->status, "Completed");

    appendHistory(reversal);
    popTransaction();

    /* Save the updated data */
    saveAccountRecords();
    saveTransactionHistory();

    printf("Transaction %d reversed successfully.\n",
           t->transactionID);
    printf("Updated balance: %.2f\n", account->balance);
}

/* Save transaction history to a text file */
void saveTransactionHistory(void)
{
    FILE *fp = fopen("transactions.txt", "w");

    if (fp == NULL)
    {
        printf("Unable to save transaction history.\n");
        return;
    }

    Transaction *current = historyHead;

    while (current != NULL)
    {
        fprintf(fp, "%d,%d,%s,%.2f,%s\n",
                current->transactionID,
                current->accountNumber,
                current->type,
                current->amount,
                current->status);

        current = current->next;
    }

    fclose(fp);
}

/* Load saved transaction history */
void loadTransactionHistory(void)
{
    freeTransactionHistory();

    FILE *fp = fopen("transactions.txt", "r");

    if (fp == NULL)
    {
        /* File does not exist on the first run */
        return;
    }

    char line[160];
    int maxID = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        Transaction *t = malloc(sizeof(Transaction));

        if (t == NULL)
        {
            printf("Memory allocation failed while loading.\n");
            break;
        }

        int fields = sscanf(
            line,
            "%d,%d,%19[^,],%f,%19[^\n]",
            &t->transactionID,
            &t->accountNumber,
            t->type,
            &t->amount,
            t->status
        );

        if (fields != 5)
        {
            free(t);
            continue;
        }

        appendHistory(t);

        if (t->transactionID > maxID)
        {
            maxID = t->transactionID;
        }

        if (strcmp(t->status, "Completed") == 0 &&
            isReversibleType(t->type))
        {
            /*
             * Keep the most recent eligible transactions
             * when rebuilding the stack.
             */
            if (top == MAX_TRANSACTIONS - 1)
            {
                for (int i = 1; i < MAX_TRANSACTIONS; i++)
                {
                    undoStack[i - 1] = undoStack[i];
                }

                top--;
            }

            pushTransaction(t);
        }
    }

    fclose(fp);
    nextTransactionID = maxID + 1;
}

/* Save account records without modifying account.c */
void saveAccountRecords(void)
{
    FILE *fp = fopen("accounts.txt", "w");

    if (fp == NULL)
    {
        printf("Unable to save account records.\n");
        return;
    }

    struct Account *current = head;

    while (current != NULL)
    {
        fprintf(fp, "%d,%s,%.2f\n",
                current->accountNumber,
                current->name,
                current->balance);

        current = current->next;
    }

    fclose(fp);
}

/* Free allocated transaction nodes */
void freeTransactionHistory(void)
{
    Transaction *current = historyHead;

    while (current != NULL)
    {
        Transaction *next = current->next;
        free(current);
        current = next;
    }

    historyHead = NULL;
    historyTail = NULL;
    top = -1;
}
