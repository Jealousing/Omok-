#include <windows.h>                                                                             //윈도우 헤더 파일

#define WNDSIZE_WIDTH 800
#define WNDSIZE_HEIGHT 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);                                            //CALLBACK 매크로
HINSTANCE g_hInst;                                                                               //인스턴트 핸들
LPCTSTR lpszClass = TEXT("Omok");                                                     //제목 표시줄의 내용


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)                                                          // 메인함수
{
    HWND hWnd;              //윈도우 핸들 선언
    MSG Message;            //메세지 구조체 변수 선언
    WNDCLASS WndClass;      // Windows Class 구조체 변수 선언
    g_hInst = hInstance;    //hinstance 값을 외부에서도 사용 할수 있도록 전역변수에 값을 저장

    WndClass.cbClsExtra = 0;                                     //예약 영역, 지금은 사용 x
    WndClass.cbWndExtra = 0;                                     //예약 영역
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //윈도우의 배경 색상
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

    switch (iMessage) {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &clientRect);
        return 0;
    }
    case WM_PAINT:
    {
        HDC hdc;
        PAINTSTRUCT ps;

        hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DESTROY: //종료
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}