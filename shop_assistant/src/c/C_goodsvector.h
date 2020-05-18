#pragma once
#include"C_goods.h"
#include"B_vector.h"
typedef struct {
	B_vector* vector;
}C_goodsVector;
C_goodsVector* C_goodsVectorCreat();//创建C_goodsVector
Rank C_goodsVectorFindInterval(C_goodsVector* goodsVector, char code[], Rank lo, Rank hi);//在区间[lo,hi)内查找商品，成功则返回商品秩，失败则返回编号不大于code的最大位置
Rank C_goodsVectorFind(C_goodsVector* goodsVector, char code[]);//根据编码查找商品，成功则返回商品秩，失败则返回编号不大于code的最大位置
C_Goods* C_goodsVectorFind_P(C_goodsVector* goodsVector, char code[]);//根据编码查找商品，成功则返回商品指针，失败则返回编号不大于code的最大位置
int C_goodsVectorInsert(C_goodsVector* goodsVector, C_Goods* e);//插入,成功返回1，失败返回0
int C_goodsVectorRemove(C_goodsVector* goodsVector, char code[]);//删除商品条目,成功返回1，失败返回0
void C_goodsVectorClear(C_goodsVector* goodsVector);//释放goodsVector