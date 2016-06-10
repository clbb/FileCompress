#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include"HuffmanTree.hpp"
#include"FileCompress.hpp"
using namespace std;

int main()
{
	/*int a[6] = {1,2,2,3,3,3};*/
	int start1 = GetTickCount();
	FileCompress Fc;
	Fc.Compress("test.txt");
	int end1 = GetTickCount();
	cout << end1 - start1 << endl;
	//HuffmanTree<int,NodeCompare<int>> Hu;
	//Hu.Create(a,6);
	int start2 = GetTickCount();
	Fc.UnCompress("test.txt");
	int end2 = GetTickCount();
	cout << end2 - start2<<endl;

	system("pause");
	return 0;
}
