#define _CRT_SECURE_MO_WARNINGS
#define SR_BTNODE_NAME_LEN 50
#include<stdio.h>
#include<stdlib.h>

#include"C_goods.h"
#include"B_list.h"
#include"B_vector.h"
#include"B_getSrc.h"

typedef struct SR_dataBTNodeT  SR_dataBTNode;
struct SR_dataBTNodeT {//B-Ê÷½Úµã
	SR_dataBTNode* parent;
	char name[SR_BTNODE_NAME_LEN];
	B_list* child;
	B_list* childCode;
	FILE* file;
};
SR_dataBTNode SR_dataBTRoot;
void SR_dataBTOpen() {
	FILE* file = fopen("root", "ab+");
	SR_dataBTNode* x = (SR_dataBTNode*)malloc(sizeof(SR_dataBTNode));
	B_getSrc(file, x,sizeof(SR_dataBTNode));
}
