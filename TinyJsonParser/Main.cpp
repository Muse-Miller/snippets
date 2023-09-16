
#include <iostream>
#include <memory>
#include<string>
#include"JsonParse.h"

using namespace std;
using namespace self_json;

int main(int argc, const char* argv[]) 
{
    const char* src = R"#(

{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cppbuild",
			"label": "C/C++: g++.exe 生成活动文件",
			"command": "C:\\Users\\muse\\w64devkit-1.19.0\\bin\\g++.exe",
			"args": [
				"-fdiagnostics-color=always",
				"-g",
				"${file}",
				"-o",
				"${fileDirname}\\${fileBasenameNoExtension}.exe"
			],
			"options": {
				"cwd": "${fileDirname}"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": {
				"kind": "build",
				"isDefault": true
			},
			"detail": "编译器: C:\\Users\\muse\\w64devkit-1.19.0\\bin\\g++.exe"
		}
	]
}




)#";
    JsonParser jr;

    jr.Test(src);
    return 0;
}


#if 0
class Test {
public:
    // 无参构造函数
    Test();
    // 有参数的构造函数
    explicit Test(int a);
    // 析构函数
    ~Test();
};

Test::Test() {
    std::cout << "无参构造函数" << std::endl;
}

Test::Test(int a) {
    std::cout << "有参构造函数，a=" << a << std::endl;
}

Test::~Test() {
    std::cout << "析构函数" << std::endl;
}
int main(int argc, const char* argv[]) {
    auto p1 = new Test; // 划分堆空间
    std::shared_ptr<Test> sp(p1); // 创建智能指针
    std::cout << sp.use_count() << std::endl; // 打印引用计数
    {
        std::shared_ptr<Test> sp2(sp); // 创建另一个智能指针
        std::cout << sp.use_count() << std::endl; // 打印引用计数
    } // sp2生命周期结束，sp引用计数减1
    std::cout << sp.use_count() << std::endl; // 打印引用计数

    return 0;


}
#endif