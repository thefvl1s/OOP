#include "shape.h"

void Shape::Set(long x1, long y1, long x2, long y2) {
    xs1 = x1; ys1 = y1; xs2 = x2; ys2 = y2;
}

void PointShape::Show(HDC hdc) {
    SetPixel(hdc, xs1, ys1, RGB(0, 0, 0));
}

void LineShape::Show(HDC hdc) {
    MoveToEx(hdc, xs1, ys1, NULL);
    LineTo(hdc, xs2, ys2);
}

void RectShape::Show(HDC hdc) {
    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Rectangle(hdc, xs1, ys1, xs2, ys2);
    SelectObject(hdc, hOldBrush);
}

void EllipseShape::Set(long x1, long y1, long x2, long y2) {
    xs1 = x1 - (x2 - x1);
    ys1 = y1 - (y2 - y1);
    xs2 = x1 + (x2 - x1);
    ys2 = y1 + (y2 - y1);
}

void EllipseShape::Show(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(144, 238, 144));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, xs1, ys1, xs2, ys2);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}

void LineOOShape::Set(long x1, long y1, long x2, long y2) {
    Shape::Set(x1, y1, x2, y2);
}

void LineOOShape::Show(HDC hdc) {
    long t_x1 = xs1, t_y1 = ys1, t_x2 = xs2, t_y2 = ys2;

    LineShape::Set(t_x1, t_y1, t_x2, t_y2);
    LineShape::Show(hdc);

    long r = 10;
    EllipseShape::Set(t_x1, t_y1, t_x1 + r, t_y1 + r);
    EllipseShape::Show(hdc);

    EllipseShape::Set(t_x2, t_y2, t_x2 + r, t_y2 + r);
    EllipseShape::Show(hdc);

    Shape::Set(t_x1, t_y1, t_x2, t_y2);
}

void CubeShape::Set(long x1, long y1, long x2, long y2) {
    Shape::Set(x1, y1, x2, y2);
}

void CubeShape::Show(HDC hdc) {
    long t_x1 = xs1, t_y1 = ys1, t_x2 = xs2, t_y2 = ys2;
    long offset = 25;

    RectShape::Set(t_x1, t_y1, t_x2, t_y2);
    RectShape::Show(hdc);

    RectShape::Set(t_x1 + offset, t_y1 - offset, t_x2 + offset, t_y2 - offset);
    RectShape::Show(hdc);

    LineShape::Set(t_x1, t_y1, t_x1 + offset, t_y1 - offset);
    LineShape::Show(hdc);

    LineShape::Set(t_x2, t_y1, t_x2 + offset, t_y1 - offset);
    LineShape::Show(hdc);

    LineShape::Set(t_x1, t_y2, t_x1 + offset, t_y2 - offset);
    LineShape::Show(hdc);

    LineShape::Set(t_x2, t_y2, t_x2 + offset, t_y2 - offset);
    LineShape::Show(hdc);

    Shape::Set(t_x1, t_y1, t_x2, t_y2);
}
