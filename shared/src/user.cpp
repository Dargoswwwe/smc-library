#include "user.hpp"

#include <random>

std::string User::hash(const std::string& pswrd)
{
    std::string pswrdhash;
    pswrdhash=QCryptographicHash::hash(pswrd.c_str(), QCryptographicHash::Sha3_256).toBase64().toStdString();
    return pswrdhash;
}

User::User()
{
    genSalt();
    username = "\0";
    password = "\0";
    read = {};
    borrowed = {};
    borrowing = {};
    active = 0;
}

User::User(const std::string& name, const std::string& pswrd)
{
    genSalt();
    username = name;
    password = pswrd + salt;
    password = hash(password);
    read = {};
    borrowed = {};
    borrowing = {};
    active = 1;
}

User::User(const std::string& name, const std::string& pswrd, const std::vector<Book>& booksRead,
    const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& activ)
{
    genSalt();
    username = name;
    password = pswrd + salt;
    password = hash(password);
    read = booksRead;
    borrowed = booksBorrowed;
    borrowing = booksBorrowing;
    active = activ;
}

void User::borrowBook(const Book& book)
{
    if (active) {
        borrowed.push_back(book);
        borrowing.push_back(book);
    }
}

void User::returnBook(const Book& book)
{
    if (active)
        if (std::find(borrowing.begin(), borrowing.end(), book) != borrowing.end()) {
            read.push_back(book);
            borrowing.erase(std::find(borrowing.begin(), borrowing.end(), book));
        }
}

bool User::hasBook(const Book& book)
{
    if (std::find(borrowing.begin(), borrowing.end(), book) != borrowing.end()) return true;
    else return false;
}

std::string User::gen_random(const int len)
{

    static const char alphanum[] = "0123456789"
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

void User::genSalt() { salt = gen_random(SALT_LENGTH); }

void User::setSalt(const std::string& slt) { salt = slt; }

void User::setActivity(const bool& actv) { active = actv; }

void User::setUsername(const std::string& name) { username = name; }

void User::setPassword(const std::string& pswrd)
{
    password = pswrd + salt;
    password = hash(password);
}

void User::setHashedPassword(const std::string& hashedPswrd)
{
    password=hashedPswrd;
}

void User::setRead(const std::vector<Book>& booksRead) { read = booksRead; }

void User::setBorrowing(const std::vector<Book>& booksBorrowing) { borrowing = booksBorrowing; }

void User::setBorrowed(const std::vector<Book>& booksBorrowed) { borrowed = booksBorrowed; }

std::vector<Book> User::getRead() const { return read; }

std::vector<Book> User::getBorrowing() const { return borrowing; }

std::vector<Book> User::getBorrowed() const { return borrowed; }

std::string User::getUsername() const { return username; }

std::string User::getPassword() const { return password; }

std::string User::getSalt() const { return salt; }

bool User::checkPassword(std::string& pswrd)
{
    pswrd = pswrd + salt;
    if (password == hash(password)) {
        return true;
    } else return false;
}

bool User::getActivity() const { return active; }
