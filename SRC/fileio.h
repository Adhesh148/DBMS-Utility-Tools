#include "pch.h"
#include <fstream>

void fileRead(int& n,int& m,vector<struct FD> &S)
{
	ifstream fin;

	fin.open("../input.txt");

	int i=0;
	fin>>n>>m;

	while(fin>>S[i].a>>S[i++].b);

	fin.close();
}