#include<iostream>
#include<vector>
#include"QuickSort.h"

using namespace std;
using namespace my_tiny_algorithm;

class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        vector<int> ret;
        int max = -100010;
        int min = 100010;
        int* rec = new int[100010] { 0 };
        cout << "1" << endl;
        for (vector<int>::iterator it = nums.begin(); it != nums.end(); ++it)
        {
            max < *it ? max = *it : max = max;
            min > * it ? min = *it : min = min;
        }
        cout << "2" << endl;
        for (vector<int>::iterator it=nums.begin(); it != nums.end(); ++it)
        {
            cout << *it << endl;
            rec[*it - min]++;
        }
        nums.clear();
        cout << "3" << endl;
        for (int i = 0; i < 100010; ++i)
        {
            while (rec[i] > 0)
            {
                nums.push_back(i + min);
                rec[i]--;
            }
        }
        return ret;
    }
};

int main(int args, char* argv)
{
    vector<int> nums{2, 5, 1, 7, 2, 9, 3,3,2,1,5,4,99,55,3,22,11,12,21,32,33,12};
    vector<int> nums2{1};
    //Test(nums2);
    Test(nums);
    return 0;
}