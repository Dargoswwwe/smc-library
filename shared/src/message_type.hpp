#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include "json.hpp"

enum class MessageType {
    REGISTER,
    LOGIN,
    GET_ALL_BOOKS,
    GET_USER_BOOKS,
    FINISHED,
    CHANGE_USERNAME,
    LOGOUT,
    DELETE_ACCOUNT,
    CHANGE_PASSWORD,
    BORROW_BOOK,
    RETURN_BOOK,
    GET_BORROWED_DATE,
    SEARCH_BOOKS
};

NLOHMANN_JSON_SERIALIZE_ENUM(MessageType,
    {
        { MessageType::REGISTER, "register" },
        { MessageType::LOGIN, "login" },
        { MessageType::GET_USER_BOOKS, "getUserBooks" },
        { MessageType::GET_ALL_BOOKS, "getAllBooks" },
        { MessageType::FINISHED, "finished" },
        { MessageType::CHANGE_USERNAME, "changeUsername" },
        { MessageType::LOGOUT, "logout" },
        { MessageType::DELETE_ACCOUNT, "deleteAccount" },
        { MessageType::CHANGE_PASSWORD, "changePassword" },
        { MessageType::BORROW_BOOK, "borrowBook" },
        { MessageType::RETURN_BOOK, "returnBook" },
        { MessageType::GET_BORROWED_DATE, "getBorrowedDate" },
        { MessageType::SEARCH_BOOKS, "searchBooks" },
    });

#endif // MESSAGE_TYPE_H
