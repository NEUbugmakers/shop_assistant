#pragma once
#include<stdio.h>
#include<stdlib.h>
int B_getSrc(FILE* file, void* pos,size_t size) {//根据数据类型大小从文件中读取数据,返回读取的大小，文件末尾返回EOF
	for (int i = 0; i < size; i++)
		if ((((char*)pos)[i] = fgetc(file)) == EOF) {
			return EOF;
		}
	return size;
}
