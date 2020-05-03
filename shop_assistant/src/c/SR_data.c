#include"SR_data.h"
void SR_dataBTInit() {//初始化B-树
	if (access("root", 0)) {//判断文件是否存在
		FILE* file = fopen("root", "rb+");
		fread(&SR_dataBTRoot, sizeof(SR_dataBTNode), 1, file);
		SR_dataBTBuild(&SR_dataBTRoot, file);
		fclose(file);
	}
	else {
		SR_dataBTNodeInit(&SR_dataBTRoot);
	}
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file) {//根据文件建立B-树,先序遍历
	if (x->childNum > 0) {//建立内部节点
		x->child = B_vectorCreat(sizeof(SR_dataBTNode*));
		x->childCode = B_vectorCreat(sizeof(char));
		x->goodsVector = NULL;
		free(x->childCode->_elem);//还原childCode
		x->childCode->_elem = (char*)malloc(sizeof(char) * x->childNum);
		x->childCode->_capicity = x->childNum;
		x->childCode->_size = x->childNum;
		fread(x->childCode->_elem, sizeof(char), 1, file);
		free(x->child->_elem);//还原child
		x->child->_elem = (char*)malloc(sizeof(SR_dataBTNode*) * x->childNum);
		x->child->_capicity = x->childNum;
		x->child->_size = x->childNum;
		for (int i = 0; i < x->childNum; i++) {
			SR_dataBTNode* t = *(SR_dataBTNode**)B_vectorGet(x->child, i) = (SR_dataBTNode*)malloc(sizeof(SR_dataBTNode));
			SR_dataBTBuild(t, file);
		}
	}
	else {//建立外部节点
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
SR_dataBTNode* SR_dataGetSortFromChild(SR_dataBTNode* pos, char sort) {//在当前节点的子节点查找分类,失败返回NULL
	if (pos->childNum < sort - 'A')
		return B_vectorGet(pos->child, sort - 'A');
	else
		return NULL;
}
SR_dataBTNode* SR_dataGetSortFromNode(SR_dataBTNode* pos, char sort[]) {//从某一节点查找分类(递归)
	if (isalpha(sort[0]))
		return SR_dataGetSortFromNode(SR_dataGetSortFromChild(sort[0], pos), sort + 1);
	else
		return pos;
}
SR_dataBTNode* SR_dataGetSort(char sort[]) {//从根节点开始查找分类
	SR_dataGetSortFromNode(&SR_dataBTRoot, sort);
}
C_Goods* SR_dataGet(char code[]) {//根据编码查找商品
	SR_dataBTNode* pos = SR_dataGetSort(code);
	C_goodsVectorFind_P(pos->goodsVector, code);
}
void SR_dataBTNodeInit(SR_dataBTNode* node) {//对创建的节点初始化
	node->name[0] = 0;
	node->childNum = 0;
	node->child = B_vectorCreat(sizeof(SR_dataBTNode));
	node->childCode = B_vectorCreat(sizeof(char));
	node->fileName[0] = 0;
	node->sort = 0;
	node->parent = 0;
	node->goodsVector = C_goodsVectorCreat();
}
char SR_dataSelectSort(SR_dataBTNode* pos) {//在当前节点挑选新增分类的分类标识符
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
void SR_dataInsertSort(char dir[], char name[]) {//插入分类
	SR_dataBTNode* pos = SR_dataGet(dir);
	SR_dataBTNode x;
	SR_dataBTNodeInit(&x);//初始化节点x
	char sort = SR_dataSelect(pos);//获取分类标识符
	x.sort = sort;
	strcpy(x.name, name);
	x.sort = pos->childNum + 'A';//分类标识符
	x.parent = pos;
	Rank r;//即将插入的分类在父类中秩
	for (r = 0; r < pos->childNum; r++)
		if (*(char*)B_vectorGet(pos->childCode, r) > sort)
			break;
	B_vectorInsert(pos->child, &x, r);
	B_vectorInsert(pos->childCode, &sort, r);
	pos->childNum++;
}
void SR_dataSavePreOrder(SR_dataBTNode* x,FILE* root) {
	if (x->goodsVector->vector->_size != 0) {//商品信息和商品目录分别存储
		FILE* fp = fopen(x->fileName, 'wb');
		fwrite(x->goodsVector->vector->_elem, x->goodsVector->vector->_esize, x->goodsVector->vector->_size, fp);
		fclose(fp);
	}
	fwrite(x, sizeof(SR_dataBTNode), 1, root);
	for (int i = 0; i < x->child->_size; i++) {
		SR_dataBTNode* t = *(SR_dataBTNode ** )B_vectorGet(x->child, i);
		SR_dataSavePreOrder(t, root);
	}
}
void SR_dataSave() {

}