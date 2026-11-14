#pragma once
#include <windows.h>
#include <commctrl.h>
#include "editor.h"

class MainApp {
private:
    HWND hToolBar = nullptr;
    Editor* pse = nullptr;
    int currentToolId = 0;
public:
    ~MainApp();
    void OnCreate(HWND hWnd, HINSTANCE hInst);
    void OnSize(HWND hWnd);
    void OnCommand(HWND hWnd, int wmId);
    void OnPaint(HWND hWnd);
    void OnNotify(HWND hWnd, LPARAM lParam);
    void OnInitMenuPopup(HWND hWnd, WPARAM wParam);
    void OnLButtonDown(HWND hWnd);
    void OnMouseMove(HWND hWnd);
    void OnLButtonUp(HWND hWnd);
};