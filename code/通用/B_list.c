#include"list.h"
B_list* B_listCreat(size_t esize) {//创建B_list，需指定元素内存大小
	B_list* list = malloc(sizeof(B_list));
	list->_esize = esize;
	B_listInit(list);
	return list;
}
void B_listInit(B_list* list) {//初始化B_list
	list->header = malloc(sizeof(B_listNode));
	list->trailer = malloc(sizeof(B_listNode));
	list->header->_elem = NULL;
	list->trailer->_elem = NULL;
	list->header->pred = NULL;
	list->header->succ = list->trailer;
	list->trailer->pred = list->header;
	list->trailer->succ = NULL;
	list->_size = 0;
	return list;
}
void B_listClear(B_list* list) {//清空列表
	B_listNode* x = list->trailer->pred->pred;
	while (x != list->header) {//从后向前释放所有元素
		free(x->succ->_elem);
		free(x->succ);
	}
	free(list->header);
	free(list->trailer);
	list->header = NULL;
	list->trailer = NULL;
}
void B_listPushBack(B_list* list, const void* ve) {//插入元素到列表尾部
	if (list->header == NULL)//如果是清空过的列表则需要初始化
		B_listInit(list);
	B_listNode* x = malloc(sizeof(B_listNode));
	x->_elem = malloc(list->_esize);
	memcpy(x->_elem, ve, list->_esize);
	x->pred = list->trailer->pred;
	x->succ = list->trailer;
	x->pred->succ = x;
	x->succ->pred = x;
	list->_size++;
	return;
}
void B_listPushFirst(B_list* list, const void* ve) {//插入元素到列表头部
	B_listNode* x = malloc(sizeof(B_listNode));
	x->_elem = malloc(list->_esize);
	memcpy(x->_elem, ve, list->_esize);
	x->pred = list->header;
	x->succ = list->header->succ;
	x->pred->succ = x;
	x->succ->pred = x;
	list->_size++;
	return;
}
void B_listInsertRank(B_list* list,const void* e, Rank r) {//插入元素到指定位置
	B_listNode* x = malloc(sizeof(B_listNode));
	B_listNode* dst = B_listGetNodeRank(list, r);//获取插入位置
	memcpy(x->_elem, e, list->_size);
	x->succ = dst;
	x->pred = dst->pred;
	x->pred->succ = x;
	x->succ->pred = x;
}
void B_listRemoveFirst(B_list* list) {//删除首元素
	list->header->succ = list->header->succ->succ;//头哨兵的后继节点指向首节点的后继节点
	free(list->header->succ->pred->_elem);//头哨兵通过现在的后继节点指向前驱节点的指针释放首元素
	free(list->header->succ->pred);
	list->header->succ->pred = list->header;//将现在的首节点指向前驱节点的指针指向哨兵节点
	list->_size--;
	return;
}
void B_listRemoveBack(B_list* list) {//删除尾元素,原理与首元素相同
	list->trailer->pred = list->trailer->pred->pred;
	free(list->trailer->pred->succ->_elem);
	free(list->trailer->pred->succ);
	list->trailer->pred->succ = list->trailer;
	list->_size--;
	return;
}
char* B_listGetFirst(B_list* list) {//获取首元素
	return list->header->succ->_elem;
}
char* B_listGetBack(B_list* list) {//获取尾元素
	return list->trailer->pred->_elem;
}

B_listNode* B_listGetNodeRank(B_list* list, Rank r) {//得到指定位置的元素对应的节点，速度较慢
	if (r <= list->_size) {//通过要比较访问的位置和整体大小，选择头或者尾部开始查找，提高查找效率
		B_listNode* x = list->header->succ;
		while (r--)
			x = x->succ;
		return x;
	}
	else {
		B_listNode* x = list->trailer->pred;
		r = list->_size - r - 1;
		while (r--)
			x = x->pred;
		return x;
	}
}
char* B_listGetRank(B_list* list, Rank r) {////得到指定位置的元素，速度较慢
	return B_listGetNodeRank(list, r)->_elem;
}
void B_listRemoveInterval(B_list* list, Rank lo, Rank hi) {//删除[lo,hi)的元素
	B_listNode* x = B_listGetNodeRank(list, lo)->pred;
	list->_size -= hi - lo;
	while (lo++ < hi) {
		x->succ = x->succ->succ;
		free(x->succ->pred->_elem);
		free(x->succ->pred);
		x->succ -> pred = x;
	}
}
void B_listRemoveRank(B_list* list, Rank r) {//删除指定位置的元素
	B_listRemoveInterval(list, r, r + 1);
}
