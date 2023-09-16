
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
			"label": "C/C++: g++.exe ���ɻ�ļ�",
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
			"detail": "������: C:\\Users\\muse\\w64devkit-1.19.0\\bin\\g++.exe"
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
    // �޲ι��캯��
    Test();
    // �в����Ĺ��캯��
    explicit Test(int a);
    // ��������
    ~Test();
};

Test::Test() {
    std::cout << "�޲ι��캯��" << std::endl;
}

Test::Test(int a) {
    std::cout << "�вι��캯����a=" << a << std::endl;
}

Test::~Test() {
    std::cout << "��������" << std::endl;
}
int main(int argc, const char* argv[]) {
    auto p1 = new Test; // ���ֶѿռ�
    std::shared_ptr<Test> sp(p1); // ��������ָ��
    std::cout << sp.use_count() << std::endl; // ��ӡ���ü���
    {
        std::shared_ptr<Test> sp2(sp); // ������һ������ָ��
        std::cout << sp.use_count() << std::endl; // ��ӡ���ü���
    } // sp2�������ڽ�����sp���ü�����1
    std::cout << sp.use_count() << std::endl; // ��ӡ���ü���

    return 0;


}
#endif