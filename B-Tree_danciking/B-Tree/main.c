#include <stdio.h>
#include <stdlib.h>
/*m阶b树
树中每个结点最多含有m个孩子（m>=2）；
除根结点和叶子结点外，其它每个结点至少有[ceil(m / 2)]个孩子（其中ceil(x)是一个取上限的函数）；
根结点至少有2个孩子（除非B树只包含一个结点：根结点）；
所有叶子结点都出现在同一层，叶子结点不包含任何关键字信息(可以看做是外部结点或查询失败的结点，指向这些结点的指针都为null)；（注：叶子节点只是没有孩子和指向孩子的指针，这些节点也存在，也有元素。类似红黑树中，每一个NULL指针即当做叶子结点，只是没画出来而已）。
每个非终端结点中包含有n个关键字信息： (n，P0，K1，P1，K2，P2，......，Kn，Pn)。其中：
a) Ki (i=1...n)为关键字，且关键字按顺序升序排序K(i-1)< Ki。
b) Pi为指向子树根的结点，且指针P(i-1)指向子树种所有结点的关键字均小于Ki，但都大于K(i-1)。
c) 关键字的个数n必须满足： [ceil(m / 2)-1]<= n <= m-1。比如有j个孩子的非叶结点恰好有j-1个关键码。
*/


/*定义 b树的结点结构*/
typedef struct B_Tree_Node
{
    int keyNum; //最大为m-1
    int *key;//关键字向量
    char **word;
    struct B_Tree_Node **child;//子树指针向量  指向儿子的指针再指向其中某一个儿子
    struct B_Tree_Node *parent;//指向父结点的指针

}Node;

///B树结构
typedef struct B_Tree
{
    int max;
    int min;
    int index;//分裂索引 = （max+1）/2

    Node *root;  //根结点

}Tree;


///创建m阶b树 关键字的个数 最多为m-1 最少为ceil（m/2） （即取m/2的上限）  且不能少于两对
int create_BTree(Tree **btree,int m)
{
    Tree *bt = NULL;

    if(m < 3)
    {
       printf("b树的阶数必须大于2./n");
       return -1;
    }
    bt = (Tree*)malloc(sizeof(Tree));
    if(bt == NULL)
    {
       return -1;
    }

    //bt->max = m-1;
    //bt->min = m/2;
    ///对新建树的初始化 包括分支的上下限
    bt->max = m - 1;
    bt->min = m/2;
    ///m为奇数
    if(m % 2 != 0)
    {
        bt->min ++;
    }
    ///结点中关键字的个数min >= （(m/2)的上限-1）  比如 m=3 则关键字最少为1 最多为2  m=4 则关键字最少为2 最多为3
    bt->min --;

    ///标记分割点
    bt->index = m / 2;
    bt->root = NULL;

    *btree = bt;
    return 0;
}

///创建结点
Node* create_node(Tree *btree)
{
    Node *node = NULL;
    node = (Node*)malloc(sizeof(Node));

    if(node == NULL)
    {
        return NULL;
    }

    ///初始化用来标记当前node中关键字个数的keyNum 待理解
    node->keyNum = 0;
    ///calloc函数通常用来为数组申请内存空间 这里key是数组
    node->key = (int*)calloc(btree->max + 1,sizeof(int));
    node->word = (char*)calloc(btree->max + 1,sizeof(char));

    if(node->word == NULL || node->key == NULL)
    {
        free(node);
        node = NULL;
        return NULL;
    }

    ///比max+1大是为了移动？？？？？ 孩子数最大为max+1=m
    node->child = (Node**)calloc(btree->max + 2,sizeof(Node));
    if(node->child == NULL)
    {
        free(node->key);
        free(node);
        node == NULL;
        return NULL;
    }

    return node;

}
/*
  1) 结点node以sidx关键字为分割点，索引(0 ~ sidx-1)关键字继续留在结点node中，索引(sidx+1 ~ num-1)关键字放入新结点node2中
  2) 而索引sidx关键字则插入node->parent中，再将新结点node2作为父结点新插入关键字的右孩子结点
  3) 判断插入node的sidx关键字后，node->parent的关键字个数num是否超过max，如果超过，则以parent为操作对象进行1)的处理；否则，处理结束
*/
/*int _btree_insert(B_Tree *btree,Node *node,int i,int k,char* word)
{
    ///经过预分裂 可以进行插入了
    int i = 0;

    for(int i=node->keyNum; i > 0 ; i--)
    {
        if(k < node->key[i-1])
        {
            ///关键字会变多
            node->key[i] = node->key[i-1];
            continue;
        }
            break;
    }
    node->key[i-1] = k;
    node->keyNum++;
    return 0;
}*/

///结点分裂处理
Node* B_TREE_SPLIT(Tree *btree,Node* node)
{
    ///用于接受分裂后产生的父结点
    Node* parentNode = NULL;
    ///用于接受分裂后的结点
    Node* tempNode = NULL;

    while(node->keyNum >= btree->max)
    {
        ///对分裂结点的一系列赋值
        tempNode->key = node->key + (btree->index);
        tempNode->child = node->child + (btree->index);
        tempNode->keyNum = btree->max - (btree->index);
        tempNode->parent = node->parent;

        ///将顶上来的结点插入原父结点
        parentNode = node->parent;
        ///若原父结点为空 即当前节点为根结点
        if(parentNode == NULL)
        {
            parentNode == create_node(btree);
            if(parentNode == NULL)
            {
                return -1;
            }
            ///根节点有两个儿子 一个关键字
            parentNode->child[0] = node;
            parentNode->key[0] = node->key[btree->index - 1];
            parentNode->keyNum = 1;
            parentNode->child[0] = tempNode;

            ///将分裂的两个节点的父结点设为根节点
            node->parent = parentNode;
            tempNode->parent = parentNode;
        }
        else
        {
            ///插入非空父结点 从右往左进行比较 好像比较慢？？？
            int i;
            for(i=parentNode->keyNum; i > 0 ; i++)
            {
                if(node->key[btree->index - 1] < parentNode->key[i-1])
                {
                    ///关键字和孩子都会变多
                    parentNode->key[i] = parentNode->key[i-1];
                    parentNode->child[i+1] = parentNode->child[i];
                    continue;
                }
                break;
            }
            ///此时将顶上去的key和指向child的指针插入
            parentNode->key[i-1] = node->key[btree->index - 1];
            parentNode->child[i] = tempNode;
            tempNode->parent = parentNode;
            parentNode->keyNum ++;
        }

        ///对原有结点的儿子和关键字进行部分清空
        node->keyNum = btree->index;
        int i;
        for(i = btree->index - 1 ;i< btree->max ; i++)
        {
            ///应该赋值多少？？？
            node->key[i] = NULL;
        }
        for(i = btree->index ;i< btree->max + 1 ; i++)
        {
            ///应该赋值多少？？？
            node->child[i] = NULL;
        }
    }
    ///最后还需要改变原结点的孩子的父亲结点 因为原来他们是一个父结点 现在分开

   /*for(int i = 0 ;i< btree->index ; i++)
    {
        if(node->child[i] != NULL)
        node->child[i]->parent = node;
    }*/
    int i;
   for(i = 0 ;i < tempNode->keyNum ; i++)
    {
        if(tempNode->child[i] != NULL)
        tempNode->child[i]->parent = tempNode;
    }
    ///到此分裂结束 不用再向上找 看父结点是否为满  返回当前分裂生成的父结点
    return parentNode;
}

int B_TREE_INSERT(Tree *btree,int k,char* word)
{
    int i = 0;
    Node* node = btree->root;
    Node* tempNode = create_node(btree);
    ///构建第一个node
    if(node == NULL)
    {
        ///根据结构体tree里的max min创建结点
        node = create_node(btree);
        if(node == NULL)
        {
            return -1;
        }
        node->keyNum = 1;
        node->key[0] = k;
        node->word[0] = word;

        ///把创建的第一个结点重新赋给根结点
        btree->root = node;
        return 0;
    }

    ///查找插入结点 同时进行判断 当前结点是不是满的 如果满的则需要将中间的关键字顶上去
    while(node != NULL)
    {

        if(node->keyNum == btree->max)
        {
            ///这里对不对？？？？进行预分裂
            node = B_TREE_SPLIT(btree,node);
            break;
        }

            for(i = 0;i < node->keyNum;i--)
            {
               if(k == node->key && word == node->word)
               {
                printf("结点已存在\n");
                return 0;
               }
                else if(k < node->key[i])
               {
                break;
               }
            }

        ///如果上边都不执行的话 i此时为node->keyNum
        if(node->child[i] != NULL)
        {
            node == node->child[i];
        }
        ///只有叶结点的孩子结点为NULL
        else
            break;
    }

    ///找到插入位置后 进行插入操作
     for(i=node->keyNum; i > 0 ; i--)
    {
        if(k < node->key[i-1])
        {
            ///关键字会变多
            node->key[i] = node->key[i-1];
            continue;
        }
            break;
    }
    node->key[i-1] = k;
    node->keyNum++;
    return 0;
}



/*
* 打印"红黑树"
*
* tree       -- 红黑树的节点
* key        -- 节点的键值
* direction  --  0，表示该节点是根节点;
*               -1，表示该节点是它的父结点的左孩子;
*                1，表示该节点是它的父结点的右孩子。
*/
/*void BTREE_Print(Tree *tree,char* word,int key,int direction)
{
    if(tree != NULL)
    {
        if(direction == 0)
        {
            printf("%10s(%4d(BLACK)) is root\n",tree->word,tree->key);
        }
        else
        {
           // printf("%4d ",tree->key);
            //printf("%6s ",tree->color);
           // printf("%4d ",tree->key);
            printf("%10s(%4d(%6s)) is  %10s(%4d)'s %7s child\n",tree->word,tree->key,tree->color == 1 ? "BLACK" : "RED",word,key,direction == 1?"right" : "left");
        }

            RBTREE_Print(tree->left,tree->word,tree->key,-1);
            RBTREE_Print(tree->right,tree->word,tree->key,1);
    }
}

///打印红黑树起点 调用RBTREE_Print函数
void Print_BTREE(Tree *tree)
{
    if(tree != NULL && tree->root != NULL)
    {
        BTREE_Print(root->root);
    }
}
*/
int main()
{
    char array[] = {
        'G','G', 'M', 'P', 'X', 'A', 'C', 'D', 'E', 'J', 'K',
        'N', 'O', 'R', 'S', 'T', 'U', 'V', 'Y', 'Z', 'F', 'X'
    };
    const int length = sizeof(array)/sizeof(char);
    Tree *btree;
    printf("====创建B树====\n");
    create_BTree(&btree,3);
    printf("====向B树插入结点====\n");
    B_TREE_INSERT(btree,10,"w");

    printf("");

}
