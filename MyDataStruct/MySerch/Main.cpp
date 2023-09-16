#include"MySerch.h"
#include <ctime>
#include <stdlib.h>
#include <iostream>
using namespace std;


int main(void)
{
	srand(time(NULL));
	int s[11] = { 0 };
	MakeIncreaseList(s, 0, 10, 1);
	ShowList(s, 0, 10);

	int r = BinSerch(s, 0, 10, 33);
	cout << endl;
	cout << r << endl;
	//
}