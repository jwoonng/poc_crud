# poc_crud

JSON 기반 회원정보 CRUD 콘솔 애플리케이션 (C++)

## 프로젝트 개요

- **언어**: C++20
- **빌드**: Visual Studio 2022 (MSVC v145, x64/Win32)
- **솔루션**: `poc_crud.slnx`
- **데이터 저장**: JSON 파일 (`members.json`)
- **JSON 파서**: nlohmann/json v3.11.3 — [jwoonng/json_library_poc](https://github.com/jwoonng/json_library_poc) POC 방식 적용

## JSON 라이브러리 사용 방식

`jwoonng/json_library_poc`의 패턴을 그대로 따른다.

### 패키지 관리 (NuGet)

`packages.config`:
```xml
<?xml version="1.0" encoding="utf-8"?>
<packages>
  <package id="nlohmann.json" version="3.11.3" targetFramework="native" />
</packages>
```

### include

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;
```

### 파일 읽기

```cpp
std::ifstream f("members.json");
json data = json::parse(f);
```

### 파일 쓰기

```cpp
std::ofstream ofs("members.json");
ofs << data.dump(4);
```

### 필드 접근 패턴

```cpp
// 단일 값 꺼내기
std::string name = j["name"].get<std::string>();
int id           = j["id"].get<int>();

// 배열 순회
for (auto& item : j["members"])
    std::cout << item["name"].get<std::string>();

// JSON 빌드
json j;
j["id"]   = 1;
j["name"] = "홍길동";
```

### flat_map 변환 패턴

```cpp
std::unordered_map<std::string, std::string> flat;
for (auto& [key, val] : j.items()) {
    if (val.is_string())
        flat[key] = val.get<std::string>();
    else
        flat[key] = val.dump();
}
```

## 회원 데이터 구조

```json
{
  "members": [
    {
      "id": 1,
      "name": "홍길동",
      "email": "hong@example.com",
      "phone": "010-1234-5678",
      "created_at": "2026-06-11"
    }
  ]
}
```

| 필드          | 타입   | 설명                |
|--------------|--------|-------------------|
| `id`         | int    | 고유 식별자 (자동 증가) |
| `name`       | string | 이름               |
| `email`      | string | 이메일              |
| `phone`      | string | 전화번호             |
| `created_at` | string | 등록일 (YYYY-MM-DD) |

## 파일 구조

```
poc_crud/
├── CLAUDE.md
├── PRD.md
├── packages.config               # NuGet 패키지 목록
├── poc_crud.slnx
├── poc_crud.vcxproj
├── poc_crud.vcxproj.filters
├── docs/
│   └── plan.md                   # 구현 계획
├── src/
│   ├── main.cpp                  # 진입점, 메뉴 루프
│   ├── Member.h                  # Member 구조체
│   └── MemberRepository.h/.cpp  # JSON 파일 I/O + CRUD 로직
└── data/
    └── members.json              # 데이터 저장 파일
```

## CRUD 기능

| 기능   | 설명                         |
|-------|------------------------------|
| Create | 새 회원 등록 (id 자동 채번)    |
| Read   | 전체 목록 조회 / id로 단건 조회 |
| Update | id로 회원 정보 수정            |
| Delete | id로 회원 삭제                |

## 빌드 및 실행

**NuGet 패키지 복원 (최초 1회):**
```
Visual Studio → 솔루션 우클릭 → NuGet 패키지 복원
```

**Visual Studio에서 빌드:**
```
Ctrl+Shift+B  →  빌드
F5            →  디버그 실행
```

**MSBuild CLI:**
```powershell
msbuild poc_crud.vcxproj /p:Configuration=Debug /p:Platform=x64
```

## 코딩 규칙

- C++20 표준 사용
- 파일 인코딩: UTF-8 (한글 지원)
- JSON 파일이 없으면 최초 실행 시 빈 구조(`{"members":[]}`)로 자동 생성
- 예외 처리: `try/catch`로 JSON 파싱 오류 및 파일 I/O 오류 처리
- poc와 동일하게 `#include <nlohmann/json.hpp>` 경로 사용 (NuGet 설치 기준)
