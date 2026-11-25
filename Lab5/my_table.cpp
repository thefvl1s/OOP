#include "my_table.h"
#include <commctrl.h>
#include <cstdio>

#define IDD_TABLE_DIALOG 201
#define IDC_TABLE_LIST   202

MyTable::MyTable() : hWndDlg(nullptr), hListView(nullptr) {}

MyTable::~MyTable() {
    Close();
}

void MyTable::InitColumns() {
    LVCOLUMNW lvc;
    ZeroMemory(&lvc, sizeof(lvc));
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    const wchar_t* headers[5] = { L"Назва", L"x1", L"y1", L"x2", L"y2" };
    int widths[5] = { 130, 50, 50, 50, 50 };

    for (int i = 0; i < 5; i++) {
        lvc.iSubItem = i;
        lvc.pszText = (LPWSTR)headers[i];
        lvc.cx = widths[i];
        ListView_InsertColumn(hListView, i, &lvc);
    }
}

void MyTable::InsertRowToListView(const TableRow& row, int index) {
    if (!hListView) return;

    LVITEMW lvi;
    ZeroMemory(&lvi, sizeof(lvi));
    lvi.mask = LVIF_TEXT;
    lvi.iItem = index;
    lvi.iSubItem = 0;
    lvi.pszText = (LPWSTR)row.name.c_str();
    ListView_InsertItem(hListView, &lvi);

    wchar_t buf[32];
    swprintf_s(buf, L"%ld", row.x1);
    ListView_SetItemText(hListView, index, 1, buf);
    swprintf_s(buf, L"%ld", row.y1);
    ListView_SetItemText(hListView, index, 2, buf);
    swprintf_s(buf, L"%ld", row.x2);
    ListView_SetItemText(hListView, index, 3, buf);
    swprintf_s(buf, L"%ld", row.y2);
    ListView_SetItemText(hListView, index, 4, buf);
}

INT_PTR CALLBACK MyTable::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    MyTable* pThis = (MyTable*)GetWindowLongPtrW(hDlg, GWLP_USERDATA);

    switch (message) {
    case WM_INITDIALOG:
        pThis = (MyTable*)lParam;
        SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)pThis);
        pThis->hWndDlg = hDlg;
        pThis->hListView = GetDlgItem(hDlg, IDC_TABLE_LIST);
        ListView_SetExtendedListViewStyle(pThis->hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        pThis->InitColumns();
        for (size_t i = 0; i < pThis->rows.size(); i++) {
            pThis->InsertRowToListView(pThis->rows[i], (int)i);
        }
        return (INT_PTR)TRUE;

    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE) {
            DestroyWindow(hDlg);
            return (INT_PTR)TRUE;
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL) {
            DestroyWindow(hDlg);
            return (INT_PTR)TRUE;
        }
        break;

    case WM_DESTROY:
        if (pThis) {
            pThis->hWndDlg = nullptr;
            pThis->hListView = nullptr;
        }
        return (INT_PTR)TRUE;
    }
    return (INT_PTR)FALSE;
}

void MyTable::Toggle(HINSTANCE hInst, HWND hParent) {
    if (hWndDlg) {
        DestroyWindow(hWndDlg);
    }
    else {
        hWndDlg = CreateDialogParamW(hInst, MAKEINTRESOURCEW(IDD_TABLE_DIALOG), hParent, DlgProc, (LPARAM)this);
        if (hWndDlg) {
            ShowWindow(hWndDlg, SW_SHOW);
        }
    }
}

void MyTable::Close() {
    if (hWndDlg) {
        DestroyWindow(hWndDlg);
        hWndDlg = nullptr;
        hListView = nullptr;
    }
}

void MyTable::Add(const wchar_t* name, long x1, long y1, long x2, long y2) {
    TableRow row = { name, x1, y1, x2, y2 };
    rows.push_back(row);
    if (hListView) {
        InsertRowToListView(row, (int)rows.size() - 1);
    }
}
