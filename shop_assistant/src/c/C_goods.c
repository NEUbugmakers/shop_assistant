
#include"C_goods.h"
C_Goods C_GoodsCreat(int out, char name[], char code[]) {//����C_Goods
	C_Goods newGoods;
	newGoods.C_out = out;
	newGoods.C_shelfKinds = 0;
	newGoods.C_stockKinds = 0;
	strcpy(newGoods.name, name);
	strcpy(newGoods.code, code);
	newGoods.C_shelfInfo = B_listCreat(sizeof(C_goodsInfo));
	newGoods.C_stockInfo = B_listCreat(sizeof(C_goodsInfo));
	return newGoods;
};
int C_goodsShelfInfoNum(C_Goods* x) {//��ȡ������Ʒ��������
	return x->C_shelfKinds;
}
int C_goodsStockInfoNum(C_Goods* x) {//��ȡ�����Ʒ��������
	return x->C_stockKinds;
}
void C_goodsListInit(C_Goods* goods) {//�������б���ʼ��
	goods->C_shelfInfo = B_listCreat(sizeof(C_goodsInfo));
	goods->C_stockInfo = B_listCreat(sizeof(C_goodsInfo));
}
void C_goodsShelfAdd(C_Goods* goods, C_goodsInfo* info) {//��goodsShelf�����������λ���
	B_listNode* x = B_listGetFirstNode(goods->C_shelfInfo);//ָ��goodsShelf�ĵ�һ���ڵ�
	while (B_listNextNode(x) != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate,info->C_rotDate))//�ҵ�����λ��
		x = B_listNextNode(x);
	B_listInsertPre(goods->C_shelfInfo, info, x);//����
}
void C_goodsStockAdd(C_Goods* goods, C_goodsInfo* info) {//��goodsStock�����������λ���
	B_listNode* x = B_listGetFirstNode(goods->C_stockInfo);//ָ��goodsStock�ĵ�һ���ڵ�
	goods->C_shelfDate_Pre = info->C_shelfDate;//�����ݵı����ڱ��Ϊ��ǰ������
	while (B_listNextNode(x) != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate, info->C_rotDate))//�ҵ�����λ��(����ʱ����ͬ�����ͬʱ�����ε����ͬʱ���Ƚ���������
		x = B_listNextNode(x);
	B_listInsertPre(goods->C_stockInfo, info, x);//����
}