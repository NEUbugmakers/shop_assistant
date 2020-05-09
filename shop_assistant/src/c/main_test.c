#define _CRT_SECURE_NO_WARNINGS
#include"C_goods.h"
#include"SR_data.h"
int main() {
	SR_dataBTInit();
	/*C_Goods goods = C_GoodsCreat(100, "B", "B1000");*/
	/*printf("%d", sizeof(SR_dataBTNode));*/
	//SR_dataBTNode* test = SR_dataGetSort("A");
	//SR_dataInsertSort("A", "sortAA");
	/*test = SR_dataGetSort("B");
	test = test;*/
	SR_dataBTNode* test = SR_dataGetSort("AA");
	test = test;
	/*SR_dataAddNewGoods(&goods);*/
	/*C_Goods* tgoods = SR_dataGet("B1000");*/
	SR_dataSave();
}
