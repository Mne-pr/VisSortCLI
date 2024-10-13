#pragma once

#include <Windows.h>
#include <sstream>

// ������ Ŭ����
class rendering {
public:
	HANDLE screen[2];
	CONSOLE_CURSOR_INFO CURSOR;
	int backScreen = 0;
	int curx = 0, cury = 0;
	int sizex, sizey;

	// ������, �Ҹ���
	rendering(int, int, char*);
	~rendering();

	// ���� ����
	void screenInit();

	// ���� �׸��� - �Ϲݹ���
	void screenRender(const char);
	// ���� �׸��� - �Ϲ��ؽ�Ʈ
	void screenRender(const char*);
	// ���� �׸��� - x��ǥ, y��ǥ, �Ϲݹ���
	void screenRender(int, int, const char);
	// ���� �׸��� - x��ǥ, y��ǥ,�Ϲ��ؽ�Ʈ
	void screenRender(int, int, const char*);

	// ���� �׸��� - �����ڵ�
	void screenRender(const wchar_t);
	// ���� �׸��� - �����ڵ��ؽ�Ʈ
	void screenRender(const wchar_t*);
	// ���� �׸��� - x��ǥ, y��ǥ, �����ڵ�
	void screenRender(int, int, const wchar_t);
	// ���� �׸��� - x��ǥ, y��ǥ, �����ڵ��ؽ�Ʈ
	void screenRender(int, int, const wchar_t*);

	// ���ۿ� ����Ʈ�� ��ü
	void screenswitch();

	// ���� - ��ǥ���� ��� �ۼ��� �� ���
	void screenEndl();

	// ���� Ŭ���� - ������ǥ, ����ǥ
	void screenClear(int, int, int, int, bool);
	void screenClear(bool);

	// �����ڵ�� ������ ���� ��ü
	void charSwitch(int, int, int, int, bool);
};