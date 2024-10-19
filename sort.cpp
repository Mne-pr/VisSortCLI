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
    int m_value = 0;
    int cur1 = 0;
    int cur2 = 0;
    int tmp = 0;

public:
    printer(int sizex, char* winName, int tickt) : ticktime(tickt) {
        panel = new rendering(90, 30, winName); // sizex,y에 따라.. 화면 크기 수정필요 -> insertData에 반영해야할 거 같음
        panel->screenRender(0, 0, L"loading...");
        panel->screenswitch();
        panel->screenRender(0, 0, L"loading...");
        return;
    }
    ~printer() { delete(panel); return; }

    // 처음에 버퍼 설정
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

        panel->screenClear(0, 0, 15, 0, true);
        panel->screenswitch();
        panel->screenClear(0, 0, 15, 0, true);

        return;
    }
    
    // 두 값 위치 변경
    void printChange(const vector<int> res) {

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
            panel->screenRender(6 + res[1] * 2, 2 + m_value, L'□');
            panel->screenRender(6 + res[3] * 2, 2 + m_value, L'□');
            if (i == 0) { panel->screenswitch(); }
        }
        Sleep(ticktime);

        panel->screenRender(0, 0, L' ');
        panel->screenswitch();
        panel->screenRender(0, 0, L' ');
        return;
    }

    // 커서 출력 - 커서가 2개인 것으로 한정했는데 확장가능성 있음
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
        return;
    }

    // 정렬 완료 후
    void endPrint() {
        panel->screenClear(0, 0, 20, 0, true);
        panel->screenRender(0, 0, L"Sorting complete");
        panel->screenswitch();

        Sleep(ticktime * 20); // 사용자 입력 받아서 종료한다던가
        return;
    }


    //


    // 두 끝 인덱스의 값 슬라이드
    void printSlide(int idx1, int idx2, int direction) {

    }

    // 특정 값 임시저장(프린터 제거)
    void pushValue(int value) {

    }

    // 특정 인덱스에 값 출력
    void popValue(int idx) {

    }
};


// 정렬 계산 부모클래스
class allSort {
protected:
    vector<int>* data = nullptr;
    int cycle = 0;
    int max_v = 0;
    int min_v = 0;

public:
    allSort() {}
    ~allSort() {}

    void insertData(vector<int>* newdata) {
        data = newdata;
        max_v = getMaxValue();
        min_v = getMinValue();
        return;
    }

    int getDatalen() {
        return data ? data->size() : 0;
    }

    int getMaxValue() {
        return data ? *max_element(data->begin(), data->end()) : 0;
    }

    int getMinValue() {
        return data ? *min_element(data->begin(), data->end()) : 0;
    }

    void vchange(int idx1, int idx2) {
        int tmp = (*data)[idx1];
        (*data)[idx1] = (*data)[idx2];
        (*data)[idx2] = tmp;
        return;
    }

    virtual int getSortType() = 0;

    virtual vector<int> go1step() = 0; // 추상 클래스임을 명확히 하는 순수가상함수

};

// 컨트롤러 부모클래스
class controller {
protected:
    printer* pt = nullptr;
    allSort* sol = nullptr;
    int ticktime = 0;

public:
    controller(char* whatsort, allSort* solution, vector<int>& data, int tickt) {
        sol = solution; ticktime = tickt;
        pt = new printer(sol->getDatalen(), whatsort, ticktime);
        pt->insertData(data);
        sol->insertData(&data);
        return;
    }
    ~controller() { delete(pt); return; }

    virtual void start() = 0;
};


// 정렬 : 최소 버블 정렬 (오름차순)
class bubbleSort : public allSort {
    int cur = 0;
public:
    bubbleSort() { return; }
    ~bubbleSort() { return; }

    int getSortType() override { return 1; }

    vector<int> go1step() override {
        // 변화유무(없음, 바꿈), 인덱스1, 인덱스2, 커서1, 커서2
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

// 정렬 : 최소 선택 정렬 (오름차순)
class selectionSort : public allSort {
    int cur1 = 0;
    int cur2 = 1;
    int minidx = 0;

public:
    selectionSort() { return; }
    ~selectionSort() { return; }

    int getSortType() override { return 1; }

    vector<int> go1step() override {
        // 변화유무, 인덱스1, 인덱스2, 커서1, 커서2
        vector<int> foret = { 0,0,0,0,0,cur1,cur2 };

        // 해당 인덱스와 minidx 값을 비교하고, 다음 커서로 넘어갈까요?
        if (cur2 < data->size()) {
            if ((*data)[cur2] < (*data)[minidx]) { minidx = cur2; }
            cur2 += 1; 
        }

        // 정렬이 끝났나요?
        else if (cur1 == data->size() - 1) { foret = { -1,0,0,0,0,cur1 - 1,cur1 }; }

        // 다음 사이클로 넘어가면 되나요? - 값 갱신
        else {
            foret = { 1,cur1,(*data)[cur1],minidx,(*data)[minidx],cur1,minidx };
            vchange(cur1, minidx); 
            cycle += 1; cur1 += 1; cur2 = cur1 + 1; minidx = cur1;
        }

        return foret;
    }
};

// 단순이동용 컨트롤러 : 정렬 계산 후 패널 조작 명령 (버블. 선택)
class controller1 : public controller {
public:

    controller1(char* ws, allSort* s, vector<int>& d, int t) : controller(ws, s, d, t) { }
    ~controller1() { }

    void start() override {
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


// 정렬 : 최소 삽입 정렬 (오름차순)
class insertionSort : public allSort {
    int cur1 = 0;
    int cur2 = 1;
    int tmp = 0;
    int first = 1;
    int slided = 1;

public:
    insertionSort() { return; }
    ~insertionSort() { return; }

    int getSortType() override { return 2; }

    vector<int> go1step() override {
        // 결과유형(없음, 슬라이드, 값저장, 배치), 인덱스1, 인덱스2, 커서1, 커서2)
        vector<int> foret = { 0,0,0,cur1,cur2 };

        // 커서 2 위치가 타겟임. 일단 값 빼고, 커서1을 움직여서 적당한 자리를 찾은 후 커서 1 위치에 배치 -> 커서 2 위치 1 증가

        // 종료 조건을 만족했나요?
        if (cur2 == getDatalen()) { foret[0] = -1; }

        else {
            // 사이클이 지난 후 초기 상태인가요?
            if (first == 1) {
                foret = { 2,cur1,0,cur1,cur2 };
                first = 0;
            }

            // 커서를 옮겨야 하나요?
            if (slided == 1) { slided = 0; }

            // 슬라이드 해야 하나요?
            else if ((*data)[cur2] < (*data)[cur1]) {
                foret = { 1,cur1,cur1 + 1,cur1,cur2 };
                vchange(cur1, cur1 + 1);
                cur1 -= 1;
                slided = 1;
            }

            // 적당한 위치 찾았나요?
            else {
                foret = { 3,cur1,0,cur1,cur2 };
                cur2 += 1;
                cur1 = cur2 - 1;
                first = 1;
                cycle += 1;
            }
        }

        return foret;
    }

};

// 정렬 : 최소 합병 정렬 (오름차순)
class mergeSort {

};

// 정렬 : 최소 퀵 정렬 (오름차순)
class quickSort {

};

// 슬라이드용 컨트롤러 : (삽입, 퀵)
class controller2 : public controller {
public:
    controller2(char* ws, allSort* s, vector<int>& d, int t) : controller(ws,s,d,t) {}
    ~controller2() {}

    void start() override {
        vector<int> res(7);

        do {
            res = sol->go1step();
            pt->printCursor(res[5], res[6]);

            switch (res[0]) {
            case 1: // 슬라이드
                pt->printSlide();
                break;
            case 2: // 값 저장
                pt->pushValue();
                break;
            case 3: // 배치
                pt->popValue();
                break;
            default:
                break;
            }
        } while (res[0] != -1);

        pt->endPrint();
        return;
    }
};


// 메인함수
int main() {

    // 데이터의 수 입력
    int datalen = 0;
    cout << "정렬할 데이터 개수를 입력해주세요. > ";
    cin >> datalen;

    // 정렬 방법 객체 생성
    //bubbleSort method;
    selectionSort method;

    // 입력받은 데이터 수 만큼 랜덤 생성
    vector<int> data(datalen);
    srand(time(0));
    for (int i = 0; i < datalen; i++) {
        data[i] = (rand() % (datalen + 5)) + 1;
    }

    // 틱 시간 설정 (1초 : 1000)
    int ticktime = 50;



    // 컨트롤러 생성, 실행
    if (method.getSortType() == 1) {
        controller1 con((char*)"title", &method, data, ticktime);
        con.start();
    }
    else if (method.getSortType() == 2) {
        controller2 con((char*)"title", &method, data, ticktime);
        con.start();
    }
    
    return 0;
}