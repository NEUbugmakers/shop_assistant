
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include"SR_data.h"
void SR_dataBTInit() {//��ʼ��B-��
	FILE* file = fopen("../src/root", "rb");
	FILE* goodsFile = fopen("../src/goods", "rb");
	if (file != NULL) {//�ж��ļ��Ƿ����
		printf("%s:%d\n", "../SRC/root", sizeof(SR_dataBTNode));
		fread(&SR_dataBTRoot, sizeof(SR_dataBTNode), 1, file);
		SR_dataBTBuild(&SR_dataBTRoot, file, goodsFile);
		fclose(file);
		fclose(goodsFile);
	}
	else {
		SR_dataBTNodeInit(&SR_dataBTRoot);
	}
	SR_dataCodeVectorBuild();//�������ֱ����������
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file, FILE* goodsFile) {//�����ļ�����B-��,�������
	x->child = B_vectorCreat(sizeof(SR_dataBTNode));
	x->childCode = B_vectorCreat(sizeof(char));
	x->goodsVector = C_goodsVectorCreat();
	if (x->SR_dataBTNodeGoodsNum > 0) {//��ԭgoodsVector
		free(x->goodsVector->vector->_elem);
		x->goodsVector->vector->_elem = malloc(sizeof(C_Goods) * x->SR_dataBTNodeGoodsNum);
		printf("%s:%d\n", "goodsVector", sizeof(C_Goods) * x->SR_dataBTNodeGoodsNum);
		fread(x->goodsVector->vector->_elem, sizeof(C_Goods), x->SR_dataBTNodeGoodsNum, goodsFile);
		x->goodsVector->vector->_capicity = x->goodsVector->vector->_size = x->SR_dataBTNodeGoodsNum;
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {//��ԭC_goodsInfo(�����⣩
			C_goodsListInit((C_Goods*)B_vectorGet(x->goodsVector->vector, i));
			C_goodsInfo info;
			for (int j = 0; j < ((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfKinds; j++) {
				printf("%s:%d\n", "shelfInfo", sizeof(C_goodsInfo));
				fread(&info, sizeof(C_goodsInfo), 1, goodsFile);
				B_listPushBack(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo, &info);
			}
			for (int j = 0; j < ((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockKinds; j++) {
				printf("%s:%d\n", "stockInfo", sizeof(C_goodsInfo));
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
		printf("%s:%d\n", "childCode", sizeof(char) * x->childNum);
		fread(x->childCode->_elem, sizeof(char), x->childNum, file);//(������*x->childNum)

		free(x->child->_elem);//��ԭchild
		x->child->_elem = (char*)malloc(sizeof(SR_dataBTNode) * x->childNum);
		x->child->_capicity = x->childNum;
		x->child->_size = x->childNum;
		for (int i = 0; i < x->childNum; i++) {
			SR_dataBTNode* t = B_vectorGet(x->child, i);
			printf("%s:%d\n", "child", sizeof(SR_dataBTNode));
			fread(t, sizeof(SR_dataBTNode), 1, file);
			SR_dataBTBuild(t, file, goodsFile);
		}
	}
}
int SR_dataGetSortFromChildInterval(SR_dataBTNode* pos, char sort, int lo, int hi) {//�ڵ�ǰ�ڵ���ӽڵ�����[lo,hi)�ҷ���,ʧ�ܷ��ز����ڵ�ǰ�����ŵ������λ��(�����⣩
	while (lo < hi) {
		int mi = (hi + lo) >> 1;
		if (sort < *(char*)B_vectorGet(pos->childCode, mi))
			hi = mi;
		else
			lo = mi + 1;
	}
	return --lo;
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
	return (C_Goods*)C_goodsVectorFind_P(pos->goodsVector, code);
}
void SR_dataBTNodeInit(SR_dataBTNode* node) {//�Դ����Ľڵ��ʼ��
	node->name[0] = 0;
	node->childNum = 0;
	node->child = B_vectorCreat(sizeof(SR_dataBTNode));
	node->childCode = B_vectorCreat(sizeof(char));
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
	Rank r;//��������ķ����ڸ�������
	for (r = 0; r < pos->childNum; r++) {
		char t = *(char*)B_vectorGet(pos->childCode, r);
		if (t > sort)
			break;
	}
	B_vectorInsert(pos->child, &x, r);
	B_vectorInsert(pos->childCode, &sort, r);
	pos->childNum++;
}
void SR_dataSaveGoodsInfo(B_list* info, FILE* file) {//����shelfInfo��stockInfo
	B_listNode* x = B_listGetFirstNode(info);
	while (x != NULL) {
		printf("%s:%d\n", "C_GoodsInfo", sizeof(C_goodsInfo));
		fwrite(x->_elem, info->_esize, 1, file);
		x = B_listNextNode(x);
	}
}
void SR_dataSavePreOrder(SR_dataBTNode* x, FILE* root, FILE* goodsFile) {//�����������ڵ㣨δ��ɣ�

	x->childNum = x->childCode->_size;
	if (x->goodsVector->vector->_size != 0)//��Ӻ���goodsVector���
		x->SR_dataBTNodeGoodsNum = x->goodsVector->vector->_size;
	printf("%s:%d\n", "child", sizeof(SR_dataBTNode));
	fwrite(x, sizeof(SR_dataBTNode), 1, root);//���浱ǰ�ڵ������
	if (x->goodsVector->vector->_size != 0) {//��Ʒ��Ϣ����ƷĿ¼�ֱ�洢
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {//����ʱ����C_shelfKinds��C_stockKinds;
			C_Goods* t = (C_Goods*)B_vectorGet(x->goodsVector->vector, i);
			t->C_shelfKinds = t->C_shelfInfo->_size;
			t->C_stockKinds = t->C_stockInfo->_size;
		}
		printf("%s:%d\n", "goodsVector", x->goodsVector->vector->_esize * x->goodsVector->vector->_size);
		fwrite(x->goodsVector->vector->_elem, x->goodsVector->vector->_esize, x->goodsVector->vector->_size, goodsFile);
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {
			SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo, goodsFile);
			SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockInfo, goodsFile);
		}
	}
	printf("%s:%d\n", "childCode", sizeof(char) * x->childNum);
	fwrite(x->childCode->_elem, sizeof(char), x->childNum, root);
	for (int i = 0; i < x->child->_size; i++) {
		SR_dataBTNode* t = (SR_dataBTNode*)B_vectorGet(x->child, i);
		SR_dataSavePreOrder(t, root, goodsFile);
	}
}
void SR_dataSave() {//��������
	FILE* file = fopen("../src/root", "wb");
	FILE* goodsFile = fopen("../src/goods", "wb");
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
C_goodsReturnPrice SR_dataSell(char code[], char batch, int amount) {//������Ʒ,������Ʒ���
	C_Goods* pos = SR_dataGet(code);
	return C_goodsSell(pos, batch, amount);
}
void SR_dataCodeVectorBuildFrom(SR_dataBTNode* pos,B_vector* vector) {//����ĳһ�����µ���Ʒ���ֱ����������(����
	C_Goods* goods;
	SR_dataCodeNode node;
	Rank firstNum;
	for (int i = 0; i < pos->goodsVector->vector->_size; i++) {//��������������ֱ���
		goods = B_vectorGet(pos->goodsVector->vector, i);
		for (firstNum = 0; firstNum < 19&&isalpha(goods->code[firstNum]); firstNum++) {
			node.sort[firstNum] = goods->code[firstNum];
		}
		node.sort[firstNum] = 0;
		strcpy(node.code, goods->code + firstNum);
		B_vectorPushBack(vector, &node);
	}
	for (int i = 0; i < pos->child->_size; i++)//��������
		SR_dataCodeVectorBuildFrom(B_vectorGet(pos->child, i), vector);
}
void SR_dataCodeNodeCmp(SR_dataCodeNode* node1,SR_dataCodeNode* node2) {//ͨ�����ֱ���Ƚ�codeNode��С
	return strcmp(node1->code, node2->code);
}
void SR_dataCodeVectorBuild() {//�������ֱ����������
	SR_dataCodeVector = B_vectorCreat(sizeof(SR_dataCodeNode));
	SR_dataCodeVectorBuildFrom(&SR_dataBTRoot, SR_dataCodeVector);
	B_vectorSort(SR_dataCodeVector, SR_dataCodeNodeCmp);
}
C_Goods* SR_dataCodeFind(char code[]) {//ͨ�����ֱ��������Ʒ
	int lo = 0, hi = SR_dataCodeVector->_size;
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		if (strcmp(code, ((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, mi))->code ) < 0)
			hi = mi;
		else
			lo = mi + 1;
	}
	lo--;
	char fullcode[C_CODE_LEN];
	strcpy(fullcode, ((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, lo))->sort);
	strcat(fullcode, ((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, lo))->code);
	return SR_dataGet(fullcode);
}

//while (lo < hi) {
//	int mi = (hi + lo) >> 1;
//	if (sort < *(char*)B_vectorGet(pos->childCode, mi))
//		hi = mi;
//	else
//		lo = mi + 1;
//}
//return --lo;
C_goodsVector* SR_dataGetRot() {//���ҹ�����Ʒ
	C_goodsVector* rotGoods = C_goodsVectorCreat();
	for (int i = 0; i < SR_dataCodeVector->_size; i++) {
		C_Goods goods = C_goodsGetRot(SR_dataCodeFind(((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, i))->code));
		if (strlen(goods.code) != 0)//�Ƿ��й�����Ʒ
			B_vectorPushBack(rotGoods->vector, &goods);
	}
	if (rotGoods->vector->_size == 0) {//����޹�����Ʒ���ͷſռ䣬������NULL
		B_vectorClear(rotGoods);
		free(rotGoods);
		rotGoods == 0;
	}
	return rotGoods;//���ع�����Ʒ������
}





void SR_dataBTPreOrderFrom(SR_dataBTNode* pos, B_list* list, char sortName[]) {//��ĳһλ�ÿ�ʼ����(δ��ɣ�
	SR_dataPrint temp;
	C_Goods* x;
	int preLen;//��ǰ��������Ƴ���
	for (int i = 0; i < pos->goodsVector->vector->_size; i++) {
		x = (C_Goods*)B_vectorGet(pos->goodsVector->vector, i);
		strcpy(temp.code, x->code);
		strcpy(temp.name, x->name);
		temp.C_out = x->C_out;
		temp.C_shelfNum = x->C_shelfTotal;
		temp.C_stockNum = x->C_stockTotal;
		if (i != pos->goodsVector->vector->_size - 1)
			temp.C_State = Default;
		else
			temp.C_State = -1;//��������û��д��
		B_listPushBack(list, &temp);
	}
	for (int i = 0; i < pos->child->_size; i++) {

	}
}
void SR_dataBTPreOrder() {//������������������Ʒ�б�δ��ɣ�

}