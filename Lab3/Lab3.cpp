#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(lib, "comctl32.lib")

#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "main_app.h"

HINSTANCE hInst;
MainApp app;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        app.OnCreate(hWnd, ((LPCREATESTRUCT)lParam)->hInstance);
        break;
    case WM_SIZE:
        app.OnSize(hWnd);
        break;
    case WM_COMMAND:
        app.OnCommand(hWnd, LOWORD(wParam));
        break;
    case WM_NOTIFY:
        app.OnNotify(hWnd, lParam);
        break;
    case WM_INITMENUPOPUP:
        app.OnInitMenuPopup(hWnd, wParam);
        break;
    case WM_LBUTTONDOWN:
        app.OnLButtonDown(hWnd);
        break;
    case WM_MOUSEMOVE:
        app.OnMouseMove(hWnd);
        break;
    case WM_LBUTTONUP:
        app.OnLButtonUp(hWnd);
        break;
    case WM_PAINT:
        app.OnPaint(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    InitCommonControls();
    hInst = hInstance;
    WNDCLASSW wcex = { 0 };

    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"Lab3Class";
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);

    RegisterClassW(&wcex);

    HWND hWnd = CreateWindowW(L"Lab3Class", L"Lab3", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}