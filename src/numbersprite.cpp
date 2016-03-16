#include "numbersprite.h"

NumberSprite::NumberSprite()
    :num(0)
    ,maxbuf(10)
    ,format("%d")
    ,content("0")
    ,h(10)
    ,w(10)
    ,k(1)
    ,align(AlignCenter)
{

}

NumberSprite::NumberSprite(const char* filename, int row, int col)
    :num(0)
    ,maxbuf(10)
    ,format("%d")
    ,content("0")
    ,h(10)
    ,w(10)
    ,k(1)
    ,align(AlignCenter)
{
    loadImage(filename, row, col);
}

void NumberSprite::loadImage(const char* filename, int row, int col)
{
    imageNum = ImagePiece::createImageSet(filename, row, col, 10);

}

void NumberSprite::setNumber(int x)
{
    num = x;
    formatContent();
}

void NumberSprite::setFormat(const char* format)
{
    if(format == nullptr)
        return;
    this->format = format;
    formatContent();
}

void NumberSprite::setMaxChar(int maxc)
{
    maxbuf = maxc;
    formatContent();
}

void NumberSprite::setCharSize(double height, double width, double spaceRatio)
{
    h = height;
    w = width;
    k = spaceRatio;
}

void NumberSprite::setAlignment(NumberSprite::Alignment align)
{
    this->align = align;
}

void NumberSprite::formatContent()
{
    char buf[maxbuf+1];
    snprintf(buf, maxbuf, format.c_str(), num);
    content = buf;
}

void NumberSprite::onRender()
{
    int size = content.size();
    if(size<=0)
        return;

    double totalw = (size-1)*w*k;
    double left = w*k*0.5;
    if(align == AlignRight)
        left = -totalw-w*k*0.5;
    else if(align == AlignCenter)
        left = -totalw*0.5;

    d3d.translate2D(left, 0);
    for(int i=0; i<size; ++i)
    {
        int digit = content[i]-'0';
        if(digit>=0 && digit<imageNum.size())
            imageNum[digit].vertice(0.5, 0.5, w, h);
        d3d.translate2D(w*k, 0);
    }

}

