#include "my_editor.h"
#include "resource.h"
#include <commctrl.h>

MyEditor* MyEditor::p_instance = nullptr;

MyEditor::MyEditor() {
    pse = new PointEditor();
    currentToolId = IDM_POINT;
    hToolBar = NULL;
}

MyEditor::~MyEditor() {
    if (pse) delete pse;
}

MyEditor* MyEditor::getInstance() {
    if (!p_instance) {
        p_instance = new MyEditor();
    }
    return p_instance;
}

void MyEditor::destroyInstance() {
    if (p_instance) {
        delete p_instance;
        p_instance = nullptr;
    }
}

void MyEditor::SetShapeAddedCallback(ShapeAddedCallback cb) {
    ShapeEditor::SetCallback(cb);
}

void MyEditor::OnCreate(HWND hWnd) {
    TBBUTTON tbb[6];
    ZeroMemory(tbb, sizeof(tbb));

    tbb[0].iBitmap = 0; tbb[0].idCommand = IDM_POINT; tbb[0].fsState = TBSTATE_ENABLED; tbb[0].fsStyle = TBSTYLE_BUTTON;
    tbb[1].iBitmap = 1; tbb[1].idCommand = IDM_LINE; tbb[1].fsState = TBSTATE_ENABLED; tbb[1].fsStyle = TBSTYLE_BUTTON;
    tbb[2].iBitmap = 2; tbb[2].idCommand = IDM_RECT; tbb[2].fsState = TBSTATE_ENABLED; tbb[2].fsStyle = TBSTYLE_BUTTON;
    tbb[3].iBitmap = 3; tbb[3].idCommand = IDM_ELLIPSE; tbb[3].fsState = TBSTATE_ENABLED; tbb[3].fsStyle = TBSTYLE_BUTTON;
    tbb[4].iBitmap = 4; tbb[4].idCommand = IDM_LINEOO; tbb[4].fsState = TBSTATE_ENABLED; tbb[4].fsStyle = TBSTYLE_BUTTON;
    tbb[5].iBitmap = 5; tbb[5].idCommand = IDM_CUBE; tbb[5].fsState = TBSTATE_ENABLED; tbb[5].fsStyle = TBSTYLE_BUTTON;

    hToolBar = CreateToolbarEx(hWnd,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TBSTYLE_TOOLTIPS,
        1, 6, GetModuleHandle(NULL), IDR_TOOLBAR1, tbb, 6,
        16, 15, 16, 15, sizeof(TBBUTTON));
}

void MyEditor::OnCommand(HWND hWnd, WPARAM wParam) {
    int wmId = LOWORD(wParam);
    switch (wmId) {
    case IDM_POINT:
        if (pse) delete pse;
        pse = new PointEditor();
        currentToolId = IDM_POINT;
        break;
    case IDM_LINE:
        if (pse) delete pse;
        pse = new LineEditor();
        currentToolId = IDM_LINE;
        break;
    case IDM_RECT:
        if (pse) delete pse;
        pse = new RectEditor();
        currentToolId = IDM_RECT;
        break;
    case IDM_ELLIPSE:
        if (pse) delete pse;
        pse = new EllipseEditor();
        currentToolId = IDM_ELLIPSE;
        break;
    case IDM_LINEOO:
        if (pse) delete pse;
        pse = new LineOOEditor();
        currentToolId = IDM_LINEOO;
        break;
    case IDM_CUBE:
        if (pse) delete pse;
        pse = new CubeEditor();
        currentToolId = IDM_CUBE;
        break;
    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    }
}

void MyEditor::OnPaint(HWND hWnd) {
    if (pse) pse->OnPaint(hWnd);
}

void MyEditor::OnNotify(HWND hWnd, LPARAM lParam) {
    LPNMHDR pnmh = (LPNMHDR)lParam;
    if (pnmh->code == TTN_GETDISPINFO) {
        LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
        switch (lpttt->hdr.idFrom) {
        case IDM_POINT:   lstrcpy(lpttt->szText, L"Крапка"); break;
        case IDM_LINE:    lstrcpy(lpttt->szText, L"Лінія"); break;
        case IDM_RECT:    lstrcpy(lpttt->szText, L"Прямокутник"); break;
        case IDM_ELLIPSE: lstrcpy(lpttt->szText, L"Еліпс"); break;
        case IDM_LINEOO:  lstrcpy(lpttt->szText, L"Лінія з кружечками"); break;
        case IDM_CUBE:    lstrcpy(lpttt->szText, L"Каркас куба"); break;
        }
    }
}

void MyEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam) {
    HMENU hMenu = (HMENU)wParam;
    CheckMenuItem(hMenu, IDM_POINT, MF_BYCOMMAND | ((currentToolId == IDM_POINT) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_LINE, MF_BYCOMMAND | ((currentToolId == IDM_LINE) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_RECT, MF_BYCOMMAND | ((currentToolId == IDM_RECT) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_ELLIPSE, MF_BYCOMMAND | ((currentToolId == IDM_ELLIPSE) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_LINEOO, MF_BYCOMMAND | ((currentToolId == IDM_LINEOO) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_CUBE, MF_BYCOMMAND | ((currentToolId == IDM_CUBE) ? MF_CHECKED : MF_UNCHECKED));
}

void MyEditor::OnLBdown(HWND hWnd) {
    if (pse) pse->OnLBdown(hWnd);
}

void MyEditor::OnLBup(HWND hWnd) {
    if (pse) pse->OnLBup(hWnd);
}

void MyEditor::OnMouseMove(HWND hWnd) {
    if (pse) pse->OnMouseMove(hWnd);
}
