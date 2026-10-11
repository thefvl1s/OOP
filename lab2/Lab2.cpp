#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

#include <windows.h>
#include "resource.h"
#include "editor.h"

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
Editor* pse = nullptr;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    hInst = hInstance;
    WNDCLASSW wcex = { 0 };

    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"Lab2Class";
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);

    RegisterClassW(&wcex);

    HWND hWnd = CreateWindowW(L"Lab2Class", L"Lab2", WS_OVERLAPPEDWINDOW,
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDM_POINT:
            if (pse) delete pse;
            pse = new PointEditor();
            SetWindowTextW(hWnd, L"Lab2 - Інструмент: Крапка");
            break;
        case IDM_LINE:
            if (pse) delete pse;
            pse = new LineEditor();
            SetWindowTextW(hWnd, L"Lab2 - Інструмент: Лінія");
            break;
        case IDM_RECT:
            if (pse) delete pse;
            pse = new RectEditor();
            SetWindowTextW(hWnd, L"Lab2 - Інструмент: Прямокутник");
            break;
        case IDM_ELLIPSE:
            if (pse) delete pse;
            pse = new EllipseEditor();
            SetWindowTextW(hWnd, L"Lab2 - Інструмент: Еліпс");
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    } break;

    case WM_LBUTTONDOWN:
        if (pse) pse->OnLBdown(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (pse) pse->OnMouseMove(hWnd);
        break;
    case WM_LBUTTONUP:
        if (pse) pse->OnLBup(hWnd);
        break;

    case WM_PAINT:
        if (pse) {
            pse->OnPaint(hWnd);
        }
        else {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        if (pse) delete pse;
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
