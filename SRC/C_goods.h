//C_Goods goods=C_goodsCreat(100,"史莱姆","A00001");标价100元的史莱姆编号为A00001;
#pragma once
#include"B_time.h"
#include"B_list.h"
#include<string.h>
#include<ctype.h>
#include<QDebug>
#include"B_vector.h"
#define C_CODE_LEN    30//编码长度
#define C_NAME_LEN   100//名称最大长度
#define C_SORT_LEN 10
typedef struct {
	Date C_producedDate;//生产日期
	Date C_entryDate;//入库日期
	Date C_rotDate;//过期日期
	float C_in;//进价
	int C_shelfDate;//保质期
	int C_amount;//数量
    char batch;//批次
} C_goodsInfo;
typedef struct {//商品信息
	float C_out;//标价（非实售价）
	char name[C_NAME_LEN];//商品名
	char code[C_CODE_LEN];//编码
	int C_shelfDate_Pre;//备选保质期，用于下次录入该商品信息时的自动填充
	int C_shelfKinds;//货架上的现存的商品总批次,不必时时维护，仅在打开文件和保存文件的时候维护即可
	int C_stockKinds;//库存中现存的商品总批次,不必时时维护，仅在打开文件和保存文件的时候维护即可
	int C_shelfTotal;//货架中商品总量
	int C_stockTotal;//库存中商品总量
	B_list* C_shelfInfo;
	B_list* C_stockInfo;
} C_Goods;
typedef struct {
	float C_in;//进价
	float C_out;//标价
}C_goodsReturnPrice;
typedef struct {
    Date date;
    int amount;
}C_salesNode;//单商品单日销量统计
typedef struct{
    char code[C_CODE_LEN];
    B_vector* salesinfo;
    int num;//单商品销量统计
}C_sales;
typedef struct {
    float C_in;//进价
    float C_out;//标价
    char name[C_NAME_LEN];
}C_goodsReturn;
C_Goods C_GoodsCreat(char sort[], float out, char name[], char code[]);//创建C_Goods
int C_goodsShelfInfoNum(C_Goods* x);//获取货架商品批次数量
int C_goodsStockInfoNum(C_Goods* x);//获取库存商品批次数量
void C_goodsListInit(C_Goods* goods);//库存货架列表初始化
void C_goodsShelfAdd(C_Goods* goods, C_goodsInfo* info);//向goodsShelf中添加新批次货物
void C_goodsStockAdd(C_Goods* goods, C_goodsInfo* info);//向goodsStock中添加新批次货物
void C_goodsRemoveShelfInfoRank(C_Goods* goods, Rank r);//删除货架上的某批商品
void C_goodsRemoveStockInfoRank(C_Goods* goods, Rank r);//删除库存中的某批商品
char C_goodsOutofStockBatch(C_Goods* goods, char batch, int amount);//商品指定批次出库上架,返回批次编号，失败返回-1
char C_goodsOutofStock(C_Goods* goods, int amount);//商品出库上架，默认最先过期的批次,返回批次编号
Rank C_goodsGetBatch(C_Goods* goods, char batch, int flag);//0为库存，1位货架，查找批次，成功返回秩，失败返回-1
C_goodsReturn* C_goodsSell(C_Goods* goods, char batch, int amount, int flag);//商品出售,返回价格信息,0为交易，1为合法性检查
char C_goodsSelectBatch(C_Goods* goods,int flag);//为即将上架或入库的商品挑选批次编码,0为入库，1位上架
C_Goods C_goodsRotPreWarning(C_Goods* goods, int pre);//提前pre天进行过期报警,未过期则返回空信息，过期则返回仅包含过期商品的列表
C_Goods C_goodsGetRot(C_Goods* goods);//获取过期商品
void C_goodsClear(C_Goods* goods);//清空商品信息
int C_goodsModify(C_Goods* goods, char name[], float price);//修改商品信息
C_Goods C_goodsCopy(C_Goods* goods);//商品复制（仅基本信息）
char C_goodsReplenish(C_Goods* goods, C_goodsInfo* info);//补货，返回批次编号，失败返回-1
void C_goodsaddsaleinfo(C_sales* goods,int amount);//更新商品销量
int C_goodsGetMultiSales(C_sales* goods, int num) ;//返回近几日的销量
