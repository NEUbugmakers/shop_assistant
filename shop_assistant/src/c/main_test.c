#define _CRT_SECURE_NO_WARNINGS
#include"SR_data.h"
int main() {
	SR_dataBTInit();
	C_Goods goods = C_GoodsCreat(100, "��ķ¶", "A10086");

	SR_dataInsertSort("", "������");
	SR_dataBTNode* test = SR_dataGetSort("A");
	strcpy(SR_dataBTRoot.name, "ROOT");
	SR_dataAddNewGoods(&goods);
	/*C_Goods* goods = SR_dataGet("A10086");*/
	SR_dataSave();
}
