#pragma once
#include <windows.h>
#include "shape.h"

typedef void (*ShapeAddedCallback)(const wchar_t* name, long x1, long y1, long x2, long y2);

class Editor {
public:
    virtual ~Editor() {}
    virtual void OnLBdown(HWND hWnd) = 0;
    virtual void OnLBup(HWND hWnd) = 0;
    virtual void OnMouseMove(HWND hWnd) = 0;
    virtual void OnPaint(HWND hWnd) = 0;
};

class ShapeEditor : public Editor {
protected:
    POINT pt;
    long xstart, ystart, xend, yend;
    bool isDrawing;
    static const int MY_SHAPE_ARRAY_SIZE = 108;
    static Shape* pcshape[MY_SHAPE_ARRAY_SIZE];
    static int shapeCount;
    static ShapeAddedCallback onShapeAdded;

    virtual void DrawRubberBand(HWND hWnd);
public:
    ShapeEditor();
    void OnPaint(HWND hWnd) override;
    static void SetCallback(ShapeAddedCallback cb);
};

class PointEditor : public ShapeEditor {
public:
    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override {}
    void OnMouseMove(HWND hWnd) override {}
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

class LineOOEditor : public ShapeEditor {
public:
    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
    void OnMouseMove(HWND hWnd) override;
};

class CubeEditor : public ShapeEditor {
protected:
    void DrawRubberBand(HWND hWnd) override;
public:
    void OnLBdown(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
    void OnMouseMove(HWND hWnd) override;
};
