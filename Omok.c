#include <windows.h>                                                                             //윈도우 헤더 파일

#define WNDSIZE_WIDTH 800
#define WNDSIZE_HEIGHT 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);                                            //CALLBACK 매크로
HINSTANCE g_hInst;                                                                               //인스턴트 핸들
LPCTSTR lpszClass = TEXT("Omok");                                                     //제목 표시줄의 내용

//새로 알게된 사실 오목판은 15x15다... 바둑판이 19x19
#define X_Omoksize 15
#define Y_Omoksize 15

#define X_Space 50
#define Y_Space 50

#define INTERVAL 25
#define INTERVAL_Half INTERVAL/2

#define XPOS(x)   (X_Space + (x) * INTERVAL) 
#define YPOS(y)   (Y_Space + (y) * INTERVAL) 

int user = 0; //순서 흑색 바둑알부터 시작

int save[400][400];

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)                                                          // 메인함수
{
    HWND hWnd;              //윈도우 핸들 선언
    MSG Message;            //메세지 구조체 변수 선언
    WNDCLASS WndClass;      // Windows Class 구조체 변수 선언
    g_hInst = hInstance;    //hinstance 값을 외부에서도 사용 할수 있도록 전역변수에 값을 저장

    WndClass.cbClsExtra = 0;                                     //예약 영역, 지금은 사용 x
    WndClass.cbWndExtra = 0;                                     //예약 영역
    WndClass.hbrBackground = CreateSolidBrush(RGB(150,75,0)); //윈도우의 배경 색상
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);              //윈도우의 마우스포인터 모양
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //윈도우의 커서 모양
    WndClass.hInstance = hInstance;                              //윈도우 클래스를 등록하는 프로그램 번호
    WndClass.lpfnWndProc = WndProc;                              //윈도우 메세지 처리 함수 지정
    WndClass.lpszClassName = lpszClass;                          //윈도우 클래스의 이름 지정
    WndClass.lpszMenuName = NULL;                                //이 프로그램의 메뉴 지정
    WndClass.style = CS_HREDRAW | CS_VREDRAW;                    //윈도우의 스타일을 정의
    RegisterClass(&WndClass);                                    //WNDCLASS 구조체의 번지를 전달
                                                  // 윈도우를 생성

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WNDSIZE_WIDTH, WNDSIZE_HEIGHT, //프로그램 창 크기 (500,300,300,500 과 같이 임의의 숫자로 창 크기를 변경가능)
        NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);



    //메세지 루프를 실행
    while (GetMessage(&Message, NULL, 0, 0)) {  //Queue에 있는 메세지를 읽어들인다
        TranslateMessage(&Message);             //키보드 입력 메세지를 가공하여 프로그램에서 쉽게 사용할 수 있다.
        DispatchMessage(&Message);              //메세지를 윈도우의 메세지 처리 함수 WndProc로 전달
    }

    return (int)Message.wParam;                 //탈출 코드 프로그램 종료
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static RECT clientRect;
    HDC hdc;
    PAINTSTRUCT ps;

    int i, l;
    static int mx, my; // 마우스좌표

    switch (iMessage) {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &clientRect);
        return 0;
    }
    case WM_PAINT:
    {

        hdc = BeginPaint(hWnd, &ps);
        //바둑판 15x15
        for (i = 0; i < X_Omoksize;i++)
        {
            MoveToEx(hdc, XPOS(i), YPOS(0), NULL);
            LineTo(hdc, XPOS(i), YPOS(Y_Omoksize-1));
        }
        for (l = 0; l < Y_Omoksize; l++)
        {
            MoveToEx(hdc, XPOS(0), YPOS(l), NULL);
            LineTo(hdc, XPOS(X_Omoksize-1), YPOS(l));
        }
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN:
        hdc = GetDC(hWnd);
        mx = LOWORD(lParam);
        my = HIWORD(lParam);

        if (mx > (XPOS(0) - INTERVAL_Half) && my > (YPOS(0) - INTERVAL_Half)&& mx < (XPOS(X_Omoksize - 1) + INTERVAL_Half)&& my < (YPOS(Y_Omoksize - 1) + INTERVAL_Half))//안나가게 설정
        {
            int x = mx / INTERVAL;
            int y = my / INTERVAL;
            x = x * INTERVAL;
            y = y * INTERVAL;

            if (save[x][y] == 1 || save[x][y] == 2)//이미 돌이 있는 곳에 클릭했는지 확인!
            {
                MessageBox(hWnd,TEXT("중복된 칸에 클릭하셨습니다."),TEXT("중복!"), MB_OK);
            }
            else// 돌이없으면 실행
            {
                if (user % 2 == 0)//흑
                {

                    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
                    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
                    Ellipse(hdc, x - INTERVAL_Half, y - INTERVAL_Half, x + INTERVAL_Half, y + INTERVAL_Half);
                    SelectObject(hdc, oldBrush);
                    DeleteObject(myBrush);

                    TextOut(hdc, 500, 100, TEXT("백돌의 차례입니다."), 10);//차례 확인
                    TCHAR str[128];
                    wsprintf(str, TEXT("현재 X좌표는 %d입니다."), x);
                    TextOut(hdc, 500, 200, str, lstrlen(str));
                    wsprintf(str, TEXT("현재 Y좌표는 %d입니다."), y);
                    TextOut(hdc, 500, 250, str, lstrlen(str));
                    wsprintf(str, TEXT("현재 나머지는 %d입니다."), user % 2);
                    TextOut(hdc, 500, 300, str, lstrlen(str));
                    user = user + 1;// 다음턴
                    save[x][y] = user % 2 + 1; //돌의 정보 저장
                }
                else//백
                {
                    Ellipse(hdc, x - INTERVAL_Half, y - INTERVAL_Half, x + INTERVAL_Half, y + INTERVAL_Half);

                    TextOut(hdc, 500, 100, TEXT("흑돌의 차례입니다."), 10);//차례 확인 
                    TCHAR str[128];
                    wsprintf(str, TEXT("현재 X좌표는 %d입니다."), x);
                    TextOut(hdc, 500, 200, str, lstrlen(str));
                    wsprintf(str, TEXT("현재 Y좌표는 %d입니다."), y);
                    TextOut(hdc, 500, 250, str, lstrlen(str));
                    wsprintf(str, TEXT("현재 나머지는 %d입니다."), user % 2);
                    TextOut(hdc, 500, 300, str, lstrlen(str));
                    user = user + 1;// 다음턴
                    save[x][y] = user % 2 + 1;//돌의 정보 저장
                }
            }

           
        }


        ReleaseDC(hWnd, hdc);
        return 0;
    case WM_DESTROY: //종료
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
