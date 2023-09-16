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

	
	//����󶥶ѣ���С��������

	//����ĳ��λ�õ�Ԫ�أ�ʹ֮��Ϊ�󶥶�
	//src �����������飬end ���һ��Ԫ�ص��±꣬ sit ��������λ��
	void _Adjust(std::vector<int>& src, int end, int sit)
	{
		int lson = sit * 2 + 1;
		int rson = sit * 2 + 2;
		
		int max = sit;
		int tmp = 0;
		while (lson <= end || rson <= end)
		{
			//�����������������
			// 1.�ȱȽ������������õ����ߵ����ֵ��λ��
			// 2.�ڱȽ����������ֵ��ȷ�����Ԫ�ص�λ��max
			if (lson <= end && rson <= end)
			{
				max = src[lson] > src[rson] ? lson : rson;
				max = src[sit] > src[max] ? sit : max;
			}
			//���ֻ����������ֻ�Ƚ�һ�μ���
			else if (lson <= end)
			{
				max = src[lson] > src[sit] ? lson : sit;
			}
			//����������������������µ���
			if (sit == max)break;
			//���򣬽������������Ԫ��
			tmp = src[sit];
			src[sit] = src[max];
			src[max] = tmp;
			//����sit����Ϊ�±�max
			// 1.�趨��������
			// 2.����������maxΪ������
			sit = max;
			lson = sit * 2 + 1;
			rson = sit * 2 + 2;
		}
	}
	//����󶥶�
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
	
	//��������Ԫ��
	void _SwapElem(std::vector<int>& src, int first, int second)
	{
		int tmp = src[first];
		src[first] = src[second];
		src[second] = tmp;
	}
	//������
	void HeapSort(std::vector<int>& src)
	{
		//�ѵ����һ��Ԫ��
		int end = src.size() - 1;
		//�����
		_MakeBigHeap(src);
		_CoutVector(src);
		while (end>=0)
		{
			//����Ԫ��
			_SwapElem(src, 0, end);
			//�����Ѷ�
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