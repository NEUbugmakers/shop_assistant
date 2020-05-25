
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include"SR_data.h"
B_vector* SR_dataCodeVector;//数字编码查找表
SR_dataBTNode SR_dataBTRoot;//根节点
B_vector* SR_dataSales;//销售信息
void SR_dataSalesInit(){//建立商品销售记录存储结构
    FILE* SR_sales = fopen("SR_sales","rb");
    if(SR_sales!=NULL){
        size_t len;
        SR_dataSales = B_vectorCreat(sizeof(C_sales));
        qDebug()<<"size"<<sizeof(size_t);
        fread(&len,sizeof(size_t),1,SR_sales);
        free(SR_dataSales->_elem);
        SR_dataSales->_elem = (char*)malloc(sizeof(C_sales)*len);
        SR_dataSales->_size = len;
        SR_dataSales->_capicity = len;
        qDebug()<<"SR_dataSales->_elem"<<sizeof(C_sales)*len;
        fread(SR_dataSales->_elem,sizeof(C_sales),len,SR_sales);
        SR_dataSalesBuild(SR_sales);
        fclose(SR_sales);
    }
    else{
        SR_dataSales = B_vectorCreat(sizeof(C_sales));
    }
}
void SR_dataSalesBuild(FILE* sales){//建立商品销售记录
    for(int i=0;i<SR_dataSales->_size;i++){
        C_sales* t = (C_sales*)B_vectorGet(SR_dataSales,i);
        t->salesinfo = B_vectorCreat(sizeof(C_salesNode));
        free(t->salesinfo->_elem);
        t->salesinfo->_elem = (char*)malloc(sizeof(C_salesNode)*t->num);
        t->salesinfo->_size = t->num;
        t->salesinfo->_capicity = t->num;
        qDebug()<<"salesinfonodes"<<sizeof(C_salesNode)*t->num;
        fread(t->salesinfo->_elem,sizeof(C_salesNode),t->num,sales);
    }
}
void SR_dataBTInit() {//初始化B-树
    FILE* SR_root = fopen("root", "rb");
    FILE* SR_goodsFile = fopen("goods", "rb");
    if (SR_root != NULL) {//判断文件是否存在
        qDebug()<<"../SRC/root"<< sizeof(SR_dataBTNode);
        fread(&SR_dataBTRoot, sizeof(SR_dataBTNode), 1, SR_root);
        SR_dataBTBuild(&SR_dataBTRoot, SR_root, SR_goodsFile);
        fclose(SR_root);
        fclose(SR_goodsFile);
    }
    else {
        SR_dataBTNodeInit(&SR_dataBTRoot);
    }
    SR_dataCodeVector = NULL;
    SR_dataCodeVectorBuild();//建立数字编码查找向量
}
void SR_dataBTBuild(SR_dataBTNode* x, FILE* file, FILE* goodsFile) {//根据文件建立B-树,先序遍历
    x->child = B_vectorCreat(sizeof(SR_dataBTNode));
    x->childCode = B_vectorCreat(sizeof(char));
    x->goodsVector = C_goodsVectorCreat();
    if (x->SR_dataBTNodeGoodsNum > 0) {//还原goodsVector
        free(x->goodsVector->vector->_elem);
        x->goodsVector->vector->_elem = (char*)malloc(sizeof(C_Goods) * x->SR_dataBTNodeGoodsNum);
        qDebug()<<"goodsVector"<<sizeof(C_Goods) * x->SR_dataBTNodeGoodsNum;
        fread(x->goodsVector->vector->_elem, sizeof(C_Goods), x->SR_dataBTNodeGoodsNum, goodsFile);
        x->goodsVector->vector->_capicity = x->goodsVector->vector->_size = x->SR_dataBTNodeGoodsNum;
        for (int i = 0; i < x->goodsVector->vector->_size; i++) {//还原C_goodsInfo(有问题）
            C_goodsListInit((C_Goods*)B_vectorGet(x->goodsVector->vector, i));
            C_goodsInfo info;
            for (int j = 0; j < ((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfKinds; j++) {
                qDebug()<<"shelfInfo"<< sizeof(C_goodsInfo);
                fread(&info, sizeof(C_goodsInfo), 1, goodsFile);
                B_listPushBack(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo, &info);
            }
            for (int j = 0; j < ((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockKinds; j++) {
                qDebug()<<"stockInfo"<< sizeof(C_goodsInfo);
                fread(&info, sizeof(C_goodsInfo), 1, goodsFile);
                B_listPushBack(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockInfo, &info);
            }
        }
    }
    if (x->childNum > 0) {//建立子节点
        free(x->childCode->_elem);//还原childCode
        x->childCode->_elem = (char*)malloc(sizeof(char) * x->childNum);
        x->childCode->_capicity = x->childNum;
        x->childCode->_size = x->childNum;
        qDebug()<<"childCode"<< sizeof(char) * x->childNum;
        fread(x->childCode->_elem, sizeof(char), x->childNum, file);//(增加了*x->childNum)

        free(x->child->_elem);//还原child
        x->child->_elem = (char*)malloc(sizeof(SR_dataBTNode) * x->childNum);
        x->child->_capicity = x->childNum;
        x->child->_size = x->childNum;
        for (int i = 0; i < x->childNum; i++) {
            SR_dataBTNode* t = (SR_dataBTNode*)B_vectorGet(x->child, i);
            qDebug()<<"child"<<sizeof(SR_dataBTNode);
            fread(t, sizeof(SR_dataBTNode), 1, file);
            t->parent = x;
            SR_dataBTBuild(t, file, goodsFile);
        }
    }
}
int SR_dataGetSortFromChildInterval(SR_dataBTNode* pos, char sort, int lo, int hi) {//在当前节点的子节点区间[lo,hi)找分类,失败返回不大于当前分类编号的最大编号位置(有问题）
    while (lo < hi) {
        int mi = (hi + lo) >> 1;
        qDebug()<<"sort::::"<<*(char*)B_vectorGet(pos->childCode, mi);
        if (sort < *(char*)B_vectorGet(pos->childCode, mi))
            hi = mi;
        else
            lo = mi + 1;
    }
    return --lo;
}
SR_dataBTNode* SR_dataGetSortFromChild(SR_dataBTNode* pos, char sort) {//在当前节点的子节点查找分类,失败返回NULL(有问题）
    Rank r = SR_dataGetSortFromChildInterval(pos, sort, 0, pos->childCode->_size);
    if(r==-1){
        return NULL;
    }
    if (*(char*)B_vectorGet(pos->childCode, r) == sort)//判断是否找到
        return (SR_dataBTNode*)B_vectorGet(pos->child, r);
    else
        return NULL;
}
SR_dataBTNode* SR_dataGetSortFromNode(SR_dataBTNode* pos, char sort[]) {//从某一节点查找分类(递归)
    if (pos == NULL)
        return NULL;
    if (isalpha(sort[0])){
        qDebug()<<"findsort::"<<sort[0];
        return SR_dataGetSortFromNode(SR_dataGetSortFromChild(pos, sort[0]), sort + 1);
    }
    else
        return pos;
}
SR_dataBTNode* SR_dataGetSort(char sort[]) {//从根节点开始查找分类
    return SR_dataGetSortFromNode(&SR_dataBTRoot, sort);
}
C_Goods* SR_dataGet(char code[]) {//根据编码查找商品
    for(int i=0;code[i]!=0;i++){
        if(!(isalpha(code[i]) || ('0'<=code[i]&&code[i]<='9'))){
            return NULL;
        }
    }
    SR_dataBTNode* pos = SR_dataGetSort(code);
    if(pos==NULL)
        return NULL;
    qDebug()<<"pos"<<pos;
    C_Goods* goods = (C_Goods*)C_goodsVectorFind_P(pos->goodsVector, code);
    if (goods!=NULL&&strcmp(goods->code, code) == 0)
        return goods;
    else
        return NULL;
}
void SR_dataBTNodeInit(SR_dataBTNode* node) {//对创建的节点初始化
    node->name[0] = 0;
    node->childNum = 0;
    node->child = B_vectorCreat(sizeof(SR_dataBTNode));
    node->childCode = B_vectorCreat(sizeof(char));
    node->sort[0] = 0;
    node->parent = 0;
    node->goodsVector = C_goodsVectorCreat();
}
char SR_dataSelectSort(SR_dataBTNode* pos) {//在当前节点挑选新增分类的分类标识符
    if (pos->childNum < 26) {
        if (pos->childCode->_size == 0)
            return 'A';
        char backSort = *(char*)B_vectorGet(pos->childCode, pos->childCode->_size - 1);
        if (backSort != 'Z')
            return ++backSort;
        else
            for (int i = 0; i < pos->childCode->_size; i++)
                if (*(char*)B_vectorGet(pos->childCode, i) - 'A' != i)
                    return 'A' + i;
    }
    else
        return -1;
}
int SR_dataInsertSort(char dir[], char name[]) {//插入分类
    SR_dataBTNode* pos = SR_dataGetSort(dir);
    if(strlen(name)==0||strlen(name)>=SR_BTNODE_NAME_LEN-1||pos==NULL||pos->child->_size>=26)
        return 0;
    if(pos==NULL||strlen(name)==0)
        return 0;
    SR_dataBTNode x;
    SR_dataBTNodeInit(&x);//初始化节点x
    char sort = SR_dataSelectSort(pos);//获取分类标识符
    strcpy(x.sort, pos->sort);
    int len = strlen(x.sort);
    x.sort[len] = sort;
    x.sort[len + 1] = 0;
    strcpy(x.name, name);
    x.parent = pos;
    Rank r;//即将插入的分类在父类中秩
    for (r = 0; r < pos->childNum; r++) {
        char t = *(char*)B_vectorGet(pos->childCode, r);
        if (t > sort)
            break;
    }
    B_vectorInsert(pos->child, &x, r);
    B_vectorInsert(pos->childCode, &sort, r);
    pos->childNum++;
    return 1;
}
void SR_dataSaveGoodsInfo(B_list* info, FILE* file) {//保存shelfInfo和stockInfo
    B_listNode* x = B_listGetFirstNode(info);
    while (x != NULL) {
        qDebug()<<"C_GoodsInfo"<<sizeof(C_goodsInfo);
        fwrite(x->_elem, info->_esize, 1, file);
        x = B_listNextNode(x);
    }
}
void SR_dataSavePreOrder(SR_dataBTNode* x, FILE* root, FILE* goodsFile) {//先序遍历保存节点（未完成）

    x->childNum = x->childCode->_size;
    x->SR_dataBTNodeGoodsNum = x->goodsVector->vector->_size;//更新商品数量
    qDebug()<<"child"<< sizeof(SR_dataBTNode);
    fwrite(x, sizeof(SR_dataBTNode), 1, root);//保存当前节点的数据
    if (x->goodsVector->vector->_size != 0) {//商品信息和商品目录分别存储
        for (int i = 0; i < x->goodsVector->vector->_size; i++) {//保存时更新C_shelfKinds和C_stockKinds;
            C_Goods* t = (C_Goods*)B_vectorGet(x->goodsVector->vector, i);
            t->C_shelfKinds = t->C_shelfInfo->_size;
            t->C_stockKinds = t->C_stockInfo->_size;
        }
        qDebug()<<"goodsVector"<< x->goodsVector->vector->_esize * x->goodsVector->vector->_size;
        fwrite(x->goodsVector->vector->_elem, x->goodsVector->vector->_esize, x->goodsVector->vector->_size, goodsFile);
        for (int i = 0; i < x->goodsVector->vector->_size; i++) {
            SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_shelfInfo, goodsFile);
            SR_dataSaveGoodsInfo(((C_Goods*)B_vectorGet(x->goodsVector->vector, i))->C_stockInfo, goodsFile);
        }
    }
    qDebug()<<"childCode"<< sizeof(char) * x->childNum;
    fwrite(x->childCode->_elem, sizeof(char), x->childNum, root);
    for (int i = 0; i < x->child->_size; i++) {
        SR_dataBTNode* t = (SR_dataBTNode*)B_vectorGet(x->child, i);
        SR_dataSavePreOrder(t, root, goodsFile);
    }
}
void SR_dataSave() {//保存数据
    FILE* file = fopen("root", "wb");
    FILE* goodsFile = fopen("goods", "wb");
    SR_dataSavePreOrder(&SR_dataBTRoot, file, goodsFile);
    fclose(file);
    fclose(goodsFile);
}
int SR_dataAddNewGoods(C_Goods* goods) {//添加新商品
    SR_dataBTNode* pos = SR_dataGetSort(goods->code);
    if(strlen(goods->code)==0||strlen(goods->code)>C_NAME_LEN-1||goods->C_out<=0)
        return 0;
    int firstNum=0;
    while(isalpha(goods->code[firstNum]))
        firstNum++;
    if(SR_dataCodeFind(goods->code+firstNum)!=NULL){
        return 0;
    }
    for (int i = 0; i < SR_dataCodeVector->_size; i++) {//查重
        C_Goods* x =SR_dataCodeFind(((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, i))->code);
        if(strcmp(goods->name,x->name)==0){
            return 0;
        }
    }
    C_goodsVectorInsert(pos->goodsVector, goods);
    SR_dataCodeVectorBuild();
    return 1;
}
char SR_dataReplenishGoods(char code[], C_goodsInfo* info) {//补货,返回批次编号,失败返回-1
    C_Goods* pos = SR_dataGet(code);
    return C_goodsReplenish(pos, info);
}
C_goodsReturn* SR_dataSell(char code[], char batch, int amount,int flag) {//出售商品,返回商品标价，0为交易，1位合法性查询
    C_goodsReturn* r;
    C_Goods* pos = SR_dataGet(code);
    if(pos==NULL)
        return NULL;
    r = C_goodsSell(pos, batch, amount, flag);
    if(flag==0){
        SR_dataSalesadd(code,amount);
    }
    SR_dataSave();
    SR_dataSalesSave();
    return r;
}
void SR_dataCodeVectorBuildFrom(SR_dataBTNode* pos, B_vector* vector) {//建立某一分类下的商品数字编码查找向量(无序）
    C_Goods* goods;
    SR_dataCodeNode node;
    Rank firstNum;
    for (int i = 0; i < pos->goodsVector->vector->_size; i++) {//分离分类编码和数字编码
        goods = (C_Goods*)B_vectorGet(pos->goodsVector->vector, i);
        for (firstNum = 0; firstNum < 19 && isalpha(goods->code[firstNum]); firstNum++) {
            node.sort[firstNum] = goods->code[firstNum];
        }
        node.sort[firstNum] = 0;
        strcpy(node.code, goods->code + firstNum);
        B_vectorPushBack(vector, &node);
    }
    for (int i = 0; i < pos->child->_size; i++)//继续深入
        SR_dataCodeVectorBuildFrom((SR_dataBTNode*)B_vectorGet(pos->child, i), vector);
}
int SR_dataCodeNodeCmp(SR_dataCodeNode* node1, SR_dataCodeNode* node2) {//通过数字编码比较codeNode大小
    return strcmp(node1->code, node2->code);
}
void SR_dataCodeVectorBuild() {//建立数字编码查找向量
    if (SR_dataCodeVector != NULL) {
        B_vectorClear(SR_dataCodeVector);
        free(SR_dataCodeVector);
    }
    SR_dataCodeVector = B_vectorCreat(sizeof(SR_dataCodeNode));
    SR_dataCodeVectorBuildFrom(&SR_dataBTRoot, SR_dataCodeVector);
    B_vectorSort(SR_dataCodeVector, (int(*)(const void*, const void*))SR_dataCodeNodeCmp);
}
C_Goods* SR_dataCodeFind(char code[]) {//通过数字编码查找商品
    if(strlen(code)==0)
            return NULL;
    for(int i=0;code[i]!=0;i++){
        if(!('0'<=code[i]&&code[i]<='9')){
            return NULL;
        }
    }
    int lo = 0, hi = SR_dataCodeVector->_size;
    while (lo < hi) {
        int mi = (lo + hi) >> 1;
        if (strcmp(code, ((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, mi))->code) < 0)
            hi = mi;
        else
            lo = mi + 1;
    }
    lo--;
    if(lo==-1)
        return NULL;
    if(strcmp(((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, lo))->code,code)!=0)
        return NULL;
    char fullcode[C_CODE_LEN];
    strcpy(fullcode, ((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, lo))->sort);
    strcat(fullcode, ((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, lo))->code);
    return SR_dataGet(fullcode);
}

//while (lo < hi) {
//	int mi = (hi + lo) >> 1;
//	if (sort < *(char*)B_vectorGet(pos->childCode, mi))
//		hi = mi;
//	else
//		lo = mi + 1;
//}
//return --lo;
C_goodsVector* SR_dataGetRot() {//查找过期商品
    C_goodsVector* rotGoods = C_goodsVectorCreat();
    for (int i = 0; i < SR_dataCodeVector->_size; i++) {
        C_Goods goods = C_goodsGetRot(SR_dataCodeFind(((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, i))->code));
        if (strlen(goods.code) != 0)//是否含有过期商品
            B_vectorPushBack(rotGoods->vector, &goods);
    }
    if (rotGoods->vector->_size == 0) {//如果无过期商品则释放空间，并返回NULL
        C_goodsVectorClear(rotGoods);
        free(rotGoods);
        rotGoods = 0;
    }
    return rotGoods;//返回过期商品的向量
}
char SR_dataOutOfStock(char code[], int amount) {//商品出库
    C_Goods* pos = SR_dataGet(code);
    return C_goodsOutofStock(pos, amount);
}
char SR_dataOutOfStockBatch(char code[],char batch, int amount){//指定商品批次出库
    C_Goods* pos = SR_dataGet(code);
    return C_goodsOutofStockBatch(pos, batch, amount);
}
B_vector* SR_dataSortVectorFrom(SR_dataBTNode* node, B_vector* vector) {//获取从某一节点开始的分类
    if (node == &SR_dataBTRoot) {//根节点不加入分类
        for (int i = 0; i < node->child->_size; i++) {
            SR_dataSortVectorFrom((SR_dataBTNode*)B_vectorGet(node->child, i), vector);
        }
    }
    else {
        char sort[20];
        strcpy(sort, node->sort);
        B_vectorPushBack(vector, sort);
        for (int i = 0; i < node->child->_size; i++) {
            SR_dataSortVectorFrom((SR_dataBTNode*)B_vectorGet(node->child, i), vector);
        }
    }
    return vector;
}
B_vector* SR_dataSortVector() {//获取全部分类
    B_vector* vector = B_vectorCreat(sizeof(char[20]));
    char sort[20] = { 0 };
    return SR_dataSortVectorFrom(&SR_dataBTRoot, vector);
}
void SR_dataRemoveGoods(char code[]) {//移除商品
    SR_dataBTNode* node = SR_dataGetSort(code);
    C_goodsVectorRemove(node->goodsVector, code);
    SR_dataCodeVectorBuild();
}
void SR_dataClearBTNode(SR_dataBTNode* node) {//清除节点
    C_goodsVectorClear(node->goodsVector);
    free(node->goodsVector);
    for (int i = 0; i < node->child->_size; i++) {
        SR_dataClearBTNode((SR_dataBTNode*)B_vectorGet(node->child, i));
    }
    B_vectorClear(node->child);
    B_vectorClear(node->childCode);
    free(node->child);
    free(node->childCode);
}
void SR_dataRemoveBTNode(SR_dataBTNode* node) {//删除节点
    SR_dataClearBTNode(node);
    char sort = node->sort[strlen(node->sort) - 1];
    Rank r = SR_dataGetSortFromChildInterval(node->parent,sort,0,node->parent->child->_size);
    if (*(char*)B_vectorGet(node->parent->childCode, r) == sort) {//判断是否找到
        B_vectorRemoveRank(node->parent->child, r);
        B_vectorRemoveRank(node->parent->childCode, r);
        return;
    }
    else
        return;
}
Rank SR_dataGetSalesRank(char code[]){//根据编码查找商品销量信息，返回对应位次；
    int lo = 0, hi = SR_dataSales->_size;
    while (lo < hi) {
        int mi = (lo + hi) >> 1;
        char tcode[C_CODE_LEN];
        strcpy(tcode,((C_sales*)B_vectorGet(SR_dataSales, mi))->code);
        if (strcmp(code, tcode) < 0)
            hi = mi;
        else
            lo = mi + 1;
    }
    return --lo;
}
void SR_dataSalesadd(char code[], int amount){//添加销售记录
    Rank r = SR_dataGetSalesRank(code);
    if(r<0||strcmp(code,((C_sales*)B_vectorGet(SR_dataSales, r))->code)!=0){
        C_sales t;
        strcpy(t.code,code);
        t.salesinfo = B_vectorCreat(sizeof(C_salesNode));
        B_vectorInsert(SR_dataSales,&t,r+1);
        qDebug()<<"code::";
        qDebug()<<"code::"<<((C_sales*)B_vectorGet(SR_dataSales, r+1))->code;
        r++;//
    }
    C_goodsaddsaleinfo((C_sales*)B_vectorGet(SR_dataSales, r),amount);
}
C_sales* SR_dataGetSales(char code[]){//根据编码查找商品销量信息
    Rank r = SR_dataGetSalesRank(code);
    if(r<0)
        return NULL;
    C_sales* sales = (C_sales*)B_vectorGet(SR_dataSales, r);
    if(strcmp(sales->code,code)==0)
        return sales;
    else
        return NULL;
}
void SR_dataSalesSave(){//保存销售记录
    FILE* SR_sales = fopen("SR_sales","wb");
    qDebug()<<"salessave";
    qDebug()<<"SR_dataSales->_size"<<sizeof(size_t);
    fwrite(&SR_dataSales->_size,sizeof(size_t),1,SR_sales);
    qDebug()<<"SR_dataSales->_size"<<sizeof(C_sales)*SR_dataSales->_size;
    for(int i=0;i<SR_dataSales->_size;i++){
        C_sales* t = (C_sales*)B_vectorGet(SR_dataSales,i);
        t->num = t->salesinfo->_size;
    }
    fwrite(SR_dataSales->_elem,sizeof(C_sales),SR_dataSales->_size,SR_sales);
    for(int i=0;i<SR_dataSales->_size;i++){
        C_sales* t = (C_sales*)B_vectorGet(SR_dataSales,i);
        qDebug()<<"C_salesNodesss"<<sizeof(C_salesNode)*t->salesinfo->_size;
        fwrite(t->salesinfo->_elem,sizeof(C_salesNode),t->salesinfo->_size,SR_sales);
    }
    fclose(SR_sales);
}
int SR_dataCheckReplenish(char code[]){//通过数字编码查找，判断是否需要补货
    C_Goods* goods = SR_dataGet(code);
    C_sales* sales = SR_dataGetSales(code);
    if(sales == NULL){//商品从来都没有出售过
        return 0;
    }
    int amount = C_goodsGetMultiSales(sales,30);
    if((goods->C_shelfTotal+goods->C_stockTotal)*30< C_goodsGetMultiSales(sales,30)*7)
        return 1;
    else
        return 0;
}
B_vector* SR_dataReplenishRemind(){//补货提醒
    B_vector* vector = B_vectorCreat(sizeof(char[C_CODE_LEN]));
    C_Goods* goods;
    for(int i = 0;i<SR_dataCodeVector->_size;i++){
        goods = SR_dataCodeFind(((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector,i))->code);
        if(SR_dataCheckReplenish(goods->code)==1){
            B_vectorPushBack(vector,&goods->code);
        }
    }
    if(vector->_size!=0)
        return vector;
    else{
        B_vectorClear(vector);
        free(vector);
        return NULL;
    }
}
char* B_strstr(const char* str,const char* sub) {
    int n = strlen(sub);
    int m = strlen(str);
    int i, j;
    for (i = 0; i < m - n + 1; i++) {
        for (j = 0; j < n; j++) {
            if (str[i + j] != sub[j])
                break;
        }
        if (j >= n)
            return (char*)str+i;
    }
    return NULL;
}

B_vector* SR_dataNameFind(char name[]){//根据名称查找商品
    B_vector* vector = B_vectorCreat(sizeof(char[C_NAME_LEN]));
    for (int i = 0; i < SR_dataCodeVector->_size; i++) {
        C_Goods* goods =SR_dataCodeFind(((SR_dataCodeNode*)B_vectorGet(SR_dataCodeVector, i))->code);
        if(B_strstr(goods->name,name)!=NULL){
            B_vectorPushBack(vector,goods->code);
        }
    }
    if(vector->_size == 0){
        B_vectorClear(vector);
        free(vector);
        return NULL;
    }
    return vector;
}
int SR_dataSortEdit(SR_dataBTNode* node,char name[]){//分类编辑
    if(strlen(name)==0||strlen(name)>=SR_BTNODE_NAME_LEN-1||node==NULL)
        return 0;
    strcpy(node->name,name);
    return 1;
}
