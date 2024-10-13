#include "rendering.h"

std::ostringstream oss;
using namespace std;

// 랜더링 클래스

// 생성자, 소멸자
rendering::rendering(int sizex, int sizey, char* winName) : sizex(sizex), sizey(sizey) {
    // 콘솔창 설정
    oss << "mode con: cols=" << sizex << " lines=" << sizey << endl;
    system(oss.str().c_str());
    system(winName);
    screenInit();
}

rendering::~rendering() {
    CloseHandle(screen[0]);
    CloseHandle(screen[1]);
}


// 버퍼 생성
void rendering::screenInit() {
    // 버퍼 생성
    screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    // 커서 속성 설정
    CURSOR.dwSize = 1;
    CURSOR.bVisible = false;
    // 버퍼에 커서 속성 지정
    SetConsoleCursorInfo(screen[0], &CURSOR);
    SetConsoleCursorInfo(screen[1], &CURSOR);
}


// 버퍼 그리기 - 일반 문자
void rendering::screenRender(const char ch) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
    curx++;
}

void rendering::screenRender(const char* text) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
    curx = curx + (int)strlen(text);
}

void rendering::screenRender(int x, int y, const char ch) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)x,(SHORT)y };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
}

void rendering::screenRender(int x, int y, const char* text) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)x,(SHORT)y };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
}


// 버퍼 그리기 - 유니코드 문자
void rendering::screenRender(const wchar_t ch) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
    curx = curx + 2; // 유니코드라서
}

void rendering::screenRender(const wchar_t* text) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], text, wcslen(text), &err, NULL);
    curx = curx + (int)wcslen(text);
}

void rendering::screenRender(int x, int y, const wchar_t ch) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)x,(SHORT)y };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
}

void rendering::screenRender(int x, int y, const wchar_t* text) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteConsoleW(screen[backScreen], text, wcslen(text), &err, NULL);
}


// 버퍼를 프론트로 교체
void rendering::screenswitch() {
    SetConsoleActiveScreenBuffer(screen[backScreen]);
    backScreen = !backScreen;
}


// 다음 줄 이동
void rendering::screenEndl() { cury++; curx = 0; }


// 버퍼 클리어 - 시작좌표, 끝좌표
void rendering::screenClear(int x, int y, int x2, int y2, bool isUnicode) {
    DWORD dw;

    for (int i = y; i <= y2; i++) {
        COORD Coor = { (SHORT)x,(SHORT)i };
        if (isUnicode) 
            FillConsoleOutputCharacterW(screen[backScreen], (const wchar_t)L' ', x2 - x + 1, Coor, &dw);
        else
            FillConsoleOutputCharacterA(screen[backScreen], (const char)' ', x2 - x + 1, Coor, &dw);
    }
    curx = cury = 0;
}

void rendering::screenClear(bool isUnicode) {
    DWORD dw;

    for (int i = 0; i <= sizey; i++) {
        COORD Coor = { (SHORT)0, (SHORT)i };
        if (isUnicode)
            FillConsoleOutputCharacterW(screen[backScreen], (const wchar_t)L' ', sizex, Coor, &dw);
        else
            FillConsoleOutputCharacterA(screen[backScreen], (const char)' ', sizex, Coor, &dw);
    }
}


// 글자 스위치 - 서로의 좌표
void rendering::charSwitch(int x1, int y1, int x2, int y2, bool isUnicode) {

    COORD pos1 = { (SHORT)x1, (SHORT)y1 };
    COORD pos2 = { (SHORT)x2, (SHORT)y2 };
    DWORD dw;

    if (isUnicode) {
        wchar_t char1[2], char2[2];
        // 각 위치의 문자 읽기
        ReadConsoleOutputCharacterW(screen[backScreen], char1, 1, pos1, &dw);
        ReadConsoleOutputCharacterW(screen[backScreen], char2, 1, pos2, &dw);

        // 서로 위치의 자신의 문자 입력
        WriteConsoleOutputCharacterW(screen[backScreen], char2, 1, pos1, &dw);
        WriteConsoleOutputCharacterW(screen[backScreen], char1, 1, pos2, &dw);
    } else {
        char char1[2], char2[2];
        ReadConsoleOutputCharacterA(screen[backScreen], char1, 1, pos1, &dw);
        ReadConsoleOutputCharacterA(screen[backScreen], char2, 1, pos2, &dw);
        WriteConsoleOutputCharacterA(screen[backScreen], char2, 1, pos1, &dw);
        WriteConsoleOutputCharacterA(screen[backScreen], char1, 1, pos2, &dw);
    }

    return;
}