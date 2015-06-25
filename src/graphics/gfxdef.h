#ifndef GFXDEF_H
#define GFXDEF_H

#include <d3d9.h>
#include <d3dx9.h>

typedef  D3DXMATRIX          Matrix  ;
typedef  LPDIRECT3DTEXTURE9  Texture ;
typedef  LPDIRECT3DSURFACE9  Surface ;
typedef  D3DXVECTOR3         Vector  ;

inline DWORD ColorBlend(DWORD col1, DWORD col2, double weight)
{
    DWORD res=0,tmp;
    for(int i=0;i<32;i+=8)
    {
        tmp=(255<<i);
        res|=(((DWORD)((col1&tmp)*weight+(col2&tmp)*(1-weight)))&tmp);
    }
    return res;
}

#endif // GFXDEF_H

