#define _CRT_SECURE_MO_WARNINGS
#define SR_BTNODE_NAME_LEN 50
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

#include"C_goods.h"
#include"B_list.h"
#include"B_vector.h"
#include"B_getSrc.h"

typedef struct SR_dataBTNodeT  SR_dataBTNode;
struct SR_dataBTNodeT {//B-���ڵ�
	SR_dataBTNode* parent;
	char name[SR_BTNODE_NAME_LEN];
	char sort;
	int childNum;//�ӽڵ������
	B_vector* child;
	B_vector* childCode;
	char fileName[10];
	B_vector* goodsVector;
};
SR_dataBTNode SR_dataBTRoot;//���ڵ�
void SR_dataBTInit();//��ʼ��B-��
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file);//�����ļ�����B-��,�������
