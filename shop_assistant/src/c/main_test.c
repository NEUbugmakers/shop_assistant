#define _CRT_SECURE_NO_WARNINGS
#include"C_goods.h"
#include"SR_data.h"
int main() {
	SR_dataBTInit();
	/*C_Goods goods = C_GoodsCreat(100, "3", "A1003");*/
	printf("%d", sizeof(SR_dataBTNode));
	SR_dataBTNode* test = SR_dataGetSort("A");
	SR_dataInsertSort("", "sort1");
	test = SR_dataGetSort("A");
	test = test;
	test = SR_dataGetSort("B");
	test = test;
	SR_dataSave();
}
