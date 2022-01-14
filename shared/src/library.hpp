#ifndef LIBRARY_HPP
#define LIBRARY_HPP
#include "book.hpp"
#include "shared_global.hpp"

#include <fstream>

class SHARED_EXPORT Library {
private:
    std::vector<Book> allBooks;

public:
    Library();
    Library(std::vector<Book> allBooks);
    Library(const Library& other);

    std::vector<Book> getAllBooks() const;

    void setAllBooks(std::vector<Book> allBooks);

    void addBook(Book book);
    void removeBook(Book book);

    void readBooksFromFile();
    friend std::ostream& operator<<(std::ostream& fout, Library& lib);

    ~Library();
};
#endif // LIBRARY_HPP
