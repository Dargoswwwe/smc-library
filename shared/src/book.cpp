#include "book.hpp"

using nlohmann::json;

// Auxiliary functions & operators
Book& Book::operator=(const Book& book)
{
    this->title = book.title;
    this->authors = book.authors;
    this->language = book.language;
    this->originalPublication = book.originalPublication;
    this->averageRating = book.averageRating;
    this->ratingsCount = book.ratingsCount;
    this->isbn = book.isbn;
    this->exemplarsAvailable = book.exemplarsAvailable;
    this->content = book.content;
    this->available = book.available;
    this->url = book.url;

    return *this;
}

bool Book::operator==(const Book& book)
{
    if (this->authors == book.authors && this->content == book.content && this->isbn == book.isbn
        && this->averageRating == book.averageRating && this->language == book.language
        && this->originalPublication == book.originalPublication && this->ratingsCount == book.ratingsCount
        && this->title == book.title && this->url == book.url)
        return true;
    return false;
}

void Book::availability()
{
    if (getExemplarsAvailable() == 0) { this->available = false; }
}

std::ostream& operator<<(std::ostream& fout, const std::vector<std::string>& authorsOrContent)
{
    for (uint32_t index = 0; index < authorsOrContent.size(); index++)
        fout << authorsOrContent[index] << " ";
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const Book& book)
{

    fout << book.getTitle() << std::endl
         << book.getAuthors() << std::endl
         << book.getLanguage() << std::endl
         << book.getOriginalPublication() << std::endl
         << book.getAverageRating() << std::endl
         << book.getRatingsCount() << std::endl
         << book.getIsbn() << std::endl
         << book.getExemplarsAvailable() << std::endl
         << book.getContent() << std::endl
         << book.getAvailability() << std::endl;
    return fout;
}

void to_json(json& j, const Book& b)
{
    j["title"] = b.title;
    j["authors"] = b.authors;
    j["language"] = b.language;
    j["originalPublication"] = b.originalPublication;
    j["averageRating"] = b.averageRating;
    j["ratingsCount"] = b.ratingsCount;
    j["isbn"] = b.isbn;
    j["exemplarsAvailable"] = b.exemplarsAvailable;
    j["content"] = b.content;
    j["availability"] = b.available;
}

void from_json(const json& j, Book& b)
{
    j.at("title").get_to(b.title);
    j.at("authors").get_to(b.authors);
    j.at("language").get_to(b.language);
    j.at("originalPublication").get_to(b.originalPublication);
    j.at("averageRating").get_to(b.averageRating);
    j.at("ratingsCount").get_to(b.ratingsCount);
    j.at("isbn").get_to(b.isbn);
    j.at("exemplarsAvailable").get_to(b.exemplarsAvailable);
    j.at("content").get_to(b.content);
    j.at("availability").get_to(b.available);
}

void Book::readBookFromFile(int index)
{
    rapidcsv::Document doc("smc-library-project\\shared\\books.csv", rapidcsv::LabelParams(0, 0));

    // get row from index "index"
    std::vector<std::string> line;
    line = doc.GetRow<std::string>(index);

    // Assign to the specific attribute
    //
    // Title
    if (!line[0].empty()) title = line[0];

    // Authors
    if (!line[1].empty()) {
        std::string lineOfAuthors = line[1];
        std::regex dividers(";");

        std::sregex_token_iterator iterator(lineOfAuthors.begin(), lineOfAuthors.end(), dividers, -1);
        std::sregex_token_iterator end;

        for (/*Empty*/; iterator != end; iterator++) {
            std::string aux = *iterator;
            authors.push_back(aux);
        }
    }

    // Language
    if (!line[2].empty()) language = line[2];

    // Original publication
    if (!line[3].empty()) originalPublication = std::stof(line[3]);

    // Average rating
    if (!line[4].empty()) averageRating = std::stof(line[4]);

    // Ratings count
    if (!line[5].empty()) ratingsCount = std::stof(line[5]);

    // Read isbn
    if (!line[6].empty()) isbn = line[6];

    // Read URL
    if (!line[7].empty()) url = line[7];
}

// Constructors
Book::Book()
{
    title = "Title";
    authors = {};
    language = '\0';
    originalPublication = 0;
    averageRating = 0.0;
    ratingsCount = 0;
    isbn = '\0';
    exemplarsAvailable = 3;
    content = "A stranger has kidnapped your child. To free them you must abduct someone else's child.Your child will "
              "be released when your victim's parents kidnap another child.Df any of these things don't happen: your "
              "child will be killed. You are now part of the chain..";
    available = true;
    url = "...";

    assert(exemplarsAvailable >= 0 && exemplarsAvailable <= 3);
}

Book::Book(const std::string& title, const std::vector<std::string>& authors, const std::string& language,
    const uint32_t& originalPublication, const float& averageRating, uint32_t& ratingsCount, const std::string& isbn,
    const uint32_t& exemplarsAvailable, const std::string& content, const bool& available, const std::string& url)
    : title { title }
    , authors { authors }
    , language { language }
    , originalPublication { originalPublication }
    , averageRating { averageRating }
    , ratingsCount { ratingsCount }
    , isbn { isbn }
    , exemplarsAvailable { exemplarsAvailable }
    , content { content }
    , available { available }
    , url { url }
{
    assert(exemplarsAvailable >= 0 && exemplarsAvailable <= 3);
}

Book::Book(const Book& book)
    : title { book.title }
    , authors { book.authors }
    , language { book.language }
    , originalPublication { book.originalPublication }
    , averageRating { book.averageRating }
    , ratingsCount { book.ratingsCount }
    , isbn { book.isbn }
    , exemplarsAvailable { book.exemplarsAvailable }
    , content { book.content }
    , available { book.available }
    , url { book.url }
{ }

// getters and setters

std::string Book::getTitle() const { return title; }

std::vector<std::string> Book::getAuthors() const { return authors; }

std::string Book::getLanguage() const { return language; }

uint32_t Book::getOriginalPublication() const { return originalPublication; }

float Book::getAverageRating() const { return averageRating; }

uint32_t Book::getRatingsCount() const { return ratingsCount; }

std::string Book::getIsbn() const { return isbn; }

uint32_t Book::getExemplarsAvailable() const { return exemplarsAvailable; }

std::string Book::getContent() const { return content; }

bool Book::getAvailability() const { return available; }

std::string Book::getUrl() const { return url; }

void Book::setTitle(const std::string& title) { this->title = title; }

void Book::setAuthors(const std::vector<std::string>& authors) { this->authors = authors; }

void Book::setLanguage(const std::string& language) { this->language = language; }

void Book::setOriginalPublication(const uint32_t& originalPublication)
{
    this->originalPublication = originalPublication;
}

void Book::setAverageRating(const float& averageRating) { this->averageRating = averageRating; }

void Book::setRatingsCount(const uint32_t& ratingsCount) { this->ratingsCount = ratingsCount; }

void Book::setIsbn(const std::string& isbn) { this->isbn = isbn; }

void Book::setExemplarsAvailable(const uint32_t& exemplarsAvailable) { this->exemplarsAvailable = exemplarsAvailable; }

void Book::setContent(std::string& content) { this->content = content; }

void Book::setAvailability(const bool& available) { this->available = available; }

void Book::setUrl(const std::string& url) { this->url = url; }
