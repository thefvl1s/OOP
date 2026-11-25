#include <windows.h>
#include <commctrl.h>
#include <cstdio>
#include "resource.h"
#include "my_editor.h"
#include "my_table.h"

#pragma comment(lib, "comctl32.lib")

HINSTANCE hInst;
MyTable myTable;

void OnShapeAdded(const wchar_t* name, long x1, long y1, long x2, long y2) {
    myTable.Add(name, x1, y1, x2, y2);

    FILE* fout = nullptr;
    _wfopen_s(&fout, L"objects.txt", L"a, ccs=UTF-8");
    if (fout) {
        fwprintf(fout, L"%s\t%ld\t%ld\t%ld\t%ld\n", name, x1, y1, x2, y2);
        fclose(fout);
    }
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    hInst = hInstance;

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
    wcex.lpszClassName = L"Lab5Class";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClassExW(&wcex);
    InitCommonControls();

    HWND hWnd = CreateWindowW(L"Lab5Class", L"Lab5", WS_OVERLAPPEDWINDOW,
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
    MyEditor* editor = MyEditor::getInstance();

    switch (message) {
    case WM_CREATE: {
        FILE* fout = nullptr;
        _wfopen_s(&fout, L"objects.txt", L"w, ccs=UTF-8");
        if (fout) fclose(fout);

        editor->SetShapeAddedCallback(OnShapeAdded);
        editor->OnCreate(hWnd);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDM_TABLE) {
            myTable.Toggle(hInst, hWnd);
        }
        else {
            editor->OnCommand(hWnd, wParam);
        }
        break;
    case WM_NOTIFY:
        editor->OnNotify(hWnd, lParam);
        break;
    case WM_INITMENUPOPUP:
        editor->OnInitMenuPopup(hWnd, wParam);
        break;
    case WM_LBUTTONDOWN:
        editor->OnLBdown(hWnd);
        break;
    case WM_LBUTTONUP:
        editor->OnLBup(hWnd);
        break;
    case WM_MOUSEMOVE:
        editor->OnMouseMove(hWnd);
        break;
    case WM_PAINT:
        editor->OnPaint(hWnd);
        break;
    case WM_DESTROY:
        myTable.Close();
        MyEditor::destroyInstance();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
