//
// Created by Charon on 2020/4/25.6666
//6666

#ifndef B_TIME_B_TIME_H
#define B_TIME_B_TIME_H
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


void IntToChar(int x, char* Str);//转换int到字符串
Date_C B_Time_C(void) ;//返回字符串型时间的函数
Date B_Time_I(void) ;//返回整型时间的函数
int B_DateIsSmall(Date x, Date y) ;//比较两个时间，x<y?1:0 ;
int B_DayPassed(Date pre, Date lat);//计算两个日期间的天数，如有数据错误，返回-1；


#endif //B_TIME_B_TIME_H
