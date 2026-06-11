#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Member {
    int id;
    std::string name;
    std::string email;
    std::string phone;
    std::string created_at;
};

inline void from_json(const json& j, Member& m) {
    m.id         = j["id"].get<int>();
    m.name       = j["name"].get<std::string>();
    m.email      = j["email"].get<std::string>();
    m.phone      = j["phone"].get<std::string>();
    m.created_at = j["created_at"].get<std::string>();
}

inline void to_json(json& j, const Member& m) {
    j["id"]         = m.id;
    j["name"]       = m.name;
    j["email"]      = m.email;
    j["phone"]      = m.phone;
    j["created_at"] = m.created_at;
}
