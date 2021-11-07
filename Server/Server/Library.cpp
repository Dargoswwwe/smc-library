#include "Library.h"
#include<algorithm>
#include <iostream>

Library::Library()
{}

Library::Library(std::vector<Book> _allBooks, std::vector<Book> _availableBooks, std::vector<Book> _borrowedBooks, std::vector<User>_users) :
	allBooks(_allBooks),
	availableBooks(_availableBooks),
	borrowedBooks(_borrowedBooks),
	users(_users)
{}

Library::Library(std::vector<Book> _allBooks, std::vector<Book> _availableBooks, std::vector<Book> _borrowedBooks) :
	allBooks(_allBooks),
	availableBooks(_availableBooks),
	borrowedBooks(_borrowedBooks)
{
}

Library::Library(const Library& other)
{
	this->allBooks = other.allBooks;
	this->availableBooks = other.availableBooks;
	this->borrowedBooks = other.borrowedBooks;
	this->users = other.users;
}
std::vector<Book> Library::GetAllBooks() const
{
	return allBooks;
}
std::vector<Book> Library::GetAvailableBooks() const
{
	return availableBooks;
}
std::vector<Book> Library::GetBorrowedBooks() const
{
	return borrowedBooks;
}
std::vector<std::string> Library::GetTitles() const
{
	std::vector<std::string> titles;
	for (auto current : allBooks)
	{
		titles.push_back(current.GetTitle());
	}
	sort(titles.begin(), titles.end());
	return titles;
}
std::vector<std::vector<std::string>> Library::GetAuthors() const
{
	std::vector<std::vector<std::string>> authors;
	for (auto current : allBooks)
	{
		authors.push_back(current.GetAuthors());
	}
	sort(authors.begin(), authors.end());
	return authors;
}

std::vector<User> Library::GetUsers() const
{
	return users;
}
void Library::SetAllBooks(std::vector<Book> _allBooks)
{
	allBooks = _allBooks;
}
void Library::SetAvailableBooks(std::vector<Book> _availableBooks)
{
	availableBooks = _availableBooks;
}
void Library::SetBorrowedBooks(std::vector<Book> _borrowedBooks)
{
	borrowedBooks = _borrowedBooks;
}
void Library::SetUsers(std::vector<User> _users)
{
	users = _users;
}

Library::~Library()
{
}
