#include"MyBinTree.h"
#include <iostream>
#include <iostream>

using namespace std;



#if 1


#define LEN 5
int main(void)
{
	const char* str = "(a(b(d(f),e),c(g,h(,i))))";
	const char* str2 = "(A(B(D(,G)),C(E,F)))";
	const char* str3 = "(a(b(,c(d,e)),f(,g(,h)))";
	const char* str4 = "ab##cd#f##eg###";
	//			   a
	//	   b			  c
	//   #   #	     d        e
	//			   #   f    g   #
	//		          # #  # #
	
	BTNode* bt = CreateBinTree(str);
	//PreVisitBinTree(bt, 0);
	//PreVisitBinTree(bt, 1);
	//MidVisitBinTree(bt, 0);
	//MidVisitBinTree(bt, 1);
	//LastVisitBinTree(bt, 0);
	//LastVisitBinTree(bt,1);
	//LevelVisitBinTree(bt, 1);
	AllInOne(bt);

	return 0;
}

#endif

#if 0
int main(void)
{
	const char str[16] = "Hello C++";
	const char* strp = "Hello C++";

	std::cout << sizeof(str) << endl;
	std::cout << sizeof(strp) << endl;
	return 0;
}

#endif