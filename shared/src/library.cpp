#include "library.hpp"

#include <algorithm>
#include <iostream>

Library::Library() { }

Library::Library(std::vector<Book> allBooks)
    : allBooks(allBooks)
{
}

Library::Library(const Library& other)
{
    this->allBooks = other.allBooks;
}

std::vector<Book> Library::getAllBooks() const { return allBooks; }
void Library::setAllBooks(std::vector<Book> allBooks) { this->allBooks = allBooks; }

void Library::addBook(Book book)
{
    allBooks.push_back(book);
}

void Library::removeBook(Book book) { allBooks.erase(find(allBooks.begin(), allBooks.end(), book)); }

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

std::ostream& operator<<(std::ostream& fout, Library& lib)
{

    fout << "The library has: " << lib.allBooks.size();
    for (Book b : lib.allBooks) {
        fout << b << "\n";
    }

    return fout;
}

Library::~Library() { }
