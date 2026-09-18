#include "module2.h"
#include "resource.h"

static wchar_t* g_outText2;

static INT_PTR CALLBACK DialogProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            GetDlgItemTextW(hDlg, IDC_EDIT1, g_outText2, 255);
            EndDialog(hDlg, 1);
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, 0);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void RunModule2(HWND hWndParent, wchar_t* outText) {
    g_outText2 = outText;
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtrW(hWndParent, GWLP_HINSTANCE);
    DialogBoxW(hInst, MAKEINTRESOURCEW(IDD_DIALOG2), hWndParent, DialogProc2);
}