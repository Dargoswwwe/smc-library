#include "book.hpp"


//Auxiliar functions & operators
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
    this->url = book.url;

    return *this;
}

bool Book::operator==(const Book& book)
{
    if (this->authors == book.authors && this->content == book.content && this->isbn == book.isbn &&
            this->avarage_rating == book.avarage_rating && this->language == book.language &&
            this->original_publication == book.original_publication && this->ratings_count == book.ratings_count &&
            this->title == book.title && this->url == book.url)
        return true;
    return false;
}

void Book::Availability()
{
    if (GetExemplars_available() == 0)
    {
        this->available = false;
    }

}

std::ostream& operator<<(std::ostream& fout, const std::vector<std::string>& authors_or_content)
{
    for (uint32_t index = 0; index < authors_or_content.size(); index++)
        fout << authors_or_content[index] << " ";
    return fout;
}

std::ostream& operator<<(std::ostream& fout, const Book& book)
{

    fout
            << book.GetTitle() << std::endl
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

void Book::ReadBookFromFile(int index)
{
    rapidcsv::Document doc("smc-library-project\\shared\\books.csv",
                           rapidcsv::LabelParams(0, 0));

    //Get row from index "index"
    std::vector<std::string> line;
    line = doc.GetRow<std::string>(index);

    //Assign to the specific attribute
    //
    //Title
    if (!line[0].empty())
        title = line[0];

    //Authors
    if (!line[1].empty())
    {
        std::string lineOfAuthors = line[1];
        std::regex dividers(";");

        std::sregex_token_iterator iterator(lineOfAuthors.begin(), lineOfAuthors.end(), dividers, -1);
        std::sregex_token_iterator end;

        for (/*Empty*/; iterator != end; iterator++)
        {
            std::string aux = *iterator;
            authors.push_back(aux);
        }
    }

    //Language
    if (!line[2].empty())
        language = line[2];

    //Original publication
    if (!line[3].empty())
        original_publication = std::stof(line[3]);

    //Avarage rating
    if (!line[4].empty())
        avarage_rating = std::stof(line[4]);

    //Ratings count
    if (!line[5].empty())
        ratings_count = std::stof(line[5]);

    //Read isbn
    if (!line[6].empty())
        isbn = line[6];

    //Read URL
    if (!line[7].empty())
        url = line[7];

}

//Constructors
Book::Book()
{
    title = "Title";
    authors = {  };
    language = '\0';
    original_publication = 0;
    avarage_rating = 0.0;
    ratings_count = 0;
    isbn = '\0';
    exemplars_available = 3;
    content = "A stranger has kidnapped your child. To free them you must abduct someone else's child.Your child will be released when your victim's parents kidnap another child.Df any of these things don't happen: your child will be killed. You are now part of the chain..";
    available = true;
    url = "...";

    assert(exemplars_available >= 0 && exemplars_available <= 3);

}

Book::Book(const std::string& title, const std::vector<std::string>& authors,
           const std::string& language, const uint32_t& original_publication, const float& avarage_rating,
           uint32_t& ratings_count, const std::string& isbn, const uint32_t& exemplars_available,
           const std::string& content, const bool& available, const std::string& url)
    : title{ title }, authors{ authors }, language{ language }, original_publication{ original_publication },
      avarage_rating{ avarage_rating }, ratings_count{ ratings_count }, isbn{ isbn },
      exemplars_available{ exemplars_available }, content{ content }, available{ available }, url{ url }
{
    assert(exemplars_available >= 0 && exemplars_available <= 3);
}

Book::Book(const Book& book)
    :
      title{ book.title }, authors{ book.authors }, language{ book.language }, original_publication{ book.original_publication },
      avarage_rating{ book.avarage_rating }, ratings_count{ book.ratings_count }, isbn{ book.isbn },
      exemplars_available{ book.exemplars_available }, content{ book.content }, available{ book.available }, url{ book.url }
{}





//Getters and setters

std::string Book::GetTitle()const
{
    return title;
}

std::vector<std::string> Book::GetAuthors() const
{
    return authors;
}

std::string Book::GetLanguage() const
{
    return language;
}

uint32_t Book::GetOriginal_publication() const
{
    return original_publication;
}

float Book::GetAvarage_rating() const
{
    return avarage_rating;
}

uint32_t Book::GetRatings_count() const
{
    return ratings_count;
}

std::string Book::GetIsbn()const
{
    return isbn;
}

uint32_t Book::GetExemplars_available()const
{
    return exemplars_available;
}

std::string Book::GetContent()const
{
    return content;
}

bool Book::GetAvailability() const
{
    return available;

}

std::string Book::GetURL() const
{
    return url;
}




void Book::SetTitle(const std::string& title)
{
    this->title = title;
}

void Book::SetAuthors(const std::vector<std::string>& authors)
{
    this->authors = authors;
}

void Book::SetLanguage(const std::string& language)
{
    this->language = language;
}

void Book::SetOriginal_publication(const uint32_t& original_publication)
{
    this->original_publication = original_publication;
}

void Book::SetAvarage_rating(const float& avarage_rating)
{
    this->avarage_rating = avarage_rating;
}

void Book::SetRating_count(const uint32_t& rating_count)
{
    this->ratings_count = rating_count;
}

void Book::SetIsbn(const std::string& isbn)
{
    this->isbn = isbn;
}

void Book::SetExemplars_available(const uint32_t& exemplars_available)
{
    this->exemplars_available = exemplars_available;
}

void Book::SetContent(std::string& content)
{
    this->content = content;
}

void Book::SetAvailability(const bool& available)
{
    this->available = available;
}

void Book::SetURL(const std::string& url)
{
    this->url = url;
}


