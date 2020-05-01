#include"SR_data.h"
void SR_dataBTInit() {//��ʼ��B-��
	FILE* file = fopen("root", "rb+");
	B_getSrc(file, &SR_dataBTRoot, sizeof(SR_dataBTNode));
	SR_dataBTBuild(&SR_dataBTRoot, file);
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file) {//�����ļ�����B-��,�������
	if (x->childNum > 0) {//�����ڲ��ڵ�
		x->child = B_vectorCreat(sizeof(SR_dataBTNode*));
		x->childCode = B_vectorCreat(sizeof(char));
		x->goodsVector = NULL;
		free(x->childCode->_elem);//��ԭchildCode
		x->childCode->_elem = (char*)malloc(sizeof(char) * x->childNum);
		x->childCode->_capicity = x->childNum;
		x->childCode->_size = x->childNum;
		B_getSrc(file, x->childCode->_elem, sizeof(char) * x->childNum);
		free(x->child->_elem);//��ԭchild
		x->child->_elem = (char*)malloc(sizeof(SR_dataBTNode*) * x->childNum);
		x->child->_capicity = x->childNum;
		x->child->_size = x->childNum;
		for (int i = 0; i < x->childNum; i++) {
			SR_dataBTNode* t = *(SR_dataBTNode**)B_vectorGet(x->child, i) = (SR_dataBTNode*)malloc(sizeof(SR_dataBTNode));
			SR_dataBTBuild(t, file);
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
SR_dataBTNode* SR_dataGetSortFromChild(SR_dataBTNode* pos, char sort) {//�ڵ�ǰ�ڵ���ӽڵ���ҷ���,ʧ�ܷ���NULL
	if (pos->childNum < sort - 'A')
		return B_vectorGet(pos->child, sort - 'A');
	else
		return NULL;
}
SR_dataBTNode* SR_dataGetSortFromNode(SR_dataBTNode* pos, char sort[]) {//��ĳһ�ڵ���ҷ���(�ݹ�)
	if (isalpha(sort[0]))
		return SR_dataGetSortFromNode(SR_dataGetSortFromChild(sort[0], pos), sort + 1);
	else
		return pos;
}
SR_dataBTNode* SR_dataGetSort(char sort[]) {//�Ӹ��ڵ㿪ʼ���ҷ���
	SR_dataGetSortFromNode(&SR_dataBTRoot, sort);
}
C_Goods* SR_dataGet(char code[]) {//���ݱ��������Ʒ
	SR_dataBTNode* pos = SR_dataGetSort(code);
	C_goodsVectorFind_P(pos->goodsVector, code);
}
