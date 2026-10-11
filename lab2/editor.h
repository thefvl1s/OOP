#pragma once
#include "shape.h"
#include <cmath>

class Editor {
public:
    virtual void OnLBdown(HWND hWnd) = 0;
    virtual void OnLBup(HWND hWnd) = 0;
    virtual void OnMouseMove(HWND hWnd) = 0;
    virtual void OnPaint(HWND hWnd) = 0;
    virtual ~Editor() {}
};

class ShapeEditor : public Editor {
protected:
    POINT pt = { 0, 0 };
    long xstart = 0, ystart = 0, xend = 0, yend = 0;
    bool isDrawing = false;
    virtual void DrawRubberBand(HWND hWnd);
public:
    void OnPaint(HWND hWnd) override;
};

class PointEditor : public ShapeEditor {
public:
    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
    void OnMouseMove(HWND hWnd) override;
};

class LineEditor : public ShapeEditor {
public:
    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
    void OnMouseMove(HWND hWnd) override;
};

class RectEditor : public ShapeEditor {
protected:
    void DrawRubberBand(HWND hWnd) override;
public:
    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
    void OnMouseMove(HWND hWnd) override;
};

class EllipseEditor : public ShapeEditor {
protected:
    void DrawRubberBand(HWND hWnd) override;
public:
    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
    void OnMouseMove(HWND hWnd) override;
};
