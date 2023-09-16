#ifndef __MY_KMP__
#define __MY_KMP__

using namespace std;



int MyKmp(const char* str, const char* str2, const int strlen, int strlen2, const int* next);

void MakeNext(const char* str, int* next, const int len);


void MakeBetterNext(const char* str, int* next, const int len);

void MakeStr(char* str, const int len);


void ShowStr(const char* str, const int len);
void ShowNext(const int* next, const int len);


#endif
