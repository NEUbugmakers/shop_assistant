#include"list.h"
B_list* B_listCreat(size_t esize) {//创建B_list，需指定元素内存大小
	B_list* list = malloc(sizeof(B_list));
	list->_esize = esize;
	init(list);
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
	list->_size;
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
	return;
}
void B_listRemoveFirst(B_list* list) {//删除首元素
	list->header->succ = list->header->succ->succ;//头哨兵的后继节点指向首节点的后继节点
	free(list->header->succ->pre->_elem);//头哨兵通过现在的后继节点指向前驱节点的指针释放首元素
	free(list->header->succ->pre);
	list->header->succ->pre = list->header;//将现在的首节点指向前驱节点的指针指向哨兵节点
	return;
}
void B_listGetFirst(B_list* list) {//获取首元素
	return list->header->succ;
}
void B_listGetBack(B_list* list) {//获取尾元素
	return list->trailer->pred;
}
