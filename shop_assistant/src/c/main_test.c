#define _CRT_SECURE_NO_WARNINGS
#include"C_goods.h"
#include"SR_data.h"
int main() {
	//SR_dataBTInit();
	//C_Goods goods;
	//printf("%d", sizeof(SR_dataBTNode));
	//SR_dataBTNode* test = SR_dataGetSort("A");
	//SR_dataInsertSort("A", "sortAA");
	//test = SR_dataGetSort("B");
	//test = test;
	//SR_dataBTNode* test = SR_dataGetSort("AA");
	//test = test;
	//SR_dataInsertSort("", "sortA");
	//SR_dataInsertSort("", "sortB");
	//SR_dataInsertSort("A", "sortAA");
	//goods = C_GoodsCreat(100, "A", "A1000");
	//SR_dataAddNewGoods(&goods);
	//goods = C_GoodsCreat(100, "AA", "AA1000");
	//SR_dataAddNewGoods(&goods);
	//goods = C_GoodsCreat(100, "B", "B1000");
	//SR_dataAddNewGoods(&goods);
	//C_goodsInfo info;
	//Date date;
	//B_DateSet(&date, 2000, 2, 2);
	//info.C_amount = 100;
	//info.C_entryDate = date;
	//info.C_in = 100;
	//B_DateSet(&date, 2000, 1, 1);
	//info.C_producedDate = date;
	//info.C_shelfDate = 30;
	//C_Goods* tgoods = SR_dataGet("A1000");
	//tgoods = SR_dataGet("B1000");
	//tgoods = SR_dataGet("AA1000");
	//SR_dataSave();
	Date date;
	B_DateSet(&date, 2000, 1, 3);
	Date later = B_DayLater(&date, 100000);
	printf("%d %d %d", later.B_Year, later.B_Months, later.B_Day);
}
