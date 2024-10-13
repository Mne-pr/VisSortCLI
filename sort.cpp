#include <iostream>
#include "rendering.h"
#include <vector> // allSort
#include <algorithm> // for_each
#include <windows.h> // sleep
#include <cstdlib> // rand
#include <ctime> // seed(rand)

using namespace std;
std::stringstream ss;

// rendering의 데이터를 조작
class printer {
    rendering* panel = nullptr;
    int ticktime;
    int m_value;
    int cur1;
    int cur2;

public:
    printer(int sizex, char* winName, int tickt) : m_value(0), ticktime(tickt), cur1(0), cur2(0) {
        panel = new rendering(90, 30, winName); // 위아래로 두칸 여유
    }
    ~printer() { delete(panel);}

    void insertData(const vector<int>& data) {
        int idx = 2; // 2 ~ 가로길이-2

        m_value = *max_element(data.begin(), data.end());
        
        for (int j = 0; j < 2; j++) {
            // 문자 가로배치
            for_each(data.begin(), data.end(), [&](int value) {
                // 문자 세로배치 
                for (int i = 0; i < value; i++) {
                    panel->screenRender(4 + idx, 2 + m_value - i, L'□');
                }
                idx += 2; // unicode 라서
            });

            if (j == 0) {
                panel->screenswitch();
                idx = 2;
            }
        }
        Sleep(ticktime*6);

    }

    void printChange(const vector<int> res) {

        //cout << res[2] << "(" << res[1] << ") <-> " << res[4] << "(" << res[3] << ")" << endl;
        
        // up
        panel->screenRender(0, 0, L'U');
        for (int i = 0; i < 2; i++) {
            panel->screenRender(6 + res[1] * 2, 2 + m_value - res[2], L'□');
            panel->screenRender(6 + res[3] * 2, 2 + m_value - res[4], L'□');
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
                panel->charSwitch(6 + res[1] * 2, 2 + m_value - j, 6 + res[3] * 2, 2 + m_value - j);
            }
            if (i == 0) { panel->screenswitch();}
        }
        Sleep(ticktime);

        // down
        panel->screenRender(0, 0, L'D');
        for (int i = 0; i < 2; i++) {
            panel->screenRender(6 + res[1] * 2, 2 + m_value - res[4], L' ');
            panel->screenRender(6 + res[3] * 2, 2 + m_value - res[2], L' ');
            panel->screenRender(6 + res[1] * 2, 2 + m_value, L'□');
            panel->screenRender(6 + res[3] * 2, 2 + m_value, L'□');
            if (i == 0) { panel->screenswitch(); }
        }
        Sleep(ticktime);

        panel->screenRender(0, 0, L' ');
        panel->screenswitch();
        panel->screenRender(0, 0, L' ');
    }

    void printCursor(int idx1, int idx2) {

        for (int i = 0; i < 2; i++) {
            panel->screenRender(0, 0, L'M');
            panel->screenRender(6 + cur1 * 2, 1, L' ');
            panel->screenRender(6 + cur2 * 2, 1, L' ');
            panel->screenRender(6 + idx1 * 2, 1, L'ˇ');
            panel->screenRender(6 + idx2 * 2, 1, L'ˇ'); //U+02C7, Caron
            if (i == 0) { panel->screenswitch();}
        }

        cur1 = idx1; cur2 = idx2;
        Sleep(ticktime);
    }

    void endPrint() {
        
        
    }

};

// 정렬 계산
class allSort {
public:
    allSort() {}
    virtual vector<int> go1step() = 0; // 추상 클래스임을 명확히 하는 순수가상함수
    virtual int getDatalen() = 0;
    virtual void insertData(vector<int>* newdata) = 0;
};

// 정렬 : 버블 정렬
class bubbleSort : public allSort {
    vector<int>* data;
    int cycle;
    int cur;

public:
    bubbleSort() : cycle(0), cur(0), data(nullptr) {}

    void insertData(vector<int>* newdata) override {
        data = newdata;
    }

    int getDatalen() override { return data ? data->size() : 0; }

    void vchange(int idx1, int idx2) {
        int tmp = (*data)[idx1];
        (*data)[idx1] = (*data)[idx2];
        (*data)[idx2] = tmp;
    }

    vector<int> go1step() override {
        // 변화유무, 인덱스1, 인덱스2, 커서1, 커서2
        vector<int> foret = { 0,0,0,0,0,cur,cur+1 };

        // cur, cur+1 값들을 바꿔야 할까요?
        if ((*data)[cur] > (*data)[cur + 1]) {
            foret = { 1, cur, (*data)[cur], cur + 1, (*data)[cur + 1], cur, cur+1 };
            vchange(cur, cur + 1);
        }

        // 다음 커서로 넘어가면 되나요?
        if (cur < data->size() - cycle - 2) { cur += 1; }

        // 정렬이 끝났나요?
        else if (cur == 0) { foret[0] = -1; }

        // 다음 사이클로 넘어가면 되나요?
        else { cycle += 1; cur = 0; }

        return foret;
    }
};

// 정렬 계산 후 패널 조작 명령
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
    }
    ~controller() { delete(pt); }

    void start() {
        vector<int> res(7);

        do {
            res = sol->go1step();


            pt->printCursor(res[5],res[6]);
            if (res[1] != res[3]) { pt->printChange(res);}
            //Sleep(ticktime * 3);
        } while (res[0] != -1);

        pt->endPrint();
    }
};

// 메인함수
int main() {

    // 인자 입력
    int datalen = 0;
    cout << "정렬할 데이터 개수를 입력해주세요. > ";
    cin >> datalen;

    // 데이터 생성
    vector<int> data(datalen);
    srand(time(0));
    for (int i = 0; i < datalen; i++) {
        data[i] = (rand() % (datalen + 5)) + 1;
    }


    // 정렬 방식은 bubbleSort로 설정
    bubbleSort method;
    controller con((char*)"title Bubble", &method, data, 150);

    // 정렬 시작
    con.start();

    return 0;
}