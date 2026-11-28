#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <cmath>

#define MSG_OBJ3_READY 10003
#define MSG_OBJ3_DONE  10004
#define MSG_OBJ3_RUN   10005

std::vector<double> dataVec;

void ReadDataFromClipboard(HWND hWnd) {
    dataVec.clear();
    if (!IsClipboardFormatAvailable(CF_TEXT)) return;
    if (!OpenClipboard(hWnd)) return;

    HGLOBAL hglb = GetClipboardData(CF_TEXT);
    if (hglb) {
        const char* pText = (const char*)GlobalLock(hglb);
        if (pText) {
            std::stringstream ss(pText);
            double val;
            while (ss >> val) {
                dataVec.push_back(val);
            }
            GlobalUnlock(hglb);
        }
    }
    CloseClipboard();
}

void DrawGraph(HDC hdc, RECT rc) {
    if (dataVec.empty()) {
        const wchar_t* msg = L"Очікування даних з Clipboard...";
        TextOutW(hdc, 20, 20, msg, (int)wcslen(msg));
        return;
    }

    int left = 65;
    int right = rc.right - 30;
    int top = 30;
    int bottom = rc.bottom - 50;
    if (right <= left + 20 || bottom <= top + 20) return;

    double minY = *std::min_element(dataVec.begin(), dataVec.end());
    double maxY = *std::max_element(dataVec.begin(), dataVec.end());
    if (fabs(maxY - minY) < 1e-6) {
        minY -= 1.0;
        maxY += 1.0;
    }

    int n = (int)dataVec.size();
    auto mapX = [&](int i) -> int {
        if (n <= 1) return (left + right) / 2;
        return left + (int)((double)i / (n - 1) * (right - left));
        };
    auto mapY = [&](double y) -> int {
        return bottom - (int)((y - minY) / (maxY - minY) * (bottom - top));
        };

    HPEN hAxisPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hAxisPen);

    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, left, bottom);
    LineTo(hdc, right, bottom);

    if (minY < 0.0 && maxY > 0.0) {
        int y0 = mapY(0.0);
        HPEN hZeroPen = CreatePen(PS_DASH, 1, RGB(150, 150, 150));
        SelectObject(hdc, hZeroPen);
        MoveToEx(hdc, left, y0, NULL);
        LineTo(hdc, right, y0);
        SelectObject(hdc, hAxisPen);
        DeleteObject(hZeroPen);
    }

    wchar_t buf[32];
    int yTicks = 5;
    for (int i = 0; i <= yTicks; i++) {
        double yVal = minY + (maxY - minY) * i / yTicks;
        int py = mapY(yVal);
        MoveToEx(hdc, left - 5, py, NULL);
        LineTo(hdc, left + 5, py);
        swprintf_s(buf, L"%.1f", yVal);
        TextOutW(hdc, 8, py - 8, buf, (int)wcslen(buf));
    }

    int stepX = (std::max)(1, n / 10);
    for (int i = 0; i < n; i += stepX) {
        int px = mapX(i);
        MoveToEx(hdc, px, bottom - 5, NULL);
        LineTo(hdc, px, bottom + 5);
        swprintf_s(buf, L"%d", i);
        TextOutW(hdc, px - 6, bottom + 8, buf, (int)wcslen(buf));
    }

    TextOutW(hdc, left - 15, top - 20, L"Y", 1);
    TextOutW(hdc, right + 8, bottom - 8, L"X", 1);

    HPEN hGraphPen = CreatePen(PS_SOLID, 2, RGB(0, 102, 204));
    SelectObject(hdc, hGraphPen);

    for (int i = 0; i < n; i++) {
        int px = mapX(i);
        int py = mapY(dataVec[i]);
        if (i == 0) {
            MoveToEx(hdc, px, py, NULL);
        }
        else {
            LineTo(hdc, px, py);
        }
    }

    HBRUSH hBrush = CreateSolidBrush(RGB(220, 40, 40));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    for (int i = 0; i < n; i++) {
        int px = mapX(i);
        int py = mapY(dataVec[i]);
        Ellipse(hdc, px - 3, py - 3, px + 3, py + 3);
    }

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hGraphPen);
    DeleteObject(hAxisPen);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        HWND hLab6 = FindWindowW(L"LAB6CLASS", NULL);
        if (hLab6) {
            PostMessageW(hLab6, WM_COMMAND, MSG_OBJ3_READY, (LPARAM)hWnd);
        }
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == MSG_OBJ3_RUN) {
            ReadDataFromClipboard(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
            HWND hLab6 = FindWindowW(L"LAB6CLASS", NULL);
            if (hLab6) {
                PostMessageW(hLab6, WM_COMMAND, MSG_OBJ3_DONE, 0);
            }
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        DrawGraph(hdc, rc);
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
    wcex.lpszClassName = L"OBJECT3CLASS";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"OBJECT3CLASS", L"Object3 - Графік y=f(x)", WS_OVERLAPPEDWINDOW,
        480, 20, 620, 680, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
