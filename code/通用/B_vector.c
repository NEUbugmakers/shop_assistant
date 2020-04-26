#include "vector.h"
B_vector* B_vectorCreat(size_t esize) {//创建B_vector,成功返回指针，失败返回NULL;
	B_vector* vector = (B_vector*)malloc(sizeof(B_vector));
	if (vector == NULL)//内存申请失败处理
		return NULL;
	vector->_elem = (char*)malloc(sizeof(char) * esize * B_Vector_DEFAULT_CAPACITY);
	if (vector->_elem == NULL)//内存申请失败处理
	{
		free(vector);
		return NULL;
	}
	vector->_esize = esize;
	vector->_size = 0;
	vector->_capicity = B_Vector_DEFAULT_CAPACITY;
	return vector;
}
void vectorcpyRank(B_vector* vector, char* _Dst, Rank _Dstr, char* _Src, Rank _Srcr) {//将_Src的元素拷贝到_Dst上
	memcpy(_Dst + _Dstr * vector->_esize, _Src + _Srcr * vector->_esize, vector->_esize);
}
int B_vectorExpand(B_vector* vector) {//有必要时扩容，返回值1表示运行正常（无需扩容或已成功扩容），0表示需要扩容但扩容失败
	if (vector->_size < vector->_capicity) return 1;//尚未满员，无需扩容
	size_t oldcapicity = vector->_capicity;
	if (vector->_capicity < B_Vector_DEFAULT_CAPACITY) vector->_capicity = B_Vector_DEFAULT_CAPACITY;//不低于最小容量；
	char* oldElem = vector->_elem;
	vector->_elem = (char*)malloc(sizeof(char) * vector->_esize * (vector->_capicity <<= 1));
	if (vector->_elem == NULL) {//内存申请失败处理
		vector->_elem = oldElem;
		vector->_capicity = oldcapicity;
		return 0;
	}
	if (oldElem != NULL)//capacity==0的时候的扩容不需要复制
		for (int i = 0; i < vector->_size; i++)
			vectorcpyRank(vector, vector->_elem, i, oldElem, i);
	free(oldElem);
	return 1;
}
int B_vectorInsert(B_vector* vector, const void* ve, Rank r) {//将新元素作为秩为r元素插入，成功返回位置，失败返回-1；
	char* e = (char*)ve;
	if (!B_vectorExpand(vector)) return -1;//扩容检测异常，插入失败；
	for (int i = vector->_size; i > r; i--)
		vectorcpyRank(vector, vector->_elem, i, vector->_elem, i-1);
	vectorcpyRank(vector, vector->_elem, r, e, 0);
	vector->_size++;
	return r;
}
int B_vectorPushBack(B_vector* vector, const void* ve) {//将新元素作为末尾元素插入，成功返回位置，失败返回-1；
	const char* e = (char*)ve;
	return B_vectorInsert(vector, e, vector->_size);
}
int B_vectorShrink(B_vector* vector) {//有必要时缩容，返回值1表示运行正常（无需缩容或已成功缩容），0表示需要缩容但缩容失败
	if (vector->_capicity <= B_Vector_DEFAULT_CAPACITY) return 1;//不致收缩到B_Vector_DEFAULT_CAPACITY以下
	if (vector->_size << 2 > vector->_capicity) return 1;//以25%为界
	char* oldElem = vector->_elem;
	int oldcapicity = vector->_capicity;
	vector->_elem = (char*)malloc(sizeof(char) * vector->_esize * (vector->_capicity >>= 1));
	if (vector->_elem == NULL) {//内存申请失败处理
		vector->_elem = oldElem;
		vector->_capicity = oldcapicity;
		return 0;
	}
	for (int i = 0; i < vector->_size; i++)
		vectorcpyRank(vector, vector->_elem, i, oldElem, i);
	free(oldElem);
	return 1;
}
int B_vectorRemoveInteral(B_vector* vector, Rank lo, Rank hi) {//删除区间[lo,hi),成功返回删除元素个数,异常则返回-1
	if (lo == hi) return 0;//出于效率考虑，单独处理退化情况，比如remove（0,0）
	while (hi < vector->_size)
		vectorcpyRank(vector, vector->_elem, lo++, vector->_elem, hi++);
	vector->_size = lo;
	if (!B_vectorShrink(vector))//缩容异常处理
		return -1;
	return hi - lo;
}
int B_vectorRemoveRank(B_vector* vector, Rank r) {//删除秩为r的元素，成功返回1，失败返回0；
	return B_vectorRemoveInteral(vector, r, r + 1) != -1;//删除并返回删除结果
}
char* B_vectorGet(B_vector* vector, Rank r) {//得到指定位置的元素,支持对元素修改
	return vector->_elem + r * vector->_esize;
}
void B_vectorClear(B_vector* vector) {//清空数据
	free(vector->_elem);
	vector->_size = 0;
	vector->_capicity = 0;
	vector->_elem = NULL;
}
