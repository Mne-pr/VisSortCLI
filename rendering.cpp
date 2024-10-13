#include "rendering.h"

std::ostringstream oss;
using namespace std;

// ������ Ŭ����

rendering::rendering(int sizex, int sizey, char* winName) {
    // �ܼ�â ����
    backScreen = curx = cury = 0;
    oss << "mode con: cols=" << sizex << " lines=" << sizey << endl;
    system(oss.str().c_str());
    system(winName);
    screenInit();
}

rendering::~rendering() {
    CloseHandle(screen[0]);
    CloseHandle(screen[1]);
}

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

// ���� �׸��� - �ؽ�Ʈ, ��ǥ����
void rendering::screenRender(int x, int y, char* text) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)x,(SHORT)y };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
}

// ���� �׸��� - ����, ��ǥ����
void rendering::screenRender(int x, int y, char ch) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)x,(SHORT)y };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
}

// ���� �׸��� - �ؽ�Ʈ
void rendering::screenRender(char* text) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)text, strlen(text), &err, NULL);
    curx = curx + (int)strlen(text);
}

void rendering::screenRender(char ch) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteFile(screen[backScreen], (char*)ch, 1, &err, NULL);
    curx++;
}

// �����ڵ�� - char���� wchar_t���� ������ �� �Ǵ��� Ȯ���� �� �ؼ� �ڿ� ���ڸ� �ϳ� �� ���� �� ������ ����
void rendering::screenRender(wchar_t ch, char unicode) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)curx,(SHORT)cury };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
    curx = curx + 2; // �����ڵ��
}

void rendering::screenRender(int x, int y, wchar_t ch, char unicode) {
    DWORD err;
    COORD backBufferCursor = { (SHORT)x,(SHORT)y };
    SetConsoleCursorPosition(screen[backScreen], backBufferCursor);
    WriteConsoleW(screen[backScreen], &ch, 1, &err, NULL);
}

// ���۸� ����Ʈ�� �ø�
void rendering::screenswitch() {
    SetConsoleActiveScreenBuffer(screen[backScreen]);
    backScreen = !backScreen;
}

void rendering::screenEndl() { cury++; curx = 0; }

// ���� Ŭ���� - ������ǥ, ����ǥ
void rendering::screenClear(int x, int y, int x2, int y2) {
    for (int i = y; i <= y2 - y; i++) {
        COORD Coor = { (SHORT)x,(SHORT)i };
        DWORD dw;
        FillConsoleOutputCharacter(screen[backScreen], ' ', x2 - x, Coor, &dw);
    }
    curx = cury = 0;
}

void rendering::screenClear(int x, int y, int x2, int y2, int unicode) {
    for (int i = y; i <= y2 - y; i++) {
        COORD Coor = { (SHORT)x,(SHORT)i };
        DWORD dw;
        FillConsoleOutputCharacter(screen[backScreen], L' ', x2 - x, Coor, &dw);
    }
    curx = cury = 0;
}

void rendering::screenClear() {
    COORD Coor = { 0 ,0 };
    DWORD dw;
    // ��ũ�� ũ�⸸ŭ ���������ٵ�
    // FillConsoleOutputCharacter(screen[backScreen],'',ũ��,Coor, &dw);
}

void rendering::charSwitch(int x1, int y1, int x2, int y2) {

    COORD pos1 = { (SHORT)x1, (SHORT)y1 }; // ù ��° ���� ��ġ
    COORD pos2 = { (SHORT)x2, (SHORT)y2 }; // �� ��° ���� ��ġ

    wchar_t char1[2], char2[2];
    DWORD charsRead;

    // ù ��° ��ġ���� ���� �б�
    ReadConsoleOutputCharacterW(screen[backScreen], char1, 1, pos1, &charsRead);
    // �� ��° ��ġ���� ���� �б�
    ReadConsoleOutputCharacterW(screen[backScreen], char2, 1, pos2, &charsRead);

    // ù ��° ��ġ�� �� ��° ���ڸ� ��
    WriteConsoleOutputCharacterW(screen[backScreen], char2, 1, pos1, &charsRead);
    // �� ��° ��ġ�� ù ��° ���ڸ� ��
    WriteConsoleOutputCharacterW(screen[backScreen], char1, 1, pos2, &charsRead);

    return;
}