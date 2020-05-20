
#include"C_goods.h"
C_Goods C_GoodsCreat(char sort[], float out, char name[], char code[]) {//创建C_Goods,失败返回的商品编码为空
	C_Goods newGoods;
	if (strlen(sort) == 0 || out <= 0 || strlen(name) == 0 || strlen(code) == 0) {
		newGoods.code[0] = 0;
	}
	newGoods.C_out = out;
	newGoods.C_shelfKinds = 0;
	newGoods.C_stockKinds = 0;
	newGoods.C_shelfTotal = 0;
	newGoods.C_stockTotal = 0;
	newGoods.C_shelfDate_Pre = 0;
	strcpy(newGoods.name, name);
	strcpy(newGoods.code, sort);
	strcat(newGoods.code, code);
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
	info->batch = C_goodsSelectBatch(goods, 1);//挑选合适的编号
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
char C_goodsOutofStockBatch(C_Goods* goods, char batch, int amount) {//商品指定批次出库上架,返回批次编号，失败返回-1
	Rank r = C_goodsGetBatch(goods, batch, 0);
	C_goodsInfo* pos = (C_goodsInfo*)B_listGetRank(goods->C_stockInfo, r);
	if (pos->C_amount < amount)//当前批次货物不足，操作失败
		return -1;
	pos->C_amount -= amount;
	goods->C_stockTotal -= amount;
	C_goodsInfo temp = *pos;
	temp.C_amount = amount;
	temp.batch = C_goodsSelectBatch(goods, 1);
	C_goodsShelfAdd(goods, &temp);
	if (pos->C_amount == 0)
		C_goodsRemoveStockInfoRank(goods, r);
	return temp.batch;
}
char C_goodsOutofStock(C_Goods* goods, int amount) {//商品出库上架，默认最先过期的批次
	char batch = ((C_goodsInfo*)B_listGetRank(goods->C_stockInfo, 0))->batch;
	return C_goodsOutofStockBatch(goods, batch, amount);
}
Rank C_goodsGetBatch(C_Goods* goods, char batch, int flag) {//0为库存，1位货架，查找批次，成功返回秩，失败返回-1
	B_listNode* x = NULL;
	if (flag == 0)
		x = B_listGetFirstNode(goods->C_shelfInfo);
	else if (flag == 1)
		x = B_listGetFirstNode(goods->C_stockInfo);
	Rank r = 0;
	while (x != NULL && ((C_goodsInfo*)x->_elem)->batch != batch) {//查找合适的批次
		x = B_listNextNode(x);
		r++;
	}
	if (x != NULL)
		return r;
	else
		return -1;
}
C_goodsReturnPrice C_goodsSell(C_Goods* goods, char batch, int amount) {//商品出售
	Rank r = C_goodsGetBatch(goods, batch, 1);
	C_goodsInfo* pos = (C_goodsInfo*)B_listGetRank(goods->C_shelfInfo, r);
	C_goodsReturnPrice price;
	price.C_in = pos->C_in;
	price.C_out = goods->C_out;
	pos->C_amount -= amount;
	goods->C_shelfTotal -= amount;
	if (pos->C_amount == 0)//检测当前批次是否售空，售空则删除当前批次
		B_listRemoveRank(goods->C_shelfInfo, r);
	return price;
}
char C_goodsSelectBatch(C_Goods* goods, int flag) {//为即将上架或入库的商品挑选批次编码,0为入库，1位上架
	char allBatch[26] = { 0 };
	if (flag == 1) {
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
	else if (flag == 0) {
		B_listNode* x = B_listGetFirstNode(goods->C_stockInfo);
		while (x != NULL) {
			allBatch[((C_goodsInfo*)x->_elem)->batch - 'A'] = 1;
			x = B_listNextNode(x);
		}
		for (int i = 0; i < 26; i++)
			if (allBatch[i] != 1)
				return 'A' + i;
		return -1;
	}
}



C_Goods C_goodsRotPreWarning(C_Goods* goods, int pre) {//提前pre天进行过期报警,未过期则返回空信息，过期则返回仅包含过期商品的列表
	C_Goods rot = C_goodsCopy(goods);
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
int C_goodsModify(C_Goods* goods, char name[], float price) {//修改商品信息
	if (strlen(name) == 0 || price <= 0)
		return 0;
	else {
		strcpy(goods->name, name);
		goods->C_out = price;
	}
}
C_Goods C_goodsCopy(C_Goods* goods) {//商品复制（仅基本信息）
	int firstNum;
	for (firstNum = 0; isalpha(goods->code[firstNum]); firstNum++);
	char sort[C_SORT_LEN];
	for (int i = 0; i < firstNum; i++) {
		sort[i] = goods->code[i];
	}
	sort[firstNum] = 0;
	return C_GoodsCreat(sort, goods->C_out, goods->name, goods->code + firstNum);
}
char C_goodsReplenish(C_Goods* goods, C_goodsInfo* info) {//补货，返回批次编号，失败返回-1
	char batch = C_goodsSelectBatch(goods, 0);
	if (batch == -1) 
		return -1;
	info->batch = batch;
	C_goodsStockAdd(goods, info);
}
//char C_goodsOutofStockBatch(C_Goods* goods, char batch, int amount) {//商品指定批次出库上架,返回批次编号，失败返回-1
//	Rank r = C_goodsGetBatch(goods, batch, 0);
//	C_goodsInfo* pos = (C_goodsInfo*)B_listGetRank(goods->C_stockInfo, r);
//	if (pos->C_amount < amount)//当前批次货物不足，操作失败
//		return -1;
//	pos->C_amount -= amount;
//	goods->C_stockTotal -= amount;
//	C_goodsInfo temp = *pos;
//	temp.C_amount = amount;
//	temp.batch = C_goodsSelectBatch(goods, 1);
//	C_goodsShelfAdd(goods, &temp);
//	if (pos->C_amount == 0)
//		C_goodsRemoveStockInfoRank(goods, r);
//	return temp.batch;
//}