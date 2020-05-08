
#include"C_goodsvector.h"
C_goodsVector* C_goodsVectorCreat() {//创建C_goodsVector
	C_goodsVector* goodsVector = (C_goodsVector*)malloc(sizeof(C_goodsVector));
	goodsVector->vector = B_vectorCreat(sizeof(C_Goods));
	return goodsVector;
}
Rank C_goodsVectorFindInterval(C_goodsVector* goodsVector, char code[], Rank lo, Rank hi) {//在区间[lo,hi)内查找商品，成功则返回商品秩，失败则返回编号不大于code的最大位置
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		if (strcmp(code, (*(C_Goods*)B_vectorGet(goodsVector->vector, mi)).code) < 0)
			hi = mi;
		else
			lo = mi + 1;
	}
	return --lo;
}
Rank C_goodsVectorFind(C_goodsVector* goodsVector, char code[]) {//根据编码查找商品，成功则返回商品秩，失败则返回编号不大于code的最大位置
	return C_goodsVectorFindInterval(goodsVector, code, 0, goodsVector->vector->_size);
}
C_Goods* C_goodsVectorFind_P(C_goodsVector* goodsVector, char code[]) {//根据编码查找商品，成功则返回商品指针，失败则返回编号不大于code的最大位置
	return B_vectorGet(goodsVector->vector, C_goodsVectorFindInterval(goodsVector, code, 0, goodsVector->vector->_size));
}

int C_goodsVectorInsert(C_goodsVector* goodsVector, C_Goods* e) {//插入,成功返回1，失败返回0
	Rank r = C_goodsVectorFind(goodsVector, e->code);
	if (strcmp((*(C_Goods*)B_vectorGet(goodsVector->vector, r)).code, e->code) == 0)//商品条目已经存在，插入失败
		return 0;
	else {
		B_vectorInsert(goodsVector->vector, e, r + 1);
		return 1;
	}
}
int C_goodsVectorRemove(C_goodsVector* goodsVector, char code[]) {//删除商品条目,成功返回1，失败返回0
	Rank r = C_goodsVectorFind(goodsVector, code);
	if (strcmp((*(C_Goods*)B_vectorGet(goodsVector->vector, r)).code, code) != 0)//商品条目不存在，删除失败
		return 0;
	else {
		B_vectorRemoveRank(goodsVector->vector, r);
		return 1;
	}
}