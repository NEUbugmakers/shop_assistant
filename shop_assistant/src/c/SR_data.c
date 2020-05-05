#include"SR_data.h"
void SR_dataBTInit() {//��ʼ��B-��
	if (access("root", 0)) {//�ж��ļ��Ƿ����
		FILE* file = fopen("root", "rb+");
		fread(&SR_dataBTRoot, sizeof(SR_dataBTNode), 1, file);
		SR_dataBTBuild(&SR_dataBTRoot, file);
		fclose(file);
	}
	else {
		SR_dataBTNodeInit(&SR_dataBTRoot);
	}
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file) {//�����ļ�����B-��,�������
	if (x->childNum > 0) {//�����ڲ��ڵ�
		x->child = B_vectorCreat(sizeof(SR_dataBTNode*));
		x->childCode = B_vectorCreat(sizeof(char));
		x->goodsVector = C_goodsVectorCreat();
		free(x->childCode->_elem);//��ԭchildCode
		x->childCode->_elem = (char*)malloc(sizeof(char) * x->childNum);
		x->childCode->_capicity = x->childNum;
		x->childCode->_size = x->childNum;
		fread(x->childCode->_elem, sizeof(char), 1, file);
		if (x->SR_dataBTNodeHaveGoods) {//��ԭgoodsVector
			FILE* tfile = fopen(x->fileName, "rb");
			fseek(tfile, 0, SEEK_END);
			int len = ftell(tfile);
			fseek(tfile, 0, SEEK_SET);
			free(x->goodsVector->vector->_elem);
			x->goodsVector->vector->_elem = malloc(len);
			fread(x->goodsVector->vector->_elem, len, 1, tfile);
			x->goodsVector->vector->_capicity = x->goodsVector->vector->_size = len / x->goodsVector->vector->_esize;
			for (int i = 0; i < x->goodsVector->vector->_size; i++) {//��ԭC_goodsInfo
				C_goodsListInit((C_Goods*)B_vectorGet(x->goodsVector->vector, i));
				C_goodsInfo info;
				for (int j = 0; j < C_goodsShelfInfoNum((C_Goods*)B_vectorGet(x->goodsVector->vector, i));j++) {
					fread(&info, sizeof(C_goodsInfo), 1, tfile);
					B_listPushBack(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo,&info);
				}
				for (int j = 0; j < C_goodsStockInfoNum((C_Goods*)B_vectorGet(x->goodsVector->vector, i)); j++) {
					fread(&info, sizeof(C_goodsInfo), 1, tfile);
					B_listPushBack(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockInfo, &info);
				}
			}
			fclose(tfile);
		}
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
		x->goodsVector->vector->_capicity = flen / sizeof(C_Goods);
		free(x->goodsVector->vector->_elem);
		x->goodsVector->vector->_elem = (char*)malloc(flen);
		fread(x->goodsVector->vector->_elem, flen, 1, _file);
		x->goodsVector->vector->_size = flen / sizeof(C_Goods);
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
	SR_dataBTNode* pos = SR_dataGet(dir);
	SR_dataBTNode x;
	SR_dataBTNodeInit(&x);//��ʼ���ڵ�x
	char sort = SR_dataSelect(pos);//��ȡ�����ʶ��
	x.sort = sort;
	strcpy(x.name, name);
	x.sort = pos->childNum + 'A';//�����ʶ��
	x.parent = pos;
	Rank r;//��������ķ����ڸ�������
	for (r = 0; r < pos->childNum; r++)
		if (*(char*)B_vectorGet(pos->childCode, r) > sort)
			break;
	B_vectorInsert(pos->child, &x, r);
	B_vectorInsert(pos->childCode, &sort, r);
	pos->childNum++;
}
void SR_dataSaveGoodsInfo(B_list* info,FILE* file) {//����shelfInfo��stockInfo
	B_listNode* x = B_listGetFirstNode(info);
	while (x != NULL){
		fwrite(x->_elem, info->_esize, 1, file);
		x = B_listNextNode(x);
	}
}
void SR_dataSavePreOrder(SR_dataBTNode* x, FILE* root) {//�����������ڵ㣨δ��ɣ�

	fwrite(x, sizeof(SR_dataBTNode), 1, root);//���浱ǰ�ڵ������
	if (x->goodsVector->vector->_size != 0) {//��Ʒ��Ϣ����ƷĿ¼�ֱ�洢
		FILE* fp = fopen(x->fileName, "wb");
		fwrite(x->goodsVector->vector->_elem, x->goodsVector->vector->_esize, x->goodsVector->vector->_size, fp);
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {
			SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo, fp);
			SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockInfo, fp);
		}
		fclose(fp);
	}
	for (int i = 0; i < x->child->_size; i++) {
		SR_dataBTNode* t = *(SR_dataBTNode**)B_vectorGet(x->child, i);
		SR_dataSavePreOrder(t, root);
	}
}
void SR_dataSave() {//��������
	FILE* file = fopen("root", "wb");;
	SR_dataSavePreOrder(&SR_dataBTRoot, file);
}