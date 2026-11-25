#pragma once
#include <windows.h>
#include "editor.h"

class MyEditor {
private:
    static MyEditor* p_instance;
    HWND hToolBar;
    Editor* pse;
    int currentToolId;

    MyEditor();
    MyEditor(const MyEditor&) = delete;
    MyEditor& operator=(const MyEditor&) = delete;
public:
    ~MyEditor();
    static MyEditor* getInstance();
    static void destroyInstance();

    void SetShapeAddedCallback(ShapeAddedCallback cb);
    void OnCreate(HWND hWnd);
    void OnCommand(HWND hWnd, WPARAM wParam);
    void OnPaint(HWND hWnd);
    void OnNotify(HWND hWnd, LPARAM lParam);
    void OnInitMenuPopup(HWND hWnd, WPARAM wParam);
    void OnLBdown(HWND hWnd);
    void OnLBup(HWND hWnd);
    void OnMouseMove(HWND hWnd);
};
