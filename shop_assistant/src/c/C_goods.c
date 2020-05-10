
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
void C_goodsListInit(C_Goods* goods) {//�������б��ʼ��
	goods->C_shelfInfo = B_listCreat(sizeof(C_goodsInfo));
	goods->C_stockInfo = B_listCreat(sizeof(C_goodsInfo));
}
void C_goodsShelfAdd(C_Goods* goods, C_goodsInfo* info) {//��goodsShelf����������λ���
	B_listNode* x = B_listGetFirstNode(goods->C_shelfInfo);//ָ��goodsShelf�ĵ�һ���ڵ�
	info->C_rotDate = B_DayLater(&info->C_producedDate, info->C_shelfDate);//���¹�������
	while (x != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate, info->C_rotDate))//�ҵ�����λ��
		x = B_listNextNode(x);
	if (x != NULL)//��ǰ��Ʒ���������ڵ�
		B_listInsertPre(goods->C_shelfInfo, info, x);//����
	else//��ǰ��Ʒ�������ڵ�
		B_listPushBack(goods->C_shelfInfo, info);
}
void C_goodsStockAdd(C_Goods* goods, C_goodsInfo* info) {//��goodsStock����������λ���
	B_listNode* x = B_listGetFirstNode(goods->C_stockInfo);//ָ��goodsStock�ĵ�һ���ڵ�
	goods->C_shelfDate_Pre = info->C_shelfDate;//�����ݵı����ڱ��Ϊ��ǰ������
	info->C_rotDate = B_DayLater(&info->C_producedDate, info->C_shelfDate);//���¹�������
	if (x == NULL) {//�޵�һ���ڵ㣬����һ�β���
		B_listPushBack(goods->C_stockInfo, info);
		return;
	}
	while (x != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate, info->C_rotDate))//�ҵ�����λ��(����ʱ����ͬ�����ͬʱ�����ε����ͬʱ���Ƚ���������
		x = B_listNextNode(x);
	if (x != NULL)//��ǰ���β��������ڵ�
		B_listInsertPre(goods->C_stockInfo, info, x);//����
	else//��ǰ��Ʒ�������ڵ�
		B_listPushBack(goods->C_stockInfo, info);
}
void C_goodsRemoveShelfInfoRank(C_Goods* goods, Rank r) {//ɾ�������ϵ�ĳ����Ʒ
	B_listRemoveRank(goods->C_shelfInfo, r);
}
void C_goodsRemoveStockInfoRank(C_Goods* goods, Rank r) {//ɾ������е�ĳ����Ʒ
	B_listRemoveRank(goods->C_stockInfo, r);
}
int C_goodsOutofStockRank(C_Goods* goods, Rank r, int amount) {//��Ʒָ�����γ����ϼ�,����1��ʾ�����ɹ���0��ʾ����ʧ��
	C_goodsInfo* pos = (C_goodsInfo*)B_listGetRank(goods->C_stockInfo, 0);
	if (pos->C_amount < amount)//��ǰ���λ��ﲻ�㣬����ʧ��
		return 0;
	pos->C_amount -= amount;
	C_goodsInfo temp = *pos;
	temp.C_amount = amount;
	C_goodsShelfAdd(goods, &temp);
	if (pos->C_amount == 0)
		C_goodsRemoveStockInfoRank(goods, r);
	return 1;
}
void C_goodsOutofStock(C_Goods* goods, int amount) {//��Ʒ�����ϼܣ�Ĭ�����ȹ��ڵ�����
	C_goodsOutofStockRank(goods, 0, amount);
}
Rank C_goodsGetShelfBatch(C_Goods* goods, char batch) {//��shelfInfo�²������Σ��ɹ������ȣ�ʧ�ܷ���-1
	B_listNode* x = B_listGetFirstNode(goods->C_shelfInfo);
	Rank r = 0;
	while (x!=NULL&&((C_goodsInfo*)x->_elem)->batch != batch) {//���Һ��ʵ�����
		x = B_listNextNode(x);
		r++;
	}
	if (x != NULL)
		return r;
	else
		return -1;
}
void C_goodsSell(C_Goods* goods, char batch, int amount) {//��Ʒ����
	Rank r = C_goodsGetShelfBatch(goods, batch);
	C_goodsInfo* pos = B_listGetRank(goods->C_shelfInfo, r);
	pos->C_amount -= amount;
	if (pos->C_amount == 0)//��⵱ǰ�����Ƿ��ۿգ��ۿ���ɾ����ǰ����
		B_listRemoveRank(goods->C_shelfInfo, r);
}