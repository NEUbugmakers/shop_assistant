//
// Created by Charon on 2020/5/7.
//

#define _CRT_SECURE_NO_WARNINGS
#include "B_time.h"
#define isPrime(year) ((year%4==0&&year%100!=0)||(year%400==0))



void IntToChar(int x, char* Str)//转换int到字符串
{
    int t;
    char* Ptr, Buf[5];
    int i = 0;
    Ptr = Str;
    if (x < 10)  // 当整数小于10，转换为0x格式,左边添0
    {
        (*Ptr++) = '0';
        (*Ptr++) = x + 0x30;//0x30 == '0';
    }
    else
    {
        while (x > 0)
        {
            t = x % 10;
            x = x / 10;
            Buf[i++] = t + 0x30; // （0x30 == '0';）
        }
        i--;
        for (; i >= 0; i--)   // 将得到的字符串倒序
        {
            *(Ptr++) = Buf[i];
        }
    }
    *Ptr = '\0';
}
Date_C B_Time_C(void) {//返回字符串型时间的函数
    time_t  timep;
    Date_C date;
    struct tm* p;
    time(&timep);//获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//取得从1900年开始的时间
    IntToChar(1900 + p->tm_year, date.B_Year);//year
    IntToChar(1 + p->tm_mon, date.B_Months);//months
    IntToChar(1 + p->tm_mday, date.B_Day);//day
    IntToChar(1 + p->tm_hour, date.B_Hour);//hour
    IntToChar(1 + p->tm_min, date.B_Minute);//minute
    IntToChar(1 + p->tm_sec, date.B_Second);//second

    return date;
}

Date B_Time_I(void) {//返回整型时间的函数
    time_t  timep;
    Date date;
    struct tm* p;
    time(&timep);//获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//取得从1900年开始的时间
    date.B_Year = 1900 + p->tm_year;//year
    date.B_Months = 1 + p->tm_mon;//months
    date.B_Day = 1 + p->tm_mday;//day
    date.B_Hour = 1 + p->tm_hour;//hour
    date.B_Minute = 1 + p->tm_min;//minute
    date.B_Second = 1 + p->tm_sec;//second

    return date;
}

int B_DateIsSmall(Date x, Date y) {//比较两个时间，x<y?1:0 ;
    if (x.B_Year < y.B_Year) {
        return 1;
    }
    else if (x.B_Year > y.B_Year) {
        return 0;
    }
    else if (x.B_Year == y.B_Year) {
        if (x.B_Months < y.B_Months) {
            return 1;
        }
        else if (x.B_Months > y.B_Months) {
            return 0;
        }
        else if (x.B_Months == y.B_Months) {
            if (x.B_Day < y.B_Day) {
                return 1;
            }
            else if (x.B_Day > y.B_Day) {
                return 0;
            }
            else if (x.B_Day == y.B_Day) {
                return 1;
            }
        }
    }

}
int B_DayPassed(Date pre, Date lat) {//计算两个日期间的天数，如有数据错误，返回-1；
    int y2, m2, d2;
    int y1, m1, d1;
    if (B_DateIsSmall(lat, pre)) {
        return -1;
    }
    else {
        m1 = (pre.B_Months + 9) % 12;
        y1 = pre.B_Year - m1 / 10;
        d1 = 365 * y1 + y1 / 4 - y1 / 100 + y1 / 400 + (m1 * 306 + 5) / 10 + (pre.B_Day - 1);

        m2 = (lat.B_Months + 9) % 12;
        y2 = lat.B_Year - m2 / 10;
        d2 = 365 * y2 + y2 / 4 - y2 / 100 + y2 / 400 + (m2 * 306 + 5) / 10 + (lat.B_Day - 1);

        return (d2 - d1);
    }
}
/////个人新增
void B_DateSet(Date* date, int y, int m, int d) {
    date->B_Year = y;
    date->B_Months = m;
    date->B_Day = d;
}
Date B_DayLater(Date* date, int day) {
    int dpm[] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//每个月的天数
    int tempday = date->B_Day + day - 1;//自当前月的一号起算需经过多少天
    int dom;//当前月的天数
    Date rdate = *date;
    while (tempday > 0) {
        if (rdate.B_Months != 2)//j计算当前月有多少天
            dom = dpm[rdate.B_Months - 1];
        else if (rdate.B_Year % 4 == 0 && rdate.B_Year % 100 != 0) {
            dom = 29;
        }
        else if (rdate.B_Year % 400 == 0) {
            dom = 29;
        }
        else
            dom = 28;

        if (tempday <= dom - 1) {//判断当前月是否是目标月，不是则月份加一，tempday减去当前月的天数
            rdate.B_Day = tempday + 1;
            tempday = 0;
        }
        else {
            tempday -= dom;
            rdate.B_Months++;
        }
        if (rdate.B_Months > 12) {//月份进位
            rdate.B_Months = 1;
            rdate.B_Year++;
        }
    }
    return rdate;
}


int B_WP_dayCompare(Date x,Date y) { //只比较年和月，相等返回1，x>y?-2:2  （数值大小）
    if ((x.B_Year==y.B_Year)&&(x.B_Months==y.B_Months)){
        return 1;
    }
    if (x.B_Year>y.B_Year){
        return -2;
    }else if ((x.B_Year==y.B_Year)&&(x.B_Months>y.B_Months)){
        return -2;
    }else if ((x.B_Year==y.B_Year)&&(x.B_Months<y.B_Months)){
        return 2;
    }else if (x.B_Year<y.B_Year){
        return -2;
    }
}
char* B_DateToStr(Date* date ,char c) {//Date转Str
    int len;
    if (c == 0)
        len = 9;
    else
        len = 11;
    char* str = (char*)malloc(sizeof(char) * len);
    int pos = 0;
    for (int i = 1000; i >0; i/=10) {
        str[pos++] = date->B_Year / i % 10 + '0';
    }
    if (c != 0)
        str[pos++] = c;
    for (int i = 10; i > 0; i /= 10) {
        str[pos++] = date->B_Months / i % 10 + '0';
    }
    if (c != 0)
        str[pos++] = c;
    for (int i = 10; i > 0; i /= 10) {
        str[pos++] = date->B_Day / i % 10 + '0';
    }
    str[pos++] = 0;
    return str;
}
