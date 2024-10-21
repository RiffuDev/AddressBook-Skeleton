#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

#define true    1
#define false   0

// #include <stdio.h>
#include <stdio_ext.h> 
#include <string.h>
#include <stdint.h>

typedef uint8_t bool;

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;

void createContact(AddressBook *addressBook);
uint8_t searchContact(AddressBook *addressBook, int8_t *indexArr);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listAllContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);

void addNewContact(Contact *newCont);
void listContact(Contact *cont);


#endif
