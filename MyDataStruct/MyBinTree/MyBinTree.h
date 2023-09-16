#pragma once
#ifndef __MY_BIN_TREE__
#define __MY_BIN_TREE__

#include<iostream>

struct BTNode
{
	char data;
	BTNode* lson;
	BTNode* rson;
};

BTNode* CreateBinTree(const char* expression);
BTNode* CreateBinTree2(const char* expression);
BTNode* CreateBinTree3(const char*& expression);



void PreVisitBinTree(BTNode* bt,int flag);

void MidVisitBinTree(BTNode* bt,int flag);

void LastVisitBinTree(BTNode* bt,int flag);

void LevelVisitBinTree(BTNode* bt, int flag);


//三种遍历方式的递归实现
void AllInOne(BTNode* root);


#endif