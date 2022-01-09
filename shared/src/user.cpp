#include "user.hpp"

#include <random>

User::User()
    : User("", "", {}, {}, {}, false) {};

User::User(const std::string& name, const std::string& password)
    : User(name, password, {}, {}, {}, true) {};

User::User(const std::string& name, const std::string& password, const std::string& salt)
    : User(name, password, {}, {}, {}, true, salt) {};

User::User(const std::string& name, const std::string& password, const std::vector<Book>& booksRead,
    const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& active)
    : User(name, password, booksRead, booksBorrowing, booksBorrowed, active, generateSalt()) {};

User::User(const std::string& name, const std::string& password, const std::vector<Book>& booksRead,
    const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& active,
    const std::string& salt)
    : username(name)
    , read(booksRead)
    , borrowing(booksBorrowing)
    , borrowed(booksBorrowed)
    , active(active)
    , salt(salt)
{
    setPassword(password);
}

std::string User::hash(const std::string& string) const
{
    std::string hash;
    hash = QCryptographicHash::hash(string.c_str(), QCryptographicHash::Sha3_256).toBase64().toStdString();
    return hash;
}

std::string User::saltedHash(const std::string& password) const { return hash(password + salt); }

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

std::string User::generateRandom(const int len)
{

    static const char alphanum[] = "0123456789"
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    result.reserve(len);

    for (int i = 0; i < len; ++i) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 999999);

        int randomIndex = distr(gen);
        result += alphanum[randomIndex % (sizeof(alphanum) - 1)];
    }

    return result;
}

std::string User::generateSalt() { return generateRandom(SALT_LENGTH); }

void User::setSalt(const std::string& salt) { this->salt = salt; }

void User::setActivity(const bool& active) { this->active = active; }

void User::setUsername(const std::string& username) { this->username = username; }

void User::setPassword(const std::string& password) { this->password = saltedHash(password); }

void User::setHashedPassword(const std::string& hashedPassword) { password = hashedPassword; }

void User::setRead(const std::vector<Book>& booksRead) { read = booksRead; }

void User::setBorrowing(const std::vector<Book>& booksBorrowing) { borrowing = booksBorrowing; }

void User::setBorrowed(const std::vector<Book>& booksBorrowed) { borrowed = booksBorrowed; }

std::vector<Book> User::getRead() const { return read; }

std::vector<Book> User::getBorrowing() const { return borrowing; }

std::vector<Book> User::getBorrowed() const { return borrowed; }

std::string User::getUsername() const { return username; }

std::string User::getPassword() const { return password; }

std::string User::getSalt() const { return salt; }

bool User::checkPassword(std::string const& password) { return this->password == saltedHash(password); }

bool User::getActivity() const { return active; }
