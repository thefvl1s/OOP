#include "module1.h"
#include "resource.h"

static wchar_t* g_outText1;

static INT_PTR CALLBACK DialogProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        SendDlgItemMessageW(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"ІМ-54");
        SendDlgItemMessageW(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"ІМ-53");
        SendDlgItemMessageW(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"ІМ-52");
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            int index = SendDlgItemMessageW(hDlg, IDC_LIST1, LB_GETCURSEL, 0, 0);
            if (index != LB_ERR) {
                SendDlgItemMessageW(hDlg, IDC_LIST1, LB_GETTEXT, index, (LPARAM)g_outText1);
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void RunModule1(HWND hWndParent, wchar_t* outText) {
    g_outText1 = outText;
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtrW(hWndParent, GWLP_HINSTANCE);
    DialogBoxW(hInst, MAKEINTRESOURCEW(IDD_DIALOG1), hWndParent, DialogProc1);
}