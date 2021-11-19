#pragma once
#include "book.hpp"
#include "shared_global.hpp"
#include <string>
#include <vector>
#include <QCryptographicHash>

class SHARED_EXPORT User {
private:
    std::string username, password, salt;
    std::vector<Book> read, borrowing, borrowed;
    bool active;
    void Setsalt();

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

    std::string  gen_random(const int len);


    std::vector<Book> GetRead() const;
    std::vector<Book> GetBorrowing() const;
    std::vector<Book> GetBorrowed() const;
    std::string GetUsername() const;
    std::string GetPassword() const;
    std::string GetSalt() const;

    bool CheckPassword(std::string& pswrd);
    bool GetActivity() const;
};