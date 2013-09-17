#pragma once
#include "D3DGraphics.h"
#include <string>
#include <gdiplus.h>
#include "Game.h"
#pragma comment( lib,"gdiplus.lib" )

class Surface
{
public:
	Surface()
	{
		
	};

	~Surface()
	{
		
	};
	void LoadSurface(std::wstring &filename)
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;
		Gdiplus::GdiplusStartup( &gdiplusToken,&gdiplusStartupInput,NULL );

		Gdiplus::Bitmap bitmap( filename.c_str() );
		Gdiplus::Color pixel;

		height = bitmap.GetHeight();
		width = bitmap.GetWidth();
		surface = new D3DCOLOR [height * width ];

		for( unsigned int y = 0; y < bitmap.GetHeight(); y++ )
		{
			for( unsigned int x = 0; x < bitmap.GetWidth(); x++ )
			{
				bitmap.GetPixel( x,y,&pixel );
				surface[ x + y * bitmap.GetWidth() ] = 
					D3DCOLOR_ARGB( pixel.GetA(),pixel.GetR(),pixel.GetG(),pixel.GetB() );
			}
		}
	}
	
	void delSurface()
	{
		delete [] surface;
	}

	void Draw(int xoff, int yoff,D3DGraphics& gfx)
	{
		const  int yStart = max(-yoff,0);
		const  int xStart = max(-xoff,0);
		const  int yEnd = min(SCREENHEIGHT - yoff,(int) height + 1 );
		const  int xEnd = min(SCREENWIDTH - xoff, (int) width - 1 );

		for(  int y = yStart; y < yEnd; y++ )
		{
			for(  int x = xStart; x < xEnd; x++ )
			{
				gfx.PutPixel( x + xoff,y + yoff,surface[ x + y * width ] );
			}
		}
	}
	void DrawKeyed(int xoff, int yoff,  D3DCOLOR key,D3DGraphics& gfx)
	{
		const  int yStart = max(-yoff,0);
		const  int xStart = max(-xoff,0);
		const  int yEnd = min(SCREENHEIGHT - yoff, (int)height );
		const  int xEnd = min(SCREENWIDTH - xoff, (int)width );

		for(  int y = yStart; y < yEnd; y++ )
		{
			for(  int x = xStart; x < xEnd; x++ )
			{
				D3DCOLOR c = surface[ x + y * width ];
				if (c != key )
				{
					gfx.PutPixel( x + xoff,y + yoff,c);
				}
			}
		}
	}
	//void DrawAniKeyed
	/*void aniKeyed ( Game::Action, Game::Direction direction, int frame, int x, int y )
	{
		
	}*/
	
private:
	unsigned int width;
	unsigned int height;
	D3DCOLOR* surface;
};