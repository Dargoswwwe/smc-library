#include "library.hpp"
#include <algorithm>
#include <iostream>

Library::Library() { }

Library::Library(std::vector<Book> allBooks, std::vector<Book> availableBooks, std::vector<Book> borrowedBooks,
    std::vector<User> users)
    : allBooks(allBooks)
    , availableBooks(availableBooks)
    , borrowedBooks(borrowedBooks)
    , users(users)
{
}

Library::Library(std::vector<Book> allBooks, std::vector<Book> availableBooks, std::vector<Book> borrowedBooks)
    : allBooks(allBooks)
    , availableBooks(availableBooks)
    , borrowedBooks(borrowedBooks)
{
}

Library::Library(const Library& other)
{
    this->allBooks = other.allBooks;
    this->availableBooks = other.availableBooks;
    this->borrowedBooks = other.borrowedBooks;
    this->users = other.users;
}
std::vector<Book> Library::getAllBooks() const { return allBooks; }
std::vector<Book> Library::getAvailableBooks() const { return availableBooks; }
std::vector<Book> Library::getBorrowedBooks() const { return borrowedBooks; }
std::vector<std::string> Library::getTitles() const
{
    std::vector<std::string> titles;
    for (auto current : allBooks) {
        titles.push_back(current.getTitle());
    }
    sort(titles.begin(), titles.end());
    return titles;
}
std::vector<std::vector<std::string>> Library::getAuthors() const
{
    std::vector<std::vector<std::string>> authors;
    for (auto current : allBooks) {
        authors.push_back(current.getAuthors());
    }
    sort(authors.begin(), authors.end());
    return authors;
}

std::vector<User> Library::getUsers() const { return users; }

void Library::setAllBooks(std::vector<Book> allBooks) { this->allBooks = allBooks; }
void Library::setAvailableBooks(std::vector<Book> availableBooks) { this->availableBooks = availableBooks; }
void Library::setBorrowedBooks(std::vector<Book> borrowedBooks) { this->borrowedBooks = borrowedBooks; }
void Library::setUsers(std::vector<User> users) { this->users = users; }

void Library::addBook(Book book)
{
    allBooks.push_back(book);
    availableBooks.push_back(book);
}

void Library::removeBook(Book book) { allBooks.erase(find(allBooks.begin(), allBooks.end(), book)); }

void Library::borrowBook(Book book)
{
    if (book.getAvailability()) {
        book.availability();
        borrowedBooks.push_back(book);
        book.setExemplarsAvailable(book.getExemplarsAvailable() - 1);
        availableBooks.erase(find(availableBooks.begin(), availableBooks.end(), book));
    } else {
        std::cout << "Book not available";
    }
}

void Library::returnBook(Book book)
{
    if (book.getAvailability() == false) { book.setAvailability(true); }
    book.setExemplarsAvailable(book.getExemplarsAvailable() + 1);
    availableBooks.push_back(book);
    borrowedBooks.erase(find(borrowedBooks.begin(), borrowedBooks.end(), book));
}

void Library::readBooksFromFile()
{
    std::fstream fin;
    fin.open("smc-library-project\\shared\\books.csv", std::ios::in);
    int rows = 0;
    std::string line;
    while (getline(fin, line))
        rows++;

    for (int index = 0; index < rows - 1; index++) {
        Book book;
        book.readBookFromFile(index);
        addBook(book);
    }
}

void sortAvailableBooks(std::vector<Book>& availableBooks)
{
    auto comparator = [](const Book& b1, const Book& b2) { return b1.getTitle() < b2.getTitle(); };
    std::sort(availableBooks.begin(), availableBooks.end(), comparator);
}

std::ostream& operator<<(std::ostream& fout, Library& lib)
{

    fout << "The library has: " << lib.allBooks.size() << " book(s), from which " << lib.availableBooks.size()
         << " available.\n";
    if (lib.availableBooks.size() != 0) {
        sortAvailableBooks(lib.availableBooks);
        fout << " The available books are:\n";
        for (Book availableBook : lib.availableBooks) {
            fout << availableBook << "\n";
        }
    }
    return fout;
}

std::ostream& operator<<(std::ostream& out, std::vector<Book> booksList_or_usersList)
{
    sortAvailableBooks(booksList_or_usersList);
    for (auto current : booksList_or_usersList)
    {
        out << current << "\n";
    }
    return out;
}

Library::~Library() { }
