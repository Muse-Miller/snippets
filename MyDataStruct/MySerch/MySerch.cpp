#include "MySerch.h"
#include<random>
#include <iostream>
#include <iomanip>
using namespace std;

void MakeIncreaseList(int s[], int bgn, int end, int min)
{
	int i = bgn + 1;
	s[bgn] = min;
	while (i <= end)
	{
		s[i] = rand() % 10 + s[i - 1];
		++i;
	}
}
void ShowList(int s[], int bgn, int end)
{
	int i = bgn;
	while (i<=end)
	{
		cout << setw(3) << s[i];
		++i;
	}
	cout << endl;
}
int BinSerch(int s[], int bgn, int end, int aim)
{
	int i = bgn;
	int j = end;

	int mid = 0;
	while (i <= j)
	{
		mid = i + (j - i) / 2;
		if (aim == s[mid])
		{
			break;
		}
		else if (aim < s[mid])
		{
			j = mid - 1;
		}
		else
		{
			i = mid + 1;
		}
	}
	if (i <= j)return mid;
	else return -1;
}
