
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
        initialize();
    }

    explicit LibraryUser(const char* name, int age = 0) {
        initialize();
        setName(name);
        setAge(age);
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

    void initialize() {
        age = 0;
        books = 0;
        name[0] = '\0';
        for (int i = 0; i < MAX_BOOKS; ++i) {
            bookTitles[i][0] = '\0';
        }
    }

    const char* getName() const { return name; }
    int getAge() const { return age; }
    int getBooksCount() const { return books; }
    const char* getBookTitle(int index) const {
        if (index >= 0 && index < books) {
            return bookTitles[index];
        }
        return nullptr;
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

class LibraryInspector : public LibraryUser {
private:
    int inspectionCount;

public:
    explicit LibraryInspector(const char* name, int age = 0)
        : LibraryUser(name, age), inspectionCount(0) {
    }

    LibraryInspector() : LibraryUser(), inspectionCount(0) {}

    void inspectUser(const LibraryUser& user) {
        inspectionCount++;
        printf("\n=== Inspection #%d ===\n", inspectionCount);
        printf("Inspecting user: %s\n", user.getName());

        if (strlen(user.getName()) == 0) {
            printf("Warning: User has no name set\n");
        }

        if (user.getAge() <= 0) {
            printf("Warning: Invalid age (%d)\n", user.getAge());
        }

        int books = user.getBooksCount();
        if (books == 0) {
            printf("Info: User has no borrowed books\n");
        }
        else {
            printf("Info: User has %d borrowed books:", books);
            for (int i = 0; i < books; i++) {
                const char* title = user.getBookTitle(i);
                if (title && strlen(title) > 0) {
                    printf("  %d. %s\n", i + 1, title);
                }
                else {
                    printf("  %d. (empty title)\n", i + 1);
                }
            }
        }

        printf("=== End of inspection ===\n");
    }

    int getInspectionCount() const {
        return inspectionCount;
    }
};

int main() {

    LibraryUser user1("Dasha", 21);
    user1.borrowBook("1984");
    user1.borrowBook("To Kill a Mockingbird");

    LibraryUser* user2 = new LibraryUser("Kate", 20);
    user2->borrowBook("The Great Gatsby");

    LibraryInspector inspector("Inspector", 45);

    inspector.inspectUser(user1);
    inspector.inspectUser(*user2);

    printf("\nUser Information:\n");
    user1.displayInfo();
    user2->displayInfo();

    printf("\nReturning books:\n");
    user1.returnBook();
    user2->returnBook();

    inspector.inspectUser(user1);
    inspector.inspectUser(*user2);

    printf("\nTotal inspections performed: %d\n", inspector.getInspectionCount());

    delete user2;

    return 0;
}