#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio> 
#include <cstring> 

class LibraryUser {
private:
    static const int MAX_NAME_LENGTH = 100;
    static const int MAX_BOOKS = 10;
    static const int MAX_BOOK_TITLE_LENGTH = 100;

    int age;
    int books;
    char name[MAX_NAME_LENGTH];
    char bookTitles[MAX_BOOKS][MAX_BOOK_TITLE_LENGTH];

public:
    void setName(const char* name);
    void setAge(int age);
    void borrowBook(const char* bookTitle);

    void displayInfo() const {
        printf("Name: %s\n", name);
        printf("Age: %d\n", age);
        printf("Books: %d\n", books);
        if (books > 0) {
            printf("Book titles: ");
            for (int i = 0; i < books; i++) {
                printf("%s", bookTitles[i]);
                if (i < books - 1) {
                    printf(", ");
                }
            }
            printf("\n");
        }
    }

    void returnBook() {
        if (books > 0) {
            books--;
            bookTitles[books][0] = '\0';
            printf("%s returned a book\n", name);
            printf("Total books borrowed: %d\n", books);
        }
        else {
            printf("Error: No books\n");
        }
    }

    void initialize() {
        age = 0;
        books = 0;
        name[0] = '\0';
        for (int i = 0; i < MAX_BOOKS; ++i) {
            bookTitles[i][0] = '\0';
        }
    }
};

void LibraryUser::setName(const char* name) {
    strncpy(this->name, name, MAX_NAME_LENGTH - 1);
    this->name[MAX_NAME_LENGTH - 1] = '\0'; 
}

void LibraryUser::setAge(int age) {
    if (age >= 0) {
        this->age = age;
    }
    else {
        printf("Error: Age < 0\n");
    }
}

void LibraryUser::borrowBook(const char* bookTitle) {
    if (books < MAX_BOOKS) {
        strncpy(bookTitles[books], bookTitle, MAX_BOOK_TITLE_LENGTH - 1);
        bookTitles[books][MAX_BOOK_TITLE_LENGTH - 1] = '\0';
        books++;
    }
    else {
        printf("Error: User can't borrow more books\n");
    }
}

int main() {
    LibraryUser user1;
    user1.initialize(); 
    user1.setName("Dasha");
    user1.setAge(21);

    user1.borrowBook("1984");
    user1.borrowBook("To Kill a Mockingbird");

    LibraryUser* user2 = new LibraryUser();
    user2->initialize();
    user2->setName("Kate");
    user2->setAge(20);

    user2->borrowBook("The Great Gatsby");

    printf("User Information:\n");
    user1.displayInfo();
    user2->displayInfo();

    printf("\nReturning books:\n");
    user1.returnBook();
    user2->returnBook();

    delete user2;

    return 0;
}