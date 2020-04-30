#pragma once
#include<stdio.h>
#include<stdlib.h>
char* B_getSrc(FILE* file, size_t size) {//根据数据类型大小从文件中读取数据
	char* _src = (char*)malloc(size);
	for (int i = 0; i < size; i++)
		_src[i] = fgetc(file);
	return _src;
}
