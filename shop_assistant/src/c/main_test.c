#define _CRT_SECURE_NO_WARNINGS
#include"C_goods.h"
#include"SR_data.h"
int main() {
	SR_dataBTInit();
	//SR_dataInsertSort("", "SortA");
	//SR_dataInsertSort("", "SortB");
	//SR_dataInsertSort("A", "SortAA");
	//SR_dataInsertSort("A", "SortAB");
	//SR_dataInsertSort("B", "SortBA");
	//SR_dataInsertSort("B", "SortBB");
	
	//printf("%d", sizeof(SR_dataBTNode));
	//SR_dataBTNode* test = SR_dataGetSort("A");
	//SR_dataInsertSort("A", "sortAA");
	//test = SR_dataGetSort("B");
	//test = test;
	//SR_dataBTNode* test = SR_dataGetSort("BA");
	//C_Goods goods;
	//goods = C_GoodsCreat(100, "A", "A1000");
	//SR_dataAddNewGoods(&goods);
	//goods = C_GoodsCreat(100, "AA", "AA1000");
	//SR_dataAddNewGoods(&goods);
	//goods = C_GoodsCreat(100, "BB", "BB1000");
	//SR_dataAddNewGoods(&goods);
	//C_goodsInfo info;
	//Date date;
	//B_DateSet(&date, 2000, 6, 15);
	//info.C_amount = 100;
	//info.C_entryDate = date;
	//info.C_in = 100;
	//B_DateSet(&date, 2000, 6, 10);
	//info.C_producedDate = date;
	//info.C_shelfDate = 30;
	//printf("666");
	C_Goods* tgoods = SR_dataGet("BB1000");
	//C_goodsOutofStock(tgoods,50);
	C_goodsSell(tgoods, 'C', 50);
	C_goodsInfo* testinfo = B_listGetRank(tgoods->C_stockInfo,0);
	testinfo = B_listGetRank(tgoods->C_stockInfo, 0);
	testinfo = B_listGetRank(tgoods->C_stockInfo, 1);
	testinfo = B_listGetRank(tgoods->C_shelfInfo, 0);
	testinfo = B_listGetRank(tgoods->C_shelfInfo, 1);
	//C_goodsStockAdd(tgoods, &info);
	//tgoods = SR_dataGet("B1000");
	//tgoods = SR_dataGet("AA1000");
	SR_dataSave();
}
