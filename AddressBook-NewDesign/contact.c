#include <stdio.h>      // Standard input-output functions
#include <stdlib.h>     // For exit() and other utility functions
#include <string.h>     // For string operations like strcmp, strcpy
#include "contact.h"    // User-defined header containing AddressBook and Contact structure definitions
#include "file.h"       // Header for file-related operations like saving/loading contacts
#include "populate.h"   // Header for initial data population, possibly for testing or defaults

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria
    printf("---------------------------------------------\n");
    printf("          ENTERED INTO LIST CONTACT          \n");
    printf("---------------------------------------------\n");
    printf("---------------------------------------------\n");
    printf("| Name\t\t| Phone number\t| Email Id\t|\n");
    printf("---------------------------------------------\n");
    // Loop through the address book and print each contact's details
    for(int i=0; i<addressBook->contactCount;i++)
    {
        printf("| %s\t%s\t%s\t\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    
}
// Initialize the address book with zero contacts and load data from file
void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);

    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    short int phone_dup;
    short int email_dup;
    short int name_dup; 
    int index = addressBook->contactCount;

    printf("Enter the name : ");
    scanf(" %[^\n]",addressBook->contacts[index].name);
    do{
        for(int i=0; addressBook->contacts[index].name[i] != '\0'; i++)
        {
            if((addressBook->contacts[index].name[i] >= 'a' && addressBook->contacts[index].name[i] <= 'z')
                || (addressBook->contacts[index].name[i] >= 'A' && addressBook->contacts[index].name[i] <= 'Z')
                || addressBook->contacts[index].name[i] == ' ' || addressBook->contacts[index].name[i] == '.')
            {
                name_dup = 0;
            }
            else
            {
                printf("Entered Name contains invalid character\n");
                printf("Enter the name : ");
                scanf(" %[^\n]",addressBook->contacts[index].name);
                name_dup = 1; 
            }
        }
    }while(name_dup);

    printf("Enter phone number: ");
    scanf("%s", addressBook->contacts[index].phone);
    do{
        phone_dup = 0;
        for(int i=0; i<addressBook->contactCount; i++)
        {
            if(strlen(addressBook->contacts[index].phone) > 10)
            {
                printf("Phone number exceeding 10 numbers\n");
                printf("Enter phone number : ");
                scanf("%s", addressBook->contacts[index].phone);
                phone_dup = 1;
            }
            if(strcmp(addressBook->contacts[index].phone,addressBook->contacts[i].phone) == 0)
            {
                printf("Phone number already exist\n");
                printf("Enter phone number : ");
                scanf("%s", addressBook->contacts[index].phone);
                phone_dup = 1;
            }
        }
    }while(phone_dup);

    //error:
    printf("Enter Email id: ");
    scanf("%s", addressBook->contacts[index].email);
    do{
        email_dup = 0;
        for(int i=0; addressBook->contacts[index].email[i] != '\0'; i++)
        {
            if((addressBook->contacts[index].email[i] >= 'a' && addressBook->contacts[index].email[i] <= 'z') 
                || (addressBook->contacts[index].email[i] >= '0' && addressBook->contacts[index].email[i] <= '9')
                || (addressBook->contacts[index].email[i] == '@') || (addressBook->contacts[index].email[i] == '.'))
                {
                    if(strstr(addressBook->contacts[index].email, ".com") == NULL){
                        email_dup = 1;
                        printf("Email does not contain proper vaildations\n");
                        goto error;
                    }
                    else
                    {
                        char check[5] = ".com";
                        int email_len = strlen(addressBook->contacts[index].email);
                        if (strcmp(&addressBook->contacts[index].email[email_len - 4], check) == 0)
                        {
                            email_dup=0;
                        }
                        else
                        {
                            printf("Email must end with .com\n");
                            goto error;
                        }
                    }
                    email_dup = 0;
                }
                else{
                    error:
                    printf("Entered Email id is invalid\n");
                    printf("Enter Email id: ");
                    scanf("%s", addressBook->contacts[index].email);
                    email_dup = 1;
                    //break;
                }
        }

        for(int i=0; i<addressBook->contactCount; i++)
        {
            if(strcmp(addressBook->contacts[index].email,addressBook->contacts[i].email) == 0)
            {
                printf("Email id already exist\n");
                printf("Enter Email id: ");
                scanf("%s", addressBook->contacts[index].email);
                email_dup = 1;
                //break;
            }
        }
    }while(email_dup);

    addressBook->contactCount++;

    printf("%s\t%s\t%s\t\n",addressBook->contacts[index].name,addressBook->contacts[index].phone,addressBook->contacts[index].email);
    printf("Contact Created successfully\n");
}

void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    printf("---------------------------------------------\n");
    printf("          ENTERED INTO SEARCH CONTACT\n");
    printf("---------------------------------------------\n");
    int choise;
    int flag=0,index;
    printf("1.Search by name\n");
    printf("2.Search by phone number\n");
    printf("3.Search by email id\n");

    printf("Enter your search type : ");
    scanf("%d",&choise);
    
    switch(choise){
        case 1:{
            char str[50];

            printf("Enter the name: ");
            scanf(" %[^\n]",str);
            printf("---------------------------------------------\n");
            printf("| Name\t\t| Phone number\t| Email Id\t|\n");
            printf("---------------------------------------------\n");
            for(int i=0; i<addressBook->contactCount; i++)
            {
                if(strstr(addressBook->contacts[i].name, str) != NULL)
                {
                    flag = 1;
                    printf("%s\t%s\t%s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                }
            }
            break;
        }
        case 2:{
            char num[20];

            printf("Enter the phone number: ");
            scanf("%s",num);
            printf("---------------------------------------------\n");
            printf("| Name\t\t| Phone number\t| Email Id\t|\n");
            printf("---------------------------------------------\n");
            for(int i=0; i<addressBook->contactCount; i++)
            {
                if(strcmp(num,addressBook->contacts[i].phone) == 0)
                {
                    flag = 1;
                    printf("%s\t%s\t%s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                }
            }
            break;
        }
        case 3:{
            char email[50];

            printf("Enter the email: ");
            scanf(" %[^\n]",email);
            printf("---------------------------------------------\n");
            printf("| Name\t\t| Phone number\t| Email Id\t|\n");
            printf("---------------------------------------------\n");
            for(int i=0; i<addressBook->contactCount; i++)
            {
                if(strcmp(email,addressBook->contacts[i].email) == 0)
                {
                    flag = 1;
                    printf("%s\t%s\t%s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                }
            }
            break;
        }

    }
    if(flag == 0)
    printf("Search not found\n");
}

void editContact(AddressBook *addressBook)
{
    printf("---------------------------------------------\n");
    printf("          ENTERED INTO EDIT CONTACT\n");
    printf("---------------------------------------------\n");

    int choice, fieldChoice;
    int flag = 0, index;
    printf("1. Search by name\n");
    printf("2. Search by phone number\n");
    printf("3. Search by email id\n");

    printf("Enter your search type: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            char str[50];
            int matchingIndices[100];
            int matchCount = 0;

            printf("Enter the name: ");
            scanf(" %[^\n]", str);

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(str, addressBook->contacts[i].name) == 0) {
                    matchingIndices[matchCount++] = i;
                }
            }

            if (matchCount == 0) {
                printf("No matching contacts found.\n");
                return;
            }

            printf("\nMatching contacts:\n");
            for (int i = 0; i < matchCount; i++) {
                int idx = matchingIndices[i];
                printf("%d. %s\t%s\t%s\n", i + 1,
                       addressBook->contacts[idx].name,
                       addressBook->contacts[idx].phone,
                       addressBook->contacts[idx].email);
            }

            printf("\nSelect option : ");
            scanf("%d", &choice);

            if (choice < 1 || choice > matchCount) {
                printf("Invalid selection.\n");
                return;
            }

            index = matchingIndices[choice - 1];
            flag = 1;
            break;
        }
    }

    if (flag == 1) {
        printf("\nSelected Contact:\n");
        printf("%s\t%s\t%s\n", addressBook->contacts[index].name, addressBook->contacts[index].phone, addressBook->contacts[index].email);

        printf("\nWhat do you want to edit?\n");
        printf("1. Name\n");
        printf("2. Phone number\n");
        printf("3. Email ID\n");
        printf("Enter your choice: ");
        scanf("%d", &fieldChoice);

        switch (fieldChoice) {
            case 1: {
                char name[50];
                printf("Enter new name: ");
                scanf(" %[^\n]", name);
                strcpy(addressBook->contacts[index].name, name);
                break;
            }
            case 2: {
                char phone[20];
                printf("Enter new phone number: ");
                scanf("%s", phone);
                strcpy(addressBook->contacts[index].phone, phone);
                break;
            }
            case 3: {
                char email[50];
                printf("Enter new email ID: ");
                scanf(" %[^\n]", email);
                strcpy(addressBook->contacts[index].email, email);
                break;
            }
            default:
                printf("Invalid field choice.\n");
        }
    } else {
        printf("Contact not found.\n");
    }
}

void deleteContact(AddressBook *addressBook)
{
    printf("---------------------------------------------\n");
    printf("          ENTERED INTO DELETE CONTACT\n");
    printf("---------------------------------------------\n");

    int choice;
    int flag = 0, index;
    printf("1. Search by name\n");
    printf("2. Search by phone number\n");
    printf("3. Search by email id\n");

    printf("Enter your search type: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            char str[50];
            int matchingIndices[100];
            int matchCount = 0;

            printf("Enter the name: ");
            scanf(" %[^\n]", str);

            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                if (strcmp(str, addressBook->contacts[i].name) == 0) 
                {
                    matchingIndices[matchCount++] = i;
                }
            }

            if (matchCount == 0) {
                printf("No matching contacts found.\n");
                return;
            }

            printf("\nMatching contacts:\n");
            for (int i = 0; i < matchCount; i++) {
                int idx = matchingIndices[i];
                printf("%d. %s\t%s\t%s\n", i + 1,
                       addressBook->contacts[idx].name,
                       addressBook->contacts[idx].phone,
                       addressBook->contacts[idx].email);
            }

            printf("\nSelect option to delete : ");
            scanf("%d", &choice);

            if (choice < 1 || choice > matchCount) {
                printf("Invalid selection.\n");
                return;
            }

            index = matchingIndices[choice - 1];
            flag = 1;
            break;
        }
    }

    if (flag == 1) {
        printf("\nSelected Contact to delete:\n");
        printf("%s\t%s\t%s\n", addressBook->contacts[index].name, addressBook->contacts[index].phone, addressBook->contacts[index].email);

        char confirm;
        printf("\nAre you sure you want to delete this contact? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            for (int i = index; i < addressBook->contactCount - 1; i++) {
                addressBook->contacts[i] = addressBook->contacts[i + 1];
            }
            addressBook->contactCount--;
            printf("Contact deleted successfully.\n");
        } else {
            printf("Delete operation cancelled.\n");
        }
    } else {
        printf("Contact not found.\n");
    }
}
