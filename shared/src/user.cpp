#include "user.hpp"

User::User()
{
    username = '\0';
    password = '\0';
    read = {};
    borrowed = {};
    borrowing = {};
    active = 0;
}

User::User(const std::string& name, const std::string& pswrd)
{
    username = name;
    password = pswrd;
    read = {};
    borrowed = {};
    borrowing = {};
    active = 1;
}

User::User(const std::string& name, const std::string& pswrd, const std::vector<Book>& booksRead,
    const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& activ)
{
    username = name;
    password = pswrd;
    read = booksRead;
    borrowed = booksBorrowed;
    borrowing = booksBorrowing;
    active = activ;
}

void User::Borrowed(const Book& book)
{
    if (active) {
        borrowed.push_back(book);
        borrowing.push_back(book);
    }
}

void User::Returned(const Book& book)
{
    if (active)
        if (std::find(borrowing.begin(), borrowing.end(), book) != borrowing.end()) {
            read.push_back(book);
            borrowing.erase(std::find(borrowing.begin(), borrowing.end(), book));
        }
}

bool User::HasBook(const Book& book)
{
    if (std::find(borrowing.begin(), borrowing.end(), book) != borrowing.end()) return true;
    else return false;
}

void User::SetActivity(const bool& actv) { active = actv; }

void User::SetUsername(const std::string& name) { username = name; }

void User::SetPassword(const std::string& pswrd) { password = pswrd; }

void User::SetRead(const std::vector<Book>& booksRead) { read = booksRead; }

void User::SetBorrowing(const std::vector<Book>& booksBorrowing) { borrowing = booksBorrowing; }

void User::SetBorrowed(const std::vector<Book>& booksBorrowed) { borrowed = booksBorrowed; }

std::vector<Book> User::GetRead() const { return read; }

std::vector<Book> User::GetBorrowing() const { return borrowing; }

std::vector<Book> User::GetBorrowed() const { return borrowed; }

std::string User::GetUsername() const { return username; }

std::string User::GetPassword() const { return password; }

bool User::GetActivity() const { return active; }
