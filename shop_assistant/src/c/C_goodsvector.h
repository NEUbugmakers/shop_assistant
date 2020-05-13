#pragma once
#include"C_goods.h"
#include"B_vector.h"
typedef struct {
	B_vector* vector;
}C_goodsVector;
C_goodsVector* C_goodsVectorCreat();//����C_goodsVector
Rank C_goodsVectorFindInterval(C_goodsVector* goodsVector, char code[], Rank lo, Rank hi);//������[lo,hi)�ڲ�����Ʒ���ɹ��򷵻���Ʒ�ȣ�ʧ���򷵻ر�Ų�����code�����λ��
Rank C_goodsVectorFind(C_goodsVector* goodsVector, char code[]);//���ݱ��������Ʒ���ɹ��򷵻���Ʒ�ȣ�ʧ���򷵻ر�Ų�����code�����λ��
C_Goods* C_goodsVectorFind_P(C_goodsVector* goodsVector, char code[]);//���ݱ��������Ʒ���ɹ��򷵻���Ʒָ�룬ʧ���򷵻ر�Ų�����code�����λ��
int C_goodsVectorInsert(C_goodsVector* goodsVector, C_Goods* e);//����,�ɹ�����1��ʧ�ܷ���0
int C_goodsVectorRemove(C_goodsVector* goodsVector, char code[]);//ɾ����Ʒ��Ŀ,�ɹ�����1��ʧ�ܷ���0
void C_goodsVectorClear(C_goodsVector* goodsVector);//�ͷ�goodsVector