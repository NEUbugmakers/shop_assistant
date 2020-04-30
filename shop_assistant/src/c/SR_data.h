#define _CRT_SECURE_MO_WARNINGS
#define SR_BTNODE_NAME_LEN 50
#include<stdio.h>
#include<stdlib.h>

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
SR_dataBTNode SR_dataBTRoot;
void SR_dataBTInit() {//��ʼ��B-��
	FILE* file = fopen("root", "rb+");
	B_getSrc(file, &SR_dataBTRoot, sizeof(SR_dataBTNode));
	SR_dataBTBuild(&SR_dataBTRoot, file);
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file) {//�����ļ�����B-��(δ��ɣ�
	if (x->childNum > 0) {//�����ڲ��ڵ�
		x->child = B_vectorCreat(sizeof(SR_dataBTNode*));
		x->childCode = B_vectorCreat(sizeof(SR_dataBTNode*));
		x->goodsVector = NULL;
		SR_dataBTNode t;//��ʱ����������vector����
		for (int i = 0; i < x->childNum; i++) {
			vect===
		}
	}
	else {//�����ⲿ�ڵ�
		x->child = NULL;
		x->childCode = NULL;
		FILE* _file;
		_file = fopen(x->fileName, "rb+");
		fseek(_file, 0, SEEK_END);
		int flen = ftell(_file);
		fseek(_file, 0, SEEK_SET);
		x->goodsVector = B_vectorCreat(sizeof(C_Goods));
		x->goodsVector->_capicity = flen / sizeof(C_Goods);
		free(x->goodsVector->_elem);
		x->goodsVector->_elem = (char*)malloc(flen);
		B_getSrc(_file, x->goodsVector->_elem, flen);
		x->goodsVector->_size = flen / sizeof(C_Goods);
	}
}
