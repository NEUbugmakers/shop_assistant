
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include"SR_data.h"
void SR_dataBTInit() {//初始化B-树
	FILE* file = fopen("../src/root", "rb");
	FILE* goodsFile = fopen("../src/goods", "rb");
	if (file != NULL) {//判断文件是否存在
		printf("%s:%d\n", "../SRC/root", sizeof(SR_dataBTNode));
		fread(&SR_dataBTRoot, sizeof(SR_dataBTNode), 1, file);
		SR_dataBTBuild(&SR_dataBTRoot, file, goodsFile);
		fclose(file);
		fclose(goodsFile);
	}
	else {
		SR_dataBTNodeInit(&SR_dataBTRoot);
	}
	SR_dataCodeVectorBuild();//建立数字编码查找向量
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file, FILE* goodsFile) {//根据文件建立B-树,先序遍历
	x->child = B_vectorCreat(sizeof(SR_dataBTNode));
	x->childCode = B_vectorCreat(sizeof(char));
	x->goodsVector = C_goodsVectorCreat();
	if (x->SR_dataBTNodeGoodsNum > 0) {//还原goodsVector
		free(x->goodsVector->vector->_elem);
		x->goodsVector->vector->_elem = malloc(sizeof(C_Goods) * x->SR_dataBTNodeGoodsNum);
		printf("%s:%d\n", "goodsVector", sizeof(C_Goods) * x->SR_dataBTNodeGoodsNum);
		fread(x->goodsVector->vector->_elem, sizeof(C_Goods), x->SR_dataBTNodeGoodsNum, goodsFile);
		x->goodsVector->vector->_capicity = x->goodsVector->vector->_size = x->SR_dataBTNodeGoodsNum;
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {//还原C_goodsInfo(有问题）
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
	if (x->childNum > 0) {//建立子节点
		free(x->childCode->_elem);//还原childCode
		x->childCode->_elem = (char*)malloc(sizeof(char) * x->childNum);
		x->childCode->_capicity = x->childNum;
		x->childCode->_size = x->childNum;
		printf("%s:%d\n", "childCode", sizeof(char) * x->childNum);
		fread(x->childCode->_elem, sizeof(char), x->childNum, file);//(增加了*x->childNum)

		free(x->child->_elem);//还原child
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
int SR_dataGetSortFromChildInterval(SR_dataBTNode* pos, char sort, int lo, int hi) {//在当前节点的子节点区间[lo,hi)找分类,失败返回不大于当前分类编号的最大编号位置(有问题）
	while (lo < hi) {
		int mi = (hi + lo) >> 1;
		if (sort < *(char*)B_vectorGet(pos->childCode, mi))
			hi = mi;
		else
			lo = mi + 1;
	}
	return --lo;
}
SR_dataBTNode* SR_dataGetSortFromChild(SR_dataBTNode* pos, char sort) {//在当前节点的子节点查找分类,失败返回NULL(有问题）
	Rank r = SR_dataGetSortFromChildInterval(pos, sort, 0, pos->childCode->_size);
	if (*(char*)B_vectorGet(pos->childCode, r) == sort)//判断是否找到
		return B_vectorGet(pos->child, r);
	else
		return NULL;
}
SR_dataBTNode* SR_dataGetSortFromNode(SR_dataBTNode* pos, char sort[]) {//从某一节点查找分类(递归)
	if (isalpha(sort[0]))
		return SR_dataGetSortFromNode(SR_dataGetSortFromChild(pos, sort[0]), sort + 1);
	else
		return pos;
}
SR_dataBTNode* SR_dataGetSort(char sort[]) {//从根节点开始查找分类
	SR_dataGetSortFromNode(&SR_dataBTRoot, sort);
}
C_Goods* SR_dataGet(char code[]) {//根据编码查找商品
	SR_dataBTNode* pos = SR_dataGetSort(code);
	return (C_Goods*)C_goodsVectorFind_P(pos->goodsVector, code);
}
void SR_dataBTNodeInit(SR_dataBTNode* node) {//对创建的节点初始化
	node->name[0] = 0;
	node->childNum = 0;
	node->child = B_vectorCreat(sizeof(SR_dataBTNode));
	node->childCode = B_vectorCreat(sizeof(char));
	node->sort = 0;
	node->parent = 0;
	node->goodsVector = C_goodsVectorCreat();
}
char SR_dataSelectSort(SR_dataBTNode* pos) {//在当前节点挑选新增分类的分类标识符
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
void SR_dataInsertSort(char dir[], char name[]) {//插入分类
	SR_dataBTNode* pos = SR_dataGetSort(dir);//(有问题）
	SR_dataBTNode x;
	SR_dataBTNodeInit(&x);//初始化节点x
	char sort = SR_dataSelectSort(pos);//获取分类标识符
	x.sort = sort;
	strcpy(x.name, name);
	x.parent = pos;
	Rank r;//即将插入的分类在父类中秩
	for (r = 0; r < pos->childNum; r++) {
		char t = *(char*)B_vectorGet(pos->childCode, r);
		if (t > sort)
			break;
	}
	B_vectorInsert(pos->child, &x, r);
	B_vectorInsert(pos->childCode, &sort, r);
	pos->childNum++;
}
void SR_dataSaveGoodsInfo(B_list* info, FILE* file) {//保存shelfInfo和stockInfo
	B_listNode* x = B_listGetFirstNode(info);
	while (x != NULL) {
		printf("%s:%d\n", "C_GoodsInfo", sizeof(C_goodsInfo));
		fwrite(x->_elem, info->_esize, 1, file);
		x = B_listNextNode(x);
	}
}
void SR_dataSavePreOrder(SR_dataBTNode* x, FILE* root, FILE* goodsFile) {//先序遍历保存节点（未完成）

	x->childNum = x->childCode->_size;
	if (x->goodsVector->vector->_size != 0)//添加含有goodsVector标记
		x->SR_dataBTNodeGoodsNum = x->goodsVector->vector->_size;
	printf("%s:%d\n", "child", sizeof(SR_dataBTNode));
	fwrite(x, sizeof(SR_dataBTNode), 1, root);//保存当前节点的数据
	if (x->goodsVector->vector->_size != 0) {//商品信息和商品目录分别存储
		for (int i = 0; i < x->goodsVector->vector->_size; i++) {//保存时更新C_shelfKinds和C_stockKinds;
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
void SR_dataSave() {//保存数据
	FILE* file = fopen("../src/root", "wb");
	FILE* goodsFile = fopen("../src/goods", "wb");
	SR_dataSavePreOrder(&SR_dataBTRoot, file, goodsFile);
	fclose(file);
	fclose(goodsFile);
}
void SR_dataAddNewGoods(C_Goods* goods) {//添加新商品
	SR_dataBTNode* pos = SR_dataGetSort(goods->code);
	C_goodsVectorInsert(pos->goodsVector, goods);
}
void SR_dataReplenishGoods(char code[], C_goodsInfo* info) {//补货
	SR_dataBTNode* posNode = SR_dataGetSort(code);
	C_Goods* pos = C_goodsVectorFind_P(posNode, code);
	C_goodsStockAdd(pos, info);
}
C_goodsReturnPrice SR_dataSell(char code[], char batch, int amount) {//出售商品,返回商品标价
	C_Goods* pos = SR_dataGet(code);
	return C_goodsSell(pos, batch, amount);
}
void SR_dataCodeVectorBuildFrom(SR_dataBTNode* pos,B_vector* vector) {//建立某一分类下的商品数字编码查找向量(无序）
	C_Goods* goods;
	SR_dataCodeNode node;
	Rank firstNum;
	for (int i = 0; i < pos->goodsVector->vector->_size; i++) {//分离分类编码和数字编码
		goods = B_vectorGet(pos->goodsVector->vector, i);
		for (firstNum = 0; firstNum < 19&&isalpha(goods->code[firstNum]); firstNum++) {
			node.sort[firstNum] = goods->code[firstNum];
		}
		node.sort[firstNum] = 0;
		strcpy(node.code, goods->code + firstNum);
		B_vectorPushBack(vector, &node);
	}
	for (int i = 0; i < pos->child->_size; i++)//继续深入
		SR_dataCodeVectorBuildFrom(B_vectorGet(pos->child, i), vector);
}
void SR_dataCodeNodeCmp(SR_dataCodeNode* node1,SR_dataCodeNode* node2) {//通过数字编码比较codeNode大小
	return strcmp(node1->code, node2->code);
}
void SR_dataCodeVectorBuild() {//建立数字编码查找向量
	SR_dataCodeVector = B_vectorCreat(sizeof(SR_dataCodeNode));
	SR_dataCodeVectorBuildFrom(&SR_dataBTRoot, SR_dataCodeVector);
	B_vectorSort(SR_dataCodeVector, SR_dataCodeNodeCmp);
}
C_Goods* SR_dataCodeFind(char code[]) {//通过数字编码查找商品
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
C_goodsVector* SR_dataGetRot() {//查找过期商品
	C_goodsVector* rotGoods = C_goodsVectorCreat();
	for (int i = 0; i < SR_dataCodeVector->_size; i++) {
		C_Goods goods = C_goodsGetRot(SR_dataCodeFind(((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, i))->code));
		if (strlen(goods.code) != 0)//是否含有过期商品
			B_vectorPushBack(rotGoods->vector, &goods);
	}
	if (rotGoods->vector->_size == 0) {//如果无过期商品则释放空间，并返回NULL
		B_vectorClear(rotGoods);
		free(rotGoods);
		rotGoods == 0;
	}
	return rotGoods;//返回过期商品的向量
}





void SR_dataBTPreOrderFrom(SR_dataBTNode* pos, B_list* list, char sortName[]) {//从某一位置开始遍历(未完成）
	SR_dataPrint temp;
	C_Goods* x;
	int preLen;//当前分类的名称长度
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
			temp.C_State = -1;//这里明显没有写完
		B_listPushBack(list, &temp);
	}
	for (int i = 0; i < pos->child->_size; i++) {

	}
}
void SR_dataBTPreOrder() {//先序遍历，用于输出商品列表（未完成）

}