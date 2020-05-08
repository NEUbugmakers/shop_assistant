
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include"SR_data.h"
void SR_dataBTInit() {//��ʼ��B-��
	FILE* file = fopen("root", "rb+");
	FILE* goodsFile = fopen("goods", "rb");
	if (file != NULL) {//�ж��ļ��Ƿ����
		fread(&SR_dataBTRoot, sizeof(SR_dataBTNode), 1, file);
		SR_dataBTBuild(&SR_dataBTRoot, file, goodsFile);
		fclose(file);
		fclose(goodsFile);
	}
	else {
		SR_dataBTNodeInit(&SR_dataBTRoot);
		strcpy(SR_dataBTRoot.fileName, "0");
	}
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file, FILE* goodsFile) {//�����ļ�����B-��,�������
	x->child = B_vectorCreat(sizeof(SR_dataBTNode*));
	x->childCode = B_vectorCreat(sizeof(char));
	x->goodsVector = C_goodsVectorCreat();
	if (goodsFile != NULL) {//��ԭgoodsVector
		fseek(goodsFile, 0, SEEK_END);
		int len = ftell(goodsFile);
		fseek(goodsFile, 0, SEEK_SET);
		free(x->goodsVector->vector->_elem);
		x->goodsVector->vector->_elem = malloc(len);
		fread(x->goodsVector->vector->_elem, len, 1, goodsFile);
		x->goodsVector->vector->_capicity = x->goodsVector->vector->_size = len / x->goodsVector->vector->_esize;
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {//��ԭC_goodsInfo
			C_goodsListInit((C_Goods*)B_vectorGet(x->goodsVector->vector, i));
			C_goodsInfo info;
			for (int j = 0; j < C_goodsShelfInfoNum((C_Goods*)B_vectorGet(x->goodsVector->vector, i)); j++) {
				fread(&info, sizeof(C_goodsInfo), 1, goodsFile);
				B_listPushBack(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo, &info);
			}
			for (int j = 0; j < C_goodsStockInfoNum((C_Goods*)B_vectorGet(x->goodsVector->vector, i)); j++) {
				fread(&info, sizeof(C_goodsInfo), 1, goodsFile);
				B_listPushBack(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockInfo, &info);
			}
		}
	}
	if (x->childNum > 0) {//�����ӽڵ�
		free(x->childCode->_elem);//��ԭchildCode
		x->childCode->_elem = (char*)malloc(sizeof(char) * x->childNum);
		x->childCode->_capicity = x->childNum;
		x->childCode->_size = x->childNum;
		fread(x->childCode->_elem, sizeof(char) * x->childNum, 1, file);//(������*x->childNum)

		free(x->child->_elem);//��ԭchild
		x->child->_elem = (char*)malloc(sizeof(SR_dataBTNode) * x->childNum);
		x->child->_capicity = x->childNum;
		x->child->_size = x->childNum;
		fread(x->child->_elem, sizeof(SR_dataBTNode), x->childNum, file);
		for (int i = 0; i < x->childNum; i++) {
			SR_dataBTNode* t = B_vectorGet(x->child, i);
			SR_dataBTBuild(t, file,goodsFile);
		}
	}
}
int SR_dataGetSortFromChildInterval(SR_dataBTNode* pos, char sort, int lo, int hi) {//�ڵ�ǰ�ڵ���ӽڵ�����[lo,hi)�ҷ���,ʧ�ܷ��ز����ڵ�ǰ�����ŵ������λ��(�����⣩
	int mi = (hi - lo) >> 1;
	if (lo < hi)
		if (*(char*)B_vectorGet(pos->childCode, mi) < sort) {
			return SR_dataGetSortFromChildInterval(pos, sort, lo, mi);
		}
		else {
			return SR_dataGetSortFromChildInterval(pos, sort, mi + 1, hi);
		}
	else {
		return lo - 1;
	}
}
SR_dataBTNode* SR_dataGetSortFromChild(SR_dataBTNode* pos, char sort) {//�ڵ�ǰ�ڵ���ӽڵ���ҷ���,ʧ�ܷ���NULL(�����⣩
	Rank r = SR_dataGetSortFromChildInterval(pos, sort, 0, pos->childCode->_size);
	if (*(char*)B_vectorGet(pos->childCode, r) == sort)//�ж��Ƿ��ҵ�
		return B_vectorGet(pos->child, r);
	else
		return NULL;
}
SR_dataBTNode* SR_dataGetSortFromNode(SR_dataBTNode* pos, char sort[]) {//��ĳһ�ڵ���ҷ���(�ݹ�)
	if (isalpha(sort[0]))
		return SR_dataGetSortFromNode(SR_dataGetSortFromChild(pos, sort[0]), sort + 1);
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
void SR_dataBTNodeInit(SR_dataBTNode* node) {//�Դ����Ľڵ��ʼ��
	node->name[0] = 0;
	node->childNum = 0;
	node->child = B_vectorCreat(sizeof(SR_dataBTNode));
	node->childCode = B_vectorCreat(sizeof(char));
	node->fileName[0] = 0;
	node->sort = 0;
	node->parent = 0;
	node->goodsVector = C_goodsVectorCreat();
}
char SR_dataSelectSort(SR_dataBTNode* pos) {//�ڵ�ǰ�ڵ���ѡ��������ķ����ʶ��
	if (pos->childNum < 26) {
		if (pos->childCode->_size == 0)
			return 'A';
		char backSort = *(char*)B_vectorGet(pos->childCode, pos->childCode->_size - 1);
		if (backSort != 'Z')
			return ++backSort;
		else
			for (int i = 0; i < pos->childCode->_size; i++)
				if (*(char*)B_vectorGet(pos->childCode, i) - 'A' != i)
					return 'A' + i;
	}
	else
		return -1;
}
void SR_dataInsertSort(char dir[], char name[]) {//�������
	SR_dataBTNode* pos = SR_dataGetSort(dir);//(�����⣩
	SR_dataBTNode x;
	SR_dataBTNodeInit(&x);//��ʼ���ڵ�x
	char sort = SR_dataSelectSort(pos);//��ȡ�����ʶ��
	x.sort = sort;
	strcpy(x.name, name);
	x.parent = pos;
	strcpy(x.fileName, dir);
	Rank r;//��������ķ����ڸ�������
	for (r = 0; r < pos->childNum; r++)
		if (*(char*)B_vectorGet(pos->childCode, r) > sort)
			break;
	B_vectorInsert(pos->child, &x, r);
	B_vectorInsert(pos->childCode, &sort, r);
	pos->childNum++;
}
void SR_dataSaveGoodsInfo(B_list* info, FILE* file) {//����shelfInfo��stockInfo
	B_listNode* x = B_listGetFirstNode(info);
	while (x != NULL) {
		fwrite(x->_elem, info->_esize, 1, file);
		x = B_listNextNode(x);
	}
}
void SR_dataSavePreOrder(SR_dataBTNode* x, FILE* root,FILE* goodsFile) {//�����������ڵ㣨δ��ɣ�

	x->childNum = x->childCode->_size;
	fwrite(x, sizeof(SR_dataBTNode), 1, root);//���浱ǰ�ڵ������
	if (x->goodsVector->vector->_size != 0) {//��Ʒ��Ϣ����ƷĿ¼�ֱ�洢

		fwrite(x->goodsVector->vector->_elem, x->goodsVector->vector->_esize, x->goodsVector->vector->_size, goodsFile);
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {
			SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo, goodsFile);
			SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockInfo, goodsFile);
		}
	}
	fwrite(x->childCode->_elem, sizeof(char), x->childNum, root);
	for (int i = 0; i < x->child->_size; i++) {
		SR_dataBTNode* t = (SR_dataBTNode*)B_vectorGet(x->child, i);
		SR_dataSavePreOrder(t, root,goodsFile);
	}
}
void SR_dataSave() {//��������
	FILE* file = fopen("root", "wb");
	FILE* goodsFile = fopen("goods", "wb");
	SR_dataSavePreOrder(&SR_dataBTRoot, file, goodsFile);
	fclose(file);
	fclose(goodsFile);
}
void SR_dataAddNewGoods(C_Goods* goods) {//�������Ʒ
	SR_dataBTNode* pos = SR_dataGetSort(goods->code);
	C_goodsVectorInsert(pos->goodsVector, goods);
}
void SR_dataReplenishGoods(char code[], C_goodsInfo* info) {//����
	SR_dataBTNode* posNode = SR_dataGetSort(code);
	C_Goods* pos = C_goodsVectorFind_P(posNode, code);
	C_goodsStockAdd(pos, info);
}
