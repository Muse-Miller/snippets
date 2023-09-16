#pragma once
#ifndef __MY_QUICK_SORT__
#define __MY_QUICK_SORT__

#include<vector>
#include<iostream>



namespace my_tiny_algorithm
{

	void _CoutVector(std::vector<int>& src)
	{
		for (int i = 0; i < src.size(); ++i)
		{
			std::cout << src[i] << " ";
		}
		std::cout << std::endl;
	}

	void quick_sort(std::vector<int>& src);

	
	//构造大顶堆，从小到大排序

	//调整某个位置的元素，使之成为大顶堆
	//src 待调整的数组，end 最后一个元素的下标， sit 待调整的位置
	void _Adjust(std::vector<int>& src, int end, int sit)
	{
		int lson = sit * 2 + 1;
		int rson = sit * 2 + 2;
		
		int max = sit;
		int tmp = 0;
		while (lson <= end || rson <= end)
		{
			//如果左右子树均存在
			// 1.先比较左右子树，得到二者的最大值的位置
			// 2.在比较树根与最大值，确定最大元素的位置max
			if (lson <= end && rson <= end)
			{
				max = src[lson] > src[rson] ? lson : rson;
				max = src[sit] > src[max] ? sit : max;
			}
			//如果只有左子树，只比较一次即可
			else if (lson <= end)
			{
				max = src[lson] > src[sit] ? lson : sit;
			}
			//如果树根最大，则无需继续向下调整
			if (sit == max)break;
			//否则，交换树根与最大元素
			tmp = src[sit];
			src[sit] = src[max];
			src[max] = tmp;
			//并将sit调整为下标max
			// 1.设定左右子树
			// 2.继续调整以max为根的树
			sit = max;
			lson = sit * 2 + 1;
			rson = sit * 2 + 2;
		}
	}
	//建造大顶堆
	void _MakeBigHeap(std::vector<int>& src)
	{
		int begin = (src.size() - 2) / 2;
		while (begin >= 0)
		{
			//std::cout << begin << std::endl;
			_Adjust(src, src.size() - 1, begin);
			--begin;
			//_CoutVector(src);
		}
	}
	
	//交换两个元素
	void _SwapElem(std::vector<int>& src, int first, int second)
	{
		int tmp = src[first];
		src[first] = src[second];
		src[second] = tmp;
	}
	//堆排序
	void HeapSort(std::vector<int>& src)
	{
		//堆的最后一个元素
		int end = src.size() - 1;
		//建造堆
		_MakeBigHeap(src);
		_CoutVector(src);
		while (end>=0)
		{
			//交换元素
			_SwapElem(src, 0, end);
			//调整堆顶
			--end;
			_Adjust(src, end, 0);
		}
	}

	void Test(std::vector<int>& src)
	{
		HeapSort(src);
		_CoutVector(src);
	}



}











#endif	//__MY_QUICK_SORT__