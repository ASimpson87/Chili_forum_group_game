#pragma once
#include <fstream>

class Object
{
	Object();
	~Object();

	void LoadOBJ(FILE* pFile);
	int strBuff[255];
	char fMatfile[255];
	char objName[255];
	char group[255];
	char material[255];
	float v;
	float vt;
	float vn;
};
