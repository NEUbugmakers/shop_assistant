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
struct SR_dataBTNodeT {//B-���ڵ�
	SR_dataBTNode* parent;
	char name[SR_BTNODE_NAME_LEN];
	char sort;//�����ʶ��
	int childNum;//�ӽڵ������
	B_vector* child;
	B_vector* childCode;
	int SR_dataBTNodeGoodsNum;//��ǰ����ֱ�Ӱ�������Ʒ����,����ʱʱά�������ڴ��ļ��ͱ����ļ���ʱ��ά������
	C_goodsVector* goodsVector;
};
typedef struct {
	float C_out;//��ۣ���ʵ�ۼۣ�
	char name[C_NAME_LEN];//��Ʒ��
	char code[C_CODE_LEN];//����
	int C_shelfNum;//�����ϵ��ִ����Ʒ������,����ʱʱά�������ڴ��ļ��ͱ����ļ���ʱ��ά������
	int C_stockNum;//������ִ����Ʒ������,����ʱʱά�������ڴ��ļ��ͱ����ļ���ʱ��ά������
	enum SR_PrintState C_State;//��ǰ��Ŀ��״̬
	int SR_sortName;
}SR_dataPrint;
typedef struct {
	char sort[20];//����
	char code[C_CODE_LEN];//���ֲ��ֱ���
}SR_dataCodeNode;

B_vector* SR_dataCodeVector;
SR_dataBTNode SR_dataBTRoot;//���ڵ�
void SR_dataBTInit();//��ʼ��B-��
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file,FILE* goodsFile);//�����ļ�����B-��,�������
SR_dataBTNode* SR_dataGetSortFromChild(SR_dataBTNode* pos, char sort);//�ڵ�ǰ�ڵ���ӽڵ���ҷ���,ʧ�ܷ���NULL
SR_dataBTNode* SR_dataGetSortFromNode(SR_dataBTNode* pos, char sort[]);//��ĳһ�ڵ���ҷ���(�ݹ�)
SR_dataBTNode* SR_dataGetSort(char sort[]);//�Ӹ��ڵ㿪ʼ���ҷ���
C_Goods* SR_dataGet(char code[]);//���ݱ��������Ʒ
void SR_dataBTNodeInit(SR_dataBTNode* node);//�Դ����Ľڵ��ʼ��
char SR_dataSelectSort(SR_dataBTNode* pos);//�ڵ�ǰ�ڵ���ѡ��������ķ����ʶ��
void SR_dataInsertSort(char dir[], char name[]);//�������
void SR_dataSaveGoodsInfo(B_list* info, FILE* file);//����shelfInfo��stockInfo
void SR_dataSavePreOrder(SR_dataBTNode* x, FILE* root,FILE* goodsFile);//�����������ڵ㣨δ��ɣ�
void SR_dataSave();//��������
void SR_dataAddNewGoods(C_Goods* goods);//�������Ʒ
void SR_dataReplenishGoods(char code[], C_goodsInfo* info);//����
C_goodsReturnPrice SR_dataSell(char code[], char batch, int amount);//������Ʒ,������Ʒ���
void SR_dataCodeVectorBuildFrom(SR_dataBTNode* pos, B_vector* vector);//����ĳһ�����µ���Ʒ���ֱ����������(����
void SR_dataCodeNodeCmp(SR_dataCodeNode* node1, SR_dataCodeNode* node2);//ͨ�����ֱ���Ƚ�codeNode��С
void SR_dataCodeVectorBuild();//�������ֱ����������
C_Goods* SR_dataCodeFind(char code[]);//ͨ�����ֱ��������Ʒ