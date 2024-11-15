// message_handler.h
#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <nlohmann/json.hpp>

std::string handleMessage(const nlohmann::json& request);

#endif
