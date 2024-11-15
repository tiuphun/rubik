// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <nlohmann/json.hpp>

std::string getCurrentTimestamp();
bool validateJSON(const nlohmann::json& data);

#endif
