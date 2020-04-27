//使用规范
//B_list* list = B_listCreat(sizeof(T));
//T为所要存储的数据类型
//只能以上面这种方式创建
//下面的创建方式会出现问题
//B_list list;
#pragma once
#include<stdlib.h>
#include<string.h>
typedef  int Rank;
//下面这两行只是typedef的时候结构体面能用定义的名称定义结构体内的变量的临时解决办法
//不影响使用，但就是感觉有点丑
struct B_tlistNode;
typedef struct B_tlistNode B_listNode;
struct B_tlistNode{//B_vector的基本数据
	B_listNode* pred;
	B_listNode* succ;
	char* _elem;
};
typedef struct {//B_vector的基本数据
	size_t _esize;
	size_t _size;
	B_listNode* header ;
	B_listNode* trailer;
} B_list;
B_list* B_listCreat(size_t esize);//创建B_list，需指定元素内存大小
void B_listInit(B_list* list);//初始化B_list
void B_listClear(B_list* list);//清空列表
void B_listPushBack(B_list* list, const void* ve);//插入元素到列表尾部
void B_listPushFirst(B_list* list, const void* ve);//插入元素到列表头部
void B_listRemoveFirst(B_list* list);//删除首元素
void B_listGetFirst(B_list* list);//获取首元素
void B_listGetBack(B_list* list);//获取尾元素
B_listNode* B_listGetNodeRank(B_list* list, Rank r);//得到指定位置的元素对应的节点，速度较慢
char* B_listGetRank(B_list* list, Rank r);////得到指定位置的元素，速度较慢
void B_listRemoveInterval(B_list* list, Rank lo, Rank hi);//删除[lo,hi)的元素
void B_listRemoveRank(B_list* list, Rank r);//删除指定位置的元素
