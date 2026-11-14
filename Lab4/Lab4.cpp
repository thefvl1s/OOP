#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "my_editor.h"

#pragma comment(lib, "comctl32.lib")

MyEditor* myEditor = nullptr;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName = L"Lab4Class";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClassExW(&wcex);

    InitCommonControls();

    HWND hWnd = CreateWindowW(L"Lab4Class", L"Lab4", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
    case WM_CREATE:
        myEditor = new MyEditor();
        myEditor->OnCreate(hWnd);
        break;
    case WM_COMMAND:
        if (myEditor) myEditor->OnCommand(hWnd, wParam);
        break;
    case WM_NOTIFY:
        if (myEditor) myEditor->OnNotify(hWnd, lParam);
        break;
    case WM_INITMENUPOPUP:
        if (myEditor) myEditor->OnInitMenuPopup(hWnd, wParam);
        break;
    case WM_LBUTTONDOWN:
        if (myEditor) myEditor->OnLBdown(hWnd);
        break;
    case WM_LBUTTONUP:
        if (myEditor) myEditor->OnLBup(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (myEditor) myEditor->OnMouseMove(hWnd);
        break;
    case WM_PAINT:
        if (myEditor) myEditor->OnPaint(hWnd);
        break;
    case WM_DESTROY:
        delete myEditor;
        myEditor = nullptr;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
