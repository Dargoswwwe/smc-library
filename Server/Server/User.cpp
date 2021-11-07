#include "User.h"

User::User(const std::string& name, const std::string& pswrd)
{
	username = name;
	password = pswrd;
}

void User::Borrowed(const Book& book)
{
	if (active)
	{
		borrowed.push_back(book);
		borrowing.push_back(book);
	}
}

void User::Returned(const Book& book)
{
	if(active)
		if (std::find(borrowing.begin(), borrowing.end(), book) != borrowing.end())
		{
			read.push_back(book);
			borrowing.erase(std::find(borrowing.begin(), borrowing.end(), book));
		}
}

void User::SetActivity(const bool& actv)
{
	active = actv;
}
