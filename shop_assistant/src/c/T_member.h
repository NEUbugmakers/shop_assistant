#pragma once
#include<stdio.h>
#include <windows.h>
#define M_NUM 11
#define null NULL
typedef struct T_date{
    int T_Year;
    int T_Months;
    int T_Day;
}T_date;
typedef struct Member{
    char M_num[M_NUM];//对应电话号码后10位  
    double M_money;//会员购买金额
	char M_level;//会员等级 '0'-注册 '1'-铜 '2'-银 '3'-金 '4'-钻
	struct T_date M_date;//会员到期时间
	struct Member *M_next;//下一个会员的位置 
	struct M_trade* M_buy;//会员购买记录指针 
}member;
struct C_price {
	float C_in;//进货价格
	float C_out;//出货价格
}price={1.0,1.0};
struct C_price* SR_datasell(char T_num[],char T_batch,int T_amount){
	return &price;
}
typedef struct M_trade{
     struct T_date T_date;
     int line;
	 struct M_trade* next; 
}M_trade;
typedef struct Trade{
	  struct T_date T_date;
      char M_num[11];
      struct T_goods* point; 
      struct Trade* next;
      float T_in;
      float T_out;
}Trade;
typedef struct T_goods{
      int T_amount;
      char T_num[19];
      float T_in;
      float T_out;
      struct T_goods* next; 
}T_goods;
typedef struct T_discount{
      char num[19];
      float discount;
      struct T_discount* next; 
}T_discount;
struct T_discount T_discounthead;
struct T_date* T_time(){//获取系统时间 
	struct T_date* p=(struct T_date*)malloc(sizeof(struct T_date));
	SYSTEMTIME sys;
    GetLocalTime(&sys);
    p->T_Day=sys.wDay;
	p->T_Months=sys.wMonth;
	p->T_Year=sys.wYear;
	return p;
}
void T_cpytime(struct T_date *p,struct T_date* q){
	p->T_Day=q->T_Day;
	p->T_Months=q->T_Months;
	p->T_Year=q->T_Year;
}
int T_comparetime(struct T_date p,struct T_date q){
	if(p.T_Year==q.T_Year){
		if(p.T_Months==q.T_Months){
			return p.T_Day<q.T_Day;
		}else return p.T_Months<q.T_Months;
	}else return p.T_Year<q.T_Year;
}//返回1时 p比q大； 
struct Member* M_load(){
	FILE* fp;
	char M_first;
	struct Member* M_head = (struct Member*)malloc(sizeof(struct Member));
	struct Member* p;
	struct Member* q;
	p=M_head;
	q=M_head;
	fp = fopen("../../data/member/member.txt", "r");
    if(fp==NULL) {
        printf("打开文件失败!");
        return NULL; 
    }
    while(!feof(fp)){
    	M_first=fgetc(fp);
    	if(M_first=='#'){
    		q=(struct Member*)malloc(sizeof(struct Member));
            fread(q->M_num,sizeof(char),10,fp);
            q->M_num[10]='\0';
            fscanf(fp,"%lf",&q->M_money);
            fgetc(fp);
            fscanf(fp,"%c",&q->M_level);
            fscanf(fp,"%d",&q->M_date.T_Day);
            fscanf(fp,"%d",&q->M_date.T_Months);
            fscanf(fp,"%d",&q->M_date.T_Year);
            p->M_next=q;
            p=q;
    	}
    }
    p->M_next=NULL;
    fclose(fp);
    return M_head; 
}
void M_setlevel(struct Member* p,int mode){
	int money=(int)p->M_money;
	char level;
	if(money==0)//printf("注册会员");
	    level='0';
	else if(money<2000)//printf("青铜会员");
		level='1';
	else if(money<10000)//printf("银牌会员");
		level='2';
	else if(money<30000)//printf("金牌会员");
		level='3';
	else if(money>30000)//printf("钻石会员");
		level='4';
	else printf("购买金额无效！");
	if((mode==0&&p->M_level!=level)||mode==1){
		p->M_level=level;
		T_cpytime(&p->M_date,T_time());
	    p->M_date.T_Year+=1;
	}
}
float M_level(struct Member* p){
	int levelchange[5]={0,100,500,2500,7500};
	if(T_comparetime(p->M_date,*T_time())){
		printf("您的会员信息已过期已为您更新\n"); 
		p->M_money-=levelchange[p->M_level-'0'];
		if(p->M_money<0) p->M_money=0;
		M_setlevel(p,1);
	}
	int money=(int)p->M_money;
	if(money==0)//printf("注册会员");
	    return 1.0;
	else if(money<2000)//printf("青铜会员");
		return 0.98;
	else if(money<10000)//printf("银牌会员");
		return 0.95;
	else if(money<30000)//printf("金牌会员");
		return 0.90;
	else if(money>30000)//printf("钻石会员");
		return 0.85;
	else printf("购买金额无效！");
	return 1.0;
}
struct Member* M_search(char num[],struct Member* M_head){
	struct Member* p=M_head;
	struct Member* q;
	int k;
	while(p->M_next!=NULL){
		q=p;
		p=p->M_next;
		k=strcmp(num,p->M_num);
		if(k<0) return q;
		else if(k==0) return p;
	}
	return p;
}
void M_display(struct Member* p){
	printf("1%s ",p->M_num);
	printf("%8.1lf ",p->M_money);
	printf("%c ",p->M_level);
	printf(" %02d\\%02d\\%02d\n",p->M_date.T_Day,p->M_date.T_Months,p->M_date.T_Year);
}
void M_add(char num[],struct Member* M_add,T_date* time){	
	struct Member* buffer;
	buffer=M_add->M_next;
	struct Member* q=(struct Member*)malloc(sizeof(struct Member));
	M_add->M_next=q;
	q->M_next=buffer;
	strcpy(q->M_num,num);
	q->M_money=0.0;
	q->M_level='0';
	q->M_date.T_Day=time->T_Day;
	q->M_date.T_Months=time->T_Months;
	q->M_date.T_Year=time->T_Year+1;
}
void M_delete(int M_number,struct Member* M_head){
	struct Member* p=M_head;
	struct Member* q=M_head;
	while(M_number--){
		M_head=p;
		p=p->M_next;
	}
	M_head->M_next=p->M_next;
	free(p);
}
int M_save(struct Member* M_head){
	FILE* fp;
	struct Member* p=M_head;
	fp = fopen("../../data/member/member.txt", "w");
    if(fp==NULL) {
        printf("打开文件失败!");
        return 1; 
    }
    while(p->M_next!=NULL){
    	p=p->M_next;
    	fputc('#',fp);
    	fwrite(p->M_num,sizeof(char),10,fp);
        fprintf(fp," %lf",p->M_money);
        fprintf(fp," %c",p->M_level);
        fprintf(fp," %d",p->M_date.T_Day);
        fprintf(fp," %d",p->M_date.T_Months);
        fprintf(fp," %d",p->M_date.T_Year);
        fputc('\n',fp);
    }
    fclose(fp);
    return 0;
} 
void T_disload(){
	T_discounthead.next=NULL;
}

void T_display(struct Trade* trade){
	printf("%d年%d月%d日 会员编码:1%s\n",trade->T_date.T_Year,trade->T_date.T_Months,trade->T_date.T_Day,trade->M_num);
	int i=1;
	struct T_goods* p=trade->point;
	printf("商品清单：\n"); 
    while(p->next!=NULL){
    	p=p->next;
    	printf("%d:%s %d %.1f %.1f\n",i++,p->T_num,p->T_amount,p->T_in,p->T_out);
    }
    printf("进价：%.1lf\n消费金额：%.1lf\n",trade->T_in,trade->T_out); 
}
float T_discout(char num[]){
     struct T_discount* p=&T_discounthead;
	 while(p->next!=NULL){
	 	p=p->next;
	 	if(strcmp(num,p->num))return p->discount;
	 }
	 return 1.0;
}
int T_settle(struct Member* mp,struct Trade* T_head,float discount){
	char T_num[19],T_batch;
	int T_amount;
	double T_money=0.0,T_cost=0.0,G_discount;
	struct Trade* tp=(struct Trade*)malloc(sizeof(struct Trade));
	struct T_goods* gp=(struct T_goods*)malloc(sizeof(struct T_goods));
	struct T_goods* G_next;
	struct C_price* pp;
	T_head->next=tp;
	tp->next=NULL;
	tp->point=gp;
	strcpy(tp->M_num,mp->M_num);
	T_cpytime(&tp->T_date,T_time());
	printf("%d年%d月%d日 会员编码:1%s\n",tp->T_date.T_Year,tp->T_date.T_Months,tp->T_date.T_Day,tp->M_num);
	while(scanf("%s",T_num)!=EOF){
		if(strcmp(T_num,"END")==0){
			break;
		}else{
			scanf("%d",&T_batch);
			scanf("%d",&T_amount);
			pp=SR_datasell(T_num,T_batch,T_amount); 
			if(pp==NULL){
				printf("输入的商品编码有误,请重新输入\n");
			}else{
				G_discount=T_discout(T_num)*discount;
				G_next=(struct T_goods*)malloc(sizeof(struct T_goods));
				strcpy(G_next->T_num,T_num);
				G_next->T_amount=T_amount;
				G_next->T_in=pp->C_in*T_amount;
				G_next->T_out=pp->C_out*T_amount*G_discount;
				gp->next=G_next;
				gp=G_next;
			}
		}
		printf("进价：%.1f售价：%.1f\n",gp->T_in,gp->T_out);
		T_cost+=gp->T_in;
		T_money+=gp->T_out;
	}
	gp->next=NULL;
	mp->M_money+=T_money;
	tp->T_in=T_cost;
	tp->T_out=T_money;
	M_setlevel(mp,0);

	return 0;
}
