//
// Created by Charon on 2020/4/25
//
#define _CRT_SECURE_NO_WARNINGS
#ifndef B_TIME_B_TIME_H
#define B_TIME_B_TIME_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
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

void intTochar(int n, char date[], int size);
char* B_timechar(Date a);//返回字符串时间
void IntToChar(int x, char* Str);//转换int到字符串
Date_C B_Time_C(void) ;//返回字符串型时间的函数
Date B_Time_I(void) ;//返回整型时间的函数
int B_DateIsSmall(Date x, Date y) ;//比较两个时间，x<y?1:0 ;
int B_DayPassed(Date pre, Date lat);//计算两个日期间的天数，如有数据错误，返回-1；
///////////////////
void B_DateSet(Date* date, int y, int m, int d);//设定日期，个人新增
Date B_DayLater(Date* date, int day);//几天后的日期
char* B_DateToStr(Date* date, char c);//Date转Str
#endif //B_TIME_B_TIME_H
