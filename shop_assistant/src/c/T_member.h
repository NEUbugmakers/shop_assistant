#pragma once
#include<stdio.h>
#include <windows.h>
#include<string.h>
#include <QtDebug>
#define M_NUM 11
#define null NULL
#define S_NUM 19
typedef struct T_date {
	int T_Year;
	int T_Months;
	int T_Day;
}T_date;
typedef struct T_shopcart {
    char T_num[S_NUM];
    char T_batch;
    int  T_amount;
    float T_in;
    float T_out;
    struct T_shopcart* next;
}T_shopcart;
typedef struct Member {
	char M_num[M_NUM];//对应电话号码后10位  
    char M_sex;
	double M_money;//会员购买金额
	char M_level;//会员等级 '0'-注册 '1'-铜 '2'-银 '3'-金 '4'-钻
    char M_date[9];//会员到期时间
	struct Member* M_next;//下一个会员的位置 
	struct M_trade* M_buy;//会员购买记录指针 
}Member;
struct C_price {
	float C_in;//进货价格
	float C_out;//出货价格
};
struct C_price* SR_datasell(char T_num[], char T_batch, int T_amount);
typedef struct M_trade {
	char M_date[10];
	int line;
    float T_in;
    float T_out;
	struct M_trade* next;
}M_trade;
typedef struct Trade {
    char date[9];
	char M_num[11];
	struct T_goods* point;
	struct Trade* next;
	float T_in;
	float T_out;
}Trade;
typedef struct T_goods {
	int T_amount;
	char T_num[19];
	float T_in;
	float T_out;
	struct T_goods* next;
}T_goods;
typedef struct T_discount {
	char num[19];
	float discount;
	struct T_discount* next;
}T_discount;
struct T_date* T_time();
void T_cpytime(struct T_date* p, struct T_date* q);
void intTochar(int n, char date[], int size);
char* T_timechar();
char* M_timechar();
void T_datefile(char filename[], char* date);
int T_comparetime(struct T_date p, struct T_date q);
struct Member* M_load(struct Member* M_head, int* M_sum);
void M_setlevel(struct Member* p, int mode);
float M_level(struct Member* p);
struct Member* M_search(char num[], struct Member* M_head);
void M_display(struct Member* p);
void M_add(char num[], struct Member* M_add, T_date* time,char M_sex);
void M_delete(int M_number, struct Member* M_head);
int M_save(struct Member* M_head);
void T_disload();
void T_display(struct Trade* trade);
float T_discout(char num[]);
void T_recordadd(struct M_trade* mp, int T_sum);
int T_settle(struct Member* mp, struct Trade* T_head, float discount, int T_sum);
struct Trade* T_load(struct Trade* T_head, int* T_sum);
int T_save(struct Trade* M_head);
void T_delete(int T_number, struct Trade* T_head);
