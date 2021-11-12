#include "user.hpp"
#include "book.hpp"
#include <iostream>
#include <QtNetwork/qsctpsocket.h>

int main(int argc, char *argv[])
{
    User u("name", "password");
    std::cout << u.GetPassword();
    Book b;
    return 0;
}
