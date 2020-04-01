#include <windows.h>                                                                             //������ ��� ����

#define WNDSIZE_WIDTH 800
#define WNDSIZE_HEIGHT 600

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);                                            //CALLBACK ��ũ��
HINSTANCE g_hInst;                                                                               //�ν���Ʈ �ڵ�
LPCTSTR lpszClass = TEXT("Omok");                                                     //���� ǥ������ ����

//���� �˰Ե� ��� �������� 15x15��... �ٵ����� 19x19
#define X_Omoksize 15
#define Y_Omoksize 15

#define X_Space 50
#define Y_Space 50

#define INTERVAL 25
#define INTERVAL_Half INTERVAL/2

#define XPOS(x)   (X_Space + (x) * INTERVAL) 
#define YPOS(y)   (Y_Space + (y) * INTERVAL) 

int user = 0; //���� ��� �ٵϾ˺��� ����

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)                                                          // �����Լ�
{
    HWND hWnd;              //������ �ڵ� ����
    MSG Message;            //�޼��� ����ü ���� ����
    WNDCLASS WndClass;      // Windows Class ����ü ���� ����
    g_hInst = hInstance;    //hinstance ���� �ܺο����� ��� �Ҽ� �ֵ��� ���������� ���� ����

    WndClass.cbClsExtra = 0;                                     //���� ����, ������ ��� x
    WndClass.cbWndExtra = 0;                                     //���� ����
    WndClass.hbrBackground = CreateSolidBrush(RGB(150,75,0)); //�������� ��� ����
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);              //�������� ���콺������ ���
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //�������� Ŀ�� ���
    WndClass.hInstance = hInstance;                              //������ Ŭ������ ����ϴ� ���α׷� ��ȣ
    WndClass.lpfnWndProc = WndProc;                              //������ �޼��� ó�� �Լ� ����
    WndClass.lpszClassName = lpszClass;                          //������ Ŭ������ �̸� ����
    WndClass.lpszMenuName = NULL;                                //�� ���α׷��� �޴� ����
    WndClass.style = CS_HREDRAW | CS_VREDRAW;                    //�������� ��Ÿ���� ����
    RegisterClass(&WndClass);                                    //WNDCLASS ����ü�� ������ ����
                                                  // �����츦 ����

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WNDSIZE_WIDTH, WNDSIZE_HEIGHT, //���α׷� â ũ�� (500,300,300,500 �� ���� ������ ���ڷ� â ũ�⸦ ���氡��)
        NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);



    //�޼��� ������ ����
    while (GetMessage(&Message, NULL, 0, 0)) {  //Queue�� �ִ� �޼����� �о���δ�
        TranslateMessage(&Message);             //Ű���� �Է� �޼����� �����Ͽ� ���α׷����� ���� ����� �� �ִ�.
        DispatchMessage(&Message);              //�޼����� �������� �޼��� ó�� �Լ� WndProc�� ����
    }

    return (int)Message.wParam;                 //Ż�� �ڵ� ���α׷� ����
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static RECT clientRect;
    HDC hdc;
    PAINTSTRUCT ps;

    int i, l;
    static int mx, my; // ���콺��ǥ

    switch (iMessage) {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &clientRect);
        return 0;
    }
    case WM_PAINT:
    {

        hdc = BeginPaint(hWnd, &ps);
        //�ٵ��� 15x15
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

        if (mx > (XPOS(0) - INTERVAL_Half) && my > (YPOS(0) - INTERVAL_Half)&& mx < (XPOS(X_Omoksize - 1) + INTERVAL_Half)&& my < (YPOS(Y_Omoksize - 1) + INTERVAL_Half))//�ȳ����� ����
        {
            if (user % 2 == 0)//��
            {
                int x = mx / INTERVAL;
                int y = my/ INTERVAL;
                x = x * INTERVAL;
                y = y * INTERVAL;
                HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
                HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
                Ellipse(hdc, x - INTERVAL_Half, y - INTERVAL_Half, x + INTERVAL_Half, y + INTERVAL_Half);
                SelectObject(hdc, oldBrush);
                DeleteObject(myBrush);
            }
            else//��
            {
                int x = mx / INTERVAL;
                int y = my / INTERVAL;
                x = x * INTERVAL;
                y = y * INTERVAL;
                Ellipse(hdc, x - INTERVAL_Half, y - INTERVAL_Half, x + INTERVAL_Half, y + INTERVAL_Half);
            }
        }

        user = user+1;// ������


        ReleaseDC(hWnd, hdc);
        return 0;
    case WM_DESTROY: //����
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
