#ifndef ACCOUNT_H
#define ACCOUNT_H

struct Account
{
    int accountNumber;
    char name[50];
    float balance;
    struct Account *next;
};

extern struct Account *head;

void clearInputBuffer();
int readAccountNumber();
float readBalance();
struct Account *findAccount(int accountNumber);
void createAccount();
void displayAccounts();
void searchAccount();
void updateAccount();
void deleteAccount();
void sortByAccountNumber();
void sortByBalance();
void freeAccounts();

#endif
