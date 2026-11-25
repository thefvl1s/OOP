#include "editor.h"

Shape* ShapeEditor::pcshape[MY_SHAPE_ARRAY_SIZE] = { nullptr };
int ShapeEditor::shapeCount = 0;
ShapeAddedCallback ShapeEditor::onShapeAdded = nullptr;

ShapeEditor::ShapeEditor() {
    isDrawing = false;
    xstart = ystart = xend = yend = 0;
}

void ShapeEditor::SetCallback(ShapeAddedCallback cb) {
    onShapeAdded = cb;
}

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
    SetBkMode(hdc, TRANSPARENT);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, xstart, ystart, NULL);
    LineTo(hdc, xend, yend);

    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void RectEditor::DrawRubberBand(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);
    SetBkMode(hdc, TRANSPARENT);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrushOld = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, xstart, ystart, xend, yend);

    SelectObject(hdc, hBrushOld);
    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void EllipseEditor::DrawRubberBand(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);
    SetBkMode(hdc, TRANSPARENT);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrushOld = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    long t_xs1 = xstart - (xend - xstart);
    long t_ys1 = ystart - (yend - ystart);
    long t_xs2 = xstart + (xend - xstart);
    long t_ys2 = ystart + (yend - ystart);

    Ellipse(hdc, t_xs1, t_ys1, t_xs2, t_ys2);

    SelectObject(hdc, hBrushOld);
    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void CubeEditor::DrawRubberBand(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);
    SetBkMode(hdc, TRANSPARENT);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrushOld = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    long offset = 25;
    Rectangle(hdc, xstart, ystart, xend, yend);
    Rectangle(hdc, xstart + offset, ystart - offset, xend + offset, yend - offset);

    MoveToEx(hdc, xstart, ystart, NULL); LineTo(hdc, xstart + offset, ystart - offset);
    MoveToEx(hdc, xend, ystart, NULL); LineTo(hdc, xend + offset, ystart - offset);
    MoveToEx(hdc, xstart, yend, NULL); LineTo(hdc, xstart + offset, yend - offset);
    MoveToEx(hdc, xend, yend, NULL); LineTo(hdc, xend + offset, yend - offset);

    SelectObject(hdc, hBrushOld);
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
        if (onShapeAdded) onShapeAdded(L"Крапка", pt.x, pt.y, pt.x, pt.y);
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

void LineEditor::OnLBdown(HWND hWnd) {
    GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x; ystart = yend = pt.y; isDrawing = true;
}
void LineEditor::OnMouseMove(HWND hWnd) {
    if (isDrawing) { DrawRubberBand(hWnd); GetCursorPos(&pt); ScreenToClient(hWnd, &pt); xend = pt.x; yend = pt.y; DrawRubberBand(hWnd); }
}
void LineEditor::OnLBup(HWND hWnd) {
    if (isDrawing) {
        DrawRubberBand(hWnd); isDrawing = false; GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
        if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
            pcshape[shapeCount] = new LineShape();
            pcshape[shapeCount]->Set(xstart, ystart, pt.x, pt.y);
            shapeCount++;
            if (onShapeAdded) onShapeAdded(L"Лінія", xstart, ystart, pt.x, pt.y);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}

void RectEditor::OnLBdown(HWND hWnd) {
    GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x; ystart = yend = pt.y; isDrawing = true;
}
void RectEditor::OnMouseMove(HWND hWnd) {
    if (isDrawing) { DrawRubberBand(hWnd); GetCursorPos(&pt); ScreenToClient(hWnd, &pt); xend = pt.x; yend = pt.y; DrawRubberBand(hWnd); }
}
void RectEditor::OnLBup(HWND hWnd) {
    if (isDrawing) {
        DrawRubberBand(hWnd); isDrawing = false; GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
        if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
            pcshape[shapeCount] = new RectShape();
            pcshape[shapeCount]->Set(xstart, ystart, pt.x, pt.y);
            shapeCount++;
            if (onShapeAdded) onShapeAdded(L"Прямокутник", xstart, ystart, pt.x, pt.y);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}

void EllipseEditor::OnLBdown(HWND hWnd) {
    GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x; ystart = yend = pt.y; isDrawing = true;
}
void EllipseEditor::OnMouseMove(HWND hWnd) {
    if (isDrawing) { DrawRubberBand(hWnd); GetCursorPos(&pt); ScreenToClient(hWnd, &pt); xend = pt.x; yend = pt.y; DrawRubberBand(hWnd); }
}
void EllipseEditor::OnLBup(HWND hWnd) {
    if (isDrawing) {
        DrawRubberBand(hWnd); isDrawing = false; GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
        if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
            pcshape[shapeCount] = new EllipseShape();
            pcshape[shapeCount]->Set(xstart, ystart, pt.x, pt.y);
            shapeCount++;
            if (onShapeAdded) onShapeAdded(L"Еліпс", xstart, ystart, pt.x, pt.y);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}

void LineOOEditor::OnLBdown(HWND hWnd) {
    GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x; ystart = yend = pt.y; isDrawing = true;
}
void LineOOEditor::OnMouseMove(HWND hWnd) {
    if (isDrawing) { DrawRubberBand(hWnd); GetCursorPos(&pt); ScreenToClient(hWnd, &pt); xend = pt.x; yend = pt.y; DrawRubberBand(hWnd); }
}
void LineOOEditor::OnLBup(HWND hWnd) {
    if (isDrawing) {
        DrawRubberBand(hWnd); isDrawing = false; GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
        if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
            pcshape[shapeCount] = new LineOOShape();
            pcshape[shapeCount]->Set(xstart, ystart, pt.x, pt.y);
            shapeCount++;
            if (onShapeAdded) onShapeAdded(L"Лінія з кружечками", xstart, ystart, pt.x, pt.y);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}

void CubeEditor::OnLBdown(HWND hWnd) {
    GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
    xstart = xend = pt.x; ystart = yend = pt.y; isDrawing = true;
}
void CubeEditor::OnMouseMove(HWND hWnd) {
    if (isDrawing) { DrawRubberBand(hWnd); GetCursorPos(&pt); ScreenToClient(hWnd, &pt); xend = pt.x; yend = pt.y; DrawRubberBand(hWnd); }
}
void CubeEditor::OnLBup(HWND hWnd) {
    if (isDrawing) {
        DrawRubberBand(hWnd); isDrawing = false; GetCursorPos(&pt); ScreenToClient(hWnd, &pt);
        if (shapeCount < MY_SHAPE_ARRAY_SIZE) {
            pcshape[shapeCount] = new CubeShape();
            pcshape[shapeCount]->Set(xstart, ystart, pt.x, pt.y);
            shapeCount++;
            if (onShapeAdded) onShapeAdded(L"Каркас куба", xstart, ystart, pt.x, pt.y);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}
