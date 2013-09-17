#pragma once

#include <d3d9.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 600
#define FILLVALUE 0x00

struct Sprite
{
	int width;
	int height;
	D3DCOLOR key;
	D3DCOLOR* surface;
};

struct Font
{
	int charWidth;
	int charHeight;
	int nCharsPerRow;
	D3DCOLOR* surface;
};

void LoadSprite( Sprite* sprite,const char* filename,
	unsigned int width,unsigned int height,D3DCOLOR key );

void LoadSpriteAlpha( Sprite* sprite );

void FreeSprite( Sprite* sprite );

void LoadFont( Font* font,D3DCOLOR* surface,const char* filename,
	int charWidth,int charHeight,int nCharsPerRow );

class D3DGraphics
{
public:

	D3DGraphics( HWND hWnd );
	~D3DGraphics();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b );
	void PutPixel( int x,int y,D3DCOLOR c );
	D3DCOLOR GetPixel( int x,int y );
	void DrawLine( int x1,int y1,int x2,int y2,int r,int g,int b );
	void DrawLine( int x1,int y1,int x2,int y2, D3DCOLOR c );
	void DrawCircle( int cx,int cy,int radius,int r,int g,int b );
	void DrawDisc( int cx,int cy,int r,int rd,int g,int b );
	void DrawSprite( int x,int y,Sprite* sprite );
	void DrawSpriteAlpha( int xoff,int yoff,Sprite* sprite );
	
	void DrawChar( char c,int x,int y,Font* font,D3DCOLOR color );
	void DrawString( const char* string,int x,int y,Font* font,D3DCOLOR color );
	
	void DrawAmmo( float x, float y);
	void DrawLetter(char letter);
	void DrawShield( unsigned int ShipX, unsigned int ShipY, unsigned int shieldRadius, float shieldStrength );
	void DrawShip(unsigned int ShipX, unsigned int ShipY, int vx, int vy, int vx2, int vy2, D3DCOLOR c);
	D3DCOLOR Blend(D3DCOLOR source1, D3DCOLOR source2, float opacity);
	void EndFrame();

private:
	IDirect3D9*			pDirect3D;
	IDirect3DDevice9*	pDevice;
	IDirect3DSurface9*	pBackBuffer;
	D3DLOCKED_RECT		backRect;
	D3DCOLOR*			pSysBuffer;
};