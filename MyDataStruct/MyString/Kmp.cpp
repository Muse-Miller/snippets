#include "Kmp.h"

#include<iostream>
#include<string>
#include<iomanip>

using namespace std;
int MyKmp(const char* str, const char* str2, const int strlen, int strlen2, const int* next)
{
	int i = 0;
	int j = 0;
	int counts = 0;
	while (strlen > i && strlen2 > j)
	{
		++counts;
		if (str[i] == str2[j] || -1 == j)
		{
			++i; ++j;
		}
		else
		{
			j = next[j];
		}
	}

	cout << "counts:\t" << counts << endl;
	if (strlen == i && strlen2 == j)return i - j;
	else if (strlen == i && strlen2 > j)return -1;
	else return i - j;
}
void MakeNext(const char* str, int* next, const int len)
{
	next[0] = -1;
	next[1] = 0;

	int j = 1;
	int k = -1;
	while (len > j + 1)
	{
		if((-1 == k)||(str[k] == str[j]))
		{
			++k; ++j;
			next[j] = k;
		}
		else
		{
			k = next[k];
		}
	}
}
void MakeBetterNext(const char* str, int* next, const int len)
{
	next[0] = -1;
	next[1] = 0;

	int j = 1;
	int k = -1;
	while (len > j + 1)
	{
		if ((-1 == k) || (str[k] == str[j]))
		{
			++k; ++j;
			if (str[k] == str[j])
			{
				next[j] = next[k];
				continue;
			}
			next[j] = k;
		}
		else
		{
			k = next[k];
		}
	}
}

void MakeStr(char* str, const int len)
{
	for (int i = 0; len > i; ++i)
	{
		str[i] = 'a' + rand() % 3;
	}
}

void ShowStr(const char* str, const int len)
{
	for (int i = 0; len > i; ++i)
	{
		cout << setw(4) << setfill(' ') << i;
	}
	cout << endl;
	for (int i = 0; len > i; ++i)
	{
		cout << setw(4) << setfill(' ') << str[i];
	}
	cout << endl;
}

void ShowNext(const int* next, const int len)
{
	for (int i = 0; len > i; ++i)
	{
		cout << setw(4) << setfill(' ') << next[i];
	}
	cout << endl;
}