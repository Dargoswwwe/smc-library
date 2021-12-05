#ifndef BOOK_HPP
#define BOOK_HPP
#include "rapidcsv.h"
#include "shared_global.hpp"

#include <cassert>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

class SHARED_EXPORT Book
{
private:
    std::string title;
    std::vector<std::string> authors;
    std::string language;
    uint32_t originalPublication;
    float averageRating;
    uint32_t ratingsCount;
    std::string isbn;
    uint32_t exemplarsAvailable;
    std::string content;
    std::string url;

    bool available;

public:
    // Constructors
    Book();
    Book(const std::string& title, const std::vector<std::string>& authors, const std::string& language,
        const uint32_t& originalPublication, const float& averageRating, uint32_t& ratingsCount,
        const std::string& isbn, const uint32_t& exemplarsAvailable, const std::string& content, const bool& available,
        const std::string& url);
    Book(const Book& book);

    // Getters & Setters
    std::string getTitle() const;
    std::vector<std::string> getAuthors() const;
    std::string getLanguage() const;
    uint32_t getOriginalPublication() const;
    float getaverageRating() const;
    uint32_t getRatingsCount() const;
    std::string getIsbn() const;
    uint32_t getExemplarsAvailable() const;
    std::string getContent() const;
    bool getAvailability() const;
    std::string getUrl() const;

    void setTitle(const std::string& title);
    void setAuthors(const std::vector<std::string>& authors);
    void setLanguage(const std::string& language);
    void setOriginalPublication(const uint32_t& originalPublication);
    void setAverageRating(const float& averageRating);
    void setRatingsCount(const uint32_t& ratingsCount);
    void setIsbn(const std::string& isbn);
    void setExemplarsAvailable(const uint32_t& exemplarsAvailable);
    void setContent(std::string& content);
    void setAvailability(const bool& available);
    void setUrl(const std::string& url);

    // Auxiliary functions & operators
    Book& operator=(const Book& book);
    bool operator==(const Book& book);
    friend std::ostream& operator<<(std::ostream& fout, const std::vector<std::string>& authorsOrContent);
    friend std::ostream& operator<<(std::ostream& fout, const Book& book);

    void availability();
    void readBookFromFile(int index);
};
#endif /* BOOK_HPP */
