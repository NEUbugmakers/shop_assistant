#include"C_goods.h"
#include"B_vector.h"
typedef struct {
	B_vector* vector;
}C_goodsVector;
C_goodsVector* C_goodsVectorCreat() {//����C_goodsVector
	C_goodsVector* goodsVector = (C_goodsVector*)malloc(sizeof(C_goodsVector));
	goodsVector->vector = B_vectorCreat(sizeof(C_Goods));
	return goodsVector;
}
void C_goodsVectorInsert(C_goodsVector* goodsVector,C_Goods* e) {//����
	int i;
	for (i = 0; i < goodsVector->vector->_size; i++) 
		if((*(C_Goods*)B_vectorGet(goodsVector->vector,i)).num)
}