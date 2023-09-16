#include<iostream>
#include <stack>
using namespace std;

void PrintStrReverse(const char* aim)
{
	if (aim == nullptr)return;
	if (*aim == '\0')return;
	PrintStrReverse(aim + 1);
	cout << *aim << endl;
}

class TreeNode
{
public:
    TreeNode(int val):val(val){}
    int val;
    TreeNode* left;
    TreeNode* right; 
};

void printStack(stack<TreeNode*>& src)
{
    while (!src.empty())
    {
        cout << src.top()->val << endl;
        src.pop();
    }
}
void testFunc(TreeNode* root, TreeNode* p,TreeNode* q)
{
    stack<TreeNode*> rec;
    TreeNode* pre{nullptr};
    while ((root != nullptr) || (!rec.empty()))
    {
        if (root != nullptr)
        {
            rec.push(root);
            //if (root == p || root == q)break;
            root = root->left;
        }
        else if (rec.top()->right == nullptr)
        {//右子树为空
            pre = rec.top();
            cout << rec.top()->val << endl;
            rec.pop();

            //root = rec.top()->right;
        }
        else if (pre == rec.top()->right)
        {//右子树搜索完成则弹出栈顶
            pre = rec.top();
            cout << rec.top()->val << endl;
            rec.pop();
            //root = rec.top()->right;
        }
        else
        {
            cout << "else" << endl;
            root = rec.top()->right;
        }
    }
}


/* {
            if (rec.top()->right == nullptr)
            {//右子树为空
                pre = rec.top();
                cout << rec.top()->val << endl;
                rec.pop();

                //root = rec.top()->right;
            }
            else if (pre == rec.top()->right)
            {//右子树搜索完成则弹出栈顶
                pre = rec.top();
                cout << rec.top()->val << endl;
                rec.pop();
                //root = rec.top()->right;
            }
            else
            {
                root = rec.top()->right;
            }


}*/

int main()
{
    TreeNode n0(0);
    TreeNode n1(1);
    TreeNode n2(2);
    TreeNode n3(3);
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n6(6);
    TreeNode n7(7);
    TreeNode n8(8);

    n3.left = &n5;
    n3.right = &n1;
    n5.left = &n6;
    n5.right = &n2;
    n2.left = &n7;
    n2.right = &n4;
    n1.left = &n0;
    n1.right = &n8;

    testFunc(&n3, &n7, &n0);
	return 0;
}



