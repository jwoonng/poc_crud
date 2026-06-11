#include "MemberRepository.h"
#include <fstream>
#include <chrono>
#include <format>
#include <algorithm>
#include <stdexcept>
#include <iostream>

MemberRepository::MemberRepository(const std::string& filePath)
    : filePath_(filePath)
{
    load();
}

void MemberRepository::load() {
    std::ifstream f(filePath_);
    if (!f.is_open()) {
        data_ = json{{"members", json::array()}};
        save();
        return;
    }
    try {
        data_ = json::parse(f);
    } catch (const json::parse_error& e) {
        std::cerr << "[MemberRepository] JSON parse error: " << e.what() << "\n";
        data_ = json{{"members", json::array()}};
    }
}

void MemberRepository::save() const {
    std::ofstream ofs(filePath_);
    ofs << data_.dump(4);
}

int MemberRepository::nextId() const {
    const auto& members = data_["members"];
    if (members.empty()) {
        return 1;
    }
    int maxId = 0;
    for (const auto& item : members) {
        int id = item["id"].get<int>();
        if (id > maxId) {
            maxId = id;
        }
    }
    return maxId + 1;
}

void MemberRepository::create(Member& m) {
    // Precondition: name, email, phone must not be empty.
    if (m.name.empty() || m.email.empty() || m.phone.empty())
        throw std::invalid_argument("name, email, phone must not be empty");

    m.id = nextId();

    auto now = std::chrono::system_clock::now();
    auto dp   = std::chrono::floor<std::chrono::days>(now);
    std::chrono::year_month_day ymd{dp};
    m.created_at = std::format("{:04d}-{:02d}-{:02d}",
        static_cast<int>(ymd.year()),
        static_cast<unsigned>(ymd.month()),
        static_cast<unsigned>(ymd.day()));

    json j;
    to_json(j, m);
    data_["members"].push_back(j);
    save();
}

std::vector<Member> MemberRepository::findAll() const {
    std::vector<Member> result;
    for (const auto& item : data_["members"]) {
        result.push_back(item.get<Member>());
    }
    std::sort(result.begin(), result.end(), [](const Member& a, const Member& b){ return a.id < b.id; });
    return result;
}

std::optional<Member> MemberRepository::findById(int id) const {
    for (const auto& item : data_["members"]) {
        if (item["id"].get<int>() == id) {
            return item.get<Member>();
        }
    }
    return std::nullopt;
}

bool MemberRepository::update(const Member& m) {
    auto& members = data_["members"];
    for (auto& item : members) {
        if (item["id"].get<int>() == m.id) {
            if (!m.name.empty())  item["name"]  = m.name;
            if (!m.email.empty()) item["email"] = m.email;
            if (!m.phone.empty()) item["phone"] = m.phone;
            save();
            return true;
        }
    }
    return false;
}

bool MemberRepository::remove(int id) {
    auto& members = data_["members"];
    auto it = std::find_if(members.begin(), members.end(),
        [id](const json& item) {
            return item["id"].get<int>() == id;
        });
    if (it == members.end()) {
        return false;
    }
    members.erase(it);
    save();
    return true;
}
