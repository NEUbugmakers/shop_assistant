#include "T_member.h"
struct T_discount* B_discounthead;
discountwidget* T_discountparent;
void T_DayLater(struct T_date* date, int day) {
    int dpm[] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//每个月的天数
    int tempday = date->T_Day + day - 1;//自当前月的一号起算需经过多少天
    int dom;//当前月的天数
    struct T_date rdate = *date;
    while (tempday > 0) {
        if (rdate.T_Months != 2)//j计算当前月有多少天
            dom = dpm[rdate.T_Months - 1];
        else if (rdate.T_Year % 4 == 0 && rdate.T_Year % 100 != 0) {
            dom = 29;
        }
        else if (rdate.T_Year % 400 == 0) {
            dom = 29;
        }
        else
            dom = 28;

        if (tempday <= dom - 1) {//判断当前月是否是目标月，不是则月份加一，tempday减去当前月的天数
            rdate.T_Day = tempday + 1;
            tempday = 0;
        }
        else {
            tempday -= dom;
            rdate.T_Months++;
            if(tempday==0) rdate.T_Day=1;
        }
        if (rdate.T_Months > 12) {//月份进位
            rdate.T_Months = 1;
            rdate.T_Year++;
        }
    }
    date->T_Year=rdate.T_Year;
    date->T_Months=rdate.T_Months;
    date->T_Day=rdate.T_Day;
}
struct T_date* T_time() {//获取系统时间
    struct T_date* p = (struct T_date*)malloc(sizeof(struct T_date));
    Date sys=B_Time_I();
    p->T_Day = sys.B_Day;
    p->T_Months = sys.B_Months;
    p->T_Year = sys.B_Year;
	return p;
}
void T_cpytime(struct T_date* p, struct T_date* q) {
	p->T_Day = q->T_Day;
	p->T_Months = q->T_Months;
	p->T_Year = q->T_Year;
}
void T_intTochar(int n, char date[], int size) {
	while (n > 0) {
		size--;
		date[size] = n % 10 + '0';
		n = n / 10;
	}
	while (size--) {
		date[size] = '0';
	}
}
char* T_timechar() {
	char* date = (char*)malloc(10 * sizeof(char));
	int d, m, y;
    Date sys=B_Time_I();
    d = sys.B_Day;
    m = sys.B_Months;
    y = sys.B_Year;
    T_intTochar(y, date, 4);
    T_intTochar(m, date + 4, 2);
    T_intTochar(d, date + 6, 2);
	date[8] = '\0';
	return date;
}
char* B_timeTochar(int year,int month,int day){
    char* date = (char*)malloc(10 * sizeof(char));
    T_intTochar(year, date, 4);
    T_intTochar(month, date + 4, 2);
    T_intTochar(day, date + 6, 2);
    date[8] = '\0';
    return date;
}
char* B_timechar() {
    char* date = (char*)malloc(20 * sizeof(char));
    int d, m, y;
    Date sys=B_Time_I();
    d = sys.B_Day;
    m = sys.B_Months;
    y = sys.B_Year;
    T_intTochar(y, date, 4);
    date[4] = '\0';
    strcat(date, "年");
    T_intTochar(m, date + 7, 2);
    date[9] = '\0';
    strcat(date, "月");
    T_intTochar(d, date + 12, 2);
    date[14] = '\0';
    strcat(date, "日");
    date[17] = '\0';
    return date;
}
char* M_timechar() {
    char* date = (char*)malloc(10 * sizeof(char));
    int d, m, y;
    Date sys=B_Time_I();
    d = sys.B_Day;
    m = sys.B_Months;
    y = sys.B_Year;
    T_intTochar(y, date, 4);
    T_intTochar(m, date + 4, 2);
    T_intTochar(d, date + 6, 2);
    date[8] = '\0';
    return date;
}
void T_datefile(char filename[], char* date) {
//	char location[30] = "../../data/trade/";
//	strcpy(filename, location);
    filename[0]='\0';
    if (date == NULL)strcpy(filename, T_timechar());
	else strcat(filename, date);
	strcat(filename, ".txt");
}
int T_comparetime(struct T_date p, struct T_date q) {
	if (p.T_Year == q.T_Year) {
		if (p.T_Months == q.T_Months) {
			return p.T_Day <= q.T_Day;
		}
		else return p.T_Months <= q.T_Months;
	}
	else return p.T_Year <= q.T_Year;
}//返回1时 p比q小；
struct Member* M_load(struct Member* M_head,int *M_sum) {
	FILE* fp;
	char M_first;
	char T_first;
	struct Member* p;
	struct Member* q;
	struct M_trade* tp;
	struct M_trade* tq;
	p = M_head;
	q = M_head;
    fp = fopen("member.txt", "r");
	if (fp == NULL) {
        qDebug() << "打开文件失败!";
        fp = fopen("member.txt", "w");
        fprintf(fp, "#0000000000 0.0 3 5 99999999&");
        fclose(fp);
        fp = fopen("member.txt", "r");
    }else  qDebug() << "打开文件成功!";
	while (!feof(fp)) {
		M_first = fgetc(fp);
		if (M_first == '#') {
            (*M_sum)++;
			q = (struct Member*)malloc(sizeof(struct Member));
			tp = (struct M_trade*)malloc(sizeof(struct M_trade));
			fread(q->M_num, sizeof(char), 10, fp);
			q->M_num[10] = '\0';
			fscanf(fp, "%lf", &q->M_money);
            fgetc(fp);
            fscanf(fp, "%c", &q->M_sex);
            fgetc(fp);
			fscanf(fp, "%c", &q->M_level);
            fgetc(fp);
            fread(q->M_date, sizeof(char), 8, fp);
            q->M_date[8] = '\0';
			q->M_buy = tp;
			while (!feof(fp)) {
				T_first = fgetc(fp);
				if (T_first == '&') break;
				tq = (struct M_trade*)malloc(sizeof(struct M_trade));
				fscanf(fp, "%s", &tq->M_date);
                fscanf(fp, "%d", &tq->line);
                fscanf(fp, "%f", &tq->T_in);
                fscanf(fp, "%f", &tq->T_out);
				tp->next = tq;
				tp = tq;
			}
			tp->next = NULL;
			p->M_next = q;
			p = q;
		}
	}
	p->M_next = NULL;
	fclose(fp);
	return M_head;
}
void M_setlevel(struct Member* p, int mode) {
    int money = (int)p->M_money;
    char level = 'a';
    if (money == 0)//printf("注册会员");
        level = '0';
    else if (money < 2000)//printf("青铜会员");
        level = '1';
    else if (money < 10000)//printf("银牌会员");
        level = '2';
    else if (money < 30000)//printf("金牌会员");
        level = '3';
    else if (money > 30000)//printf("钻石会员");
        level = '4';
    else printf("购买金额无效！");
    if ((mode == 0 && p->M_level != level) || mode == 1) {
        p->M_level = level;
        T_date* Tdate = T_time();
        char *date=B_timeTochar(Tdate->T_Year+1,Tdate->T_Months,Tdate->T_Day);
        strcpy(p->M_date, date);
        free(date);
        free(Tdate);
    }
}
float M_level(struct Member* p) {
    if(p->M_sex=='3') return 1.0f;
	int levelchange[5] = { 0,100,500,2500,7500 };
    char *date=T_timechar();
    if (strcmp(p->M_date,date)<0) {
		printf("您的会员信息已过期已为您更新\n");
		p->M_money -= levelchange[p->M_level - '0'];
		if (p->M_money < 0) p->M_money = 0;
		M_setlevel(p, 1);
	}
    free(date);
	int money = (int)p->M_money;
	if (money == 0)//printf("注册会员");
		return 1.0f;
	else if (money < 2000)//printf("青铜会员");
		return 0.98f;
	else if (money < 10000)//printf("银牌会员");
		return 0.95f;
	else if (money < 30000)//printf("金牌会员");
		return 0.90f;
	else if (money > 30000)//printf("钻石会员");
		return 0.85f;
	else printf("购买金额无效！");
	return 1.0;
}
struct Member* M_search(char num[], struct Member* M_head) {
	struct Member* p = M_head;
	struct Member* q;
	int k;
	while (p->M_next != NULL) {
		q = p;
		p = p->M_next;
		k = strcmp(num, p->M_num);
		if (k < 0) return q;
		else if (k == 0) return p;
	}
	return p;
}
void M_add(char num[], struct Member* M_add, T_date* time,char M_sex) {
	struct Member* buffer;
	buffer = M_add->M_next;
	struct Member* q = (struct Member*)malloc(sizeof(struct Member));
	M_add->M_next = q;
	q->M_next = buffer;
	strcpy(q->M_num, num);
	q->M_money = 0.0;
	q->M_level = '0';
    q->M_sex=M_sex;
    char *date=T_timechar();
    strcpy(q->M_date,M_timechar());
    free(date);
	q->M_buy = (struct M_trade*)malloc(sizeof(struct M_trade));
	q->M_buy->next = NULL;
}
void M_delete(int M_number, struct Member* M_head,int sum) {
	struct Member* p = M_head;
    struct Member* q = M_head;
    while (M_number-- && p->M_next != NULL) {
        q = p;
		p = p->M_next;
	}
    if (M_number == -1) {
        while(p->M_next!=NULL&&sum--) p=p->M_next;
        q->M_next = p->M_next;
    }else if(M_number==0&&p->M_next==NULL){
        q->M_next=NULL;
    }else qDebug()<<"你输入的值有误\n";
	free(p);
}
void T_delete(int T_number, struct Trade* T_head,int sum) {
    struct Trade* p = T_head;
    struct Trade* q = T_head;
    while (T_number--&&p->next!=NULL) {
        q = p;
        p = p->next;
    }
    if (T_number == -1) {
        while(p->next!=NULL&&sum--) p=p->next;
        q->next = p->next;
    }else if(T_number==0&&p->next==NULL){
        q->next=NULL;
    }else qDebug()<<"你输入的值有误\n";
    free(p);
}
void T_cartdelete(int T_number, struct T_shopcart* T_carthead,int sum) {
    struct T_shopcart* p = T_carthead;
    struct T_shopcart* q = T_carthead;
    while (T_number-- && p->next != NULL) {
        q = p;
        p = p->next;
    }
    if (T_number == -1) {
        while(p->next!=NULL&&sum--) p=p->next;
        q->next = p->next;
    }else if(T_number==0&&p->next==NULL){
        q->next=NULL;
    }else qDebug()<<"你输入的值有误\n";
    free(p);
}
void T_discountdelete(int T_number, struct T_discount* T_discounthead,int sum) {
    struct T_discount* p = T_discounthead;
    struct T_discount* q = T_discounthead;
    while (T_number-- && p->next != NULL) {
        q = p;
        p = p->next;
    }
    if (T_number == -1) {
        while(p->next!=NULL&&sum--) p=p->next;
        q->next = p->next;
    }else if(T_number==0&&p->next==NULL){
        q->next=NULL;
    }else qDebug()<<"你输入的值有误\n";
    free(p);
}
int M_save(struct Member* M_head) {
	FILE* fp;
	struct Member* p = M_head;
	struct M_trade* tp;
    fp = fopen("member.txt", "w");
	if (fp == NULL) {
        qDebug() << "打开文件失败!";
		return 1;
	}
    fprintf(fp, "#0000000000 0.0 3 5 99999999&\n");
	while (p->M_next != NULL) {
		p = p->M_next;
		fputc('#', fp);
		fwrite(p->M_num, sizeof(char), 10, fp);
        fprintf(fp, " %.2lf", p->M_money);
        fprintf(fp, " %c", p->M_sex);
		fprintf(fp, " %c", p->M_level);
        fprintf(fp, " %s", p->M_date);
		tp = p->M_buy;
		if (tp != NULL) {
			while (tp->next != NULL) {
				tp = tp->next;
				fprintf(fp, " %s", tp->M_date);
                fprintf(fp, " %d", tp->line);
                fprintf(fp, " %.2f", tp->T_in);
                fprintf(fp, " %.2f", tp->T_out);
			}
		}
		fputc('&', fp);
		fputc('\n', fp);
	}
	fclose(fp);
	return 0;
}
int T_discountsave(struct T_discount* T_discounthead) {
    FILE* fp;
    struct T_discount* p = T_discounthead;
    struct M_trade* tp;
    fp = fopen("discount.txt", "w");
    if (fp == NULL) {
        qDebug() << "打开文件失败!";
        return 1;
    }
    while (p->next != NULL) {
        p = p->next;
        fputc('#', fp);
        fprintf(fp, "%s", p->T_num);
        fprintf(fp, " %c", p->T_batch);
        fprintf(fp, " %s", p->T_name);
        fprintf(fp, " %.2lf", p->T_in);
        fprintf(fp, " %.2lf", p->T_sell);
        fprintf(fp, " %.2f", p->T_discount);
        fprintf(fp, " %.2f", p->T_out);
        fputc('\n', fp);
    }
    fclose(fp);
    return 0;
}
void T_disload(struct T_discount* T_head) {//优惠券加载系统
    FILE* fp;
    char T_first;
    struct T_discount* p;
    struct T_discount* q;
    p = T_head;
    q = T_head;
    fp = fopen("discount.txt", "r");
    if (fp == NULL) {
        qDebug() << "当日无交易记录!";
        fp = fopen("discount.txt", "w");
        fclose(fp);
        T_head->next = NULL;
    }else{
        while (!feof(fp)) {
            T_first = fgetc(fp);
            if (T_first == '#') {
                q = (struct T_discount*)malloc(sizeof(struct T_discount));
                fscanf(fp, "%s", q->T_num);
                fgetc(fp);
                fscanf(fp, "%c", &q->T_batch);
                fscanf(fp, "%s", q->T_name);
                fscanf(fp, "%f", &q->T_in);
                fscanf(fp, "%f", &q->T_sell);
                fscanf(fp, "%f", &q->T_discount);
                fscanf(fp, "%f", &q->T_out);
                p->next = q;
                p = q;
            }
        }
        p->next = NULL;
        fclose(fp);
    }
}
void T_display(struct Trade* trade) {
	int i = 1;
	struct T_goods* p = trade->point;
	printf("商品清单：\n");
	while (p->next != NULL) {
		p = p->next;
        printf("%d:%s %d %.2f %.2f\n", i++, p->T_num, p->T_amount, p->T_in, p->T_out);
	}
	printf("进价：%.1lf\n消费金额：%.1lf\n", trade->T_in, trade->T_out);
}
struct T_discount* T_discout(char num[],char batch) {
    struct T_discount* p = B_discounthead;
    struct T_discount* buffer;
    while (p->next != NULL) {
        buffer = p;
        p = p->next;
        if (strcmp(num, p->T_num)==0){
           if(p->T_batch==batch) return buffer;
        }
	}
    return nullptr;
}
void T_recordadd(struct M_trade* mp, int T_sum,float T_cost,float T_money) {
	struct M_trade* tp = (struct M_trade*)malloc(sizeof(struct M_trade));
	struct M_trade* tq = mp->next;
	mp->next = tp;
	tp->next = tq;
	tp->line = T_sum;
    tp->T_in = T_cost;
    tp->T_out = T_money;
	strcpy(tp->M_date, T_timechar());
}
struct Trade* T_load(struct Trade* T_head, int* T_sum) {
	FILE* fp;
	char M_first;
	char G_first;
	struct Trade* p;
	struct Trade* q;
	struct T_goods* gp;
	struct T_goods* gq;
	p = T_head;
	q = T_head;
	int mode = 0;
	char* filename = (char*)malloc(40 * sizeof(char));;
	T_datefile(filename, NULL);
	fp = fopen(filename, "r");
	if (fp == NULL) {
        qDebug() << "当日无交易记录!";
		fp = fopen(filename, "w");
		fclose(fp);
		T_head->next = NULL;
		return T_head;
	}
	while (!feof(fp)) {
		M_first = fgetc(fp);
		if (M_first == '#') {
			(*T_sum)++;
			q = (struct Trade*)malloc(sizeof(struct Trade));
			gp = (struct T_goods*)malloc(sizeof(struct T_goods));
			fread(q->M_num, sizeof(char), 10, fp);
			q->M_num[10] = '\0';
            fscanf(fp, "%s", &q->date);
			fscanf(fp, "%f", &q->T_in);
			fscanf(fp, "%f", &q->T_out);
			q->point = gp;
			while (!feof(fp)) {
				G_first = fgetc(fp);
				if (G_first == '&') break;
				mode = 1;//该商品交易下有记录
				gq = (struct T_goods*)malloc(sizeof(struct T_goods));
				fscanf(fp, "%d", &gq->T_amount);
				fscanf(fp, "%s", &gq->T_num);
				fscanf(fp, "%f", &gq->T_in);
				fscanf(fp, "%f", &gq->T_out);
				gp->next = gq;
				gp = gq;
			}
			if (mode == 0)q->point = NULL;
			gp->next = NULL;
			p->next = q;
			p = q;
		}
	}
	p->next = NULL;
	fclose(fp);
	free(filename);
	return p;
}
int T_save(struct Trade* T_head) {
	FILE* fp;
	struct Trade* p;
	struct T_goods* gp;
    p = T_head;
	char* filename = (char*)malloc(40 * sizeof(char));;
	T_datefile(filename, NULL);
    fp = fopen(filename, "a");
	if (fp == NULL) {
        fp = fopen(filename, "w");
        qDebug() << "当日交易记录已自动创建";
	}
    free(filename);
	while (p->next != NULL) {
		p = p->next;
		fputc('#', fp);
		fwrite(p->M_num, sizeof(char), 10, fp);
        fprintf(fp, " %s", p->date);
        fprintf(fp, " %.2f", p->T_in);
        fprintf(fp, " %.2f", p->T_out);
		gp = p->point;
		if (gp != NULL) {
			while (gp->next != NULL) {
				gp = gp->next;
				fprintf(fp, " %d", gp->T_amount);
                fprintf(fp, " %s", gp->T_num);
                fprintf(fp, " %.2f", gp->T_in);
                fprintf(fp, " %.2f", gp->T_out);
			}
		}
		fputc('&', fp);
		fputc('\n', fp);
	}
	fclose(fp);
	free(filename);
	return 0;
}
