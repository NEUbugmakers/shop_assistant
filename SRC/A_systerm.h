
#ifndef C_SRC_A_SYSTERM_H
#define C_SRC_A_SYSTERM_H

#include "WP_file.h"

typedef struct _A_account A_account;


struct _A_account {
    int A_num;//工号
    char A_password[8];//密码，六位
    int A_right;//权限标识，0是管理员，1是员工。
    A_account* next;
};

int A_signUp(A_account* type,A_account * systerm);//用户登录验证，成功返回1，失败或结构体为空返回0

A_account *  A_readFile();//读取系统文件的管理员的账号密码;

int account(int  i);//生成密匙

#endif //C_SRC_A_SYSTERM_H
