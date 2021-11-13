#pragma once
#include "shared_global.hpp"
#include  "rapidcsv.h"

#include <cassert>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

class SHARED_EXPORT Book {
private:
    std::string title;
    std::vector<std::string> authors;
    std::string language;
    uint32_t original_publication;
    float avarage_rating;
    uint32_t ratings_count;
    std::string isbn;
    uint32_t exemplars_available;
    std::string content;
    std::string url;

    bool available;
public:

    //Constructors
    Book();
    Book(const std::string& title, const std::vector<std::string>& authors,
         const std::string& language,const uint32_t& original_publication, const float& avarage_rating,
         uint32_t& ratings_count,const std::string& isbn, const uint32_t& exemplars_available,
         const std::string& content,	const bool& available, const std::string& url);
    Book(const Book& book);


    //Getters & Setters
    std::string GetTitle() const;
    std::vector<std::string> GetAuthors()const;
    std::string GetLanguage()const;
    uint32_t GetOriginal_publication()const;
    float GetAvarage_rating()const;
    uint32_t GetRatings_count()const;
    std::string GetIsbn()const;
    uint32_t GetExemplars_available()const;
    std::string GetContent()const;
    bool GetAvailability() const;
    std::string GetURL() const;

    void SetTitle(const std::string& title);
    void SetAuthors(const std::vector<std::string>& authors);
    void SetLanguage(const std::string& language);
    void SetOriginal_publication(const uint32_t& original_publication);
    void SetAvarage_rating(const float& avarage_rating);
    void SetRating_count(const uint32_t& rating_count);
    void SetIsbn(const std::string& isbn);
    void SetExemplars_available(const uint32_t& exemplars_available);
    void SetContent(std::string& content);
    void SetAvailability(const bool& available);
    void SetURL(const std::string& url);


    //Auxiliar functions & operators
    Book& operator=(const Book& book);
    bool operator ==(const Book& book);
    friend std::ostream& operator <<(std::ostream& fout, const std::vector<std::string>& authors_or_content);
    friend std::ostream& operator <<(std::ostream& fout, const Book& book);

    void Availability();
    void ReadBookFromFile(int index);
};
