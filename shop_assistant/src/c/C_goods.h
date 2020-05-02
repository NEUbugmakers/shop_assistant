//C_Goods goods=C_goodsCreat(100,"史莱姆","A00001");标价100元的史莱姆编号为A00001;
#pragma once
#include"B_time.h"
#include"B_list.h"
#include<string.h>
#define C_CODE_LEN    30//编码长度
#define C_NAME_LEN   100//名称最大长度
typedef struct {
	Date C_producedDate;//生产日期
	Date C_entryDate;//入库日期
	Date C_rotDate;//过期日期
	int C_in;//进价
	int C_shelfDate;//保质期
} C_goodsInfo;
typedef struct {//商品信息
	int C_out;//标价（非实售价）
	char name[C_NAME_LEN];//商品名
	char code[C_CODE_LEN];//编码
	int C_shelfDate_Pre;//备选保质期，用于下次录入该商品信息时的自动填充
	B_list* C_shelfInfo;
	B_list* C_stockInfo;
} C_Goods;
C_Goods C_GoodsCreat(int out,char name[],char code[]){//创建C_Goods
	C_Goods newGoods;
	newGoods.C_out = out;
	strcpy(newGoods.name, name);
	strcpy(newGoods.code, code);
	newGoods.C_shelfInfo = B_listCreat(sizeof(C_goodsInfo));
	newGoods.C_stockInfo = B_listCreat(sizeof(C_goodsInfo));
	return newGoods;
};