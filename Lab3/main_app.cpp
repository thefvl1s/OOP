#include "main_app.h"
#include "resource.h"

MainApp::~MainApp() {
    if (pse) delete pse;
}

void MainApp::OnCreate(HWND hWnd, HINSTANCE hInst) {
    TBBUTTON tbb[4];
    ZeroMemory(tbb, sizeof(tbb));

    tbb[0].iBitmap = 0;
    tbb[0].fsState = TBSTATE_ENABLED;
    tbb[0].fsStyle = TBSTYLE_BUTTON;
    tbb[0].idCommand = IDM_POINT;

    tbb[1].iBitmap = 1;
    tbb[1].fsState = TBSTATE_ENABLED;
    tbb[1].fsStyle = TBSTYLE_BUTTON;
    tbb[1].idCommand = IDM_LINE;

    tbb[2].iBitmap = 2;
    tbb[2].fsState = TBSTATE_ENABLED;
    tbb[2].fsStyle = TBSTYLE_BUTTON;
    tbb[2].idCommand = IDM_RECT;

    tbb[3].iBitmap = 3;
    tbb[3].fsState = TBSTATE_ENABLED;
    tbb[3].fsStyle = TBSTYLE_BUTTON;
    tbb[3].idCommand = IDM_ELLIPSE;

    hToolBar = CreateToolbarEx(hWnd,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP | TBSTYLE_TOOLTIPS,
        IDC_MY_TOOLBAR,
        4, hInst, IDR_TOOLBAR1,
        tbb, 4,
        16, 16, 16, 16,
        sizeof(TBBUTTON));
}

void MainApp::OnSize(HWND hWnd) {
    if (hToolBar) {
        RECT rc, rw;
        GetClientRect(hWnd, &rc);
        GetWindowRect(hToolBar, &rw);
        MoveWindow(hToolBar, 0, 0, rc.right - rc.left, rw.bottom - rw.top, FALSE);
    }
}

void MainApp::OnCommand(HWND hWnd, int wmId) {
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
    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    }
}

void MainApp::OnPaint(HWND hWnd) {
    if (pse) {
        pse->OnPaint(hWnd);
    }
    else {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
}

void MainApp::OnNotify(HWND hWnd, LPARAM lParam) {
    LPNMHDR pnmh = (LPNMHDR)lParam;
    if (pnmh->code == TTN_GETDISPINFO) {
        LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;
        switch (lpttt->hdr.idFrom) {
        case IDM_POINT:
            lstrcpy(lpttt->szText, L"Крапка");
            break;
        case IDM_LINE:
            lstrcpy(lpttt->szText, L"Лінія");
            break;
        case IDM_RECT:
            lstrcpy(lpttt->szText, L"Прямокутник");
            break;
        case IDM_ELLIPSE:
            lstrcpy(lpttt->szText, L"Еліпс");
            break;
        }
    }
}

void MainApp::OnInitMenuPopup(HWND hWnd, WPARAM wParam) {
    HMENU hMenu = (HMENU)wParam;
    CheckMenuItem(hMenu, IDM_POINT, MF_BYCOMMAND | ((currentToolId == IDM_POINT) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_LINE, MF_BYCOMMAND | ((currentToolId == IDM_LINE) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_RECT, MF_BYCOMMAND | ((currentToolId == IDM_RECT) ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_ELLIPSE, MF_BYCOMMAND | ((currentToolId == IDM_ELLIPSE) ? MF_CHECKED : MF_UNCHECKED));
}

void MainApp::OnLButtonDown(HWND hWnd) {
    if (pse) pse->OnLBdown(hWnd);
}

void MainApp::OnMouseMove(HWND hWnd) {
    if (pse) pse->OnMouseMove(hWnd);
}

void MainApp::OnLButtonUp(HWND hWnd) {
    if (pse) pse->OnLBup(hWnd);
}
