#include <iostream>
#include "rendering.h"
#include <vector> // allSort
#include <algorithm> // for_each
#include <windows.h> // sleep
#include <cstdlib> // rand
#include <ctime> // seed(rand)

using namespace std;
std::stringstream ss;

// rendering�� �����͸� ����
class printer {
    rendering* panel = nullptr;
    int ticktime;
    int m_value = 0;
    int cur1 = 0;
    int cur2 = 0;

public:
    printer(int sizex, char* winName, int tickt) : ticktime(tickt) {
        panel = new rendering(90, 30, winName); // sizex,y�� ����.. ȭ�� ũ�� �����ʿ� -> insertData�� �ݿ��ؾ��� �� ����
        panel->screenRender(0, 0, L"loading...");
        panel->screenswitch();
        panel->screenRender(0, 0, L"loading...");
        return;
    }
    ~printer() { delete(panel); return; }

    void insertData(const vector<int>& data) {

        int idx = 2; // 2 ~ ���α���-2

        m_value = *max_element(data.begin(), data.end());
        
        for (int j = 0; j < 2; j++) {
            // ���� ���ι�ġ
            for_each(data.begin(), data.end(), [&](int value) {
                // ���� ���ι�ġ 
                for (int i = 0; i < value; i++) {
                    panel->screenRender(4 + idx, 2 + m_value - i, L'��');
                }
                idx += 2; // unicode ��
            });

            if (j == 0) {
                panel->screenswitch();
                idx = 2;
            }
        }
        Sleep(ticktime*6);

        panel->screenClear(0, 0, 15, 0, true);
        panel->screenswitch();
        panel->screenClear(0, 0, 15, 0, true);

        return;
    }

    void printChange(const vector<int> res) {

        // up
        panel->screenRender(0, 0, L'U');
        for (int i = 0; i < 2; i++) {
            panel->screenRender(6 + res[1] * 2, 2 + m_value - res[2], L'��');
            panel->screenRender(6 + res[3] * 2, 2 + m_value - res[4], L'��');
            panel->screenRender(6 + res[1] * 2, 2 + m_value, L' ');
            panel->screenRender(6 + res[3] * 2, 2 + m_value, L' ');
            if (i == 0) { panel->screenswitch(); }
        }
        Sleep(ticktime);

        // change
        panel->screenRender(0, 0, L'C');
        int chlen = (res[2] > res[4]) ? res[2] : res[4];
        
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j <= chlen; j++) {
                panel->charSwitch(6 + res[1] * 2, 2 + m_value - j, 6 + res[3] * 2, 2 + m_value - j, true);
            }
            if (i == 0) { panel->screenswitch();}
        }
        Sleep(ticktime);

        // down
        panel->screenRender(0, 0, L'D');
        for (int i = 0; i < 2; i++) {
            panel->screenRender(6 + res[1] * 2, 2 + m_value - res[4], L' ');
            panel->screenRender(6 + res[3] * 2, 2 + m_value - res[2], L' ');
            panel->screenRender(6 + res[1] * 2, 2 + m_value, L'��');
            panel->screenRender(6 + res[3] * 2, 2 + m_value, L'��');
            if (i == 0) { panel->screenswitch(); }
        }
        Sleep(ticktime);

        panel->screenRender(0, 0, L' ');
        panel->screenswitch();
        panel->screenRender(0, 0, L' ');
        return;
    }

    void printCursor(int idx1, int idx2) {
        return;
        for (int i = 0; i < 2; i++) {
            panel->screenRender(0, 0, L'M');
            panel->screenRender(6 + cur1 * 2, 1, L' ');
            panel->screenRender(6 + cur2 * 2, 1, L' ');
            panel->screenRender(6 + idx1 * 2, 1, L'��');
            panel->screenRender(6 + idx2 * 2, 1, L'��'); //U+02C7, Caron
            if (i == 0) { panel->screenswitch();}
        }

        cur1 = idx1; cur2 = idx2;
        Sleep(ticktime);
        return;
    }

    void endPrint() {
        panel->screenClear(0, 0, 20, 0, true);
        panel->screenRender(0, 0, L"Sorting complete");
        panel->screenswitch();

        Sleep(ticktime * 20); // ����� �Է� �޾Ƽ� �����Ѵٴ���
        return;
    }
};

// ���� ���
class allSort {
public:
    allSort() {}
    ~allSort() {}
    virtual vector<int> go1step() = 0; // �߻� Ŭ�������� ��Ȯ�� �ϴ� ���������Լ�
    virtual int getDatalen() = 0;
    virtual void insertData(vector<int>* newdata) = 0;
};

// ���� : ���� ����
class bubbleSort : public allSort {
    vector<int>* data = nullptr;
    int cycle = 0;
    int cur = 0;

public:
    bubbleSort() { return; }
    ~bubbleSort() { return; }

    void insertData(vector<int>* newdata) override {
        data = newdata;
        return;
    }

    int getDatalen() override { 
        return data ? data->size() : 0; 
    }

    void vchange(int idx1, int idx2) {
        int tmp = (*data)[idx1];
        (*data)[idx1] = (*data)[idx2];
        (*data)[idx2] = tmp;
        return;
    }

    vector<int> go1step() override {
        // ��ȭ����, �ε���1, �ε���2, Ŀ��1, Ŀ��2
        vector<int> foret = { 0,0,0,0,0,cur,cur+1 };

        // cur, cur+1 ������ �ٲ�� �ұ��?
        if ((*data)[cur] > (*data)[cur + 1]) {
            foret = { 1, cur, (*data)[cur], cur + 1, (*data)[cur + 1], cur, cur+1 };
            vchange(cur, cur + 1);
        }

        // ���� Ŀ���� �Ѿ�� �ǳ���?
        if (cur < data->size() - cycle - 2) { cur += 1; }

        // ������ ��������?
        else if (cur == 0) { foret[0] = -1; }

        // ���� ����Ŭ�� �Ѿ�� �ǳ���?
        else { cycle += 1; cur = 0; }

        return foret;
    }
};

// ���� ��� �� �г� ���� ���
class controller {
private:
    printer* pt;
    allSort* sol;
    int ticktime;

public:

    controller(char* whatsort, allSort* solution, vector<int>& data, int tickt) : sol(solution), ticktime(tickt) {
        pt = new printer(sol->getDatalen(), whatsort, ticktime);
        pt->insertData(data);
        sol->insertData(&data);
        return;
    }
    ~controller() { delete(pt); return; }

    void start() {
        vector<int> res(7);

        do {
            res = sol->go1step();
            pt->printCursor(res[5],res[6]);
            if (res[1] != res[3]) { pt->printChange(res);}
        } while (res[0] != -1);

        pt->endPrint();
        return;
    }
};

// �����Լ�
int main() {

    // �������� �� �Է�
    int datalen = 0;
    cout << "������ ������ ������ �Է����ּ���. > ";
    cin >> datalen;

    // ���� ����� bubbleSort ����
    bubbleSort method;

    // �Է¹��� ������ �� ��ŭ ���� ����
    vector<int> data(datalen);
    srand(time(0));
    for (int i = 0; i < datalen; i++) {
        data[i] = (rand() % (datalen + 5)) + 1;
    }

    // ƽ �ð� ���� (1�� : 1000)
    int ticktime = 50;

    // ��Ʈ�ѷ� ����
    controller con((char*)"title Bubble", &method, data, ticktime);

    // ���� ����
    con.start();

    return 0;
}