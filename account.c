#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "transaction_history.h"

struct Account *head = NULL;

void clearInputBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

int readAccountNumber()
{
    int accountNumber;
    if (scanf("%d", &accountNumber) != 1 || accountNumber <= 0)
    {
        clearInputBuffer();
        printf("Invalid account number! It must be a positive number.\n");
        return -1;
    }
    clearInputBuffer();
    return accountNumber;
}

float readBalance()
{
    float balance;
    if (scanf("%f", &balance) != 1)
    {
        clearInputBuffer();
        printf("Invalid balance! Please enter a number.\n");
        return -1;
    }
    clearInputBuffer();
    if (balance < 0)
    {
        printf("Balance cannot be negative!\n");
        return -1;
    }
    return balance;
}

struct Account *findAccount(int accountNumber)
{
    struct Account *temp = head;
    while (temp != NULL)
    {
        if (temp->accountNumber == accountNumber)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void createAccount()
{
    int accountNumber;
    char name[50];
    float balance;
    struct Account *newAccount;
    struct Account *temp;

    printf("\nEnter Account Number: ");
    accountNumber = readAccountNumber();
    if (accountNumber == -1)
    {
        return;
    }
    if (findAccount(accountNumber) != NULL)
    {
        printf("Account number already exists!\n");
        return;
    }

    printf("Enter Account Holder Name: ");
    scanf(" %49[^\n]", name);
    clearInputBuffer();

    printf("Enter Initial Balance: ");
    balance = readBalance();
    if (balance == -1)
    {
        return;
    }

    newAccount = (struct Account *)malloc(sizeof(struct Account));
    if (newAccount == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    newAccount->accountNumber = accountNumber;
    strcpy(newAccount->name, name);
    newAccount->balance = balance;
    newAccount->next = NULL;

    if (head == NULL)
    {
        head = newAccount;
    }
    else
    {
        temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newAccount;
    }
    printf("\nAccount created successfully!\n");
    saveAccountRecords();
}

void displayAccounts()
{
    struct Account *temp = head;
    if (head == NULL)
    {
        printf("\nNo accounts available.\n");
        return;
    }
    printf("\n========== ALL ACCOUNTS ==========\n");
    while (temp != NULL)
    {
        printf("Account Number : %d\n", temp->accountNumber);
        printf("Account Holder : %s\n", temp->name);
        printf("Balance        : %.2f\n", temp->balance);
        printf("----------------------------------\n");
        temp = temp->next;
    }
}

void searchAccount()
{
    int accountNumber;
    struct Account *account;

    if (head == NULL)
    {
        printf("\nNo accounts available.\n");
        return;
    }
    printf("\nEnter Account Number to search: ");
    accountNumber = readAccountNumber();
    if (accountNumber == -1)
    {
        return;
    }
    account = findAccount(accountNumber);
    if (account == NULL)
    {
        printf("\nAccount not found.\n");
        return;
    }
    printf("\nAccount Found!\n");
    printf("Account Number : %d\n", account->accountNumber);
    printf("Account Holder : %s\n", account->name);
    printf("Balance        : %.2f\n", account->balance);
}

void updateAccount()
{
    int accountNumber;
    char newName[50];
    float newBalance;
    struct Account *account;

    if (head == NULL)
    {
        printf("\nNo accounts available.\n");
        return;
    }
    printf("\nEnter Account Number to update: ");
    accountNumber = readAccountNumber();
    if (accountNumber == -1)
    {
        return;
    }
    account = findAccount(accountNumber);
    if (account == NULL)
    {
        printf("\nAccount not found.\n");
        return;
    }

    printf("Enter New Account Holder Name: ");
    scanf(" %49[^\n]", newName);
    clearInputBuffer();

    printf("Enter New Balance: ");
    newBalance = readBalance();
    if (newBalance == -1)
    {
        printf("Account was not updated.\n");
        return;
    }

    strcpy(account->name, newName);
    account->balance = newBalance;
    printf("\nAccount updated successfully!\n");
    saveAccountRecords();
}

void deleteAccount()
{
    int accountNumber;
    struct Account *temp = head;
    struct Account *previous = NULL;

    if (head == NULL)
    {
        printf("\nNo accounts available.\n");
        return;
    }
    printf("\nEnter Account Number to delete: ");
    accountNumber = readAccountNumber();
    if (accountNumber == -1)
    {
        return;
    }

    while (temp != NULL && temp->accountNumber != accountNumber)
    {
        previous = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("\nAccount not found.\n");
        return;
    }

    if (previous == NULL)
    {
        head = temp->next;
    }
    else
    {
        previous->next = temp->next;
    }
    free(temp);
    printf("\nAccount deleted successfully!\n");
    saveAccountRecords();
}

void swapAccountData(struct Account *first, struct Account *second)
{
    int tempAccountNumber;
    char tempName[50];
    float tempBalance;

    tempAccountNumber = first->accountNumber;
    first->accountNumber = second->accountNumber;
    second->accountNumber = tempAccountNumber;

    strcpy(tempName, first->name);
    strcpy(first->name, second->name);
    strcpy(second->name, tempName);

    tempBalance = first->balance;
    first->balance = second->balance;
    second->balance = tempBalance;
}

void sortByAccountNumber()
{
    struct Account *current;
    int swapped;

    if (head == NULL || head->next == NULL)
    {
        printf("\nNot enough accounts to sort.\n");
        return;
    }
    do
    {
        swapped = 0;
        current = head;
        while (current->next != NULL)
        {
            if (current->accountNumber > current->next->accountNumber)
            {
                swapAccountData(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped == 1);
    printf("\nAccounts sorted by Account Number.\n");
}

void sortByBalance()
{
    struct Account *current;
    int swapped;

    if (head == NULL || head->next == NULL)
    {
        printf("\nNot enough accounts to sort.\n");
        return;
    }
    do
    {
        swapped = 0;
        current = head;
        while (current->next != NULL)
        {
            if (current->balance > current->next->balance)
            {
                swapAccountData(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped == 1);
    printf("\nAccounts sorted by Balance.\n");
}

void freeAccounts()
{
    struct Account *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
