
#include"C_goods.h"
C_Goods C_GoodsCreat(int out, char name[], char code[]) {//����C_Goods
	C_Goods newGoods;
	newGoods.C_out = out;
	newGoods.C_shelfKinds = 0;
	newGoods.C_stockKinds = 0;
	newGoods.C_shelfTotal = 0;
	newGoods.C_stockTotal = 0;
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
	info->batch = C_goodsSelectBatch(goods);//��ѡ���ʵı��
	while (x != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate, info->C_rotDate))//�ҵ�����λ��
		x = B_listNextNode(x);
	if (x != NULL)//��ǰ��Ʒ���������ڵ�
		B_listInsertPre(goods->C_shelfInfo, info, x);//����
	else//��ǰ��Ʒ�������ڵ�
		B_listPushBack(goods->C_shelfInfo, info);
	goods->C_shelfTotal += info->C_amount;
}
void C_goodsStockAdd(C_Goods* goods, C_goodsInfo* info) {//��goodsStock����������λ���
	B_listNode* x = B_listGetFirstNode(goods->C_stockInfo);//ָ��goodsStock�ĵ�һ���ڵ�
	goods->C_shelfDate_Pre = info->C_shelfDate;//�����ݵı����ڱ��Ϊ��ǰ������
	info->C_rotDate = B_DayLater(&info->C_producedDate, info->C_shelfDate);//���¹�������
	goods->C_stockTotal += info->C_amount;
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
	goods->C_shelfTotal -= ((C_goodsInfo*)B_listGetRank(goods->C_shelfInfo, r))->C_amount;
	B_listRemoveRank(goods->C_shelfInfo, r);
}
void C_goodsRemoveStockInfoRank(C_Goods* goods, Rank r) {//ɾ������е�ĳ����Ʒ
	goods->C_stockTotal -= ((C_goodsInfo*)B_listGetRank(goods->C_stockInfo, r))->C_amount;
	B_listRemoveRank(goods->C_stockInfo, r);
}
int C_goodsOutofStockRank(C_Goods* goods, Rank r, int amount) {//��Ʒָ�����γ����ϼ�,����1��ʾ�����ɹ���0��ʾ����ʧ��
	C_goodsInfo* pos = (C_goodsInfo*)B_listGetRank(goods->C_stockInfo, 0);
	if (pos->C_amount < amount)//��ǰ���λ��ﲻ�㣬����ʧ��
		return 0;
	pos->C_amount -= amount;
	goods->C_stockTotal -= amount;
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
C_goodsReturnPrice C_goodsSell(C_Goods* goods, char batch, int amount) {//��Ʒ����
	Rank r = C_goodsGetShelfBatch(goods, batch);
	C_goodsInfo* pos = B_listGetRank(goods->C_shelfInfo, r);
	C_goodsReturnPrice price;
	price.C_in = pos->C_in;
	price.C_out = goods->C_out;
	pos->C_amount -= amount;
	goods->C_shelfTotal -= amount;
	if (pos->C_amount == 0)//��⵱ǰ�����Ƿ��ۿգ��ۿ���ɾ����ǰ����
		B_listRemoveRank(goods->C_shelfInfo, r);
	return price;
}
char C_goodsSelectBatch(C_Goods* goods) {//Ϊ�����ϼܵ���Ʒ��ѡ���α���
	char allBatch[26] = {0};
	B_listNode* x = B_listGetFirstNode(goods->C_shelfInfo);
	while (x != NULL) {
		allBatch[((C_goodsInfo*)x->_elem)->batch - 'A'] = 1;
		x = B_listNextNode(x);
	}
	for (int i = 0; i < 26; i++)
		if (allBatch[i] != 1)
			return 'A' + i;
	return -1;
}
C_Goods C_goodsRotPreWarning(C_Goods* goods,int pre) {//��ǰpre����й��ڱ���,δ�����򷵻ؿ���Ϣ�������򷵻ؽ�����������Ʒ���б�
	C_Goods rot = C_GoodsCreat(goods->C_out, goods->name, goods->code);
	B_listNode* travel = B_listGetFirstNode(goods->C_shelfInfo);
	while (travel != NULL) {
		if (B_DateIsSmall(B_Time_I(), B_DayLater(&((C_goodsInfo*)travel->_elem)->C_rotDate, pre)))
			C_goodsShelfAdd(&rot, (C_goodsInfo*)travel->_elem);
	}
	travel = B_listGetFirstNode(goods->C_stockInfo);
	while (travel != NULL) {
		if (B_DateIsSmall(B_Time_I(), B_DayLater(&((C_goodsInfo*)travel->_elem)->C_rotDate, pre)))
			C_goodsStockAdd(&rot, (C_goodsInfo*)travel->_elem);
	}
	if (rot.C_shelfInfo->_size == 0 && rot.C_stockInfo->_size == 0) {//���û�й�����Ʒ�����rot��Ʒ��Ϣstrlen(code)==0
		C_goodsClear(&rot);
	}
	return rot;
}
C_Goods C_goodsGetRot(C_Goods* goods) {//��ȡ������Ʒ
	return C_goodsRotPreWarning(goods, 0);
}
void C_goodsClear(C_Goods* goods) {//�����Ʒ��Ϣ
	B_listClear(goods->C_shelfInfo);
	B_listClear(goods->C_stockInfo);
	free(goods->C_shelfInfo);
	free(goods->C_stockInfo);
	goods->C_shelfInfo = NULL;
	goods->C_stockInfo = NULL;
	goods->code[0] = 0;
	goods->name[0] = 0;
	goods->C_out = 0;
}