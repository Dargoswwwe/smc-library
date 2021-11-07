#pragma once
#include"Book.h"
#include"User.h"
#include<fstream>

class Library
{
private:

	std::vector<Book> allBooks;
	std::vector<Book> availableBooks;
	std::vector<Book> borrowedBooks;
	std::vector<User> users;

public:
	Library();
	Library(std::vector<Book> _allBooks, std::vector<Book> _availableBooks, std::vector<Book> _borrowedBooks, std::vector<User> _users);
	Library(std::vector<Book> _allBooks, std::vector<Book> _availableBooks, std::vector<Book> _borrowedBooks);
	Library(const Library& other);

	std::vector<Book> GetAllBooks()const;
	std::vector<Book> GetAvailableBooks()const;
	std::vector<Book> GetBorrowedBooks()const;
	std::vector<std::string> GetTitles()const;
	std::vector<std::vector<std::string>> GetAuthors()const;
	std::vector<User> GetUsers()const;

	void SetAllBooks(std::vector<Book> _allBooks);
	void SetAvailableBooks(std::vector<Book> _availableBooks);
	void SetBorrowedBooks(std::vector<Book> _borrowedBooks);
	void SetUsers(std::vector<User> _users);

	~Library();
};