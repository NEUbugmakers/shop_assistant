#pragma once
#include<stdio.h>
#include<stdlib.h>
void B_getSrc(FILE* file, void* pos,size_t size) {//根据数据类型大小从文件中读取数据
	for (int i = 0; i < size; i++)
		((char*)pos)[i] = fgetc(file);
	return;
}
