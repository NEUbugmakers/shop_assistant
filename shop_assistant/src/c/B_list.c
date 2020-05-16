
#include"B_list.h"
int (*B_listCmpTemp)(void*, void*);//临时保存排序函数
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
	return ;
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
	x->_elem = (char*)malloc(list->_esize);
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
	x->_elem = (char*)malloc(list->_esize);
	memcpy(x->_elem, ve, list->_esize);
	x->pred = list->header;
	x->succ = list->header->succ;
	x->pred->succ = x;
	x->succ->pred = x;
	list->_size++;
	return;
}
void B_listInsertRank(B_list* list, const void* e, Rank r) {//插入元素到指定位置
	B_listNode* x = malloc(sizeof(B_listNode));
	B_listNode* dst = B_listGetNodeRank(list, r);//获取插入位置
	x->_elem = (char*)malloc(list->_esize);
	memcpy(x->_elem, e, list->_esize);
	x->succ = dst;
	x->pred = dst->pred;
	x->pred->succ = x;
	x->succ->pred = x;
	list->_size++;
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
		x->succ->pred = x;
	}
}
void B_listRemoveRank(B_list* list, Rank r) {//删除指定位置的元素
	B_listRemoveInterval(list, r, r + 1);
}
B_listNode* B_listNextNode(B_listNode* x) {//获得当前节点的下一个节点，若为最后一个节点则返回NULL
	if (x->succ->succ != NULL)//判断下一个节点是不是尾哨兵哨兵节点
		return x->succ;
	else
		return NULL;
}
char* B_listGetFromNode(B_listNode* x) {//获取当前节点的保存的数据
	return x->_elem;
}
B_listNode* B_listGetFirstNode(B_list* list) {//获取首元素
	if (list->header->succ != list->trailer)//不含元素则返回NULL
		return list->header->succ;
	else
		return NULL;
}
void B_listInsertPre(B_list* list, const void* e, B_listNode* listNode) {//插入元素到指定位置(需保证listNode为list下的节点）
	B_listNode* x = malloc(sizeof(B_listNode));
	B_listNode* dst = listNode;
	x->_elem = (char*)malloc(list->_esize);
	memcpy(x->_elem, e, list->_esize);
	x->succ = dst;
	x->pred = dst->pred;
	x->pred->succ = x;
	x->succ->pred = x;
	list->_size++;
}
int B_listCmp(B_listNode** node1, B_listNode** node2) {//内部排序函数
	return B_listCmpTemp((*node1)->_elem, (*node2)->_elem);
}
void B_listSort(B_list* list, int (*cmp)(const void*,const void*)) {//链表排序
	if (list->_size < 2)//数量小于2，无需排序
		return;
	B_listCmpTemp = cmp;
	B_vector* node_p = B_vectorCreat(sizeof(B_listNode*));
	B_listNode* x = B_listGetFirstNode(list);
	B_listNode** travelpre=NULL, ** travelsucc=NULL;
	while (x != NULL) {
		B_vectorPushBack(node_p, &x);
		x = B_listNextNode(x);
	}
	B_vectorSort(node_p, (int(*)(const void*, const void*))B_listCmp);
	travelpre = (B_listNode**)B_vectorGet(node_p, 0);
	(**travelpre).pred = list->header;
	list->header->succ = travelpre[0];
	for (int i = 1; i < node_p->_size; i++) {
		travelsucc = (B_listNode**)B_vectorGet(node_p, i);
		(**travelpre).succ = (*travelsucc);
		(**travelsucc).pred = *travelpre;
		travelpre = travelsucc;
	}
	(**travelsucc).succ = list->trailer;
	list->trailer->succ = *travelsucc;
	B_vectorClear(node_p);
	free(node_p);
	node_p = NULL;
	B_listCmpTemp = NULL;
}