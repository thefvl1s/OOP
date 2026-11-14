#pragma once
#include <windows.h>
#include "editor.h"

class MyEditor {
private:
    HWND hToolBar;
    Editor* pse;
    int currentToolId;
public:
    MyEditor();
    ~MyEditor();
    void OnCreate(HWND hWnd);
    void OnCommand(HWND hWnd, WPARAM wParam);
    void OnPaint(HWND hWnd);
    void OnNotify(HWND hWnd, LPARAM lParam);
    void OnInitMenuPopup(HWND hWnd, WPARAM wParam);
    void OnLBdown(HWND hWnd);
    void OnLBup(HWND hWnd);
    void OnMouseMove(HWND hWnd);
};
