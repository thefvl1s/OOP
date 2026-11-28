#include <windows.h>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#define MSG_OBJ2_READY 10001
#define MSG_OBJ2_DONE  10002

struct Params {
    int n;
    double minVal;
    double maxVal;
};

std::vector<double> dataVec;

void PutTextToClipboard(HWND hWnd, const std::string& text) {
    if (text.empty()) return;
    HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hglb) return;
    char* pTmp = (char*)GlobalLock(hglb);
    if (pTmp) {
        memcpy(pTmp, text.c_str(), text.size() + 1);
        GlobalUnlock(hglb);
    }
    if (OpenClipboard(hWnd)) {
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hglb);
        CloseClipboard();
    }
    else {
        GlobalFree(hglb);
    }
}

void GenerateData(HWND hWnd, const Params& p) {
    dataVec.clear();
    std::string clipText;
    char buf[64];

    for (int i = 0; i < p.n; i++) {
        double r = (double)rand() / (double)RAND_MAX;
        double val = p.minVal + r * (p.maxVal - p.minVal);
        dataVec.push_back(val);
        sprintf_s(buf, "%.2f\r\n", val);
        clipText += buf;
    }

    PutTextToClipboard(hWnd, clipText);
    InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        srand((unsigned int)time(NULL));
        HWND hLab6 = FindWindowW(L"LAB6CLASS", NULL);
        if (hLab6) {
            PostMessageW(hLab6, WM_COMMAND, MSG_OBJ2_READY, (LPARAM)hWnd);
        }
        break;
    }
    case WM_COPYDATA: {
        COPYDATASTRUCT* cds = (COPYDATASTRUCT*)lParam;
        if (cds && cds->cbData == sizeof(Params)) {
            Params* p = (Params*)cds->lpData;
            GenerateData(hWnd, *p);
            HWND hLab6 = (HWND)wParam;
            if (hLab6) {
                PostMessageW(hLab6, WM_COMMAND, MSG_OBJ2_DONE, 0);
            }
        }
        return TRUE;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);

        int colWidth = 85;
        int rowHeight = 22;
        int cols = (rc.right - 20) / colWidth;
        if (cols < 1) cols = 1;

        wchar_t buf[64];
        for (size_t i = 0; i < dataVec.size(); i++) {
            int r = (int)i / cols;
            int c = (int)i % cols;
            int x = 15 + c * colWidth;
            int y = 15 + r * rowHeight;
            swprintf_s(buf, L"[%zu] %.2f", i, dataVec[i]);
            TextOutW(hdc, x, y, buf, (int)wcslen(buf));
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"OBJECT2CLASS";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"OBJECT2CLASS", L"Object2 - Вектор даних", WS_OVERLAPPEDWINDOW,
        20, 220, 440, 480, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
