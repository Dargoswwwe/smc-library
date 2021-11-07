#pragma once
#include <string>
#include <vector>
#include "Book.h"

class User
{
private:
	std::string username, password;
	std::vector<Book> read, borrowing, borrowed;
	bool active;

public:
	User(const std::string& name, const std::string& pswrd);

	void Borrowed(const Book& book);
	void Returned(const Book& book);
	void SetActivity(const bool& actv);

}

