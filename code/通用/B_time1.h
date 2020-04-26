//
// Created by Charon on 2020/4/25.
//

#ifndef B_TIME_B_TIME1_H
#define B_TIME_B_TIME1_H
#include <stdio.h>
#include <time.h>
#include <math.h>
#define isPrime(year) ((year%4==0&&year%100!=0)||(year%400==0))


typedef struct {
    char B_Day[4];
    char B_Year[4];
    char B_Months[4];
    char B_Hour[4];
    char B_Minute[4];
    char B_Second[4];
} Date_C;

typedef struct {
    int B_Day;
    int B_Year;
    int B_Months;
    int B_Hour;
    int B_Minute;
    int B_Second;
} Date;


void IntToChar(int x,char *Str)//转换int到字符串
{
    int t;
    char *Ptr,Buf[5];
    int i = 0;
    Ptr = Str;
    if(x < 10)  // 当整数小于10，转换为0x格式,左边添0
    {
        (*Ptr ++) = '0';
        (*Ptr ++) = x+0x30;//0x30 == '0';
    }
    else
    {
        while(x > 0)
        {
            t = x % 10;
            x = x / 10;
            Buf[i++] = t+0x30; // （0x30 == '0';）
        }
        i -- ;
        for(;i >= 0;i --)   // 将得到的字符串倒序
        {
            *(Ptr++) = Buf[i];
        }
    }
    *Ptr = '\0';
}





Date_C B_Time_C(void){//返回字符串型时间的函数
    time_t  timep;
    Date_C date;
    struct tm *p ;
    time(&timep);//获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//取得从1900年开始的时间
    IntToChar(1900+p->tm_year,date.B_Year);//year
    IntToChar(1+p->tm_mon,date.B_Months);//months
    IntToChar(1+p->tm_mday,date.B_Day);//day
    IntToChar(1+p->tm_hour,date.B_Hour);//hour
    IntToChar(1+p->tm_min,date.B_Minute);//minute
    IntToChar(1+p->tm_sec,date.B_Second);//second

    return date;
}

Date B_Time_I(void){//返回整型时间的函数
    time_t  timep;
    Date date;
    struct tm *p ;
    time(&timep);//获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//取得从1900年开始的时间
    date.B_Year = 1900+p->tm_year;//year
    date.B_Months = 1+p->tm_mon;//months
    date.B_Day = 1+p->tm_mday;//day
    date.B_Hour = 1+p->tm_hour;//hour
    date.B_Minute = 1+p->tm_min;//minute
    date.B_Second = 1+p->tm_sec;//second

    return date;
}

int B_DateIsSmall(Date x,Date y){//比较两个时间，x<y?1:0 ;
    if (x.B_Year<y.B_Year){
        return 1;
    } else if (x.B_Year>y.B_Year){
        return 0;
    } else if (x.B_Year==y.B_Year){
        if (x.B_Months<y.B_Months){
            return 1;
        }else if (x.B_Months>y.B_Months){
            return 0;
        }else if (x.B_Months == y.B_Months){
            if (x.B_Day < y.B_Day){
                return 1;
            }else if (x.B_Day > y.B_Day){
                return 0;
            }else if (x.B_Day == y.B_Day){
                return 1;
            }
        }
    }

}
int B_DayPassed(Date pre, Date lat){//计算两个日期间的天数，如有数据错误，返回-1；
    int y2, m2, d2;
    int y1, m1, d1;
    if (B_DateIsSmall(lat,pre)){
        return -1;
    }else {
        m1 = (pre.B_Months + 9) % 12;
        y1 = pre.B_Year - m1 / 10;
        d1 = 365 * y1 + y1 / 4 - y1 / 100 + y1 / 400 + (m1 * 306 + 5) / 10 + (pre.B_Day - 1);

        m2 = (lat.B_Months + 9) % 12;
        y2 = lat.B_Year - m2 / 10;
        d2 = 365 * y2 + y2 / 4 - y2 / 100 + y2 / 400 + (m2 * 306 + 5) / 10 + (lat.B_Day - 1);

        return (d2 - d1);
    }
}


#endif //B_TIME_B_TIME1_H
