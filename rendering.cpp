#include "rendering.h"

std::ostringstream oss;
using namespace std;

// ������ Ŭ����

// ������, �Ҹ���
rendering::rendering(int sizex, int sizey, char* winName) : sizex(sizex), sizey(sizey) {
    // �ܼ�â ����
    oss << "mode con: cols=" << sizex << " lines=" << sizey << endl;
    system(oss.str().c_str());
    system(winName);
    screenInit();
}

rendering::~rendering() {
    CloseHandle(screen[0]);
    CloseHandle(screen[1]);
}


// ���� ����
void rendering::screenInit() {
    // ���� ����
    screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    // Ŀ�� �Ӽ� ����
    CURSOR.dwSize = 1;
    CURSOR.bVisible = false;
    // ���ۿ� Ŀ�� �Ӽ� ����
    SetConsoleCursorInfo(screen[0], &CURSOR);
    SetConsoleCursorInfo(screen[1], &CURSOR);
}


// ���� �׸��� - �Ϲ� ����
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


// ���� �׸��� - �����ڵ� ����
void rendering::screenRender(const wchar_t ch) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
    curx = curx + 2; // �����ڵ��
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


// ���۸� ����Ʈ�� ��ü
void rendering::screenswitch() {
    SetConsoleActiveScreenBuffer(screen[backScreen]);
    backScreen = !backScreen;
}


// ���� �� �̵�
void rendering::screenEndl() { cury++; curx = 0; }


// ���� Ŭ���� - ������ǥ, ����ǥ
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


// ���� ����ġ - ������ ��ǥ
void rendering::charSwitch(int x1, int y1, int x2, int y2, bool isUnicode) {

    COORD pos1 = { (SHORT)x1, (SHORT)y1 };
    COORD pos2 = { (SHORT)x2, (SHORT)y2 };
    DWORD dw;

    if (isUnicode) {
        wchar_t char1[2], char2[2];
        // �� ��ġ�� ���� �б�
        ReadConsoleOutputCharacterW(screen[backScreen], char1, 1, pos1, &dw);
        ReadConsoleOutputCharacterW(screen[backScreen], char2, 1, pos2, &dw);

        // ���� ��ġ�� �ڽ��� ���� �Է�
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