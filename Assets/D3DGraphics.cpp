
#include "D3DGraphics.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <GdiPlus.h>
#include "Bitmap.h"
#include "FrameTimer.h"
#pragma comment( lib,"gdiplus.lib" )

void LoadSprite( Sprite* sprite,const char* filename,
	unsigned int width,unsigned int height,D3DCOLOR key )
{
	sprite->surface = (D3DCOLOR*)malloc( sizeof( D3DCOLOR ) * width * height );
	LoadBmp( filename,sprite->surface );
	
	sprite->height = height;
	sprite->width = width;
	sprite->key = key;
}

void LoadSpriteAlpha( Sprite* sprite )
{	
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartup( &gdiplusToken,&gdiplusStartupInput,NULL );

	Gdiplus::Bitmap bitmap( L"alphahalf.png" );
	Gdiplus::Color pixel;

	sprite->height = bitmap.GetHeight();
	sprite->width = bitmap.GetWidth();
	sprite->key = 0x010301F3;
	sprite->surface = (D3DCOLOR*)malloc( sizeof( D3DCOLOR ) * sprite->height * sprite->width );

	for( unsigned int y = 0; y < bitmap.GetHeight(); y++ )
	{
		for( unsigned int x = 0; x < bitmap.GetWidth(); x++ )
		{
			bitmap.GetPixel( x,y,&pixel );
			sprite->surface[ x + y * bitmap.GetWidth() ] = 
				D3DCOLOR_ARGB( pixel.GetA(),pixel.GetR(),pixel.GetG(),pixel.GetB() );
		}
	}
}

void FreeSprite( Sprite* sprite )
{
	free( sprite->surface );
}

void LoadFont( Font* font,D3DCOLOR* surface,const char* filename,
	int charWidth,int charHeight,int nCharsPerRow )
{
	LoadBmp( filename,surface );
	font->charHeight = charHeight;
	font->charWidth = charWidth;
	font->nCharsPerRow = nCharsPerRow;
	font->surface = surface;
}

D3DGraphics::D3DGraphics( HWND hWnd )
	:
pDirect3D( NULL ),
pDevice( NULL ),
pBackBuffer( NULL ),
pSysBuffer( NULL )
{
	HRESULT result;

	backRect.pBits = NULL;
	
	pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
	assert( pDirect3D != NULL );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp,sizeof( d3dpp ) );
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	
    result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,&d3dpp,&pDevice );
	assert( !FAILED( result ) );

	result = pDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer );
	assert( !FAILED( result ) );

	pSysBuffer = new D3DCOLOR[ SCREENWIDTH * SCREENHEIGHT ];
}

D3DGraphics::~D3DGraphics()
{
	if( pDevice )
	{
		pDevice->Release();
		pDevice = NULL;
	}
	if( pDirect3D )
	{
		pDirect3D->Release();
		pDirect3D = NULL;
	}
	if( pBackBuffer )
	{
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
	if( pSysBuffer )
	{
		delete pSysBuffer;
		pSysBuffer = NULL;
	}
}

void D3DGraphics::DrawAmmo(float x, float y)
{
	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 2; i++)
		{
			PutPixel(x + i, y + j, D3DCOLOR_XRGB(255, 255, 255));
		}
	}
}

void D3DGraphics::DrawLetter(char letter)
{
	
	switch (letter)
	{
	case 'A':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 0;
			Point[y][i] = 31;
			i++;
			// Point 1
			Point[x][i] = 13;
			Point[y][i] = 0;
			i++;
			// Point 2
			Point[x][i] = 24;
			Point[y][i] = 31;
			i++;
			// Point 3
			Point[x][i] = 5;
			Point[y][i] = 22;
			i++;
			// Point 4
			Point[x][i] = 21;
			Point[y][i] = 22;
			
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255,128 );
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255,128 );
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255,128 );
			break;
		}
	case 'a':
		{
			char Point[2][6];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 1
			Point[x][i] = 16;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 16;
			Point[y][i] = 22;
			i++;
			// Point 3
			Point[x][i] = 0;
			Point[y][i] = 22;
			i++;
			// Point 4
			Point[x][i] = 0;
			Point[y][i] = 31;
			i++;
			// Point 5
			Point[x][i] = 16;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 );
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 );
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 );
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
			DrawLine(Point[x][5], Point[y][5], Point[x][2], Point[y][2], 128, 255, 128 );
			break;
		}
	case 'B':
		{
			char Point[2][6];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 16;
			Point[y][i] = 7;
			i++;
			// Point 2
			Point[x][i] = 16;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 4
			Point[x][i] = 15;
			Point[y][i] = 31;
			i++;
			// Point 5
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  _
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_|
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][0], Point[y][0], 128, 255, 128 );
			DrawLine(Point[x][2], Point[y][2], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
			DrawLine(Point[x][5], Point[y][5], Point[x][3], Point[y][3], 128, 255, 128 );

			break;
		}
	case 'b':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );

			break;
		}
	case 'C':
		{
			char Point[2][6];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 31;
			Point[y][i] = 10;
			i++;
			// Point 1
			Point[x][i] = 15;
			Point[y][i] = 0;
			i++;
			// Point 2
			Point[x][i] = 2;
			Point[y][i] = 10;
			i++;
			// Point 3
			Point[x][i] = 2;
			Point[y][i] = 21;
			i++;
			// Point 4
			Point[x][i] = 15;
			Point[y][i] = 31;
			i++;
			// Point 5
			Point[x][i] = 31;
			Point[y][i] = 21;

									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
			break;
		}
	case 'c':
		{
			char Point[2][6];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 15;
			Point[y][i] = 21;
			i++;
			// Point 1
			Point[x][i] = 7;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 2;
			Point[y][i] = 21;
			i++;
			// Point 3
			Point[x][i] = 2;
			Point[y][i] = 26;
			i++;
			// Point 4
			Point[x][i] = 7;
			Point[y][i] = 31;
			i++;
			// Point 5
			Point[x][i] = 15;
			Point[y][i] = 26;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
			break;
		}
	case 'D':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 15;
			Point[y][i] = 0;
			i++;
			// Point 2
			Point[x][i] = 31;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 15;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'd':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 15;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 15;
			Point[y][i] = 31;
			i++;
			// Point 2
			Point[x][i] = 2;
			Point[y][i] = 31;
			i++;
			// Point 3
			Point[x][i] = 2;
			Point[y][i] = 16;
			i++;
			// Point 4
			Point[x][i] = 15;
			Point[y][i] = 16;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			break;
		}
	case 'E':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 31;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 2
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 23;
			Point[y][i] = 15;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
			i++;
			// Point 5
			Point[x][i] = 31;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][2], Point[y][2], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
			break;
		}
	case 'e':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'F':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'f':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'G':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'g':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'H':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'h':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'I':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'i':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'J':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'j':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'K':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'k':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'L':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'l':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'M':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'm':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'N':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'n':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'O':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'o':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'P':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'p':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'Q':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'q':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'S':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 's':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'T':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 't':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'U':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'u':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'V':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'v':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'W':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'w':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'X':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'x':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'Y':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'y':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'Z':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case 'z':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '0':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '1':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '2':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '3':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '4':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '5':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '6':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '7':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '8':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	case '9':
		{
			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
			break;
		}
	}

			char Point[2][5];
			char x = 0;
			char y = 1;
			char i = 0;
			// Point 0
			Point[x][i] = 2;
			Point[y][i] = 0;
			i++;
			// Point 1
			Point[x][i] = 2;
			Point[y][i] = 15;
			i++;
			// Point 2
			Point[x][i] = 12;
			Point[y][i] = 15;
			i++;
			// Point 3
			Point[x][i] = 12;
			Point[y][i] = 31;
			i++;
			// Point 4
			Point[x][i] = 2;
			Point[y][i] = 31;
									
			DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
			DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
			DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
			DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
			DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
}

void D3DGraphics::PutPixel( int x,int y,int r,int g,int b )
{	
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	pSysBuffer[ x + SCREENWIDTH * y ] = D3DCOLOR_XRGB( r,g,b );
}

void D3DGraphics::PutPixel( int x,int y,D3DCOLOR c )
{	
	//assert( x >= 0 );
	//assert( y >= 0 );
	//assert( x < SCREENWIDTH );
	//assert( y < SCREENHEIGHT );
	pSysBuffer[ x + SCREENWIDTH * y] = c;
}

D3DCOLOR D3DGraphics::GetPixel( int x,int y )
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < SCREENWIDTH );
	assert( y < SCREENHEIGHT );
	return pSysBuffer[ x + SCREENWIDTH * y ];
}

void D3DGraphics::BeginFrame()
{
	memset( pSysBuffer,FILLVALUE,sizeof( D3DCOLOR ) * SCREENWIDTH * SCREENHEIGHT );
}

void D3DGraphics::EndFrame()
{
	HRESULT result;

	result = pBackBuffer->LockRect( &backRect,NULL,NULL );
	assert( !FAILED( result ) );

	for( int y = 0; y < SCREENHEIGHT; y++ )
	{
		memcpy( &((BYTE*)backRect.pBits)[ backRect.Pitch * y ],&pSysBuffer[ SCREENWIDTH * y ],sizeof( D3DCOLOR ) * SCREENWIDTH );
	}

	result = pBackBuffer->UnlockRect();
	assert( !FAILED( result ) );

	result = pDevice->Present( NULL,NULL,NULL,NULL );
	assert( !FAILED( result ) );
}

void D3DGraphics::DrawDisc( int cx,int cy,int r,int rd,int g,int b )
{
	for( int x = cx - r; x < cx + r; x++ )
	{
		for( int y = cy - r; y < cy + r; y++ )
		{
			int dist = sqrt( (float)( (x - cx)*(x - cx) + (y - cy)*(y - cy) ) );
			
			if( dist < r )
			{
				PutPixel( x,y,rd,g,b );
			}
		}
	}
}

void D3DGraphics::DrawLine( int x1,int y1,int x2,int y2,int r,int g,int blu )
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( x1,y1,r,g,blu );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dx / (float)dy;
		float b = x1 - m*y1;
		for( int y = y1; y <= y2; y = y + 1 )
		{
			int x = (int)(m*y + b + 0.5f);
			PutPixel( x,y,r,g,blu );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dy / (float)dx;
		float b = y1 - m*x1;
		for( int x = x1; x <= x2; x = x + 1 )
		{
			int y = (int)(m*x + b + 0.5f);
			PutPixel( x,y,r,g,blu );
		}
	}
}

void D3DGraphics::DrawLine( int x1,int y1,int x2,int y2,D3DCOLOR c )
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( x1,y1,c );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dx / (float)dy;
		float b = x1 - m*y1;
		for( int y = y1; y <= y2; y = y + 1 )
		{
			int x = (int)(m*y + b + 0.5f);
			PutPixel( x,y,c );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dy / (float)dx;
		float b = y1 - m*x1;
		for( int x = x1; x <= x2; x = x + 1 )
		{
			int y = (int)(m*x + b + 0.5f);
			PutPixel( x,y,c );
		}
	}
}

void D3DGraphics::DrawCircle( int centerX,int centerY,int radius,int r,int g,int b )
{
	int rSquared = radius*radius;
	int xPivot = (int)(radius * 0.707107f + 0.5f);
	for( int x = 0; x <= xPivot; x++ )
	{
		int y = (int)(sqrt( (float)( rSquared - x*x ) ) + 0.5f);
		PutPixel( centerX + x,centerY + y,r,g,b );
		PutPixel( centerX - x,centerY + y,r,g,b );
		PutPixel( centerX + x,centerY - y,r,g,b );
		PutPixel( centerX - x,centerY - y,r,g,b );
		PutPixel( centerX + y,centerY + x,r,g,b );
		PutPixel( centerX - y,centerY + x,r,g,b );
		PutPixel( centerX + y,centerY - x,r,g,b );
		PutPixel( centerX - y,centerY - x,r,g,b );
	}
}

void D3DGraphics::DrawSprite( int xoff,int yoff,Sprite* sprite )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			D3DCOLOR c = sprite->surface[ x + y * sprite->width ];
			if( c != sprite->key )
			{
				PutPixel( x + xoff,y + yoff,c );
			}
		}
	}
}

void D3DGraphics::DrawSpriteAlpha( int xoff,int yoff,Sprite* sprite )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			// load source and destination pixels
			const D3DCOLOR src = sprite->surface[ x + y * sprite->width ];
			const D3DCOLOR dst = GetPixel( x + xoff,y + yoff );

			// extract channels
			const unsigned char srcAlpha =	(const unsigned char)((src & 0xFF000000) >> 24);
			const unsigned char srcRed =	(const unsigned char)((src & 0x00FF0000) >> 16);
			const unsigned char srcGreen =	(const unsigned char)((src & 0x0000FF00) >>  8);
			const unsigned char srcBlue =	(const unsigned char)((src & 0x000000FF));
			const unsigned char dstRed =	(const unsigned char)((dst & 0x00FF0000) >> 16);
			const unsigned char dstGreen =	(const unsigned char)((dst & 0x0000FF00) >>  8);
			const unsigned char dstBlue =	(const unsigned char)((dst & 0x000000FF));

			// blend channels
			const unsigned char rltRed = (srcRed * srcAlpha + dstRed * (255 - srcAlpha)) / 255;
			const unsigned char rltGreen = (srcGreen * srcAlpha + dstGreen * (255 - srcAlpha)) / 255;
			const unsigned char rltBlue = (srcBlue * srcAlpha + dstBlue * (255 - srcAlpha)) / 255;

			// pack channels back into pixel and fire pixel onto backbuffer
			PutPixel( x + xoff,y + yoff,D3DCOLOR_XRGB( rltRed,rltGreen,rltBlue ) );
		}
	}
}

void D3DGraphics::DrawChar( char c,int xoff,int yoff,Font* font,D3DCOLOR color )
{
	if( c < ' ' || c > '~' )
		return;

	const int sheetIndex = c - ' ';
	const int sheetCol = sheetIndex % font->nCharsPerRow;
	const int sheetRow = sheetIndex / font->nCharsPerRow;
	const int xStart = sheetCol * font->charWidth;
	const int yStart = sheetRow * font->charHeight;
	const int xEnd = xStart + font->charWidth;
	const int yEnd = yStart + font->charHeight;
	const int surfWidth = font->charWidth * font->nCharsPerRow;

	for( int y = yStart; y < yEnd; y++ )
	{
		for( int x = xStart; x < xEnd; x++ )
		{
			if( font->surface[ x + y * surfWidth ] == D3DCOLOR_XRGB( 0,0,0 ) )
			{
				PutPixel( x + xoff - xStart,y + yoff - yStart,color );
			}
		}
	}
}

void D3DGraphics::DrawString( const char* string,int xoff,int yoff,Font* font,D3DCOLOR color )
{
	for( int index = 0; string[ index ] != '\0'; index++ )
	{
		DrawChar( string[ index ],xoff + index * font->charWidth,yoff,font,color );
	}
}

void D3DGraphics::DrawShield(unsigned int ShipX, unsigned int ShipY, 
							 unsigned int shieldRadius, float shieldStrength )
{
	
	if(shieldStrength > 1.0f)
	{
		shieldStrength = 1.0f;
	}
	int rd = (255 - (255 * shieldStrength)) / 4;
	int g = (255 * shieldStrength) / 4;
	int b = 0;
	int nRad = -shieldRadius;
	int pRad = shieldRadius;

	for( int x = nRad; x < pRad; x++ )
	{
		for( int y = nRad; y < pRad; y++ )
		{

			int dist = sqrt( (float)( (x)*(x) + (y)*(y) ) );
			
			if( dist < shieldRadius )
			{
				float shade = (float)dist / shieldRadius;
				int shieldX = x + ShipX;
				int shieldY = y + ShipY;
				if((shieldX >= 0 && shieldX < SCREENWIDTH) && (shieldY >= 0 && shieldY < SCREENHEIGHT))
				{
					// src1 is the color of the pixel in the System Memory buffer
					// src2 is the color of the pixel of the transparent object
					D3DCOLOR src1 = GetPixel(shieldX, shieldY);
					D3DCOLOR src2 = D3DCOLOR_XRGB(rd, g, b);
					D3DCOLOR result = Blend(src1, src2, shade);
					PutPixel( shieldX, shieldY, result );
				}
			}
		}
	}
}

D3DCOLOR D3DGraphics::Blend(D3DCOLOR source1, D3DCOLOR source2, float opacity)
{	
	const unsigned char srcRed1 =	(const unsigned char)((source1 & 0x00FF0000) >> 16);
	const unsigned char srcGreen1 =	(const unsigned char)((source1 & 0x0000FF00) >>  8);
	const unsigned char srcBlue1 =	(const unsigned char)((source1 & 0x000000FF));
	const unsigned char srcRed2 =	(const unsigned char)((source2 & 0x00FF0000) >> 16);
	const unsigned char srcGreen2 =	(const unsigned char)((source2 & 0x0000FF00) >>  8);
	const unsigned char srcBlue2 =	(const unsigned char)((source2 & 0x000000FF));
	const unsigned char rltRed = (srcRed1 * (255 - (255 * opacity)) + srcRed2 * (255 * opacity)) / 255;
	const unsigned char rltGreen = (srcGreen1 * (255 - (255 * opacity)) + srcGreen2 * (255 * opacity)) / 255;
	const unsigned char rltBlue = (srcBlue1 * (255 - (255 * opacity)) + srcBlue2 * (255 * opacity)) / 255;	
	return D3DCOLOR_XRGB(rltRed, rltGreen, rltBlue);
}

void D3DGraphics::DrawShip( unsigned int ShipX, unsigned int ShipY, int vx, int vy, int vx2, int vy2, D3DCOLOR c)
{
	DrawLine(ShipX + vx, ShipY + vy, ShipX + vx2, ShipY + vy2, c);
}