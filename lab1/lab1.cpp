#include "framework.h"
#include "resource.h"
#include "module1.h"
#include "module2.h"

wchar_t g_szText[256] = L"";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"Lab1Class";

    RegisterClassW(&wc);

    HWND hWnd = CreateWindowW(L"Lab1Class", L"Лабораторна 1",
        WS_OVERLAPPEDWINDOW, 100, 100, 500, 400,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        HMENU hMenu = CreateMenu();
        HMENU hSubMenu = CreatePopupMenu();
        AppendMenuW(hSubMenu, MF_STRING, IDM_WORK1, L"Робота 1");
        AppendMenuW(hSubMenu, MF_STRING, IDM_WORK2, L"Робота 2");
        AppendMenuW(hSubMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hSubMenu, MF_STRING, IDM_EXIT, L"Вихід");
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Меню");
        SetMenu(hWnd, hMenu);
        break;
    }
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDM_WORK1:
            RunModule1(hWnd, g_szText);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_WORK2:
            RunModule2(hWnd, g_szText);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (g_szText[0] != L'\0') {
            TextOutW(hdc, 50, 50, g_szText, lstrlenW(g_szText));
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}