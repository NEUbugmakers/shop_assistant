/*
 * @Description: 
 * @Version: 2.0
 * @Autor: Charol
 * @Date: 2020-04-27 15:44:29
 * @LastEditors: 
 * @LastEditTime: 2020-04-28 23:13:55
 */
#pragma once
#include"B_time.h"
#define C_NUM_LEN    19//编码长度
#define C_NAME_LEN   100//名称最大长度
typedef struct{//商品信息
	int C_in;//进价
	int C_out;//标价（非实售价）
	char name[C_NAME_LEN];//商品名
	char num[C_NUM_LEN];//编码
	int C_shelfDate;//保质期
	Date C_producedDate;//生产日期
	Date C_entryDate;//入库日期
	Date C_rotDate;//过期日期
} C_Goods ;
int C_Goods_LessThan(C_Goods AGoods, C_Goods BGoods);//比较较商品过期日期的大小
