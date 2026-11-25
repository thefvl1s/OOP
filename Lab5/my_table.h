#pragma once
#include <windows.h>
#include <vector>
#include <string>

struct TableRow {
    std::wstring name;
    long x1, y1, x2, y2;
};

class MyTable {
private:
    HWND hWndDlg;
    HWND hListView;
    std::vector<TableRow> rows;

    void InitColumns();
    void InsertRowToListView(const TableRow& row, int index);
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
public:
    MyTable();
    ~MyTable();
    void Toggle(HINSTANCE hInst, HWND hParent);
    void Close();
    void Add(const wchar_t* name, long x1, long y1, long x2, long y2);
};
