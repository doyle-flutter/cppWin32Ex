// CreateWindow 가 창(window)만 만드는 api는 아니며, 위치를 표시하는 내용이 중요
// * 타이틀 변경 시 오류가 발생한다면 설정을 수정해야함 : 하단 내용 참고


#include "framework.h"
#include "WindowsProject1.h"
#define MAX_LOADSTRING 100
#define MENU_EXIT_EVENTS 3
#define MENU_SUB_SUB_EVENTS 4

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


HMENU hMenu;
HWND hEditTitle;

// Menu
void AddMenu(HWND);
void AddControls(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// 버튼컨트롤 ID
#define IDC_SEND 1003 
#define IDC_SEND_CH 1044

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // WPARAM wParam : 값을 사용 할 때
    // LPARAM lParam : 포인터를 사용 할 때

    static char string[100]; 
    static int nIndex;
    HFONT font; 
    

    switch (message)
    {
    case WM_CREATE:
        {

            // Create Menu
            AddMenu(hWnd);

            //
            AddControls(hWnd);

            // Dialog
            MessageBox(NULL, L"Hello", L"My Win32", MB_OK);

            HICON hIcon1 = LoadIcon(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX));
            DrawIcon((HDC)hInst, 200, 200, hIcon1);

            font = CreateFont(35, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET,
                0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("맑은 고딕"));

            CreateWindow(TEXT("button"), TEXT("Click me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                150, 160, 100, 30, hWnd, (HMENU)IDC_SEND, hInst, NULL);
            // 클래스, 내부, 버튼 스타일(위치, 표현 등), x, y , width, height, window, ID, HINSTANCE, customDATA
            // 클릭이벤트 처리 -> ID를 통해 WM_COMMAND || WM_NOTIFY 에서 처리

            return 0;
        }
    case WM_CHAR:
        {
            string[nIndex] = (char)wParam;
            nIndex++;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            
            // Menu Events 
            case 1101: {
                MessageBox(NULL, L"Hello Menu", L"My Menu File", MB_OK);
                break;
            }
            case 2202: {
                MessageBox(NULL, L"Sub Menu", L"Select!", MB_OKCANCEL);
                break;
            }
            case MENU_EXIT_EVENTS: {
                DestroyWindow(hWnd);
                break;
            }
            case MENU_SUB_SUB_EVENTS: {
                MessageBeep(MB_ICONERROR); // Error Sound
                break;
            }
            // 버튼 ID 를 통해 사용
            case IDC_SEND: {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            }
            case IDC_SEND_CH: {
                // * 만약 한글자만 변경 되는 경우 설정을 변경할 것
                // * 메뉴 -> 프로젝트 -> 속성 -> C/C++ -> 전처리기 -> 정의 -> (내용 중 마지막에 ;) _UNICODE;UNICODE;
                wchar_t text[100];
                GetWindowTextW(hEditTitle, text, 100);
                SetWindowTextW(hWnd, text);
                MessageBox(NULL, text, L"CHANGE_TITLE", MB_OKCANCEL);
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            TextOut(hdc, 10, 10, (LPCWSTR) string, strlen(string));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void AddMenu(HWND hWnd) {
    hMenu = CreateMenu();
    
    // none Events
    AppendMenu(hMenu, MF_STRING, NULL,L"File(N_E)");
    // Events
    AppendMenu(hMenu, MF_STRING, 1101, L"File(E)");

    // sub - sub
    HMENU hhFileMenu = CreateMenu();
    AppendMenu(hhFileMenu, MF_STRING, MENU_SUB_SUB_EVENTS, L"sub-sub-MENU1");

    // Menu -> subMunu's'
    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR) hhFileMenu, L"sub-MENU1");
    AppendMenu(hFileMenu, MF_STRING, 2202, L"sub-MENU2");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, L"File(s)");
    
    // Exit Events
    AppendMenu(hMenu, MF_STRING, MENU_EXIT_EVENTS, L"Exit1");
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"Exit2");

    
    SetMenu(hWnd, hMenu);

}

void AddControls(HWND hWnd) {
    // VIew Widget
    // - 구분, 내용, STYLE, Left, Top, Width, Height, HWND, ., ., .);
    CreateWindowW(L"Static", L"ENTER :", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 100, 100, 25, hWnd, 0, 0, 0);
    CreateWindowW(L"Edit", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 320, 100, 100, 25, hWnd, 0, 0, 0);

    CreateWindowW(L"Static", L"Change Title :", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 200, 100, 25, hWnd, 0, 0, 0);
    hEditTitle = CreateWindowW(L"Edit", L"Write...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 320, 200, 100, 25, hWnd, 0, 0, 0);
    CreateWindowW(L"Button", L"C_BTN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 430, 200, 100, 25, hWnd, (HMENU)IDC_SEND_CH, 0, 0);
    // CreateWindow(TEXT("button"), TEXT("Click me"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 160, 100, 30, hWnd, (HMENU)IDC_SEND, hInst, NULL);

    // * STYLE
    // - ES_MULTILINE : 사이즈가 있어야 가능 -> AUTO Scroll 이 있다면 상관 없음
    // - ES_AUTOVSCROLL | ES_AUTOHSCROLL : 'V' ertical | 'H' orizontal
    return;
}
