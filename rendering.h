#pragma once

#include <Windows.h>
#include <sstream>

// 랜더링 클래스
class rendering {
public:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int backScreen = 0;
	int curx = 0, cury = 0;
	int sizex, sizey;

	// 생성자, 소멸자
	rendering(int, int, char*);
	~rendering();

	// 버퍼 생성
	void screenInit();

	// 버퍼 그리기 - 일반문자
	void screenRender(const char);
	// 버퍼 그리기 - 일반텍스트
	void screenRender(const char*);
	// 버퍼 그리기 - x좌표, y좌표, 일반문자
	void screenRender(int, int, const char);
	// 버퍼 그리기 - x좌표, y좌표,일반텍스트
	void screenRender(int, int, const char*);

	// 버퍼 그리기 - 유니코드
	void screenRender(const wchar_t);
	// 버퍼 그리기 - 유니코드텍스트
	void screenRender(const wchar_t*);
	// 버퍼 그리기 - x좌표, y좌표, 유니코드
	void screenRender(int, int, const wchar_t);
	// 버퍼 그리기 - x좌표, y좌표, 유니코드텍스트
	void screenRender(int, int, const wchar_t*);

	// 버퍼와 프론트를 교체
	void screenswitch();

	// 엔터 - 좌표없이 계속 작성할 때 사용
	void screenEndl();

	// 버퍼 클리어 - 시작좌표, 끝좌표
	void screenClear(int, int, int, int, bool);
	void screenClear(bool);

	// 유니코드용 버퍼의 문자 교체
	void charSwitch(int, int, int, int, bool);
};