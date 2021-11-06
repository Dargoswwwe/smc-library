#include "Book.h"


//Auxiliar function
std::ifstream fin;

std::vector<std::string> Book::DefaultContent(std::ifstream& fin)
{
	std::vector<std::string> default_content;
	std::string line;

	for (std::ifstream fin("default_content.in"); !fin.eof();)
	{
		fin >> line;
		default_content.push_back(line);
	}

	return default_content;
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
	exemplars_available = 0;
	content = Book::DefaultContent(fin);
	available = false;

	assert(exemplars_available >= 0 && exemplars_available <= 3);

}

Book::Book(const std::string& title, const std::vector<std::string>& authors, const std::string& language,
	const uint32_t& original_publication, const float& avarage_rating, uint32_t& ratings_count,
	const std::string& isbn, const uint32_t& exemplars_available, const std::vector<std::string>& content, const bool& available)
	:title{ title }, authors{ authors }, language{ language }, original_publication{ original_publication },
	avarage_rating{ avarage_rating }, ratings_count{ ratings_count }, isbn{ isbn }, exemplars_available{ exemplars_available },
	content{ content }, available{ available }
{
	assert(exemplars_available >= 0 && exemplars_available <= 3);
}

Book::Book(const Book& book)
	:title{ book.title }, authors{ book.authors }, language{ book.language }, original_publication{ book.original_publication },
	avarage_rating{ book.avarage_rating }, ratings_count{ book.ratings_count }, isbn{ book.isbn },
	exemplars_available{ book.exemplars_available }, content{ book.content }, available{ book.available }
{}



//Getters & setters
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

std::vector<std::string> Book::GetContent()const
{
	return content;
}

bool Book::GetAvailability() const
{
	return available;

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

void Book::SetContent(std::vector<std::string>& content)
{
	this->content = content;
}

void Book::SetAvailability(const bool& available)
{
	this->available = available;
}


