#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include "json.hpp"

enum class MessageType
{
    REGISTER,
    LOGIN,
    GET_BOOKS,
    CHANGE_USERNAME,
    LOGOUT,
    DELETE_ACCOUNT,
    CHANGE_PASSWORD,
};

NLOHMANN_JSON_SERIALIZE_ENUM(MessageType,
                             {
                                 {MessageType::REGISTER,   "register"},
                                 { MessageType::LOGIN,     "login"   },
                                 { MessageType::GET_BOOKS, "getBooks"},
                                 { MessageType::CHANGE_USERNAME, "changeUsername"},
                                 { MessageType::LOGOUT, "logout"},
                                 { MessageType::DELETE_ACCOUNT, "deleteAccount"},
                                 { MessageType::CHANGE_PASSWORD, "changePassword"},
                             }
                             );

#endif // MESSAGE_TYPE_H
