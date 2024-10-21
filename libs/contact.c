#include "contact.h"

// private funtions
static uint8_t validateName(char *name);
static uint8_t validateNumber(char *num);
static uint8_t validateEmail(char *id);
static bool getInput(char *buff, uint8_t size,
                     const char *prompt, bool (*validateFn)(char *));
static bool compare_contacts(const Contact *c1, const Contact *c2);

static const char *getName(const Contact *c);
static const char *getNumber(const Contact *c);
static const char *getEmail(const Contact *c);

/*

1. Read Name and store in temp string
    validate: only aplbhabets and space, Unique
    error: repeat agian
 */

static bool validateName(char *name)
{

    if (*name == '\0')
        return false;

    // A to Z or a to z or SPACE
    for (; *name != '\0'; name++)
    {
        if (!((*name >= 'A' && *name <= 'Z') ||
              (*name >= 'a' && *name <= 'z') ||
              (*name == ' ')))
        {
            return false;
        }
    }
    return true;
}

/*
2. read phone num
    validate: only numbers, and 10 digits, Unique
    error: repeat agian
 */
static bool validateNumber(char *num)
{
    if (*num == '\0')
        return false;

    // check 0 - 9
    for (uint8_t i = 0; i < strlen(num); i++)
    {
        if (!((strlen(num) == 10) &&
              ((num[i] >= '0') && (num[i] <= '9'))))
        {
            return 0;
        }
    }
    return 1;
}

// check @ and . exsist
static bool validateEmail(char *id)
{
    if (*id == '\0')
        return false;

    // gets the pointer to @
    char *at = strchr(id, '@');

    // //Look for '.' after @
    return (at != NULL && strchr(at, '.') != NULL);
}

static bool getInput(char *buff, uint8_t size, const char *prompt, bool (*validateFn)(char *))
{
    while (true)
    {
        printf("Enter %s: ", prompt);

        // get input from user and clear the old buffer
        __fpurge(stdin);
        if (fgets(buff, size, stdin) == NULL)
        {
            printf("Error reading input.\n");
            return false;
        }
        buff[strcspn(buff, "\n")] = 0;
        // printf("Buff: %s\n", buff);

        // Funtion pointer to respective validate funtion
        // bool valid = validateFn(buff);
        if (validateFn(buff))
        {
            return true; // base state to break the loop
        }

        printf("Invalid %s Input! TryAgain...\n\n", prompt);
    }
}

// Compare two contacts
static bool compare_contacts(const Contact *c1, const Contact *c2)
{
    return (strcmp(c1->name, c2->name) == 0) &&
           (strcmp(c1->phone, c2->phone) == 0) &&
           (strcmp(c1->email, c2->email) == 0);
}

void addNewContact(Contact *newCont)
{
    // temp struture to hold buffers
    // Contact newCont;

    // get inputs and assign them to respective tasks
    getInput(newCont->name, sizeof(newCont->name), "Name", validateName);
    getInput(newCont->phone, sizeof(newCont->phone), "Phone Number", validateNumber);
    getInput(newCont->email, sizeof(newCont->email), "Email-ID", validateEmail);

    // return newCont;
}

/*
1. Read Name and store in temp string
    validate: only aplbhabets and space, Unique
    error: repeat agian

2. read phone num
    validate: only numbers, and 10 digits, Unique
    error: repeat agian

3. read email id
    validate: is @ there and .com at end
    error: repeat agian

4. Store to Array:
    by index:
        put the next index data to current and the next to current and so on

5. Increment Contact Count

 */
void createContact(AddressBook *addressBook)
{
    // temp struture to hold buffers
    Contact newCont;
    addNewContact(&newCont);

    for (int i = 0; i < (addressBook->contactCount) + 1; i++)
    {
        if (compare_contacts(&newCont, &(addressBook->contacts[i])))
        {
            printf("\n-------CONTACT ALREADY EXSISTS!!-------\n\n");
            return;
        }
    }

    // Append the temp Contatct struture to Address Book and increment the couunt
    addressBook->contacts[addressBook->contactCount++] = newCont;

    printf("Contact created successfully.\n\n");
}

// Callback function for function pointer in
static const char *getName(const Contact *c) { return c->name; }
static const char *getNumber(const Contact *c) { return c->phone; }
static const char *getEmail(const Contact *c) { return c->email; }

/*

1. Menu: name, number, mail
2. read option

3. <option>:
    loop 0 to count:
        strcasestr or strstr:
            found: print the detail
            not found: Not found and go to start
 */

// returns count of founded result
uint8_t searchContact(AddressBook *addressBook, int8_t *indexArr)
{
    printf("Please enter any search term(Accepts all Name, Number, email): ");

    // get user input for search query
    char query[50];
    __fpurge(stdin);
    scanf("%s", query);
    printf("\n");
    // printf("Buff: %s\n", query);

    if (query[0] == '\0')
    {
        printf("No input entered..\n");
        return 0;
    }

    // Fuunction pointer which get to point to respective funtion from input
    const char *(*getCallbck)(const Contact *);

    // checking what type of input is got
    if (validateName(query))
    {
        printf("Searching by Name...\n");
        getCallbck = getName; // assigning to Name funtion
    }
    else if (validateEmail(query))
    {
        printf("Searching by Email...\n");
        getCallbck = getEmail; // assigning to Email funtion
    }
    else
    {
        printf("Searching by Number...\n");
        getCallbck = getNumber; // assigning to Email funtion
    }

    uint8_t index, count;

    printf("------------------------------------------------------------------------\n");
    printf("%-5s\t %-20s %-15s %-30s\n", "Sl.no", "Name", "Phone", "Email");
    printf("------------------------------------------------------------------------\n");

    for (count = 0, index = 0; index < addressBook->contactCount; index++)
    {
        if (strstr(getCallbck(&addressBook->contacts[index]), query) != NULL)
        {
            printf("%-5d\t ", count++);
            listContact(&(addressBook->contacts[index]));

            if (indexArr != NULL)
                *indexArr++ = index;
        }
    }

    return count;
}

/*
1. search funtion
2. confirm the contact to be edited by entering the accurate name or id
3. print the respective contact
4. Ask to Update:
    read details again
    Validate
 */
void editContact(AddressBook *addressBook)
{

    int8_t indexArr[10];
    // Initialize all elements with -1
    memset(indexArr, -1, sizeof(indexArr));

    uint8_t searchCount = searchContact(addressBook, indexArr);
    if (!searchCount)
    {
        printf("No Results Found!\n");
        return;
    }
    printf("Found %d results.\n\n", searchCount);

    // get input from user for to edit index
    uint8_t editIndex = 0;
    while (1)
    {
        printf("Enter the Sl.no of the contact you want to edit: ");
        __fpurge(stdin);
        scanf("%hhd", &editIndex);

        if (editIndex < searchCount)
        {
            break; // break the loop only when valid input received
        }
        printf("Invalid Input! Enter under the range.\n\n");
    }

    // printf("Edit Index: %d\n", indexArr[editIndex]);

    printf("----------------Update new details----------------\n");

    // temp contact and replace it in the main AddressBook
    Contact newCont;
    addNewContact(&newCont);

    // List the Contact for Confirmation
    printf("New Updated Details: \n");
    listContact(&newCont);
    addressBook->contacts[indexArr[editIndex]] = newCont;

    // List All the Contacts for verification
    //  listContacts(addressBook);
}

/*
1. Get temp Book and index arrray from searchContacts
2. Prompt user to select the Sl.no from Temp array while search Contact
3. use the Sl.no from tempBook as the index to get Real index for Real Address book
4. Iterate by placing the next next index to element to current and follow till the end of Count

 */
void deleteContact(AddressBook *addressBook)
{
    int8_t indexArr[10];
    // Initialize all elements with -1
    memset(indexArr, -1, sizeof(indexArr));

    uint8_t searchCount = searchContact(addressBook, indexArr);
    if (!searchCount)
    {
        printf("No Results Found!\n");
        return;
    }
    printf("Found %d results.\n\n", searchCount);

    // get input from user for to edit index
    uint8_t delIndex = 0;
    while (1)
    {
        printf("Enter the Sl.no of the contact you want to Delete: ");
        __fpurge(stdin);
        scanf("%hhd", &delIndex);

        if (delIndex < searchCount)
        {
            break; // break the loop only when valid input received
        }
        printf("Invalid Input! Enter under the range.\n\n");
    }

    printf("Are you sure you want to Delete this entry: \n");
    printf("------------------------------------------------------------------------\n");
    listContact(&addressBook->contacts[indexArr[delIndex]]);
    printf("------------------------------------------------------------------------\n");

    printf("Enter 'Y' for Yes and 'N' for No: ");
    char confirm;
    __fpurge(stdin);
    scanf("%c", &confirm);

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Delete Operation Canceled...\n\n");
        return;
    }

    // printf("Delete Index: %d\n", indexArr[delIndex]);

    /* Delete the selected index by shift all the values right from selected index  */
    for (int i = indexArr[delIndex]; i < addressBook->contactCount; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    // decrement 1 from the current size since we removed one.
    addressBook->contactCount--;

    printf("Deletion Successful...\n\n");
}

void listAllContacts(AddressBook *addressBook)
{
    printf("Listing all the contacts\n");

    printf("------------------------------------------------------------------------\n");
    printf("%-5s\t %-20s %-15s %-30s\n", "Sl.no", "Name", "Phone", "Email");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf("%-5d\t %-20s %-15s %-30s\n", i,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
}
void listContact(Contact *cont)
{
    printf("%-20s %-15s %-30s\n",
           cont->name, cont->phone, cont->email);
}