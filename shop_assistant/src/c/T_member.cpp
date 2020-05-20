#include "T_member.h"
struct T_discount T_discounthead;
struct C_price* SR_datasell(char T_num[], char T_batch, int T_amount) {
    struct C_price* price = (struct C_price*)malloc(sizeof(struct C_price));
    price->C_in=1.0;
    price->C_out=1.0;
    return price;
}
struct T_date* T_time() {//获取系统时间 
	struct T_date* p = (struct T_date*)malloc(sizeof(struct T_date));
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	p->T_Day = sys.wDay;
	p->T_Months = sys.wMonth;
	p->T_Year = sys.wYear;
	return p;
}
void T_cpytime(struct T_date* p, struct T_date* q) {
	p->T_Day = q->T_Day;
	p->T_Months = q->T_Months;
	p->T_Year = q->T_Year;
}
void intTochar(int n, char date[], int size) {
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
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	d = sys.wDay;
	m = sys.wMonth;
	y = sys.wYear;
	intTochar(y, date, 4);
	intTochar(m, date + 4, 2);
	intTochar(d, date + 6, 2);
	date[8] = '\0';
	return date;
}
char* M_timechar() {
    char* date = (char*)malloc(10 * sizeof(char));
    int d, m, y;
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    d = sys.wDay;
    m = sys.wMonth;
    y = sys.wYear+1;
    intTochar(y, date, 4);
    intTochar(m, date + 4, 2);
    intTochar(d, date + 6, 2);
    date[8] = '\0';
    return date;
}
void T_datefile(char filename[], char* date) {
	char location[30] = "../../data/trade/";
	strcpy(filename, location);
	if (date == NULL)strcat(filename, T_timechar());
	else strcat(filename, date);
	strcat(filename, ".txt");
}
int T_comparetime(struct T_date p, struct T_date q) {
	if (p.T_Year == q.T_Year) {
		if (p.T_Months == q.T_Months) {
			return p.T_Day < q.T_Day;
		}
		else return p.T_Months < q.T_Months;
	}
	else return p.T_Year < q.T_Year;
}//返回1时 p比q大； 
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
	int mode = 0;
	fp = fopen("../../data/member/member.txt", "r");
	if (fp == NULL) {
        qDebug() << "打开文件失败!";
		return NULL;
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
				mode = 1;//该会员有商品购买记录
				tq = (struct M_trade*)malloc(sizeof(struct M_trade));
				fscanf(fp, "%s", &tq->M_date);
                fscanf(fp, "%d", &tq->line);
                fscanf(fp, "%f", &tq->T_in);
                fscanf(fp, "%f", &tq->T_out);
				tp->next = tq;
				tp = tq;
			}
			tp->next = NULL;
			if (mode == 0)q->M_buy = NULL;
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
        char *date=T_timechar();
        strcpy(p->M_date, date);
        free(date);
	}
}
float M_level(struct Member* p) {
	int levelchange[5] = { 0,100,500,2500,7500 };
    char *date=T_timechar();
    if (strcmp(p->M_date,date)) {
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
;	strcpy(q->M_date,M_timechar());
    free(date);
	q->M_buy = (struct M_trade*)malloc(sizeof(struct M_trade));
	q->M_buy->next = NULL;

}
void M_delete(int M_number, struct Member* M_head) {
	struct Member* p = M_head;
	while (p->M_next != NULL && M_number--) {
		M_head = p;
		p = p->M_next;
	}
    if (M_number == -1) {
		M_head->M_next = p->M_next;
	}
	else printf("你输入的值有误\n");
	free(p);
}
int M_save(struct Member* M_head) {
	FILE* fp;
	struct Member* p = M_head;
	struct M_trade* tp;
	fp = fopen("../../data/member/member.txt", "w");
	if (fp == NULL) {
        qDebug() << "打开文件失败!";
		return 1;
	}
	while (p->M_next != NULL) {
		p = p->M_next;
		fputc('#', fp);
		fwrite(p->M_num, sizeof(char), 10, fp);
		fprintf(fp, " %.1lf", p->M_money);
        fprintf(fp, " %c", p->M_sex);
		fprintf(fp, " %c", p->M_level);
        fprintf(fp, " %a", p->M_date);
		tp = p->M_buy;
		if (tp != NULL) {
			while (tp->next != NULL) {
				tp = tp->next;
				fprintf(fp, " %s", tp->M_date);
                fprintf(fp, " %d", tp->line);
                fprintf(fp, " %.1f", tp->T_in);
                fprintf(fp, " %.1f", tp->T_out);
			}
		}
		fputc('&', fp);
		fputc('\n', fp);
	}
	fclose(fp);
	return 0;
}
void T_disload() {//优惠券加载系统
	T_discounthead.next = NULL;
}
void T_display(struct Trade* trade) {
	int i = 1;
	struct T_goods* p = trade->point;
	printf("商品清单：\n");
	while (p->next != NULL) {
		p = p->next;
		printf("%d:%s %d %.1f %.1f\n", i++, p->T_num, p->T_amount, p->T_in, p->T_out);
	}
	printf("进价：%.1lf\n消费金额：%.1lf\n", trade->T_in, trade->T_out);
}
float T_discout(char num[]) {
	struct T_discount* p = &T_discounthead;
	while (p->next != NULL) {
		p = p->next;
		if (strcmp(num, p->num))return p->discount;
	}
	return 1.0;
}
void T_recordadd(struct M_trade* mp, int T_sum) {
	struct M_trade* tp = (struct M_trade*)malloc(sizeof(struct M_trade));
	struct M_trade* tq = mp->next;
	mp->next = tp;
	tp->next = tq;
	tp->line = T_sum;
	strcpy(tp->M_date, T_timechar());
}
int T_settle(struct Member* mp, struct Trade* T_head, float discount, int T_sum) {
	char T_num[19], T_batch;
	int T_amount;
	float T_money = 0.0, T_cost = 0.0, G_discount;
	struct Trade* tp = (struct Trade*)malloc(sizeof(struct Trade));
	struct T_goods* gp = (struct T_goods*)malloc(sizeof(struct T_goods));
	struct T_goods* G_next;
	struct C_price* pp;
	T_head->next = tp;
	tp->next = NULL;
	tp->point = gp;
	strcpy(tp->M_num, mp->M_num);
    char *date=T_timechar();
    strcpy(tp->date, date);
    free(date);
	while (scanf("%s", T_num) != EOF) {
		if (strcmp(T_num, "END") == 0) {
			break;
		}
		else {
			getchar();
			scanf("%c", &T_batch);
			scanf("%d", &T_amount);
			pp = SR_datasell(T_num, T_batch, T_amount);
			if (pp == NULL) {
				printf("输入的商品编码有误,请重新输入\n");
			}
			else {
				G_discount = T_discout(T_num) * discount;
				G_next = (struct T_goods*)malloc(sizeof(struct T_goods));
				strcpy(G_next->T_num, T_num);
				G_next->T_amount = T_amount;
				G_next->T_in = pp->C_in * T_amount;
				G_next->T_out = pp->C_out * T_amount * G_discount;
				gp->next = G_next;
				gp = G_next;
                free(pp);
			}
		}
		printf("进价：%.1f售价：%.1f\n", gp->T_in, gp->T_out);
		T_cost += gp->T_in;
		T_money += gp->T_out;
	}
	gp->next = NULL;
	mp->M_money += T_money;
	tp->T_in = T_cost;
	tp->T_out = T_money;
	M_setlevel(mp, 0);
	T_recordadd(mp->M_buy, T_sum + 1);
	printf("今日第%d笔交易 总进价：%.1f 总售价:%.1f", T_sum + 1, T_cost, T_money);
    return 0;
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
int T_save(struct Trade* M_head) {
	FILE* fp;
	struct Trade* p;
	struct T_goods* gp;
	p = M_head;
	char* filename = (char*)malloc(40 * sizeof(char));;
	T_datefile(filename, NULL);
	fp = fopen(filename, "w");
	if (fp == NULL) {
        qDebug() << "当日交易记录自动创建失败!";
	}
	while (p->next != NULL) {
		p = p->next;
		fputc('#', fp);
		fwrite(p->M_num, sizeof(char), 10, fp);
        fprintf(fp, " %s", p->date);
		fprintf(fp, " %.1f", p->T_in);
		fprintf(fp, " %.1f", p->T_out);
		gp = p->point;
		if (gp != NULL) {
			while (gp->next != NULL) {
				gp = gp->next;
				fprintf(fp, " %d", gp->T_amount);
				fprintf(fp, " 1%s", gp->T_num);
				fprintf(fp, " %.1f", gp->T_in);
				fprintf(fp, " %.1f", gp->T_out);
			}
		}
		fputc('&', fp);
		fputc('\n', fp);
	}
	fclose(fp);
	free(filename);
	return 0;
}
void T_delete(int T_number, struct Trade* T_head) {
	struct Trade* p = T_head;
	struct Trade* q = T_head;
	while (T_number--) {
		T_head = p;
		p = p->next;
	}
	T_head->next = p->next;
	free(p);
}
