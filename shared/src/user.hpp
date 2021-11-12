#pragma once
#include "book.hpp"
#include "shared_global.hpp"
#include <string>
#include <vector>

class SHARED_EXPORT User {
private:
    std::string username, password;
    std::vector<Book> read, borrowing, borrowed;
    bool active;

public:
    User();
    User(const std::string& name, const std::string& pswrd); // recommended
    User(const std::string& name, const std::string& pswrd, const std::vector<Book>& booksRead,
        const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& activ);

    void Borrowed(const Book& book); // recommended
    void Returned(const Book& book); // recommended
    bool HasBook(const Book& book);

    void SetActivity(const bool& actv);
    void SetUsername(const std::string& name);
    void SetPassword(const std::string& pswrd);
    void SetRead(const std::vector<Book>& booksRead);
    void SetBorrowing(const std::vector<Book>& booksBorrowing);
    void SetBorrowed(const std::vector<Book>& booksBorrowed);

    std::vector<Book> GetRead() const;
    std::vector<Book> GetBorrowing() const;
    std::vector<Book> GetBorrowed() const;
    std::string GetUsername() const;
    std::string GetPassword() const;
    bool GetActivity() const;
};
