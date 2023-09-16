#include"Kmp.h"

#include<cstdlib>
#include<ctime>
#include<iostream>
using namespace std;
#define MAXLEN 50
#define MINLEN 4


int main(void)
{
	srand(time(NULL));

	char str[MAXLEN] = { NULL };
	char str2[MINLEN] = { NULL };
	int next[MINLEN] = { 0 };

	MakeStr((char*)str, MAXLEN);
	MakeStr((char*)str2, MINLEN);
	ShowStr(str, MAXLEN);
	ShowStr(str2, MINLEN);

	MakeNext(str2, (int*)next, MINLEN);
	ShowNext(next, MINLEN);
	int r = MyKmp(str, str2, MAXLEN, MINLEN, next);
	cout << r << endl;
	//MakeBetterNext(str2, (int*)next, MINLEN);
	//ShowNext(next, MINLEN);
	return 0;
}