#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "resource.h"

#define MSG_OBJ2_READY 10001
#define MSG_OBJ2_DONE  10002
#define MSG_OBJ3_READY 10003
#define MSG_OBJ3_DONE  10004
#define MSG_OBJ3_RUN   10005

struct Params {
    int n;
    double minVal;
    double maxVal;
};

HINSTANCE hInst;
HWND hWndMain = nullptr;
HWND hWndObj2 = nullptr;
HWND hWndObj3 = nullptr;
Params currentParams = { 15, -10.0, 20.0 };

void LaunchExe(const char* exeName) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string dir(path);
    size_t pos = dir.find_last_of("\\/");
    if (pos != std::string::npos) {
        dir = dir.substr(0, pos + 1);
    }
    std::string cmd = "\"" + dir + exeName + "\"";
    WinExec(cmd.c_str(), SW_SHOW);
}

void SendParamsToObj2(HWND hWndSrc, HWND hWndDest) {
    COPYDATASTRUCT cds;
    cds.dwData = 1;
    cds.cbData = sizeof(Params);
    cds.lpData = &currentParams;
    SendMessageW(hWndDest, WM_COPYDATA, (WPARAM)hWndSrc, (LPARAM)&cds);
}

void StartSequence(HWND hWnd) {
    hWndObj2 = FindWindowW(L"OBJECT2CLASS", NULL);
    if (hWndObj2) {
        SendParamsToObj2(hWnd, hWndObj2);
    }
    else {
        LaunchExe("Object2.exe");
    }
}

INT_PTR CALLBACK ParamsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        wchar_t buf[64];
        swprintf_s(buf, L"%d", currentParams.n);
        SetDlgItemTextW(hDlg, IDC_EDIT_N, buf);
        swprintf_s(buf, L"%.2f", currentParams.minVal);
        SetDlgItemTextW(hDlg, IDC_EDIT_MIN, buf);
        swprintf_s(buf, L"%.2f", currentParams.maxVal);
        SetDlgItemTextW(hDlg, IDC_EDIT_MAX, buf);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            wchar_t buf[64];
            GetDlgItemTextW(hDlg, IDC_EDIT_N, buf, 64);
            int n = _wtoi(buf);
            GetDlgItemTextW(hDlg, IDC_EDIT_MIN, buf, 64);
            double minV = _wtof(buf);
            GetDlgItemTextW(hDlg, IDC_EDIT_MAX, buf, 64);
            double maxV = _wtof(buf);

            if (n <= 0 || n > 500 || minV >= maxV) {
                MessageBoxW(hDlg, L"Введіть коректні значення (0 < n <= 500, Min < Max).", L"Помилка", MB_OK | MB_ICONERROR);
                return (INT_PTR)TRUE;
            }
            currentParams.n = n;
            currentParams.minVal = minV;
            currentParams.maxVal = maxV;
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_WORK:
            if (DialogBoxW(hInst, MAKEINTRESOURCEW(IDD_PARAMS), hWnd, ParamsDlgProc) == IDOK) {
                StartSequence(hWnd);
            }
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case MSG_OBJ2_READY:
            hWndObj2 = (HWND)lParam;
            if (hWndObj2) {
                SendParamsToObj2(hWnd, hWndObj2);
            }
            break;
        case MSG_OBJ2_DONE:
            hWndObj3 = FindWindowW(L"OBJECT3CLASS", NULL);
            if (hWndObj3) {
                PostMessageW(hWndObj3, WM_COMMAND, MSG_OBJ3_RUN, 0);
            }
            else {
                LaunchExe("Object3.exe");
            }
            break;
        case MSG_OBJ3_READY:
            hWndObj3 = (HWND)lParam;
            if (hWndObj3) {
                PostMessageW(hWndObj3, WM_COMMAND, MSG_OBJ3_RUN, 0);
            }
            break;
        case MSG_OBJ3_DONE:
            SetForegroundWindow(hWnd);
            break;
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        const wchar_t* info = L"Менеджер Lab6. Оберіть «Робота -> Запустити обробку...»";
        TextOutW(hdc, 15, 20, info, (int)wcslen(info));
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: {
        HWND h2 = FindWindowW(L"OBJECT2CLASS", NULL);
        if (h2) PostMessageW(h2, WM_CLOSE, 0, 0);
        HWND h3 = FindWindowW(L"OBJECT3CLASS", NULL);
        if (h3) PostMessageW(h3, WM_CLOSE, 0, 0);
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    hInst = hInstance;

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName = L"LAB6CLASS";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassExW(&wcex);

    hWndMain = CreateWindowW(L"LAB6CLASS", L"Lab6 - Менеджер", WS_OVERLAPPEDWINDOW,
        20, 20, 440, 180, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
