#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef int type;

int m;///记录排序序号
int sum;

/*定义 b树的结点结构*/
typedef struct B_Tree_Node
{
    int keyNum; //最大为m-1
    type *key;//关键字向量
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



///创建m阶b树 关键字的个数 最多为m-1 最少为ceil（m/2）-1 （即取m/2的上限）  且不能少于两对
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
    ///结点中关键字的个数min >= （(m/2)的上限-1）  比如 m=3 则关键字最少为1 最多为2  m=4 则关键字最少为1 最多为3
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
    int i;
    Node *node = NULL;
    node = (Node*)malloc(sizeof(Node));

    if(node == NULL)
    {
        return NULL;
    } //tempNode->child = node->child + (btree->index);

    ///初始化用来标记当前node中关键字个数的keyNum 待理解
    node->keyNum = 0;
    ///calloc函数通常用来为数组申请内存空间 这里key是数组
    node->key = (int*)calloc(btree->max + 1,sizeof(int));
    node->word = (char**)calloc(btree->max + 1,sizeof(char));
    for(i = 0;i < btree->max + 1;i++)
    node->word[i] = (char*)calloc(1,sizeof(char));

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
        free(node->word);
        free(node);
        node == NULL;
        return NULL;
    }

    return node;

}

///结点分裂处理 由node分裂出tempnode  pos是插入位置
void B_TREE_SPLIT(Tree *btree,Node* parentNode,Node* node,int pos)
{
    Node* tempNode;
    int i;
    ///为新分裂出的结点申请内存
    tempNode = create_node(btree);
        ///对分裂结点的一系列赋值
        for (i = 0; i<btree->index - 1; i++)
			{
			    tempNode->key[i] = node->key[i + btree->index];
			    tempNode->word[i] = node->word[i + btree->index];
			}
        //tempNode->key = node->key + (btree->index);
        tempNode->keyNum = btree->max - (btree->index);
        ///如果当前结点不是叶结点 复制孩子指针
        if(node->child[0] != NULL)
            //tempNode->child = node->child + (btree->index);
           {
             for (i = 0; i<btree->index; i++)
			tempNode->child[i] = node->child[i + btree->index];
           }
        node->keyNum = btree->index - 1;

        //将中间数作为索引插入到双亲节点中
        //插入点后面的关键字和指针都往后移动一个位置
         for(i=parentNode->keyNum; i > pos ; i--)
            {
                    ///关键字和孩子都会变多
                    parentNode->key[i] = parentNode->key[i-1];
                    parentNode->word[i] = parentNode->word[i-1];
                    parentNode->child[i+1] = parentNode->child[i];

            }
            ///此时将顶上去的key和指向child的指针插入
            parentNode->key[pos] = node->key[btree->index - 1];
            parentNode->word[pos] = node->word[btree->index - 1];
            parentNode->child[pos+1] = tempNode;
            tempNode->parent = parentNode;
            parentNode->keyNum ++;

            ///对原有结点的儿子和关键字进行部分清空
       // node->keyNum = btree->index - 1;

        for(i = btree->index - 1 ;i< btree->max ; i++)
        {
            ///应该赋值多少？？？
            node->key[i] = NULL;
            node->word[i] = NULL;
        }
        for(i = btree->index ;i< btree->max + 1 ; i++)
        {
            ///应该赋值多少？？？
            node->child[i] = NULL;
        }

        for(i = 0 ;i < tempNode->keyNum ; i++)
        {
           if(tempNode->child[i] != NULL)
              tempNode->child[i]->parent = tempNode;
        }
        for(i = btree->index - 1 ;i< btree->max ; i++)
        {
            ///应该赋值多少？？？
            node->key[i] = NULL;
            node->word[i] = NULL;
        }
}

void btree_InsertNoneFull(Tree *btree,Node *node,type key,char *word)
{
    int i;
    int j=0;///在找到熟悉度key后 再用j进行循环得到 根据单词序的插入位置
    Node* child;

    i = node->keyNum;

    ///如果是叶结点 直接插入
    if(node->child[0] == NULL)
    {
        for(i=node->keyNum; i > 0 ; i--)
      {
        if(key < node->key[i-1])
        {
            ///关键字会变多
            node->key[i] = node->key[i-1];
            node->word[i] = node->word[i-1];
            continue;
        }
        else
            if(key == node->key[i-1])
            {
                if(strcmpi(word,node->word[i-1]) <0)
                {
                    ///关键字会变多
                    node->key[i] = node->key[i-1];
                    node->word[i] = node->word[i-1];
                    continue;
                }

            }
            break;
      }

      if(strcmpi(word,node->word[i]) == 0)
      {
          for(j = i;j < node->keyNum;j++)
          {
              node->key[j] = node->key[j+1];
            node->word[j] = node->word[j+1];
          }
      }
      else{
        ///此处i是多少 需调整
      node->key[i] = key;
      node->word[i] = word;
      node->keyNum++;
      }
    }
    else
   {
            ///找出应该插入的孩子节点
      /*   for(i=node->keyNum; i > 0 ; i--)
      {
        if(key < node->key[i-1])
        {
            continue;
        }
            break;
      }*/

      for(i=node->keyNum; i > 0 ; i--)
      {
        if(key < node->key[i-1])
        {
            continue;
        }
        else
            if(key == node->key[i-1])
            {
                if(strcmpi(word,node->word[i-1]) <0)
                {
                    continue;
                }
                else if(strcmpi(word,node->word[i-1]) == 0)
                {
                    return;
                }
            }
        break;
      }

      child = node->child[i];
    if(child->keyNum == btree->max)
    {
        B_TREE_SPLIT(btree,node,child,i);

        ///若 key比兴顶上来的值大 则 接下来应该切向右孩子 即i++
        if(key > node->key[i])
            i++;
            else if(key == node->key[i])
            {
                if(strcmpi(word,node->word[i]) > 0)
                {
                    i++;
                }

            }

    }
    ///将结点切向孩子节点
    child = node->child[i];
    btree_InsertNoneFull(btree,child,key,word);
    }
}

Node* btree_insert(Tree *btree,Node* node,type key,char* word)
{
    Node* newNode;
    /* 检查是否根节点已满，如果已满，分裂并生成新的根节点 */
    if(node->keyNum == btree->max)
        {
            ///这里对不对？？？？进行预分裂 newnode即分裂后产生的父结点 此处为新的根节点
            newNode =create_node(btree);
            newNode->keyNum = 0;
            newNode->child[0] = node;
            B_TREE_SPLIT(btree,newNode,node,0);

            btree_InsertNoneFull(btree,newNode,key,word);
            return newNode;
            //node = btree->root;
            //continue;
        }
        else///未满
        {
            btree_InsertNoneFull(btree,node,key,word);
            return btree->root;
        }
}

Node* btree_max(Node* node)
{
    Node* newNode = NULL;
    newNode = node;
    if(newNode->child[0] == NULL)
    {
        return newNode;
    }
    else
    {
        newNode = btree_max(node->child[node->keyNum]);
    }
    return newNode;
}

Node* btree_min(Node* node)
{
    Node* newNode = NULL;
    newNode = node;
    if(newNode->child[0] == NULL)
    {
        return newNode;
    }
    else
    {
        newNode = btree_min(node->child[0]);
    }
    return newNode;
}

void right_rotate(Node* parentNode,Node* currentNode,Node* nearNode,int i)
{
    int j;
    for(j = currentNode->keyNum - 1;j>=0;j--)
        {
            currentNode->key[j+1] = currentNode->key[j];
            currentNode->word[j+1] = currentNode->word[j];
        }


    currentNode->key[0] = parentNode->key[i-1];
    currentNode->word[0] = parentNode->word[i-1];
    parentNode->key[i-1] = nearNode->key[nearNode->keyNum - 1];
    parentNode->word[i-1] = nearNode->word[nearNode->keyNum - 1];
    nearNode->key[nearNode->keyNum - 1] = NULL;
    nearNode->word[nearNode->keyNum - 1] = NULL;

    if(currentNode->child[0] != NULL)
    {
        for(j = currentNode->keyNum;j>=0;j--)
            currentNode->child[j+1] = currentNode->child[j];

        currentNode->child[0] = nearNode->child[nearNode->keyNum];
    }

    currentNode->keyNum ++;
    nearNode->keyNum --;
}
void left_rotate(Node* parentNode,Node* currentNode,Node* nearNode,int i)
{
    int j;
    currentNode->key[currentNode->keyNum] = parentNode->key[i];
    currentNode->word[currentNode->keyNum] = parentNode->word[i];
    parentNode->key[i] = nearNode->key[0];
    parentNode->word[i] = nearNode->word[0];

    for(j = 0;j<nearNode->keyNum - 1;j++)
        {
            nearNode->key[j] = nearNode->key[j+1];
            nearNode->word[j] = nearNode->word[j+1];
        }

    nearNode->key[nearNode->keyNum - 1] = NULL;
    nearNode->word[nearNode->keyNum - 1] = NULL;

    if(currentNode->child[0] != NULL)
    {
        currentNode->child[currentNode->keyNum +1] = nearNode->child[0];

        for(j = 0;j<nearNode->keyNum;j++)
            nearNode->child[j] = nearNode->child[j+1];
    }

    currentNode->keyNum ++;
    nearNode->keyNum --;
}
    //remove
    // 对 tree 中的节点 node 进行合并孩子节点处理.
	// 注意：孩子节点的 keynum 必须均已达到下限，即均等于 BTree_D - 1
    // 将 tree 中索引为 index 的 key 下移至左孩子结点中，
	// 将 node 中索引为 index + 1 的孩子节点合并到索引为 index 的孩子节点中，右孩子合并到左孩子结点中。
	// 并调相关的 key 和指针。
	int BTree_merge_child(Tree* btree, Node* parent, int loc)
	{
	    int i;
	    Node* lNode;
	    Node* rNode;

	    if(loc == parent->keyNum)
        {
          loc --;
        }

        lNode = parent->child[loc];
        rNode = parent->child[loc + 1];

        int temp = lNode->keyNum;

        lNode->key[lNode->keyNum] = parent->key[loc];
        lNode->word[lNode->keyNum] = parent->word[loc];

        lNode->keyNum++;
        for(i = 0;i < rNode->keyNum;i++)
        {
            lNode->key[lNode->keyNum+i] = rNode->key[i];
            lNode->word[lNode->keyNum+i] = rNode->word[i];
            lNode->keyNum++;
        }
        if(rNode->child[0] !=NULL)
        {
            for(i=0;i<=rNode->keyNum;i++)
                lNode->child[temp+i+1] = rNode->child[i];
        }

        rNode->keyNum = 0;
       // lNode->keyNum = btree->max;

        for(i = loc;i<parent->keyNum - 1;i++)
        {
            parent->key[i]= parent->key[i+1];
            parent->word[i]= parent->word[i+1];
            parent->child[i+1] = parent->child[i+2];
        }

        parent->key[parent->keyNum - 1] = NULL;
        parent->word[parent->keyNum - 1] = NULL;
        parent->child[parent->keyNum] = NULL;
        parent->keyNum --;
        rNode = NULL;
        //if(parent->keyNum == 0)
       // {
         //   parent = lNode;
        //}


        return loc;
/*	    ///当前节点是根结点 不必合并
	    if(parent == NULL)
        {
            if(node->keyNum == 0)
            {
                if(node->child[0] != NULL)
                 {
                    btree->root = node->child[0];
                    node->child[0]->parent = NULL;
                 }

                else
                {
                 btree->root = NULL;
                }
                free(node);
            }
        }

        for(i = 0;i <= parent->keyNum;i++)
        {
            if(node == parent->child[i])
            {
                break;
            }
        }
        ///待删除key在最后一个孩子结点上
        if(i == parent->keyNum)
        {
            int m = parent->child[i-1]->keyNum;
            ///如果兄弟不够借 合并
            if(m <= btree->min)
            {
            parent->child[i-1]->key[m] = parent->key[i-1];
            for(i = 1;i<node->keyNum;i++)
            parent->child[i-1]->key[m + i] = node->key[i];

            parent->key[i-1] = NULL;
            parent->word[i-1] = NULL;
            parent->child[i] = NULL;
            free(node);
            }
            else
            {///如果兄弟够借 那就借一个 进行局部右旋
                node->key[node->keyNum] = parent->key[parent->keyNum - 1];
                node->keyNum ++;
                parent->key[parent->keyNum - 1] = parent->child[i - 1]->key[m-1];
                parent->child[i - 1]->key[m-1] = NULL;
                parent->child[i - 1]->keyNum --;
                ///转向case1 删除
                btree_delete(btree,node,key);
            }
        }
        else///如果在中间的孩子结点上
        {
            if()
        }*/
	}

	///删除主要分两种情况 一种是要删除key不在当前节点 一种是要删除key在当前结点

	void btree_delete_node(Tree *btree,Node* node,type key,char *word)
	{
	    int i = 0;
	    int j;
	    type tempkey;
	    char *tempword;

	    ///找出删除位置 当当前节点遍历完 或者 key小于等于当前节点某个key时 跳出循环
	    while(i < node->keyNum && key > node->key[i])
        {
            i++;
        }

        ///当key小于等于当前key时 需要再根据字典序查找删除位置
        if(key == node->key[i])
        {
            while(i < node->keyNum && strcmpi(word,node->word[i]) > 0)
                {
                    i++;
                }
        }



	/*======================含有key的当前结点时的情况========node->key[i]============
	2. node:
	3. index of Key:            i-1  i  i+1
	4.                         +---+---+---+---+
	5.                           *  key   *
	6.                     +---+---+---+---+---+
	7.                            /     \
	8. index of Child:           i      i+1
	9.                          /         \
	10.                     +---+---+      +---+---+
	11.                       *   *           *   *
	12.                 +---+---+---+  +---+---+---+
	13.                     leftChild     rightChild
	14. ============================================================*/
         ///在该层 且 可以找到该值
         if(i < node->keyNum && key == node->key[i] && strcmp(word,node->word[i]) == 0)
         {


             /// case1:如果是叶子结点 直接删除
             if(node->child[0] == NULL)
             {

                     for(j = i;j < node->keyNum - 1;j++)
                     {
                         node->key[j]=node->key[j+1];
                         node->word[j]=node->word[j+1];
                     }
                 node->key[node->keyNum - 1] = 0;
                 node->word[node->keyNum - 1] = 0;
                 node->keyNum --;

                 //if(node->keyNum < btree->min)
                // {
                  //  BTree_merge_child(btree,node,node->parent);
                // }
                // for(j = )
             }
            else
             {
                  /*删除的值在当前节点 就找其前驱或后继（必是叶节点） 调换后删除叶节点
	              * 当前于该位置的关键字的左子结点关键字个数大于等于T时，
	              * 寻找该位置的关键的前驱（左子结点的最大关键字）
	              */

                 if(node->child[i]->keyNum > btree->min)/////////////////////////////XIUGAI
                 {
                     Node* newNode;
                     newNode = btree_max(node->child[i]);
                     tempkey = node->key[i];
                     tempword = node->word[i];
                     node->key[i] = newNode->key[newNode->keyNum - 1];
                     node->word[i] = newNode->word[newNode->keyNum - 1];
                     newNode->key[newNode->keyNum - 1] = tempkey;
                     newNode->word[newNode->keyNum - 1] = tempword;
                 }
                 else if(node->child[i+1]->keyNum > btree->min)////////////////////////////////////////////XIUGAI
                 {
                     Node* newNode;
                     newNode = btree_min(node->child[i+1]);
                     tempkey = node->key[i];
                     tempword = node->word[i];
                     node->key[i] = newNode->key[0];
                     node->word[i] = newNode->word[0];
                     newNode->key[0] = tempkey;
                     newNode->word[0] = tempword;
                     ///交换完  找到相应的i 执行删除操作
                     i++;
                 }
                 ///两个儿子都很穷 只能合体了
                 else
                 {
                     i = BTree_merge_child(btree,node,i);
                 }
                 ///交换完  找到相应的i 执行删除操作 递归调用 一层一层往下
                 btree_delete_node(btree,node->child[i],key,word);
             }
         }

         ///当前节点找不到key 但在下一层可以找到key 即要删除的值不在当前结点
         else
         {
             //if(i == node->keyNum)
      // {
          //  i--;
        //}
             if(node->child[0] == NULL)
             {
                 printf("***不存在要删除的值\n");
             }
             else
             {
                 ///如果要删除的值在当前结点的孩子分支上 执行递归删除 ////////////////需判断是等于还是大于
                 if(node->child[i]->keyNum > btree->min)
                    btree_delete_node(btree,node->child[i],key,word);
                ///如果这个孩子分支太穷 那就要看他的兄弟 兄弟可以借就借
                 else
                 {
                      if(i>0 && node->child[i-1]->keyNum > btree->min)
                      {
                        ///进行局部右旋 左兄弟富裕
                        right_rotate(node,node->child[i],node->child[i-1],i);
                      }/////////////////////////////////////////////////////////////////////////////////////////////////
                       else if(i < node->keyNum  && node->child[i+1]->keyNum >btree->min)
                      {
                        ///进行局部左旋 右兄弟富裕
                        left_rotate(node,node->child[i],node->child[i+1],i);
                      }
                       else
                         i = BTree_merge_child(btree,node,i);
                       ///需要合并三个结点 才能执行删除
                       btree_delete_node(btree,node->child[i],key,word);
                 }
             }
         }
}

	void btree_delete(Tree* btree,type key,char* word)
	{
	    btree_delete_node(btree,btree->root,key,word);

	    if(btree->root->keyNum == 0 && btree->root->child[0] != NULL)
        {
            btree->root = btree->root->child[0];
        }
	}



/*
int B_TREE_INSERT(Tree *btree,int k,char* word)
{
    int i = 0;
    Node* node = btree->root;
    Node* newnode = NULL;
    //Node* tempNode = create_node(btree);
    ///构建第一个node
    if(node == NULL)
    {
        ///根据结构体tree里的max min创建结点if(i>0 && node->child[i-1]->keyNum > btree->min)
                      {
                        ///进行局部右旋 左兄弟富裕
                        right_rotate(node,node->child[i],node->child[i-1],i);
                      }/////////////////////////////////////////////////////////////////////////////////////////////////
                       else if(i < node->keyNum - 1 && node->child[i+1]->keyNum >btree->min)
                      {
                        ///进行局部左旋 右兄弟富裕
                        left_rotate(node,node->child[i],node->child[i+1],i);
                      }
                       else
                         i = BTree_merge_child(btree,node,i);
                       ///需要合并三个结点 才能执行删除
                       btree_delete_node(btree,node->child[i],key);
        node = create_node(btree);
        if(node == NULL)
        {
            return -1;
        }
        node->keyNum = 1;
        node->key[0] = k;
        node->word[0] = word;
        node->parent = NULL;

        ///把创建的第一个结点重新赋给根结点
        btree->root = node;
        return 0;
    }

    ///查找插入结点 同时进行判断 当前结点是不是满的 如果满的则需要将中间的关键字顶上去
    while(node != NULL)
    {

        if(btree->root->keyNum == btree->max)
        {
            ///这里对不对？？？？进行预分裂
            newnode =create_node(btree);
            newnode->keyNum = 0;
            newnode->child[0] = node;
            B_TREE_SPLIT(btree,node,newnode);

            //node = btree->root;
            //continue;
        }

             for(i = 0;i < node->keyNum;i++)
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
            node = node->child[i];
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
            node->word[i] = node->word[i-1];
            continue;
        }
            break;
    }
    ///应该是对key[i]赋值
    node->key[i] = k;
    node->word[i] = word;
    node->keyNum++;
    ///重新赋值？？？？？
   // btree->root = node;
    return 0;
}
*/

char* inorder_BTree(Node *node,int num)
{
    int i;
    char* a;
    if(node != NULL)
    {
        for(i = node->keyNum;i > 0;i--)
        {
           //if(key == node->key[i])


           if(node->child[i] != NULL)
           inorder_BTree(node->child[i],num);

           //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
           if(m++ == num)
           {
               printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
               printf("== 熟悉度为第%d大的单词为%s\n",num,node->word[i-1]);
               return node->word[i-1];
           }
        }
         if(node->child[0] != NULL)
        inorder_BTree(node->child[0],num);
    }

}

int Aggregate_BTree(Node *node,int num)
{
    int i;
    char* a;
    if(node != NULL)
    {
        for(i = node->keyNum;i > 0;i--)
        {
           //if(key == node->key[i])


           if(node->child[i] != NULL)
           Aggregate_BTree(node->child[i],num);

           //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
           if(m++ == num)
           {
               printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
               printf("== 熟悉度为第%d大的单词为%s\n",num,node->word[i-1]);
               sum += node->key[i-1];
           }
        }
         if(node->child[0] != NULL)
        Aggregate_BTree(node->child[0],num);
    }

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
void BTREE_Print(Node *node,int layer,int ith)
{
    int i;
    if(node != NULL)
    {
        printf("===第%d层====第%d个结点:",layer,ith);

        for(i=0;i< node->keyNum;i++)
        {
            if(node->key[i] >= 0)
            printf("(%d)%s ",node->key[i],node->word[i]);
        }
        printf("\n");

        layer++;
        for(i = 0;i < node->keyNum + 1;i++)
        {
            if(node->child[i] != NULL)
                BTREE_Print(node->child[i],layer,i+1);
        }
    }
    else
        printf("==树为空\n");
}


int main()
{
    //type a[] = {'F','S','Q','K','C','L','H','T','V','W','M','R','N','P','A','B','X','Y','D','Z','E','H','T','V','W','M','R','N','P','M','R','N','P','A','B','X','Y','D','Z','E'};
    //type c[] = {'Y','W','Q','X','K','B','H','P','T','F','M'};
    char b[][9] = {"B","abandon","avoid","amuse","bind","bike","beyond","clone","click","cling","clicking","J","J","j","j","N","A","Z","G","E","Q","k","k","l","k","k","k","l","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k"};
   static type a[]={12,12,4,5,7,3,5,0,0,0,0,66,45,34,435,3,435,4,56,867,54,7,97,565,3,2,2,23,465465,32,234,54654,634,6,46,4576,543,34,5,35,46,999,534,6,34,634,6,546345,52,5,43,64,6,4,56435,4,545};
   type c[]={12,3,4,5,7,3,5,0,3289,45,324,66,45,34,435,3,435,4,56,867,54,7,97,565,3,2,2,23,465465,32,234,54654,634,6,46,4576};
    int i, ilen=sizeof(a)/sizeof(a[0]);
    int clen = sizeof(c)/sizeof(a[0]);

    Tree *btree;
    //Tree *btree;
    printf("====创建B树====\n");
    create_BTree(&btree,4);
    printf("====向B树插入结点====\n");
    btree->root = create_node(btree);
//    btree_insert(btree,btree->root,30);
    for(i=0; i < ilen; i++)
     {
         //b[i][9] = "adf";
          btree->root = btree_insert(btree,btree->root,a[i],b[i]);
//#if CHECK_INSERT
         printf("== 添加节点: %d\n", a[i]);
         printf("== 树的详细信息: \n");
         BTREE_Print(btree->root,1,1);
         printf("\n");
//#endif
     }
     inorder_BTree(btree->root,12);

    /* for(i=0; i<ilen; i++)
     {
         btree_delete(btree,a[i],b[i]);
//#if CHECK_DELETE
         printf("== 删除节点: %d\n", a[i]);
         printf("== 树的详细信息: \n");
         BTREE_Print(btree->root,1,1);
         printf("\n");
//#endif
     }*/

   int op;
   int N;
   int j = 0;
   int k;
   int l,r;
   char* word;

   printf("请输入操作次数: \n");
   scanf("%d",&N);


   while(j++ < N)
   {
    ///执行操作前先初始化数据
    ///sum:case5中的熟悉度综合
    ///m:从大到小遍历红黑树时的序号
    sum = 0;
    m = 1;
    printf("请输入操作类型和第几个数: \n");
    //scanf("%d %d",&op,&k);
    scanf("%d %d %d",&op,&l,&r);
    switch(op){
    ///Find
    case 1:

        word = inorder_BTree(btree->root,k);
        //printf("== 熟悉度为第%d大的单词为%s\n",k,word);
        break;
    ///Insert
    case 2:
        btree->root = btree_insert(btree,btree->root,20,"storm");
        printf("== 添加结点 树的详细信息: \n");
        BTREE_Print(btree->root,1,1);
        break;
    ///Modify
    case 3:
        //inorder_DeleteNode(root->node,"click");
        //RB_DELETE(root,deleteNode->key,deleteNode->word);
        //RB_INSERT(root,10,"click");
        btree_delete(btree,12,"B");
        btree->root = btree_insert(btree,btree->root,17,"B");
        printf("== 修改树的详细信息: \n");
        BTREE_Print(btree->root,1,1);
        break;
    ///Delete
    case 4:
        //inorder_DeleteNode(root->node,"amuse");
        //RB_DELETE(root,deleteNode->key,deleteNode->word);
        btree_delete(btree,12,"B");
        printf("== 删除树的详细信息: \n");
        BTREE_Print(btree->root,1,1);
        break;
    ///Aggregate
    case 5:
        //m = 1;
       // inorder_Aggregate(root->node,l,r);
       for(i = l;i<=r;i++)
       {
           m = 1;
           Aggregate_BTree(btree->root,i);
       }
        printf("== 从第%dth到第%dth的熟悉度总和为%d: \n",l,r,sum);
        break;

    default:
        break;
   }

   }

   system("pause");

}
