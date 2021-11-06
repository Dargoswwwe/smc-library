#pragma once
#include <string>
#include <vector>
#include<fstream>
#include<cassert>

class Book
{
private:
	std::string title;
	std::vector<std::string> authors;
	std::string language;
	uint32_t original_publication;
	float avarage_rating;
	uint32_t ratings_count;
	std::string isbn;
	uint32_t exemplars_available;
	std::vector<std::string> content;

	bool available;
public:

	//Constructors
	Book();
	Book(const std::string& title, const std::vector<std::string>& authors, const std::string& language,
		const uint32_t& original_publication, const float& avarage_rating, uint32_t& ratings_count,
		const std::string& isbn, const uint32_t& exemplars_available, const std::vector<std::string>& content,
		const bool& available);
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
	std::vector<std::string> GetContent()const;
	bool GetAvailability() const;

	void SetTitle(const std::string& title);
	void SetAuthors(const std::vector<std::string>& authors);
	void SetLanguage(const std::string& language);
	void SetOriginal_publication(const uint32_t& original_publication);
	void SetAvarage_rating(const float& avarage_rating);
	void SetRating_count(const uint32_t& rating_count);
	void SetIsbn(const std::string& isbn);
	void SetExemplars_available(const uint32_t& exemplars_available);
	void SetContent(std::vector<std::string>& content);
	void SetAvailability(const bool& available);


	//Auxiliar functions & operators
	std::vector<std::string>DefaultContent(std::ifstream& fin);
	Book& operator=(const Book& book);
	bool operator ==(const Book& book);
	friend std::ostream& operator <<(std::ostream& fout, const std::vector<std::string>& authors_or_content);
	friend std::ostream& operator <<(std::ostream& fout, const Book& book);


};

