#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime> 
using namespace std;

const int MAX_BOOKS = 50;
const int MAX_REQUESTS = 10; 
const int TITLE_LENGTH = 100;
const int AUTHOR_LENGTH = 100;
const int PUBLISHER_LENGTH = 100;
const int USER_LENGTH = 100;
const int DATE_LENGTH = 11; 
const int MAX_BORROWS = 5; 

class Book {
public:
    char title[TITLE_LENGTH];
    char author[AUTHOR_LENGTH];
    char publisher[PUBLISHER_LENGTH];
    int copies;
    int rackPosition;

    Book() : copies(0), rackPosition(0) {
        title[0] = '\0';   
        author[0] = '\0';  
        publisher[0] = '\0';
    }

    Book(const char* t, const char* a, const char* p, int c, int r) {
        strncpy(title, t, TITLE_LENGTH);
        strncpy(author, a, AUTHOR_LENGTH);
        strncpy(publisher, p, PUBLISHER_LENGTH);
        copies = c;
        rackPosition = r;
    }

    bool match(const char* t, const char* a) const {
        return strcmp(title, t) == 0 && strcmp(author, a) == 0;
    }

    bool isAvailable() const {
        return copies > 0;
    }

    void issueBook() {
        if (copies > 0) {
            copies--;
        }
    }

    void returnBook() {
        copies++;
    }
};

class Student {
public:
    char name[USER_LENGTH];
    char borrowedBooks[MAX_BORROWS][TITLE_LENGTH];
    char borrowedAuthors[MAX_BORROWS][AUTHOR_LENGTH];
    char dueDates[MAX_BORROWS][DATE_LENGTH];
    int borrowCount;

    Student() : borrowCount(0) {
        name[0] = '\0';
    }

    void borrowBook(const char* title, const char* author, const char* dueDate) {
        if (borrowCount < MAX_BORROWS) {
            strncpy(borrowedBooks[borrowCount], title, TITLE_LENGTH);
            strncpy(borrowedAuthors[borrowCount], author, AUTHOR_LENGTH);
            strncpy(dueDates[borrowCount], dueDate, DATE_LENGTH);
            borrowCount++;
        }
    }

    void returnBook(const char* title, const char* author) {
        for (int i = 0; i < borrowCount; i++) {
            if (strcmp(borrowedBooks[i], title) == 0 && strcmp(borrowedAuthors[i], author) == 0) {
                for (int j = i; j < borrowCount - 1; j++) {
                    strcpy(borrowedBooks[j], borrowedBooks[j + 1]);
                    strcpy(borrowedAuthors[j], borrowedAuthors[j + 1]);
                    strcpy(dueDates[j], dueDates[j + 1]);
                }
                borrowCount--;
                return;
            }
        }
    }

    bool hasBorrowed(const char* title, const char* author) const {
        for (int i = 0; i < borrowCount; i++) {
            if (strcmp(borrowedBooks[i], title) == 0 && strcmp(borrowedAuthors[i], author) == 0) {
                return true;
            }
        }
        return false;
    }
};

class Library {
private:
    Book books[MAX_BOOKS];
    Student students[MAX_BOOKS];
    struct Request {
        char title[TITLE_LENGTH];
        char author[AUTHOR_LENGTH];
        char user[USER_LENGTH];
    } requests[MAX_REQUESTS];

    int bookCount;
    int studentCount;
    int requestCount;

public:
    Library() : bookCount(0), studentCount(0), requestCount(0) {
        loadBooksFromFile();
        loadStudentsFromFile();
        loadRequestsFromFile();
    }

    void addBook(const Book& b) {
        if (bookCount < MAX_BOOKS) {
            books[bookCount] = b;
            bookCount++;
            saveBooksToFile();
        } else {
            cout << "Library is full." << endl;
        }
    }

    void loadBooksFromFile() {
        ifstream file("books.txt");
        if (file.is_open()) {
            while (file.good() && bookCount < MAX_BOOKS) {
                Book b;
                file >> b.title >> b.author >> b.publisher >> b.copies >> b.rackPosition;
                addBook(b);
            }
            file.close();
        }
    }

    void saveBooksToFile() {
        ofstream file("books.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < bookCount; i++) {
                file << books[i].title << " "
                     << books[i].author << " "
                     << books[i].publisher << " "
                     << books[i].copies << " "
                     << books[i].rackPosition << endl;
            }
            file.close();
        }
    }

    void loadStudentsFromFile() {
        ifstream file("students.txt");
        if (file.is_open()) {
            while (file.good() && studentCount < MAX_BOOKS) {
                Student s;
                file >> s.name;
                for (int i = 0; i < MAX_BORROWS; i++) {
                    file >> s.borrowedBooks[i] >> s.borrowedAuthors[i] >> s.dueDates[i];
                }
                students[studentCount++] = s;
            }
            file.close();
        }
    }

    void saveStudentsToFile() {
        ofstream file("students.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < studentCount; i++) {
                file << students[i].name << " ";
                for (int j = 0; j < students[i].borrowCount; j++) {
                    file << students[i].borrowedBooks[j] << " "
                         << students[i].borrowedAuthors[j] << " "
                         << students[i].dueDates[j] << " ";
                }
                file << endl;
            }
            file.close();
        }
    }

    void loadRequestsFromFile() {
        ifstream file("requests.txt");
        if (file.is_open()) {
            while (file.good() && requestCount < MAX_REQUESTS) {
                Request r;
                file >> r.title >> r.author >> r.user;
                requests[requestCount++] = r;
            }
            file.close();
        }
    }

    void saveRequestsToFile() {
        ofstream file("requests.txt", ios::trunc);
        if (file.is_open()) {
            for (int i = 0; i < requestCount; i++) {
                file << requests[i].title << " "
                     << requests[i].author << " "
                     << requests[i].user << endl;
            }
            file.close();
        }
    }

 void addRequestedBook(const char* title, const char* author) {
        
        for (int i = 0; i < requestCount; i++) {
            if (strcmp(requests[i].title, title) == 0 && strcmp(requests[i].author, author) == 0) {
             
                int copies;
                cout << "Request found for " << requests[i].user << ". Enter number of copies to add: ";
                cin >> copies;
                cin.ignore();
                
                
                Book newBook(title, author, "Unknown Publisher", copies, bookCount + 1); 
                addBook(newBook);
                
                for (int j = i; j < requestCount - 1; j++) {
                    requests[j] = requests[j + 1];
                }
                requestCount--;
                saveRequestsToFile(); 
                cout << "Requested book added to the library!" << endl;
                return;
            }
        }
        cout << "No matching request found." << endl;
    }

    void searchBook(const char* title, const char* author) const {
        for (int i = 0; i < bookCount; i++) {
            if (books[i].match(title, author)) {
                cout << "Book found!" << endl;
                cout << "Title: " << books[i].title << endl;
                cout << "Author: " << books[i].author << endl;
                cout << "Publisher: " << books[i].publisher << endl;
                cout << "Copies: " << books[i].copies << endl;
                cout << "Rack Position: " << books[i].rackPosition << endl;
                cout << "Availability: " << (books[i].isAvailable() ? "Available" : "Not Available") << endl;
                return;
            }
        }
        cout << "Book not found." << endl;
    }

    void issueBook(const char* title, const char* author, const char* user) {
        for (int i = 0; i < bookCount; i++) {
            if (books[i].match(title, author) && books[i].isAvailable()) {
                time_t now = time(0);
                tm* ltm = localtime(&now);
                char dueDate[DATE_LENGTH];
                snprintf(dueDate, sizeof(dueDate), "%04d-%02d-%02d", 
                         1900 + ltm->tm_year, 
                         1 + ltm->tm_mon, 
                         ltm->tm_mday + 15); 

                for (int j = 0; j < studentCount; j++) {
                    if (strcmp(students[j].name, user) == 0) {
                        students[j].borrowBook(title, author, dueDate);
                        books[i].issueBook();
                        saveStudentsToFile();
                        cout << "Book issued to " << user << ". Return by " << dueDate << "." << endl;
                        return;
                    }
                }

                Student newStudent;
                strncpy(newStudent.name, user, USER_LENGTH);
                newStudent.borrowBook(title, author, dueDate);
                students[studentCount++] = newStudent;
                books[i].issueBook();
                saveStudentsToFile();
                cout << "Book issued to " << user << ". Return by " << dueDate << "." << endl;
                return;
            }
        }
        cout << "Book not found or not available." << endl;
    }

    void returnBook(const char* title, const char* author, const char* user) {
        for (int i = 0; i < bookCount; i++) {
            if (books[i].match(title, author)) {
                for (int j = 0; j < studentCount; j++) {
                    if (strcmp(students[j].name, user) == 0 && students[j].hasBorrowed(title, author)) {
                        time_t now = time(0);
                        tm* ltm = localtime(&now);
                        char currentDate[DATE_LENGTH];
                        snprintf(currentDate, sizeof(currentDate), "%04d-%02d-%02d", 
                                 1900 + ltm->tm_year, 
                                 1 + ltm->tm_mon, 
                                 ltm->tm_mday);
                        if (strcmp(currentDate, students[j].dueDates[0]) > 0) {
                            cout << "Book returned late! Fine imposed." << endl;
                        }

                        students[j].returnBook(title, author);
                        books[i].returnBook();
                        saveStudentsToFile(); 
                        cout << "Book returned by " << user << "." << endl;
                        return;
                    }
                }
                cout << "No record of this user borrowing the book." << endl;
                return;
            }
        }
        cout << "Book not found." << endl;
    }

    void addRequest(const char* title, const char* author, const char* user) {
        if (requestCount < MAX_REQUESTS) {
            strncpy(requests[requestCount].title, title, TITLE_LENGTH);
            strncpy(requests[requestCount].author, author, AUTHOR_LENGTH);
            strncpy(requests[requestCount].user, user, USER_LENGTH);
            requestCount++;
            saveRequestsToFile(); 
            cout << "Request added for " << user << " to notify when the book becomes available." << endl;
        } else {
            cout << "Request list is full. Cannot add more requests." << endl;
        }
    }

    void viewRequests() const {
        if (requestCount == 0) {
            cout << "No current requests." << endl;
            return;
        }
        cout << "Current book requests:" << endl;
        for (int i = 0; i < requestCount; i++) {
            cout << "Request " << (i + 1) << ":" << endl;
            cout << "Title: " << requests[i].title << endl;
            cout << "Author: " << requests[i].author << endl;
            cout << "Requested by: " << requests[i].user << endl;
        }
    }
};

int main() {
    Library lib;

    lib.addBook(Book("C++ Programming", "Harsha", "Tech Publishers", 5, 101));
    lib.addBook(Book("Data Structures", "Ramcharan", "Edu Publishers", 3, 102));
    lib.addBook(Book("Algorithms", "Akshith", "Code Books", 4, 103));
    lib.addBook(Book("Operating Systems", "Sharan", "System Publishers", 2, 104));
    lib.addBook(Book("Frontend", "Adarsh", "Database Experts", 6, 105));

    int choice;
    do {
        cout << "\nLibrary Menu:\n";
        cout << "1. Add Requested Book (Management Only)\n";
        cout << "2. Search Book\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. View Requests\n";
        cout << "6. Request Book\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); 

        char title[TITLE_LENGTH], author[AUTHOR_LENGTH], user[USER_LENGTH];

        switch (choice) {
            case 1:
          cout << "Enter book title: ";
                cin.getline(title, TITLE_LENGTH);
                cout << "Enter book author: ";
                cin.getline(author, AUTHOR_LENGTH);
                lib.addRequestedBook(title, author); 
                break;
            case 2:
                cout << "Enter book title: ";
                cin.getline(title, TITLE_LENGTH);
                cout << "Enter book author: ";
                cin.getline(author, AUTHOR_LENGTH);
                lib.searchBook(title, author);
                break;
            case 3:
                cout << "Enter book title: ";
                cin.getline(title, TITLE_LENGTH);
                cout << "Enter book author: ";
                cin.getline(author, AUTHOR_LENGTH);
                cout << "Enter user name: ";
                cin.getline(user, USER_LENGTH);
                lib.issueBook(title, author, user);
                break;
            case 4:
                cout << "Enter book title: ";
                cin.getline(title, TITLE_LENGTH);
                cout << "Enter book author: ";
                cin.getline(author, AUTHOR_LENGTH);
                cout << "Enter user name: ";
                cin.getline(user, USER_LENGTH);
                lib.returnBook(title, author, user);
                break;
            case 5:
                lib.viewRequests();
                break;
            case 6:
                cout << "Enter book title: ";
                cin.getline(title, TITLE_LENGTH);
                cout << "Enter book author: ";
                cin.getline(author, AUTHOR_LENGTH);
                cout << "Enter user name: ";
                cin.getline(user, USER_LENGTH);
                lib.addRequest(title, author, user);
                break;
            case 7:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 6);

    return 0;
}
