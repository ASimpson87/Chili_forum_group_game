#include "OBJ_Import.h"
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


Object::Object ()
	:
	v(0),
	vt(0),
	vn(0)
{
	for (int index = 0; index < 255; index++)
	{
		fMatfile[index] = 0;
		objName[index] = 0;
		group[index] = 0;
		material[index] = 0;
		strBuff[index] = 0;

	}
	
}
Object::~Object()
{}
void Object::LoadOBJ (FILE* pFile)
{
//	FILE* pFile;

	pFile = fopen ("cube.obj", "r");
	char index;
	do
	{
		
		for (index = 0; index < 255; index++)
		{
			strBuff[index] = getc(pFile);
		}
		
	}while (strBuff[index] != EOF);
}