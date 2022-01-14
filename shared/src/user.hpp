#ifndef USER_HPP
#define USER_HPP

#include "book.hpp"
#include "json.hpp"
#include "shared_global.hpp"

#include <QCryptographicHash>
#include <string>
#include <vector>

class SHARED_EXPORT User {
private:
    static const int SALT_LENGTH = 8;

    std::string username, password, salt;
    std::vector<Book> read, borrowing, borrowed;
    bool active;

    std::string generateSalt();
    std::string hash(const std::string& password) const;

public:
    User();
    User(const std::string& name, const std::string& password); // recommended
    User(const std::string& name, const std::string& password, const std::string& salt); // for client
    User(const std::string& name, const std::string& password, const std::vector<Book>& booksRead,
        const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& active);
    User(const std::string& name, const std::string& password, const std::vector<Book>& booksRead,
        const std::vector<Book>& booksBorrowing, const std::vector<Book>& booksBorrowed, const bool& active,
        const std::string& salt);

    void borrowBook(const Book& book); // recommended
    void returnBook(const Book& book); // recommended
    bool hasBook(const Book& book);

    void setSalt(const std::string& salt);
    void setActivity(const bool& active);
    void setUsername(const std::string& name);
    void setPassword(const std::string& password);
    void setHashedPassword(const std::string& hashedPassword);
    void setRead(const std::vector<Book>& booksRead);
    void setBorrowing(const std::vector<Book>& booksBorrowing);
    void setBorrowed(const std::vector<Book>& booksBorrowed);

    static std::string generateRandom(const int len);

    std::vector<Book> getRead() const;
    std::vector<Book> getBorrowing() const;
    std::vector<Book> getBorrowed() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getSalt() const;

    bool checkPassword(std::string const& password);
    std::string saltedHash(const std::string& password) const;
    bool getActivity() const;

    using json = nlohmann::json;

    friend void to_json(json& j, User const& u)
    {
        j = json();
        j["username"] = u.username;
        j["password"] = u.password;
        j["salt"] = u.salt;
        j["active"] = u.active;
    }

    friend void from_json(json const& j, User& u)
    {
        j.at("username").get_to(u.username);
        j.at("password").get_to(u.password);
        j.at("salt").get_to(u.salt);
        j.at("active").get_to(u.active);
    }
};
#endif // USER_HPP
