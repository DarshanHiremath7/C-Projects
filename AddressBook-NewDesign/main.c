/*
    Project     : Address Book System
    Name        : Darshan Hiremath
    Date        : 3th May 2025
    Description : A C program to manage contacts by allowing
                  users to create, search, edit, list, and 
                  delete contacts with validation.
*/
#include <stdio.h> // Standard input-output header for functions like printf and scanf
#include "contact.h" // Custom header file that contains declarations related to the address book

int main() {
    int choice;// Variable to store the user's menu choice
    AddressBook addressBook;// Declare an AddressBook structure to store all contact details
    //printf("Before initialize\n");
    initialize(&addressBook); // Initialize the address book
    //printf("After initialize\n");
    do {
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts and Exit\n");		
        //printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createContact(&addressBook);// Function to add a new contact
                break;
            case 2:
                searchContact(&addressBook); // Function to search for a contact by name or number
                break;
            case 3:
                editContact(&addressBook); // Function to modify an existing contact
                break;
            case 4:
                deleteContact(&addressBook);// Function to remove a contact from the list
                break;
            case 5:          
                listContacts(&addressBook);// Function to display all saved contacts
                break;
            case 6:
                printf("Saving and Exiting\n");
                saveContactsToFile(&addressBook);// Function to save all contacts to a file before exiting
                break;
            default:
                printf("Invalid choice. Please try again.\n");// Handle unexpected input
        }
    } while (choice != 6);// Keep looping until the user selects option 6
    
       return 0;// End of program
}
