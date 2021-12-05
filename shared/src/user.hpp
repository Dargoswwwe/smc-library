#ifndef USER_HPP
#define USER_HPP
#include "book.hpp"
#include "shared_global.hpp"

#include <QCryptographicHash>
#include <string>
#include <vector>

class SHARED_EXPORT User
{
private:
    static const int SALT_LENGTH = 8;
    std::string username, password, salt;
    std::vector<Book> read, borrowing, borrowed;
    bool active;

    void setSalt();

public:
    User();
    User(const std::string& name, const std::string& pswrd); // recommended
    User(const std::string& name, const std::string& pswrd, const std::vector<Book>& booksRead,
        const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& activ);

    void borrowBook(const Book& book); // recommended
    void returnBook(const Book& book); // recommended
    bool hasBook(const Book& book);

    void setActivity(const bool& actv);
    void setUsername(const std::string& name);
    void setPassword(const std::string& pswrd);
    void setRead(const std::vector<Book>& booksRead);
    void setBorrowing(const std::vector<Book>& booksBorrowing);
    void setBorrowed(const std::vector<Book>& booksBorrowed);

    static std::string gen_random(const int len);

    std::vector<Book> getRead() const;
    std::vector<Book> getBorrowing() const;
    std::vector<Book> getBorrowed() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getSalt() const;

    bool checkPassword(std::string& pswrd);
    bool getActivity() const;
};
#endif /* USER_HPP */
