
#include"C_goods.h"
C_Goods C_GoodsCreat(int out, char name[], char code[]) {//创建C_Goods
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
	info->batch = C_goodsSelectBatch(goods);//挑选合适的编号
	while (x != NULL && B_DateIsSmall(((C_goodsInfo*)x->_elem)->C_rotDate, info->C_rotDate))//找到插入位置
		x = B_listNextNode(x);
	if (x != NULL)//当前商品不是最后过期的
		B_listInsertPre(goods->C_shelfInfo, info, x);//插入
	else//当前商品是最后过期的
		B_listPushBack(goods->C_shelfInfo, info);
	goods->C_shelfTotal += info->C_amount;
}
void C_goodsStockAdd(C_Goods* goods, C_goodsInfo* info) {//向goodsStock中添加新批次货物
	B_listNode* x = B_listGetFirstNode(goods->C_stockInfo);//指向goodsStock的第一个节点
	goods->C_shelfDate_Pre = info->C_shelfDate;//将备份的保质期变更为当前保质期
	info->C_rotDate = B_DayLater(&info->C_producedDate, info->C_shelfDate);//更新过期日期
	goods->C_stockTotal += info->C_amount;
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
	goods->C_shelfTotal -= ((C_goodsInfo*)B_listGetRank(goods->C_shelfInfo, r))->C_amount;
	B_listRemoveRank(goods->C_shelfInfo, r);
}
void C_goodsRemoveStockInfoRank(C_Goods* goods, Rank r) {//删除库存中的某批商品
	goods->C_stockTotal -= ((C_goodsInfo*)B_listGetRank(goods->C_stockInfo, r))->C_amount;
	B_listRemoveRank(goods->C_stockInfo, r);
}
int C_goodsOutofStockRank(C_Goods* goods, Rank r, int amount) {//商品指定批次出库上架,返回1表示操作成功，0表示操作失败
	C_goodsInfo* pos = (C_goodsInfo*)B_listGetRank(goods->C_stockInfo, 0);
	if (pos->C_amount < amount)//当前批次货物不足，操作失败
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
C_goodsReturnPrice C_goodsSell(C_Goods* goods, char batch, int amount) {//商品出售
	Rank r = C_goodsGetShelfBatch(goods, batch);
	C_goodsInfo* pos = B_listGetRank(goods->C_shelfInfo, r);
	C_goodsReturnPrice price;
	price.C_in = pos->C_in;
	price.C_out = goods->C_out;
	pos->C_amount -= amount;
	goods->C_shelfTotal -= amount;
	if (pos->C_amount == 0)//检测当前批次是否售空，售空则删除当前批次
		B_listRemoveRank(goods->C_shelfInfo, r);
	return price;
}
char C_goodsSelectBatch(C_Goods* goods) {//为即将上架的商品挑选批次编码
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
C_Goods C_goodsRotPreWarning(C_Goods* goods,int pre) {//提前pre天进行过期报警,未过期则返回空信息，过期则返回仅包含过期商品的列表
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
	if (rot.C_shelfInfo->_size == 0 && rot.C_stockInfo->_size == 0) {//如果没有过期商品则清空rot商品信息strlen(code)==0
		C_goodsClear(&rot);
	}
	return rot;
}
C_Goods C_goodsGetRot(C_Goods* goods) {//获取过期商品
	return C_goodsRotPreWarning(goods, 0);
}
void C_goodsClear(C_Goods* goods) {//清空商品信息
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