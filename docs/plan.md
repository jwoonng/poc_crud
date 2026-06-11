# 구현 계획 — poc_crud

## 단계 개요

| 단계 | 내용                        | 산출물                              |
|-----|-----------------------------|------------------------------------|
| 1   | 프로젝트 환경 세팅            | `packages.config`, 디렉토리 구조     |
| 2   | 데이터 파일 준비              | `data/members.json`                |
| 3   | Member 구조체 정의            | `src/Member.h`                     |
| 4   | MemberRepository 구현         | `src/MemberRepository.h/.cpp`      |
| 5   | main 메뉴 루프 구현           | `src/main.cpp`                     |
| 6   | vcxproj에 소스 파일 등록       | `poc_crud.vcxproj` 수정             |
| 7   | 빌드 및 동작 검증             | 전체 CRUD 시나리오 수동 테스트        |

---

## 단계 1 — 프로젝트 환경 세팅

**목표**: NuGet 패키지 설정 및 디렉토리 구조 생성

- [ ] `packages.config` 생성 (nlohmann.json v3.11.3)
- [ ] `src/` 디렉토리 생성
- [ ] `data/` 디렉토리 생성
- [ ] Visual Studio에서 NuGet 패키지 복원

```xml
<!-- packages.config -->
<?xml version="1.0" encoding="utf-8"?>
<packages>
  <package id="nlohmann.json" version="3.11.3" targetFramework="native" />
</packages>
```

---

## 단계 2 — 데이터 파일 준비

**목표**: 초기 JSON 파일 생성 (빈 구조)

- [ ] `data/members.json` 생성

```json
{
  "members": []
}
```

> 파일이 없을 경우 프로그램 실행 시 자동 생성하는 로직도 `MemberRepository`에 포함

---

## 단계 3 — Member 구조체 정의

**파일**: `src/Member.h`

- [ ] `Member` 구조체 정의
  - 필드: `id(int)`, `name`, `email`, `phone`, `created_at` (모두 string, id 제외)
- [ ] `json → Member` 변환 함수 (`from_json`)
- [ ] `Member → json` 변환 함수 (`to_json`)

```cpp
struct Member {
    int         id;
    std::string name;
    std::string email;
    std::string phone;
    std::string created_at;
};

// nlohmann/json 변환 헬퍼
void from_json(const json& j, Member& m);
void to_json(json& j, const Member& m);
```

---

## 단계 4 — MemberRepository 구현

**파일**: `src/MemberRepository.h` / `src/MemberRepository.cpp`

### 4-1. 인터페이스 (MemberRepository.h)

- [ ] 생성자: JSON 파일 경로를 받아 로드, 없으면 자동 생성
- [ ] `create(Member&)` — id 자동 채번 후 저장
- [ ] `findAll()` — 전체 목록 반환
- [ ] `findById(int)` — 단건 반환 (없으면 nullopt)
- [ ] `update(Member&)` — id 기준 수정 후 저장
- [ ] `remove(int)` — id 기준 삭제 후 저장

```cpp
class MemberRepository {
public:
    explicit MemberRepository(const std::string& filePath);

    void                         create(Member& m);
    std::vector<Member>          findAll() const;
    std::optional<Member>        findById(int id) const;
    bool                         update(const Member& m);
    bool                         remove(int id);

private:
    std::string        filePath_;
    json               data_;      // { "members": [...] }

    void load();
    void save() const;
    int  nextId() const;
};
```

### 4-2. 구현 포인트 (MemberRepository.cpp)

- [ ] `load()`: `std::ifstream` + `json::parse()` — poc 패턴 동일
- [ ] `save()`: `std::ofstream` + `data_.dump(4)`
- [ ] `nextId()`: `members` 배열에서 최대 id + 1
- [ ] `create`: `created_at`에 현재 날짜 자동 기록
- [ ] `update`/`remove`: id로 배열 탐색 → 수정/삭제 후 `save()` 호출

---

## 단계 5 — main 메뉴 루프 구현

**파일**: `src/main.cpp`

- [ ] 콘솔 메뉴 출력 함수
- [ ] 각 메뉴 항목별 핸들러 함수
  - `handleCreate()` — 이름·이메일·전화번호 입력 → `repo.create()`
  - `handleReadAll()` — `repo.findAll()` → 테이블 형식 출력
  - `handleReadOne()` — id 입력 → `repo.findById()` → 상세 출력
  - `handleUpdate()` — id 입력 → 필드별 새 값 입력 → `repo.update()`
  - `handleDelete()` — id 입력 → 확인(Y/N) → `repo.remove()`
- [ ] `while` 루프로 메뉴 반복, `0` 입력 시 종료

```
[메뉴 흐름]
main()
 └─ while(true)
     ├─ printMenu()
     ├─ 입력 받기
     └─ switch(choice)
         ├─ 1 → handleCreate(repo)
         ├─ 2 → handleReadAll(repo)
         ├─ 3 → handleReadOne(repo)
         ├─ 4 → handleUpdate(repo)
         ├─ 5 → handleDelete(repo)
         └─ 0 → break
```

---

## 단계 6 — vcxproj에 소스 파일 등록

- [ ] `poc_crud.vcxproj`에 아래 파일 추가

```xml
<ItemGroup>
  <ClCompile Include="src\main.cpp" />
  <ClCompile Include="src\MemberRepository.cpp" />
</ItemGroup>
<ItemGroup>
  <ClInclude Include="src\Member.h" />
  <ClInclude Include="src\MemberRepository.h" />
</ItemGroup>
```

---

## 단계 7 — 빌드 및 동작 검증

### 검증 시나리오

| 순서 | 동작                              | 확인 항목                          |
|-----|-----------------------------------|------------------------------------|
| 1   | 앱 최초 실행                       | `members.json` 자동 생성 여부       |
| 2   | 회원 2명 등록 (Create × 2)         | id 1, 2 자동 채번 / 파일 저장 확인  |
| 3   | 전체 목록 조회 (Read All)          | 2명 출력                           |
| 4   | id=1 단건 조회 (Read One)          | 상세 정보 출력                      |
| 5   | id=1 이메일 수정 (Update)          | 파일에 변경값 반영 확인             |
| 6   | id=2 삭제 (Delete)                | 파일에서 제거 확인                  |
| 7   | 앱 재실행 후 목록 조회             | id=1만 남아있는지 확인 (영속성)      |
| 8   | 존재하지 않는 id로 조회/수정/삭제  | 오류 메시지 출력 확인               |

---

## 파일 생성 순서 요약

```
1. packages.config
2. data/members.json
3. src/Member.h
4. src/MemberRepository.h
5. src/MemberRepository.cpp
6. src/main.cpp
7. poc_crud.vcxproj (소스 파일 등록)
```
