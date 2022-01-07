#ifndef LIBRARY_HPP
#define LIBRARY_HPP
#include "book.hpp"
#include "shared_global.hpp"
#include "user.hpp"

#include <fstream>

class SHARED_EXPORT Library
{
private:
    std::vector<Book> allBooks;
    std::vector<Book> availableBooks;
    std::vector<Book> borrowedBooks;
    std::vector<User> users;

public:
    Library();
    Library(std::vector<Book> allBooks, std::vector<Book> availableBooks, std::vector<Book> borrowedBooks,
        std::vector<User> users);
    Library(std::vector<Book> allBooks, std::vector<Book> availableBooks, std::vector<Book> borrowedBooks);
    Library(const Library& other);

    std::vector<Book> getAllBooks() const;
    std::vector<Book> getAvailableBooks() const;
    std::vector<Book> getBorrowedBooks() const;
    std::vector<std::string> getTitles() const;
    std::vector<std::vector<std::string>> getAuthors() const;
    std::vector<User> getUsers() const;

    void setAllBooks(std::vector<Book> allBooks);
    void setAvailableBooks(std::vector<Book> availableBooks);
    void setBorrowedBooks(std::vector<Book> borrowedBooks);
    void setUsers(std::vector<User> users);

    void addBook(Book book);
    void removeBook(Book book);
    void borrowBook(Book book);
    void returnBook(Book book);

    void readBooksFromFile();
    friend std::ostream& operator<<(std::ostream& fout, Library& lib);
    friend std::ostream& operator<<(std::ostream& out, std::vector<Book> booksList_or_usersList);

    ~Library();
};
#endif // LIBRARY_HPP
