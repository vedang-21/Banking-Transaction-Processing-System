#include <stdio.h>
#include <string.h>
#include "transaction.h"

struct Transaction queue[MAX_TRANSACTIONS];
int front = -1;
int rear = -1;
int transactionCounter = 1;

int isQueueEmpty()
{
    return front == -1;
}

int isQueueFull()
{
    return rear == MAX_TRANSACTIONS - 1;
}

void enqueue(char type[], int accountNumber, int receiverAccountNumber, float amount)
{
    if (isQueueFull())
    {
        printf("Transaction queue is full!\n");
        return;
    }
    if (front == -1)
    {
        front = 0;
    }
    rear++;
    queue[rear].transactionId = transactionCounter;
    strcpy(queue[rear].type, type);
    queue[rear].accountNumber = accountNumber;
    queue[rear].receiverAccountNumber = receiverAccountNumber;
    queue[rear].amount = amount;
    transactionCounter++;
    printf("Transaction ID %d recorded.\n", queue[rear].transactionId);
}

void dequeue()
{
    if (isQueueEmpty())
    {
        printf("\nTransaction queue is empty.\n");
        return;
    }
    printf("\nProcessed Transaction ID %d (%s, Amount: %.2f)\n", queue[front].transactionId, queue[front].type, queue[front].amount);
    if (front == rear)
    {
        front = -1;
        rear = -1;
    }
    else
    {
        front++;
    }
}

void displayQueue()
{
    int i;
    if (isQueueEmpty())
    {
        printf("\nTransaction queue is empty.\n");
        return;
    }
    printf("\n========== TRANSACTION QUEUE ==========\n");
    for (i = front; i <= rear; i++)
    {
        printf("ID: %d | %s | Account: %d", queue[i].transactionId, queue[i].type, queue[i].accountNumber);
        if (strcmp(queue[i].type, "Transfer") == 0)
        {
            printf(" | To: %d", queue[i].receiverAccountNumber);
        }
        printf(" | Amount: %.2f\n", queue[i].amount);
    }
}

float readAmount()
{
    float amount;
    if (scanf("%f", &amount) != 1)
    {
        clearInputBuffer();
        printf("Invalid amount!\n");
        return 0;
    }
    clearInputBuffer();
    if (amount <= 0)
    {
        printf("Amount must be greater than zero!\n");
        return 0;
    }
    return amount;
}

struct Account *getAccount(char prompt[])
{
    int accountNumber;
    struct Account *account;
    printf("%s", prompt);
    accountNumber = readAccountNumber();
    if (accountNumber == -1)
    {
        return NULL;
    }
    account = findAccount(accountNumber);
    if (account == NULL)
    {
        printf("Account not found!\n");
    }
    return account;
}

void deposit()
{
    struct Account *account;
    float amount;

    if (isQueueFull())
    {
        printf("\nTransaction queue is full! Process pending transactions first.\n");
        return;
    }
    account = getAccount("\nEnter Account Number: ");
    if (account == NULL)
    {
        return;
    }
    printf("Enter Deposit Amount: ");
    amount = readAmount();
    if (amount == 0)
    {
        return;
    }
    account->balance = account->balance + amount;
    printf("\nDeposit successful! Updated Balance: %.2f\n", account->balance);
    enqueue("Deposit", account->accountNumber, 0, amount);
}

void withdraw()
{
    struct Account *account;
    float amount;

    if (isQueueFull())
    {
        printf("\nTransaction queue is full! Process pending transactions first.\n");
        return;
    }
    account = getAccount("\nEnter Account Number: ");
    if (account == NULL)
    {
        return;
    }
    printf("Enter Withdrawal Amount: ");
    amount = readAmount();
    if (amount == 0)
    {
        return;
    }
    if (amount > account->balance)
    {
        printf("Insufficient balance! Available Balance: %.2f\n", account->balance);
        return;
    }
    account->balance = account->balance - amount;
    printf("\nWithdrawal successful! Updated Balance: %.2f\n", account->balance);
    enqueue("Withdrawal", account->accountNumber, 0, amount);
}

void transfer()
{
    struct Account *sender;
    struct Account *receiver;
    float amount;

    if (isQueueFull())
    {
        printf("\nTransaction queue is full! Process pending transactions first.\n");
        return;
    }
    sender = getAccount("\nEnter Sender Account Number: ");
    if (sender == NULL)
    {
        return;
    }
    receiver = getAccount("Enter Receiver Account Number: ");
    if (receiver == NULL)
    {
        return;
    }
    if (sender == receiver)
    {
        printf("Sender and receiver cannot be the same account!\n");
        return;
    }
    printf("Enter Transfer Amount: ");
    amount = readAmount();
    if (amount == 0)
    {
        return;
    }
    if (amount > sender->balance)
    {
        printf("Insufficient balance! Available Balance: %.2f\n", sender->balance);
        return;
    }
    sender->balance = sender->balance - amount;
    receiver->balance = receiver->balance + amount;
    printf("\nTransfer successful!\n");
    printf("Sender Balance   : %.2f\n", sender->balance);
    printf("Receiver Balance : %.2f\n", receiver->balance);
    enqueue("Transfer", sender->accountNumber, receiver->accountNumber, amount);
}
