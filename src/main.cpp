#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>
#include "MemberRepository.h"

void printMenu() {
    std::cout << "==============================\n";
    std::cout << "  회원 관리 시스템\n";
    std::cout << "==============================\n";
    std::cout << " 1. 회원 등록 (Create)\n";
    std::cout << " 2. 전체 목록 조회 (Read All)\n";
    std::cout << " 3. 회원 조회 (Read One)\n";
    std::cout << " 4. 회원 수정 (Update)\n";
    std::cout << " 5. 회원 삭제 (Delete)\n";
    std::cout << " 0. 종료\n";
    std::cout << "------------------------------\n";
    std::cout << "선택 > ";
}

void handleCreate(MemberRepository& repo) {
    Member m{};

    std::cout << "\n[회원 등록]\n";

    while (true) {
        std::cout << "이름 > ";
        std::getline(std::cin, m.name);
        if (!m.name.empty()) break;
        std::cout << "이름은 필수 입력 항목입니다. 다시 입력해주세요.\n";
    }

    while (true) {
        std::cout << "이메일 > ";
        std::getline(std::cin, m.email);
        if (!m.email.empty()) break;
        std::cout << "이메일은 필수 입력 항목입니다. 다시 입력해주세요.\n";
    }

    while (true) {
        std::cout << "전화번호 > ";
        std::getline(std::cin, m.phone);
        if (!m.phone.empty()) break;
        std::cout << "전화번호는 필수 입력 항목입니다. 다시 입력해주세요.\n";
    }

    repo.create(m);
    std::cout << "회원이 등록되었습니다. (부여된 ID: " << m.id << ")\n\n";
}

void handleReadAll(MemberRepository& repo) {
    std::cout << "\n[전체 회원 목록]\n";

    auto members = repo.findAll();
    if (members.empty()) {
        std::cout << "등록된 회원이 없습니다.\n\n";
        return;
    }

    // Table header
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(16) << "이름"
              << std::setw(30) << "이메일"
              << std::setw(16) << "전화번호"
              << std::setw(14) << "등록일"
              << "\n";
    std::cout << std::string(82, '-') << "\n";

    for (const auto& m : members) {
        std::cout << std::left
                  << std::setw(6)  << m.id
                  << std::setw(16) << m.name
                  << std::setw(30) << m.email
                  << std::setw(16) << m.phone
                  << std::setw(14) << m.created_at
                  << "\n";
    }
    std::cout << "\n";
}

void handleReadOne(MemberRepository& repo) {
    std::cout << "\n[회원 조회]\n";
    std::cout << "ID > ";
    std::string input;
    std::getline(std::cin, input);

    int id = 0;
    try {
        id = std::stoi(input);
    } catch (...) {
        std::cout << "유효하지 않은 ID입니다.\n\n";
        return;
    }

    auto result = repo.findById(id);
    if (!result) {
        std::cout << "ID " << id << "에 해당하는 회원이 없습니다.\n\n";
        return;
    }

    const auto& m = *result;
    std::cout << "------------------------------\n";
    std::cout << "ID       : " << m.id         << "\n";
    std::cout << "이름     : " << m.name        << "\n";
    std::cout << "이메일   : " << m.email       << "\n";
    std::cout << "전화번호 : " << m.phone       << "\n";
    std::cout << "등록일   : " << m.created_at  << "\n";
    std::cout << "------------------------------\n\n";
}

void handleUpdate(MemberRepository& repo) {
    std::cout << "\n[회원 수정]\n";
    std::cout << "수정할 회원 ID > ";
    std::string input;
    std::getline(std::cin, input);

    int id = 0;
    try {
        id = std::stoi(input);
    } catch (...) {
        std::cout << "유효하지 않은 ID입니다.\n\n";
        return;
    }

    auto result = repo.findById(id);
    if (!result) {
        std::cout << "ID " << id << "에 해당하는 회원이 없습니다.\n\n";
        return;
    }

    const auto& current = *result;
    std::cout << "현재 정보:\n";
    std::cout << "  이름     : " << current.name  << "\n";
    std::cout << "  이메일   : " << current.email << "\n";
    std::cout << "  전화번호 : " << current.phone << "\n";
    std::cout << "(빈 입력 시 기존 값 유지)\n";

    Member m = current;

    std::cout << "새 이름 [" << current.name << "] > ";
    std::string newName;
    std::getline(std::cin, newName);
    if (!newName.empty()) m.name = newName;

    std::cout << "새 이메일 [" << current.email << "] > ";
    std::string newEmail;
    std::getline(std::cin, newEmail);
    if (!newEmail.empty()) m.email = newEmail;

    std::cout << "새 전화번호 [" << current.phone << "] > ";
    std::string newPhone;
    std::getline(std::cin, newPhone);
    if (!newPhone.empty()) m.phone = newPhone;

    if (repo.update(m)) {
        std::cout << "회원 정보가 수정되었습니다.\n";
        std::cout << "  이름     : " << m.name  << "\n";
        std::cout << "  이메일   : " << m.email << "\n";
        std::cout << "  전화번호 : " << m.phone << "\n\n";
    } else {
        std::cout << "수정에 실패했습니다.\n\n";
    }
}

void handleDelete(MemberRepository& repo) {
    std::cout << "\n[회원 삭제]\n";
    std::cout << "삭제할 회원 ID > ";
    std::string input;
    std::getline(std::cin, input);

    int id = 0;
    try {
        id = std::stoi(input);
    } catch (...) {
        std::cout << "유효하지 않은 ID입니다.\n\n";
        return;
    }

    auto result = repo.findById(id);
    if (!result) {
        std::cout << "ID " << id << "에 해당하는 회원이 없습니다.\n\n";
        return;
    }

    const auto& m = *result;
    std::cout << "삭제 대상:\n";
    std::cout << "  ID       : " << m.id    << "\n";
    std::cout << "  이름     : " << m.name  << "\n";
    std::cout << "  이메일   : " << m.email << "\n";
    std::cout << "  전화번호 : " << m.phone << "\n";
    std::cout << "삭제하시겠습니까? (Y/N) > ";

    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "Y" || confirm == "y") {
        if (repo.remove(id)) {
            std::cout << "회원이 삭제되었습니다.\n\n";
        } else {
            std::cout << "삭제에 실패했습니다.\n\n";
        }
    } else {
        std::cout << "삭제가 취소되었습니다.\n\n";
    }
}

int main() {
    // UTF-8 콘솔 출력 설정 (Windows)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    MemberRepository repo("data/members.json");

    while (true) {
        printMenu();

        std::string line;
        std::getline(std::cin, line);

        int choice = -1;
        try {
            if (!line.empty()) choice = std::stoi(line);
        } catch (...) {
            choice = -1;
        }

        switch (choice) {
            case 1: handleCreate(repo);  break;
            case 2: handleReadAll(repo); break;
            case 3: handleReadOne(repo); break;
            case 4: handleUpdate(repo);  break;
            case 5: handleDelete(repo);  break;
            case 0:
                std::cout << "프로그램을 종료합니다.\n";
                return 0;
            default:
                std::cout << "잘못된 선택입니다. 다시 입력해주세요.\n\n";
                break;
        }
    }

    return 0;
}
