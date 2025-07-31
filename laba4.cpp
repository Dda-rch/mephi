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


    LibraryUser() {
        age = 0;
        books = 0;
        memset(name, 0, MAX_NAME_LENGTH);
        for (int i = 0; i < MAX_BOOKS; ++i) {
            memset(bookTitles[i], 0, MAX_BOOK_TITLE_LENGTH);
        }
    }
  
 
    LibraryUser(const char* name, int age, const char* firstBook = nullptr) {
        this->age = age;
        this->books = 0;
        memset(this->name, 0, MAX_NAME_LENGTH);
        for (int i = 0; i < MAX_BOOKS; ++i) {
            memset(bookTitles[i], 0, MAX_BOOK_TITLE_LENGTH);
        }

        setName(name);
        if (firstBook != nullptr) {
            borrowBook(firstBook);
        }
    }

 
    LibraryUser(const LibraryUser& other) {
        age = other.age;
        books = other.books;

        strncpy(name, other.name, MAX_NAME_LENGTH - 1);
        name[MAX_NAME_LENGTH - 1] = '\0';

        for (int i = 0; i < MAX_BOOKS; ++i) {
            strncpy(bookTitles[i], other.bookTitles[i], MAX_BOOK_TITLE_LENGTH - 1);
            bookTitles[i][MAX_BOOK_TITLE_LENGTH - 1] = '\0';
        }
    }

   
    ~LibraryUser() {
        printf("Destructor called for %s\n", name);
    }

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

    static void saveAgesToFile(const LibraryUser& user1, const LibraryUser* user2) {
        FILE* file = fopen("Ages.txt", "w");
        if (file) {
            fprintf(file, "User1 age: %d\n", user1.age);
            fprintf(file, "User2 age: %d\n", user2->age);
            if (fclose(file) != 0) {
                perror("Error close Ages.txt");
            }
        }
        else {
            perror("Failed to open Ages file");
        }
    }

    static void saveNumberOfBooksToFile(const LibraryUser& user1, const LibraryUser* user2) {
        FILE* file = fopen("Number.txt", "w");
        if (file) {
            fprintf(file, "User1 number of books: %d\n", user1.books);
            fprintf(file, "User2 number of books: %d\n", user2->books);
            if (fclose(file) != 0) {
                perror("Error close Number.txt");
            }
        }
        else {
            perror("Failed to open Number file");
        }
    }

    static void saveNamesToFile(const LibraryUser& user1, const LibraryUser* user2) {
        FILE* file = fopen("Names.txt", "w");
        if (file) {
            fprintf(file, "User1 name: %s\n", user1.name);
            fprintf(file, "User2 name: %s\n", user2->name);
            if (fclose(file) != 0) {
                perror("Error close Names.txt");
            }
        }
        else {
            perror("Failed to open Names file");
        }
    }

    static void saveBooksToFile(const LibraryUser& user1, const LibraryUser* user2) {
        FILE* file = fopen("BookTitless.txt", "w");
        if (file) {
            fprintf(file, "User1 books (%d)\n", user1.books);
            for (int i = 0; i < user1.books; i++) {
                fprintf(file, "- %s\n", user1.bookTitles[i]);
            }

            fprintf(file, "\nUser2 books (%d)\n", user2->books);
            for (int i = 0; i < user2->books; i++) {
                fprintf(file, "No books %s\n", user2->bookTitles[i]);
            }
            if (fclose(file) != 0) {
                perror("Error close Books.txt");
            }
        }
        else {
            perror("Failed to open books file");
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
  
    LibraryUser user1("Dasha", 22, "1984");
    LibraryUser* user2 = new LibraryUser("July", 21);
    user2->borrowBook("To Kill a Mockingbird");
    user2->borrowBook("The Great Gatsby");

    LibraryUser user3 = user1;
    user3.setName("Copy of Dasha");

    printf("User Information:\n");
    user1.displayInfo();
    user2->displayInfo();
    user3.displayInfo();

    printf("\nReturning books:\n");
    user1.returnBook();
    user2->returnBook();

    LibraryUser::saveAgesToFile(user1, user2);
    LibraryUser::saveNamesToFile(user1, user2);
    LibraryUser::saveBooksToFile(user1, user2);
    LibraryUser::saveNumberOfBooksToFile(user1, user2);

    delete user2;

    return 0;
}