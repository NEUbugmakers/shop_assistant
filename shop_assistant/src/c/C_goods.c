
#include"C_goods.h"
C_Goods C_GoodsCreat(int out, char name[], char code[]) {//创建C_Goods
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
int C_goodsShelfInfoNum(C_Goods* x) {//获取货架商品批次数量
	return x->C_shelfKinds;
}
int C_goodsStockInfoNum(C_Goods* x) {//获取库存商品批次数量
	return x->C_stockKinds;
}
void C_goodsListInit(C_Goods* goods) {//库存货架列表初始化
	goods->C_shelfInfo = B_listCreat(sizeof(C_goodsInfo));
	goods->C_stockInfo = B_listCreat(sizeof(C_goodsInfo));
}
void C_goodsShelfAdd(C_Goods* goods, C_goodsInfo* info) {//向goodsShelf中添加新批次货物
	B_listNode* x = B_listGetFirstNode(goods->C_shelfInfo);//指向goodsShelf的第一个节点
	info->C_rotDate = B_DayLater(&info->C_producedDate, info->C_shelfDate);//更新过期日期
	while (x != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate, info->C_rotDate))//找到插入位置
		x = B_listNextNode(x);
	if (x != NULL)//当前商品不是最后过期的
		B_listInsertPre(goods->C_shelfInfo, info, x);//插入
	else//当前商品是最后过期的
		B_listPushBack(goods->C_shelfInfo, info);
}
void C_goodsStockAdd(C_Goods* goods, C_goodsInfo* info) {//向goodsStock中添加新批次货物
	B_listNode* x = B_listGetFirstNode(goods->C_stockInfo);//指向goodsStock的第一个节点
	goods->C_shelfDate_Pre = info->C_shelfDate;//将备份的保质期变更为当前保质期
	info->C_rotDate = B_DayLater(&info->C_producedDate, info->C_shelfDate);//更新过期日期
	if (x == NULL) {//无第一个节点，即第一次插入
		B_listPushBack(goods->C_stockInfo, info);
		return;
	}
	while (x != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate, info->C_rotDate))//找到插入位置(过期时间相同则放在同时间批次的最后“同时间先进先卖”）
		x = B_listNextNode(x);
	if (x != NULL)//当前批次不是最后过期的
		B_listInsertPre(goods->C_stockInfo, info, x);//插入
	else//当前商品是最后过期的
		B_listPushBack(goods->C_stockInfo, info);
}
void C_goodsRemoveShelfInfoRank(C_Goods* goods, Rank r) {//删除货架上的某批商品
	B_listRemoveRank(goods->C_shelfInfo, r);
}
void C_goodsRemoveStockInfoRank(C_Goods* goods, Rank r) {//删除库存中的某批商品
	B_listRemoveRank(goods->C_stockInfo, r);
}
int C_goodsOutofStockRank(C_Goods* goods, Rank r, int amount) {//商品指定批次出库上架,返回1表示操作成功，0表示操作失败
	C_goodsInfo* pos = (C_goodsInfo*)B_listGetRank(goods->C_stockInfo, 0);
	if (pos->C_amount < amount)//当前批次货物不足，操作失败
		return 0;
	pos->C_amount -= amount;
	C_goodsInfo temp = *pos;
	temp.C_amount = amount;
	C_goodsShelfAdd(goods, &temp);
	if (pos->C_amount == 0)
		C_goodsRemoveStockInfoRank(goods, r);
	return 1;
}
void C_goodsOutofStock(C_Goods* goods, int amount) {//商品出库上架，默认最先过期的批次
	C_goodsOutofStockRank(goods, 0, amount);
}
Rank C_goodsGetShelfBatch(C_Goods* goods, char batch) {//在shelfInfo下查找批次，成功返回秩，失败返回-1
	B_listNode* x = B_listGetFirstNode(goods->C_shelfInfo);
	Rank r = 0;
	while (x!=NULL&&((C_goodsInfo*)x->_elem)->batch != batch) {//查找合适的批次
		x = B_listNextNode(x);
		r++;
	}
	if (x != NULL)
		return r;
	else
		return -1;
}
void C_goodsSell(C_Goods* goods, char batch, int amount) {//商品出售
	Rank r = C_goodsGetShelfBatch(goods, batch);
	C_goodsInfo* pos = B_listGetRank(goods->C_shelfInfo, r);
	pos->C_amount -= amount;
	if (pos->C_amount == 0)//检测当前批次是否售空，售空则删除当前批次
		B_listRemoveRank(goods->C_shelfInfo, r);
}