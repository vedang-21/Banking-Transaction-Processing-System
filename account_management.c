#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "transaction.h"
#include "transaction_history.h"



// Main function
int main()
{
    int choice;

    // Load persisted data on startup
    loadAccountRecords();
    loadTransactionHistory();

    do
    {
        printf("\n==================================================\n");
        printf("    BANKING TRANSACTION PROCESSING SYSTEM\n");
        printf("==================================================\n");
        printf("--- Account Management ---\n");
        printf(" 1. Create New Account\n");
        printf(" 2. Display All Accounts\n");
        printf(" 3. Search Account\n");
        printf(" 4. Update Account\n");
        printf(" 5. Delete Account\n");
        printf(" 6. Sort by Account Number\n");
        printf(" 7. Sort by Balance\n");
        printf("--- Transaction Processing ---\n");
        printf(" 8. Deposit Funds\n");
        printf(" 9. Withdraw Funds\n");
        printf("10. Transfer Funds\n");
        printf("--- Queue Operations (FIFO) ---\n");
        printf("11. Display Transaction Queue\n");
        printf("12. Process/Dequeue Transaction\n");
        printf("--- Transaction History & Reversal (LIFO) ---\n");
        printf("13. Display Transaction History\n");
        printf("14. Undo / Reverse Latest Transaction\n");
        printf("--------------------------------------------------\n");
        printf("15. Exit\n");

        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf("\nInvalid choice! Please enter a number.\n");
            continue;
        }
        clearInputBuffer();

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
                deposit();
                break;

            case 9:
                withdraw();
                break;

            case 10:
                transfer();
                break;

            case 11:
                displayQueue();
                break;

            case 12:
                dequeue();
                break;

            case 13:
                displayTransactionHistory();
                break;

            case 14:
                reverseLatestTransaction();
                break;

            case 15:
                printf("\nExiting Banking System...\n");
                saveAccountRecords();
                saveTransactionHistory();
                freeAccounts();
                freeTransactionHistory();
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 15);

    return 0;
}