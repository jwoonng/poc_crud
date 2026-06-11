#pragma once

#include "Member.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <optional>
#include <string>

class MemberRepository {
public:
    explicit MemberRepository(const std::string& filePath);

    void create(Member& m);
    std::vector<Member> findAll() const;
    std::optional<Member> findById(int id) const;
    bool update(const Member& m);
    bool remove(int id);

private:
    std::string filePath_;
    nlohmann::json data_;

    void load();
    void save() const;
    int nextId() const;
};
