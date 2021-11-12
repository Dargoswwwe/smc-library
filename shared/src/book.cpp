#include "book.hpp"
#include <iostream>

// Auxiliar functions & operators
std::ifstream fin;

std::vector<std::string> Book::DefaultContent(std::ifstream& fin)
{
    std::vector<std::string> default_content;
    std::string line;

    for (std::ifstream fin("default_content.in"); !fin.eof();) {
        fin >> line;
        default_content.push_back(line);
    }

    return default_content;
}

Book& Book::operator=(const Book& book)
{
    this->title = book.title;
    this->authors = book.authors;
    this->language = book.language;
    this->original_publication = book.original_publication;
    this->avarage_rating = book.avarage_rating;
    this->ratings_count = book.ratings_count;
    this->isbn = book.isbn;
    this->exemplars_available = book.exemplars_available;
    this->content = book.content;
    this->available = book.available;

    return *this;
}

bool Book::operator==(const Book& book)
{
    if (this->authors == book.authors && this->content == book.content && this->isbn == book.isbn
        && this->avarage_rating == book.avarage_rating && this->language == book.language
        && this->original_publication == book.original_publication && this->ratings_count == book.ratings_count
        && this->title == book.title)
        return true;
    return false;
}

std::ostream& operator<<(std::ostream& fout, const std::vector<std::string>& authors_or_content)
{
    for (uint32_t index = 0; index < authors_or_content.size(); index++)
        fout << authors_or_content[index] << " ";
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const Book& book)
{

    fout << book.GetTitle() << std::endl
         << book.GetAuthors() << std::endl
         << book.GetLanguage() << std::endl
         << book.GetOriginal_publication() << std::endl
         << book.GetAvarage_rating() << std::endl
         << book.GetRatings_count() << std::endl
         << book.GetIsbn() << std::endl
         << book.GetExemplars_available() << std::endl
         << book.GetContent() << std::endl
         << book.GetAvailability() << std::endl;
    return fout;
}

void Book::Availability()
{
    if (GetExemplars_available() == 0) { this->available = false; }
}

void Book::ReadBookFromFile(std::ifstream& fin)
{
    // read title
    getline(fin, title);

    // read authors
    std::string line;
    std::regex dividers(",");
    getline(fin, line);

    std::sregex_token_iterator iterator(line.begin(), line.end(), dividers, -1);
    std::sregex_token_iterator end;

    for (/*Empty*/; iterator != end; iterator++) {
        std::string aux = *iterator;
        authors.push_back(aux);
    }

    // read langueage
    getline(fin, language);

    // read original publication
    getline(fin, line);
    original_publication = std::stof(line);

    // read avarage rating
    getline(fin, line);
    avarage_rating = std::stof(line);

    // read rating_count
    getline(fin, line);
    ratings_count = std::stof(line);

    // read isbn
    getline(fin, isbn);

    // read exemplars_available
    getline(fin, line);
    exemplars_available = std::stof(line);

    // read content
    content.clear();
    getline(fin, line);
    std::regex dividers1("\\s+");

    iterator = std::sregex_token_iterator(line.begin(), line.end(), dividers1, -1);

    for (/*Empty*/; iterator != end; iterator++) {
        std::string aux = *iterator;
        content.push_back(aux);
    }

    // read availability
    getline(fin, line);
    if (line == "true") available = true;
    else if (line == "false") available = false;
}

// Constructors
Book::Book()
{
    title = "Title";
    authors = {};
    language = '\0';
    original_publication = 0;
    avarage_rating = 0.0;
    ratings_count = 0;
    isbn = '\0';
    exemplars_available = 0;
    std::cout << "HELLOOOO" << std::endl;
    content = Book::DefaultContent(fin);
    std::cout << "goodbye" << std::endl;
    available = false;

    assert(exemplars_available >= 0 && exemplars_available <= 3);
}

Book::Book(const std::string& title, const std::vector<std::string>& authors, const std::string& language,
    const uint32_t& original_publication, const float& avarage_rating, uint32_t& ratings_count, const std::string& isbn,
    const uint32_t& exemplars_available, const std::vector<std::string>& content, const bool& available)
    : title { title }
    , authors { authors }
    , language { language }
    , original_publication { original_publication }
    , avarage_rating { avarage_rating }
    , ratings_count { ratings_count }
    , isbn { isbn }
    , exemplars_available { exemplars_available }
    , content { content }
    , available { available }
{
    assert(exemplars_available >= 0 && exemplars_available <= 3);
}

Book::Book(const Book& book)
    : title { book.title }
    , authors { book.authors }
    , language { book.language }
    , original_publication { book.original_publication }
    , avarage_rating { book.avarage_rating }
    , ratings_count { book.ratings_count }
    , isbn { book.isbn }
    , exemplars_available { book.exemplars_available }
    , content { book.content }
    , available { book.available }
{
}

// Getters and setters
std::string Book::GetTitle() const { return title; }

std::vector<std::string> Book::GetAuthors() const { return authors; }

std::string Book::GetLanguage() const { return language; }

uint32_t Book::GetOriginal_publication() const { return original_publication; }

float Book::GetAvarage_rating() const { return avarage_rating; }

uint32_t Book::GetRatings_count() const { return ratings_count; }

std::string Book::GetIsbn() const { return isbn; }

uint32_t Book::GetExemplars_available() const { return exemplars_available; }

std::vector<std::string> Book::GetContent() const { return content; }

bool Book::GetAvailability() const { return available; }

void Book::SetTitle(const std::string& title) { this->title = title; }

void Book::SetAuthors(const std::vector<std::string>& authors) { this->authors = authors; }

void Book::SetLanguage(const std::string& language) { this->language = language; }

void Book::SetOriginal_publication(const uint32_t& original_publication)
{
    this->original_publication = original_publication;
}

void Book::SetAvarage_rating(const float& avarage_rating) { this->avarage_rating = avarage_rating; }

void Book::SetRating_count(const uint32_t& rating_count) { this->ratings_count = rating_count; }

void Book::SetIsbn(const std::string& isbn) { this->isbn = isbn; }

void Book::SetExemplars_available(const uint32_t& exemplars_available)
{
    this->exemplars_available = exemplars_available;
}

void Book::SetContent(std::vector<std::string>& content) { this->content = content; }

void Book::SetAvailability(const bool& available) { this->available = available; }
