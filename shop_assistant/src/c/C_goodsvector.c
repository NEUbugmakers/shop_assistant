
#include"C_goodsvector.h"
C_goodsVector* C_goodsVectorCreat() {//����C_goodsVector
	C_goodsVector* goodsVector = (C_goodsVector*)malloc(sizeof(C_goodsVector));
	goodsVector->vector = B_vectorCreat(sizeof(C_Goods));
	return goodsVector;
}
Rank C_goodsVectorFindInterval(C_goodsVector* goodsVector, char code[], Rank lo, Rank hi) {//������[lo,hi)�ڲ�����Ʒ���ɹ��򷵻���Ʒ�ȣ�ʧ���򷵻ر�Ų�����code�����λ��
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		if (strcmp(code, (*(C_Goods*)B_vectorGet(goodsVector->vector, mi)).code) < 0)
			hi = mi;
		else
			lo = mi + 1;
	}
	return --lo;
}
Rank C_goodsVectorFind(C_goodsVector* goodsVector, char code[]) {//���ݱ��������Ʒ���ɹ��򷵻���Ʒ�ȣ�ʧ���򷵻ر�Ų�����code�����λ��
	return C_goodsVectorFindInterval(goodsVector, code, 0, goodsVector->vector->_size);
}
C_Goods* C_goodsVectorFind_P(C_goodsVector* goodsVector, char code[]) {//���ݱ��������Ʒ���ɹ��򷵻���Ʒָ�룬ʧ���򷵻ر�Ų�����code�����λ��
	return B_vectorGet(goodsVector->vector, C_goodsVectorFindInterval(goodsVector, code, 0, goodsVector->vector->_size));
}

int C_goodsVectorInsert(C_goodsVector* goodsVector, C_Goods* e) {//����,�ɹ�����1��ʧ�ܷ���0
	Rank r = C_goodsVectorFind(goodsVector, e->code);
	if (strcmp((*(C_Goods*)B_vectorGet(goodsVector->vector, r)).code, e->code) == 0)//��Ʒ��Ŀ�Ѿ����ڣ�����ʧ��
		return 0;
	else {
		B_vectorInsert(goodsVector->vector, e, r + 1);
		return 1;
	}
}
int C_goodsVectorRemove(C_goodsVector* goodsVector, char code[]) {//ɾ����Ʒ��Ŀ,�ɹ�����1��ʧ�ܷ���0
	Rank r = C_goodsVectorFind(goodsVector, code);
	if (strcmp((*(C_Goods*)B_vectorGet(goodsVector->vector, r)).code, code) != 0)//��Ʒ��Ŀ�����ڣ�ɾ��ʧ��
		return 0;
	else {
		B_vectorRemoveRank(goodsVector->vector, r);
		return 1;
	}
}