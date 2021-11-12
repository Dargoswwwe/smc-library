#include "library.hpp"
#include <algorithm>
#include <iostream>

Library::Library() { }

Library::Library(std::vector<Book> _allBooks, std::vector<Book> _availableBooks, std::vector<Book> _borrowedBooks,
    std::vector<User> _users)
    : allBooks(_allBooks)
    , availableBooks(_availableBooks)
    , borrowedBooks(_borrowedBooks)
    , users(_users)
{
}

Library::Library(std::vector<Book> _allBooks, std::vector<Book> _availableBooks, std::vector<Book> _borrowedBooks)
    : allBooks(_allBooks)
    , availableBooks(_availableBooks)
    , borrowedBooks(_borrowedBooks)
{
}

Library::Library(const Library& other)
{
    this->allBooks = other.allBooks;
    this->availableBooks = other.availableBooks;
    this->borrowedBooks = other.borrowedBooks;
    this->users = other.users;
}
std::vector<Book> Library::GetAllBooks() const { return allBooks; }
std::vector<Book> Library::GetAvailableBooks() const { return availableBooks; }
std::vector<Book> Library::GetBorrowedBooks() const { return borrowedBooks; }
std::vector<std::string> Library::GetTitles() const
{
    std::vector<std::string> titles;
    for (auto current : allBooks) {
        titles.push_back(current.GetTitle());
    }
    sort(titles.begin(), titles.end());
    return titles;
}
std::vector<std::vector<std::string>> Library::GetAuthors() const
{
    std::vector<std::vector<std::string>> authors;
    for (auto current : allBooks) {
        authors.push_back(current.GetAuthors());
    }
    sort(authors.begin(), authors.end());
    return authors;
}

std::vector<User> Library::GetUsers() const { return users; }

void Library::SetAllBooks(std::vector<Book> _allBooks) { allBooks = _allBooks; }
void Library::SetAvailableBooks(std::vector<Book> _availableBooks) { availableBooks = _availableBooks; }
void Library::SetBorrowedBooks(std::vector<Book> _borrowedBooks) { borrowedBooks = _borrowedBooks; }
void Library::SetUsers(std::vector<User> _users) { users = _users; }

void Library::addBook(Book book)
{
    allBooks.push_back(book);
    availableBooks.push_back(book);
}

void Library::removeBook(Book book) { allBooks.erase(find(allBooks.begin(), allBooks.end(), book)); }

void Library::borrowBook(Book book)
{
    if (book.GetAvailability()) {
        book.Availability();
        borrowedBooks.push_back(book);
        book.SetExemplars_available(book.GetExemplars_available() - 1);
        availableBooks.erase(find(availableBooks.begin(), availableBooks.end(), book));
    } else {
        std::cout << "Book not available";
    }
}

void Library::returnBook(Book book)
{
    if (book.GetAvailability() == false) { book.SetAvailability(true); }
    book.SetExemplars_available(book.GetExemplars_available() + 1);
    availableBooks.push_back(book);
    borrowedBooks.erase(find(borrowedBooks.begin(), borrowedBooks.end(), book));
}

void Library::readBooksFromFile(std::ifstream& fin)
{
    while (!fin.eof()) {
        Book b;
        b.ReadBookFromFile(fin);
        addBook(b);
    }
}

void sortAvailableBooks(std::vector<Book>& availableBooks)
{
    auto comparator = [](const Book& b1, const Book& b2) { return b1.GetTitle() < b2.GetTitle(); };
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
    for (auto current : booksList_or_usersList) {
        out << current << "\n";
    }
    return out;
}

Library::~Library() { }
