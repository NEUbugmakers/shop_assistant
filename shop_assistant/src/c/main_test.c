#define _CRT_SECURE_NO_WARNINGS
#include"B_list.h"
#include<stdio.h>
struct test {
	int a;
	int b;
	int c;
};
int main() {
	struct test t;
	t.a = 1;
	t.b = 2;
	t.c = 3;
	B_list* list = B_listCreat(sizeof(t));
	B_listPushBack(list,&t);
	struct test* cpy = B_listGetFirst(list);
	printf("%d\n", cpy->b);
	B_listRemoveFirst(list);
	printf("%d", cpy->c);
}
