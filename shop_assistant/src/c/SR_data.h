#pragma once
#define _CRT_SECURE_MO_WARNINGS
#define SR_BTNODE_NAME_LEN 50
#define SR_DATA_FILENAME_LEN 10
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<io.h>

#include"C_goods.h"
#include"B_list.h"
#include"B_vector.h"
#include"C_goodsvector.h"
enum SR_PrintState{Default,SubSortSucc,EndOfSort};
typedef struct SR_dataBTNodeT  SR_dataBTNode;
struct SR_dataBTNodeT {//B-树节点
	SR_dataBTNode* parent;
	char name[SR_BTNODE_NAME_LEN];
	char sort;//分类标识符
	int childNum;//子节点的数量
	B_vector* child;
	B_vector* childCode;
	int SR_dataBTNodeGoodsNum;//当前分类直接包含的商品个数,不必时时维护，仅在打开文件和保存文件的时候维护即可
	C_goodsVector* goodsVector;
};
typedef struct {
	float C_out;//标价（非实售价）
	char name[C_NAME_LEN];//商品名
	char code[C_CODE_LEN];//编码
	int C_shelfNum;//货架上的现存的商品总批次,不必时时维护，仅在打开文件和保存文件的时候维护即可
	int C_stockNum;//库存中现存的商品总批次,不必时时维护，仅在打开文件和保存文件的时候维护即可
	enum SR_PrintState C_State;//当前条目的状态
	int SR_sortName;
}SR_dataPrint;
typedef struct {
	char sort[20];//分类
	char code[C_CODE_LEN];//数字部分编码
}SR_dataCodeNode;

B_vector* SR_dataCodeVector;
SR_dataBTNode SR_dataBTRoot;//根节点
void SR_dataBTInit();//初始化B-树
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file,FILE* goodsFile);//根据文件建立B-树,先序遍历
SR_dataBTNode* SR_dataGetSortFromChild(SR_dataBTNode* pos, char sort);//在当前节点的子节点查找分类,失败返回NULL
SR_dataBTNode* SR_dataGetSortFromNode(SR_dataBTNode* pos, char sort[]);//从某一节点查找分类(递归)
SR_dataBTNode* SR_dataGetSort(char sort[]);//从根节点开始查找分类
C_Goods* SR_dataGet(char code[]);//根据编码查找商品
void SR_dataBTNodeInit(SR_dataBTNode* node);//对创建的节点初始化
char SR_dataSelectSort(SR_dataBTNode* pos);//在当前节点挑选新增分类的分类标识符
void SR_dataInsertSort(char dir[], char name[]);//插入分类
void SR_dataSaveGoodsInfo(B_list* info, FILE* file);//保存shelfInfo和stockInfo
void SR_dataSavePreOrder(SR_dataBTNode* x, FILE* root,FILE* goodsFile);//先序遍历保存节点（未完成）
void SR_dataSave();//保存数据
void SR_dataAddNewGoods(C_Goods* goods);//添加新商品
void SR_dataReplenishGoods(char code[], C_goodsInfo* info);//补货
C_goodsReturnPrice SR_dataSell(char code[], char batch, int amount);//出售商品,返回商品标价
void SR_dataCodeVectorBuildFrom(SR_dataBTNode* pos, B_vector* vector);//建立某一分类下的商品数字编码查找向量(无序）
void SR_dataCodeNodeCmp(SR_dataCodeNode* node1, SR_dataCodeNode* node2);//通过数字编码比较codeNode大小
void SR_dataCodeVectorBuild();//建立数字编码查找向量
C_Goods* SR_dataCodeFind(char code[]);//通过数字编码查找商品