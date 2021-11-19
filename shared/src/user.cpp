#include "user.hpp"
#include <random>


User::User()
{
    Setsalt();
    username = '\0';
    password = '\0';
    read = {};
    borrowed = {};
    borrowing = {};
    active = 0;
}

User::User(const std::string& name, const std::string& pswrd)
{
    Setsalt();
    username = name;
    password=pswrd+salt;
    password = QCryptographicHash::hash(password.c_str(), QCryptographicHash::Sha3_256).toBase64().toStdString();
    read = {};
    borrowed = {};
    borrowing = {};
    active = 1;
}

User::User(const std::string& name, const std::string& pswrd, const std::vector<Book>& booksRead,
    const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& activ)
{
    Setsalt();
    username = name;
    password=pswrd+salt;
    password = QCryptographicHash::hash(password.c_str(), QCryptographicHash::Sha3_256).toBase64().toStdString();
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

std::string User::gen_random(const int len)
{

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 999999);

        int randomIndex = distr(gen);
        tmp_s += alphanum[randomIndex % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

void User::Setsalt()
{
   salt= gen_random(8);//lenght of salt
}

void User::SetActivity(const bool& actv) { active = actv; }

void User::SetUsername(const std::string& name) { username = name; }

void User::SetPassword(const std::string& pswrd)
{
    password=pswrd+salt;
    password = QCryptographicHash::hash(password.c_str(), QCryptographicHash::Sha3_256).toBase64().toStdString();
}

void User::SetRead(const std::vector<Book>& booksRead) { read = booksRead; }

void User::SetBorrowing(const std::vector<Book>& booksBorrowing) { borrowing = booksBorrowing; }

void User::SetBorrowed(const std::vector<Book>& booksBorrowed) { borrowed = booksBorrowed; }

std::vector<Book> User::GetRead() const { return read; }

std::vector<Book> User::GetBorrowing() const { return borrowing; }

std::vector<Book> User::GetBorrowed() const { return borrowed; }

std::string User::GetUsername() const { return username; }

std::string User::GetPassword() const { return password; }

std::string User::GetSalt() const { return salt; }

bool User::CheckPassword(std::string& pswrd)
{
    pswrd=pswrd+salt;
    if(password == QCryptographicHash::hash(pswrd.c_str(), QCryptographicHash::Sha3_256).toBase64().toStdString())
    {
        return true;
    }
    else
        return false;
}

bool User::GetActivity() const { return active; }
