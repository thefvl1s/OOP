#include "editor.h"

const int MY_SHAPE_ARRAY_SIZE = 107;
Shape* pcshape[MY_SHAPE_ARRAY_SIZE] = { nullptr };
int shapeCount = 0;

void ShapeEditor::OnPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    for (int i = 0; i < shapeCount; i++) {
        if (pcshape[i]) {
            pcshape[i]->Show(hdc);
        }
    }
    EndPaint(hWnd, &ps);
}

void ShapeEditor::DrawRubberBand(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, xstart, ystart, NULL);
    LineTo(hdc, xend, yend);

    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void PointEditor::OnLBdown(HWND hWnd) {
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
        pcshape[shapeCount] = new PointShape();
        pcshape[shapeCount]->Set(pt.x, pt.y, pt.x, pt.y);
        shapeCount++;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}
void PointEditor::OnMouseMove(HWND hWnd) {}
void PointEditor::OnLBup(HWND hWnd) {}

void LineEditor::OnLBdown(HWND hWnd) {
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x;
    ystart = yend = pt.y;
    isDrawing = true;
}
void LineEditor::OnMouseMove(HWND hWnd) {
    if (!isDrawing) return;
    DrawRubberBand(hWnd);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;
    DrawRubberBand(hWnd);
}
void LineEditor::OnLBup(HWND hWnd) {
    if (!isDrawing) return;
    DrawRubberBand(hWnd);
    isDrawing = false;
    if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
        pcshape[shapeCount] = new LineShape();
        pcshape[shapeCount]->Set(xstart, ystart, xend, yend);
        shapeCount++;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

void RectEditor::DrawRubberBand(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrushOld = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    long dx = abs(xend - xstart);
    long dy = abs(yend - ystart);
    Rectangle(hdc, xstart - dx, ystart - dy, xstart + dx, ystart + dy);

    SelectObject(hdc, hBrushOld);
    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}
void RectEditor::OnLBdown(HWND hWnd) {
    isDrawing = true;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x;
    ystart = yend = pt.y;
}
void RectEditor::OnMouseMove(HWND hWnd) {
    if (!isDrawing) return;
    DrawRubberBand(hWnd);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;
    DrawRubberBand(hWnd);
}
void RectEditor::OnLBup(HWND hWnd) {
    if (!isDrawing) return;
    DrawRubberBand(hWnd);
    isDrawing = false;
    long dx = abs(xend - xstart);
    long dy = abs(yend - ystart);
    if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
        pcshape[shapeCount] = new RectShape();
        pcshape[shapeCount]->Set(xstart - dx, ystart - dy, xstart + dx, ystart + dy);
        shapeCount++;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

void EllipseEditor::DrawRubberBand(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrushOld = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Ellipse(hdc, xstart, ystart, xend, yend);

    SelectObject(hdc, hBrushOld);
    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}
void EllipseEditor::OnLBdown(HWND hWnd) {
    isDrawing = true;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x;
    ystart = yend = pt.y;
}
void EllipseEditor::OnMouseMove(HWND hWnd) {
    if (!isDrawing) return;
    DrawRubberBand(hWnd);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;
    DrawRubberBand(hWnd);
}
void EllipseEditor::OnLBup(HWND hWnd) {
    if (!isDrawing) return;
    DrawRubberBand(hWnd);
    isDrawing = false;
    if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
        pcshape[shapeCount] = new EllipseShape();
        pcshape[shapeCount]->Set(xstart, ystart, xend, yend);
        shapeCount++;
        InvalidateRect(hWnd, NULL, TRUE);
    }
}
