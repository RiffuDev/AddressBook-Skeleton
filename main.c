#include "libs/contact.h"

void menuWait()
{
    // printf("Enter 'M' to go to Main Menu: ");
    printf("\nPress Enter key to go Main Menu\n");
    char confirm;
    __fpurge(stdin);
    scanf("%c", &confirm);
}

int main()
{
    printf("\n-------------------------------------------------------------------------------------------------------------\n");
    printf("\n\t\t\t\t\t\033[1;31m*Rifayudeen's AddressBook*\033[0m\n");
    printf("\n-------------------------------------------------------------------------------------------------------------\n");

    AddressBook addBook;
    initialize(&addBook);

    while (1)
    {
        printf("\n------------------------------------------------------------------------\n");
        // printf("MAIN MENU :\n");
        printf("\033[1;33mMAIN MENU :\033[0m\n");

        printf("1. Add contacts\n"
               "2. Delete contact\n"
               "3. Search contact\n"
               "4. Edit contact\n"
               "5. List all contacts\n"
               "6. Save\n"
               "7. Quit\n\n");

        printf("Enter your choice: ");
        uint8_t choice;
        __fpurge(stdin);
        scanf("%hhd", &choice);
        printf("\n");

        // if wrong input skip this iteration
        if (choice > 7)
        {
            printf("Invalid Input! Enter under the range.\n\n");
            continue;
        }

        switch (choice)
        {
        case 1: // Add contacts
            createContact(&addBook);
            menuWait();
            break;
        case 2: // Delete contact
            deleteContact(&addBook);
            menuWait();
            break;
        case 3: // Search contact
            searchContact(&addBook, NULL);
            menuWait();
            break;
        case 4: // Edit contact
            editContact(&addBook);
            menuWait();
            break;
        case 5: // List all contacts
            listAllContacts(&addBook);
            menuWait();
            break;
        case 6: // Save
            /* TODO */
            break;
        case 7: // Quit
            printf("Exiting the Application...\n");
            return 0;
            break;

        default:
            break;
        }
        printf("------------------------------------------------------------------------\n");
    }

    return 0;
}
