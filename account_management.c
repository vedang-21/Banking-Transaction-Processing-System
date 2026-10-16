#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for one bank account
struct Account
{
    int accountNumber;
    char name[50];
    float balance;
    struct Account *next;
};

// Head pointer of the linked list
struct Account *head = NULL;


// Function to create a new account
void createAccount()
{
    struct Account *newAccount;
    struct Account *temp;

    newAccount = (struct Account *)malloc(sizeof(struct Account));

    if (newAccount == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &newAccount->accountNumber);

    // Check whether account number already exists
    temp = head;

    while (temp != NULL)
    {
        if (temp->accountNumber == newAccount->accountNumber)
        {
            printf("Account number already exists!\n");
            free(newAccount);
            return;
        }

        temp = temp->next;
    }

    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", newAccount->name);

    printf("Enter Initial Balance: ");
    scanf("%f", &newAccount->balance);

    if (newAccount->balance < 0)
    {
        printf("Balance cannot be negative!\n");
        free(newAccount);
        return;
    }

    newAccount->next = NULL;

    // Insert the new account at the end
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
}


// Function to display all accounts
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
        printf("\nAccount Number : %d", temp->accountNumber);
        printf("\nAccount Holder : %s", temp->name);
        printf("\nBalance        : %.2f", temp->balance);
        printf("\n----------------------------------");

        temp = temp->next;
    }

    printf("\n");
}


// Function to search for an account
void searchAccount()
{
    int accountNumber;
    struct Account *temp = head;

    printf("\nEnter Account Number to search: ");
    scanf("%d", &accountNumber);

    while (temp != NULL)
    {
        if (temp->accountNumber == accountNumber)
        {
            printf("\nAccount Found!\n");
            printf("Account Number : %d\n", temp->accountNumber);
            printf("Account Holder : %s\n", temp->name);
            printf("Balance        : %.2f\n", temp->balance);

            return;
        }

        temp = temp->next;
    }

    printf("\nAccount not found.\n");
}


// Function to update account details
void updateAccount()
{
    int accountNumber;
    struct Account *temp = head;

    printf("\nEnter Account Number to update: ");
    scanf("%d", &accountNumber);

    while (temp != NULL)
    {
        if (temp->accountNumber == accountNumber)
        {
            printf("\nAccount Found!\n");

            printf("Enter New Account Holder Name: ");
            scanf(" %[^\n]", temp->name);

            printf("Enter New Balance: ");
            scanf("%f", &temp->balance);

            if (temp->balance < 0)
            {
                printf("Balance cannot be negative!\n");
                return;
            }

            printf("\nAccount updated successfully!\n");

            return;
        }

        temp = temp->next;
    }

    printf("\nAccount not found.\n");
}


// Function to delete an account
void deleteAccount()
{
    int accountNumber;
    struct Account *temp = head;
    struct Account *previous = NULL;

    printf("\nEnter Account Number to delete: ");
    scanf("%d", &accountNumber);

    // Search for the account
    while (temp != NULL)
    {
        if (temp->accountNumber == accountNumber)
        {
            // If deleting the first node
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
            return;
        }

        previous = temp;
        temp = temp->next;
    }

    printf("\nAccount not found.\n");
}


// Function to sort accounts by account number
void sortByAccountNumber()
{
    struct Account *i;
    struct Account *j;

    int tempAccountNumber;
    float tempBalance;
    char tempName[50];

    if (head == NULL || head->next == NULL)
    {
        printf("\nNot enough accounts to sort.\n");
        return;
    }

    // Bubble sort
    for (i = head; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (i->accountNumber > j->accountNumber)
            {
                tempAccountNumber = i->accountNumber;
                i->accountNumber = j->accountNumber;
                j->accountNumber = tempAccountNumber;

                tempBalance = i->balance;
                i->balance = j->balance;
                j->balance = tempBalance;

                strcpy(tempName, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, tempName);
            }
        }
    }

    printf("\nAccounts sorted by Account Number.\n");
}


// Function to sort accounts by balance
void sortByBalance()
{
    struct Account *i;
    struct Account *j;

    int tempAccountNumber;
    float tempBalance;
    char tempName[50];

    if (head == NULL || head->next == NULL)
    {
        printf("\nNot enough accounts to sort.\n");
        return;
    }

    // Bubble sort
    for (i = head; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (i->balance > j->balance)
            {
                tempAccountNumber = i->accountNumber;
                i->accountNumber = j->accountNumber;
                j->accountNumber = tempAccountNumber;

                tempBalance = i->balance;
                i->balance = j->balance;
                j->balance = tempBalance;

                strcpy(tempName, i->name);
                strcpy(i->name, j->name);
                strcpy(j->name, tempName);
            }
        }
    }

    printf("\nAccounts sorted by Balance.\n");
}


// Main function
int main()
{
    int choice;

    do
    {
        printf("\n\n========== BANK ACCOUNT MANAGEMENT ==========\n");
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Update Account\n");
        printf("5. Delete Account\n");
        printf("6. Sort by Account Number\n");
        printf("7. Sort by Balance\n");
        printf("8. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                createAccount();
                break;

            case 2:
                displayAccounts();
                break;

            case 3:
                searchAccount();
                break;

            case 4:
                updateAccount();
                break;

            case 5:
                deleteAccount();
                break;

            case 6:
                sortByAccountNumber();
                break;

            case 7:
                sortByBalance();
                break;

            case 8:
                printf("\nExiting Account Management Module...\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 8);

    return 0;
}