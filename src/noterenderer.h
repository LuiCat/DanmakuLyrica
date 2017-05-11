// 编码：Utf-8 without BOM
// Created by LuiCat

#ifndef _NOTE_RENDERER_H_
#define _NOTE_RENDERER_H_

class NoteRenderer
{
private:
    double radius;
    double width;
public:
    NoteRenderer();
    void setRadius(double radius, double width);
    void render(double angle);
};


#endif // _NOTE_RENDERER_H_

