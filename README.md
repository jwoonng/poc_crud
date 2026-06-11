# poc_crud

JSON 파일 기반 회원정보 CRUD 콘솔 애플리케이션 (C++20 / Windows)

---

## 요구사항

| 항목 | 버전 |
|-----|------|
| Visual Studio | 2022 (v145) |
| C++ 표준 | C++20 |
| OS | Windows 10/11 (x64) |
| JSON 라이브러리 | nlohmann/json 3.11.3 (NuGet 자동 설치) |

---

## 시작하기

### 1. 저장소 클론

```bash
git clone <repository-url>
cd poc_crud
```

### 2. NuGet 패키지 복원

**Visual Studio:**
```
솔루션 탐색기 → 솔루션 우클릭 → NuGet 패키지 복원
```

**또는 CLI (nuget.exe):**
```powershell
nuget restore packages.config -PackagesDirectory packages
```

### 3. 빌드

**Visual Studio:**
```
Ctrl+Shift+B
```

**또는 MSBuild CLI:**
```powershell
msbuild poc_crud.vcxproj /p:Configuration=Debug /p:Platform=x64
```

### 4. 실행

```powershell
# 프로젝트 루트에서 실행 (data/members.json 경로 기준)
.\x64\Debug\poc_crud.exe
```

> 최초 실행 시 `data/members.json`이 없으면 자동으로 생성됩니다.

---

## 사용법

실행하면 아래 메뉴가 표시됩니다.

```
==============================
  회원 관리 시스템
==============================
 1. 회원 등록 (Create)
 2. 전체 목록 조회 (Read All)
 3. 회원 조회 (Read One)
 4. 회원 수정 (Update)
 5. 회원 삭제 (Delete)
 0. 종료
------------------------------
선택 >
```

### 1. 회원 등록

번호 `1` 입력 후 이름, 이메일, 전화번호를 입력합니다.
ID와 등록일은 자동으로 부여됩니다.

```
선택 > 1

[회원 등록]
이름 > 홍길동
이메일 > hong@example.com
전화번호 > 010-1234-5678
회원이 등록되었습니다. (부여된 ID: 1)
```

### 2. 전체 목록 조회

번호 `2` 입력 시 전체 회원을 ID 오름차순으로 출력합니다.

```
선택 > 2

[전체 회원 목록]
ID    이름            이메일                         전화번호          등록일
----------------------------------------------------------------------------------
1     홍길동          hong@example.com               010-1234-5678     2026-06-11
2     김철수          kim@example.com                010-9876-5432     2026-06-11
```

### 3. 회원 조회 (단건)

번호 `3` 입력 후 조회할 회원 ID를 입력합니다.

```
선택 > 3

[회원 조회]
ID > 1
------------------------------
ID       : 1
이름     : 홍길동
이메일   : hong@example.com
전화번호 : 010-1234-5678
등록일   : 2026-06-11
------------------------------
```

존재하지 않는 ID 입력 시:
```
ID 99에 해당하는 회원이 없습니다.
```

### 4. 회원 수정

번호 `4` 입력 후 수정할 회원 ID를 입력합니다.
각 필드에서 **Enter만 누르면 기존 값이 유지**됩니다.

```
선택 > 4

[회원 수정]
수정할 회원 ID > 1
현재 정보:
  이름     : 홍길동
  이메일   : hong@example.com
  전화번호 : 010-1234-5678
(빈 입력 시 기존 값 유지)
새 이름 [홍길동] >                      ← Enter (기존값 유지)
새 이메일 [hong@example.com] > new@example.com
새 전화번호 [010-1234-5678] >           ← Enter (기존값 유지)
회원 정보가 수정되었습니다.
  이름     : 홍길동
  이메일   : new@example.com
  전화번호 : 010-1234-5678
```

### 5. 회원 삭제

번호 `5` 입력 후 삭제할 회원 ID를 입력합니다.
삭제 전 확인 메시지가 표시됩니다.

```
선택 > 5

[회원 삭제]
삭제할 회원 ID > 2
삭제 대상:
  ID       : 2
  이름     : 김철수
  이메일   : kim@example.com
  전화번호 : 010-9876-5432
삭제하시겠습니까? (Y/N) > Y
회원이 삭제되었습니다.
```

### 0. 종료

```
선택 > 0
프로그램을 종료합니다.
```

---

## 테스트

테스트 파일은 `tmp/` 디렉토리에 위치하며 git에서 제외됩니다.

### 테스트 구성

| 파일 | 종류 | 케이스 | Assertions |
|-----|------|--------|-----------|
| `tmp/test_crud.cpp` | Regression | 15 | 28 |
| `tmp/test_safety.cpp` | Safety | 21 | 36 |

### 전체 실행

```powershell
# 프로젝트 루트에서 실행
.\tmp\run_tests.ps1
```

### 스위트별 실행

```powershell
# Regression 테스트만
.\tmp\run_tests.ps1 -Suite regression

# Safety 테스트만
.\tmp\run_tests.ps1 -Suite safety
```

### 테스트 범위

**Regression** — CRUD 기능 정상 동작 검증
- Create: ID 자동 채번, created_at 자동 기록, 필수 필드 공백 거부
- Read: 전체 목록 id 오름차순, 단건 조회, 없는 ID nullopt 반환
- Update: 필드 수정, 빈 입력 시 기존값 유지
- Delete: 삭제, 없는 ID false 반환
- 데이터 영속성: 재실행 후 데이터 유지

**Safety** — 비정상 입력 및 파일 I/O 방어 검증
- 입력값 경계: 공백 전용 값, 10,000자 초과 필드
- 특수문자/인젝션: JSON 특수문자, JSON 구조 인젝션 시도, null 바이트, 유니코드
- 파일 I/O: 손상된 JSON, 빈 파일, `members` 키 누락, 불완전 레코드
- 경계값/ID: 음수·0 ID, 1,000건 대량 삽입, 중복 데이터

> **참고**: 테스트 실행 전 NuGet 패키지 복원이 완료되어 있어야 합니다.

---

## 데이터 저장 위치

모든 회원 데이터는 `data/members.json`에 저장됩니다.

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

---

## 파일 구조

```
poc_crud/
├── README.md
├── CLAUDE.md
├── PRD.md
├── packages.config               # NuGet 패키지 목록
├── poc_crud.slnx
├── poc_crud.vcxproj
├── docs/
│   └── plan.md                   # 구현 계획
├── src/
│   ├── main.cpp                  # 진입점, 메뉴 루프
│   ├── Member.h                  # Member 구조체
│   └── MemberRepository.h/.cpp  # JSON 파일 I/O + CRUD 로직
└── data/
    └── members.json              # 회원 데이터 저장 파일
```
