#define _CRT_SECURE_NO_WARNINGS
#include"SR_data.h"
int main() {
	SR_dataBTInit();
	C_Goods goods = C_GoodsCreat(100, "��ķ¶", "A10086");

	SR_dataInsertSort("", "������");
	
	strcpy(SR_dataBTRoot.name, "ROOT");
	SR_dataAddNewGoods(&goods);
	SR_dataSave();
}
