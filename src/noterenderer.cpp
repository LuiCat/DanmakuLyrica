// 编码：Utf-8 without BOM
// Created by LuiCat

#include "noterenderer.h"
#include "graphics.h"
#include "mathhelper.h"

NoteRenderer::NoteRenderer()
    : radius(0)
    , width(0)
{

}

void NoteRenderer::setRadius(double radius, double width)
{
    this->radius = radius;
    this->width = width;
}

void NoteRenderer::render(double angle)
{
    for (int i = 0; i <= 8; ++i)
    {
        d3d.vertex(radius*sin(angle + i * M_PI / 4), radius*cos(angle + i * M_PI / 4), 0);
        d3d.vertex(radius*sin(angle + i * M_PI / 4), radius*cos(angle + i * M_PI / 4), width);
    }
    d3d.shape(Shape_Triangles);
}
