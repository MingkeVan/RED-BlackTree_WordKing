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
#define RED 0
#define BLACK 1

typedef int type;
typedef char typestring;
typedef double LL;

int m;///记录排序序号
LL sum;
int boolval = 0;///标记是否找到要找到的单词

/*定义 b树的结点结构*/
typedef struct B_Tree_Node
{
    int keyNum; //最大为m-1
    type *key;//关键字向量
    char **word;
    struct B_Tree_Node **child;//子树指针向量  指向儿子的指针再指向其中某一个儿子
    struct B_Tree_Node *parent;//指向父结点的指针

}BNode;

///B树结构
typedef struct B_Tree
{
    int max;
    int min;
    int index;//分裂索引 = （max+1）/2

    BNode *root;  //根结点

}Tree;

///红黑树的定义:包含父结点，左孩子，右孩子 以及当前节点的颜色和值（key）
///定义红黑树的结点
typedef struct RBTree_Node
{
    unsigned char color;
    ///定义word为单词
    typestring *word;
    ///在这里key定义为单词的熟悉度 0<=key<=100
    type key;
    struct RBTree_Node *parent;
    struct RBTree_Node *left;
    struct RBTree_Node *right;
}rbNode,*RBTree;//Node是struct RBTree_Node的别名 但是因为*parent *left *right都是在结构体里边 所以不能用别名定义

///定义红黑树的根
typedef struct RB_RootNode
{
    rbNode *node;
}RBRoot;

///全局根结点
RBRoot *rbroot = NULL;
rbNode *deleteNode;

///定义x节点的左旋操作  其实就是每个结点的孩子，父亲绑定 左旋操作主要涉及x的右孩子（即y） x的父亲 y的左孩子 y的父亲 以及y的左孩子的父亲
void LEFT_ROTATE(RBRoot *root,rbNode *xnode)
{
    rbNode *y;

    ///设置y为x的右孩子
    y = xnode->right;
    ///设置x的右孩子为y的左孩子
    xnode->right = y->left;
    ///设置y的左孩子的父亲为x，如果y的左孩子不为空
    if(y->left != NULL)
    y->left->parent = xnode;

    ///设置y的父亲为x的父亲
    y->parent = xnode->parent;

    ///如果x结点的父亲为空，即x为根节点，则将y设为根节点
    if(xnode->parent == NULL)
        root->node = y;
    ///如果x结点的父亲不为空，即x为其父亲节点的左孩子或者右孩子，就将y设为左孩子或右孩子
    else
    {
        if(xnode == xnode->parent->left)
        xnode->parent->left = y;
        if(xnode == xnode->parent->right)
        xnode->parent->right = y;
    }

    ///剩下y与x之间的关系没有明确 将x的父亲设为y 将y的左孩子设为x
    xnode->parent = y;
    y->left = xnode;



}
///以结点x为轴 将x的孩子y变为 y是x的父亲
///定义x节点的右旋操作  其实就是每个结点的孩子，父亲绑定。
///右旋操作主要涉及x的右孩子（即y） x的父亲 y的左孩子 y的父亲 以及y的左孩子的父亲
void RIGHT_ROTATE(RBRoot *root,rbNode *xnode)
{
    rbNode *y;

    ///设置y为当前结点x的右孩子
    y = xnode->left;
    ///设置x的右孩子为y的左孩子
    xnode->left = y->right;
    ///设置y的左孩子的父亲为x，如果y的左孩子不为空
    if(y->right != NULL)
    y->right->parent = xnode;

    ///设置y的父亲为x的父亲
    y->parent = xnode->parent;

    ///如果x结点的父亲为空，即x为根节点，则将y设为根节点
    if(xnode->parent == NULL)
        root->node = y;
    ///如果x结点的父亲不为空，即x为其父亲节点的左孩子或者右孩子，就将y设为左孩子或右孩子
    else
    {
        if(xnode == xnode->parent->left)
        xnode->parent->left = y;
        if(xnode == xnode->parent->right)
        xnode->parent->right = y;
    }

    ///剩下y与x之间的关系没有明确 将x的父亲设为y 将y的左孩子设为x
    xnode->parent = y;
    y->right = xnode;
}
///添加操作  将红黑树当做一个二叉查找树 将结点插入 并将结点着色为红色（这样不会违背红黑树的特性5 即黑高度不变）fan 插入的应该是键值key
void RB_INSERT(RBRoot *root,type key,typestring *word)//node为插入节点
{
    ///定义一个新的节点 键值为插入的key
    rbNode *z = (rbNode*)malloc(sizeof(rbNode));
    z->key = key;
    z->word = word;


    rbNode *y = NULL;//把新插入结点看做空结点
    rbNode *x=root->node;//把x当做根节点

   // if(x == NULL)
    //{
    //  root->node = z;
    //}
    ///从根节点往下找出插入节点应该插入的位置
    while(x != NULL)
    {
        y = x;
        ///按字典序插入
        if(strcmp(z->word,x->word)<0)
            x= x->left;
        else
            x= x->right;


    }
    z->parent = y;//这里将x赋给node的父母是否合适   不合适 x已经为NULL 故用y接受x 并将y作为node的父结点

    if(y == NULL)
    {
        root->node = z;
        //z->parent = NULL;
    }
    else
    {
        ///按字典序插入
        if(strcmp(z->word,y->word)<0)
            y->left = z;
        else
            y->right = z;

    }

    z->left = NULL;
    z->right = NULL;
    z->color = RED;//////////////////////////////

    RB_INSERT_FIXUP(root,z);
}

///插入节点后  对红黑树的调整操作 其中当前节点为刚插入节点z  当前节点肯定是红的？
void RB_INSERT_FIXUP(RBRoot *root,rbNode *z)
{
    rbNode *parent = (rbNode*)malloc(sizeof(rbNode));
    rbNode *grandparent;//定义父结点 祖父节点
    rbNode *y = (rbNode*)malloc(sizeof(rbNode));

    //if(z->parent != NULL)
    //{parent = z->parent;
    //if(z->parent->parent != NULL)
    //grandparent = z->parent->parent;
    //}
    ///父母是p[z],祖父是p[p[z]] y是z的uncle 0是红色   （！！！父结点是红色才继续进行 如果是黑色，则停止！！！）

        while( z ->parent != NULL && z->parent->color == RED )
    {
        ///如果z的父亲是z的祖父的左孩子
        if(z->parent == z->parent->parent->left)
        {
            ///z的叔叔
            y = z->parent->parent->right;

            ///case1 z的叔叔为红色
            if(y != NULL && y->color == 0)
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;//当前节点切换为z的祖父
            }

            ///case2 z的叔叔为黑色  (z的叔叔可能为空节点 所以用else 即不是红节点就默认为黑  只有这样才能执行接下来的操作！！！！！)
            else
            {
                ///case2 z是右孩子 且它的叔叔是黑色 需要左旋
                if(z == z->parent->right)
                {
                    z = z->parent;//当前节点切换为z的父结点
                    LEFT_ROTATE(root,z);//左旋操作已经将当前节点z切换为z的左儿子

                    ///当前结点z的父亲是红色 其本身也是红色 所以颜色不用调整
                    //z->color = 0;///???
                    //z->parent->color = 1;///???
                }

                    ///case3 z是左孩子 且他的叔叔是黑色 右旋。  当前节点不用继续调整 因为当前节点的父结点已经调整为黑色
                    //z = z->parent->parent;
                    ///实现将z的父结点设为黑色 祖父节点设为红色 然后进行右旋操作 父结点替换掉祖父节点
                    z->parent->color = 1;
                    z->parent->parent->color = 0;

                    ///以z的祖父为轴进行右旋
                    RIGHT_ROTATE(root,z->parent->parent);
                    //z = z->parent->parent;RBTREE_Print

            }

        }

       ///z的父结点是z的祖父节点的右孩子
        else
        {
            ///z的叔叔是z的祖父的左孩子
            y = z->parent->parent->left;

            ///case1 z的叔叔为红色
            if(y != NULL && y->color == 0)
            {
                ///调整颜色 父节点和叔叔节点都变黑 祖父节点变红
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;

                ///当前节点切换为z的祖父
                z = z->parent->parent;
            }

            ///case2 z的叔叔为黑色
            else
            {
                ///case2 z是左孩子 且它的叔叔是黑色 需要右旋
                if(z == z->parent->left)
                {
                    ///当前节点切换为z的父结点
                    z = z->parent;

                    ///右旋操作已经将当前节点z切换为z的左儿子
                    RIGHT_ROTATE(root,z);

                    //当前结点z的父亲是红色 其本身也是红色 所以颜色不用调整
                    //z->parent->color = 1;///???
                    //z->parent->parent->color = 0;///??? 当前节点z的父亲调整为黑色 祖父调整为红色？？？？
                }
                //else

                    ///case3 z是右孩子 且他的叔叔是黑色 左旋。  当前节点不用继续调整 因为当前节点的父结点已经调整为黑色
                    //z = z->parent->parent;
                    ///实现将z的父结点设为黑色 祖父节点设为红色 然后进行左旋操作 父结点替换掉祖父节点
                    z->parent->color = 1;
                    z->parent->parent->color = 0;

                    ///以z的祖父为轴进行左
                    LEFT_ROTATE(root,z->parent->parent);
                    //z = z->parent->parent;

            }
        }

    }

    ///根节点设置为黑色  在while循环外边执行本句
    root->node->color = BLACK;
}

///找出以node为根节点的子树的最小值
rbNode* TreeMinimum(rbNode *node)
{
    if(node->left != NULL)
        return TreeMinimum(node->left);
    else
        return node;
}

///找出结点x的后继
rbNode* RBTree_Successor(rbNode *x)
{
    rbNode *y;

    ///如果x的右子树不为空 则右子树中最小的即为x的后继
    if(x->right != NULL)
    {
        return TreeMinimum(x->right);
    }

    ///如果x的右子树为空，则x的后继只能是从往上找父母 第一个拐点的父结点 因为既然x没有右子树 则x是当前这个分支上最大的数 它的后继只能是统领这个分支的结点
    y = x->parent;

    ///当x不是y的右孩子时结束循环  如果在y==null时结束循环 则说明分支不存在拐点 即不存在后继
    while(y != NULL && x == y->right)
    {
        ///巧妙的语句
        x = y;
        y = x->parent;
    }
    return y;

}

///根据单词和熟悉度来查找
rbNode* RB_SEARCH(rbNode *node,typestring *word)
{

    if(node == NULL || (strcmp(node->word,word)==0))
        return node;

    if(node != NULL)
    {
       if(strcmp(word,node->word) < 0)
         return RB_SEARCH(node->left,word);
       if(strcmp(word,node->word) > 0)
         return RB_SEARCH(node->right,word);
       if(strcmp(word,node->word) == 0)
         return node;
    }
}


///红黑树删除结点
///z为插入节点 也是当前节点
void RB_DELETE(RBRoot *root,typestring *word)
{
    rbNode *z;
    rbNode *y;///用于接受 待删除节点
    rbNode *x;///用于接受 待删除节点的儿子

    ///要删除的结点
    z = RB_SEARCH(root->node,word);
    ///如果待删除结点只有一个儿子或者没有儿子
    if(z->left == NULL || z->right == NULL)
    {
        ///将其赋给y 对y进行操作 即y目前是待删除节点
        y = z;
    }
    else
    {
        ///待删除结点有两个儿子 则 用待删除结点的后继替换待删除结点 然后删去后继即可
        y = RBTree_Successor(z);
    }

    ///case1：z没有儿子 则赋给x的是NULL
    ///case2: z只有一个儿子 则 赋给x的是z的左儿子或右儿子
    ///case3: z有两个儿子 此时y不是z 而是z的后继 判断z的后继有没有左儿子（ 若有左儿子，赋给x左儿子 若无左儿子 赋给x为NULL）
    if(y->left != NULL)
    {
        x = y->left;
    }
    else
    {
        if(y->right != NULL)
            x = y->right;
        else
            x = NULL;
    }

    ///case1: 不执行
    ///case2: x即待删除节点的儿子的父结点设为待删除节点的父结点
    ///case3: 若x是左儿子 则将父结点设为原后继的父结点 若无左儿子 不执行
    if(x != NULL)
    {
       ///直接把待删除结点的父结点与待删除结点的儿子连接  不管y是否是根节点 首先将y的父结点赋给x的父结点
       x->parent = y->parent;
    }


    ///case1: 如果待删除结点为根节点 则将根节点设为x 此时根节点被删除为空 complete
    ///case2: 如果待删除结点为根节点 则将根节点设为待删除节点的儿子 ；如果待删除结点不是根结点 则待删除节点的父结点的左儿子或右儿子 设为待删除节点的左儿子或右儿子 complete
    ///case3: y此时是后继 y必定在z的右子树上 并且y必为右儿子 且最多一个儿子 y的父结点不为空；如果后继y有左儿子 把左儿子设为其父结点的有儿子；若无 则为空
    if(y->parent == NULL)
    {
        ///如果y是根节点 则x调整为根节点
        root->node = x;
    }
    else
    {
        ///y是左孩子
        if(y == y->parent->left)
        {
            ///待删除节点的父节点的左儿子为x
            y->parent->left = x;
        }
        else
            y->parent->right = x;///case3:后继y的父结点的右儿子用y的左儿子替换  即用后继y的儿子替换掉y
    }
    ///用于z有两个儿子的情况  z的后继与z肯定不相同  此时将z的值用后继的值覆盖掉 同时删除z的后继
    if(y != z)
    {
        z->key = y->key;
        z->word = y->word;
    }

    ///case1、case2:y是待删除结点 待删除结点为红色 则不用调整；若待删除结点为黑色 则需要调整
    ///case3:y是待删除结点的后继 等于说删掉了后继 （替换y后 需要调整）
    if(y->color == 1)
        RB_DELETE_FIXUP(root,x,y->parent);
    ///这里为什么return y？？？
   // return y;
}


///红黑树删除结点调整函数
void RB_DELETE_FIXUP(RBRoot *root,rbNode *x,rbNode *parent)
{

    rbNode *w;

    ///替换后的结点x为黑色 并且x不是root结点 循环进行
    while(x== NULL ||(x != root->node && x->color == 1))
    {
        if(x == NULL && x == root->node)
            break;
        ///x为左儿子
        if(x == parent->left)
        {
            ///将w看做x的兄弟
            w = parent->right;
            ///case1：如果x的兄弟是红色
            if(w != NULL && w->color == 0)
            {
                ///case1： 把w的颜色换为黑色 w和x的父结点颜色改为红色 然后进行左旋操作，以x的父结点为轴 然后把
                w->color = 1;
                parent->color = 0;
                LEFT_ROTATE(root,parent);

                ///再把x的兄弟赋给w 其实现在w是原w的左儿子 肯定是黑的 因为经历了左旋操作
                w = parent->right;
            }


            ///case2: （现在w是黑的） 同时w的两个孩子颜色都为黑
            //if(w != NULL && w->left!=NULL && w->right!=NULL)
            //{
                if((w->left == NULL || w->left->color == 1)&& (w->right == NULL || w->right->color == 1))
            {
                ///case2 把w的颜色换成红色 当前节点x移到x的父结点
                w->color = 0;
                x = parent;
                parent = x->parent;
            }
            //}

            ///case3：w的右孩子是黑色的
            else
            {


                if(w->right == NULL ||w->right->color == 1)
                {
                    ///把w的左孩子也设为黑色 w本身的颜色设为红色 进行以w为轴的右旋操作
                    w->left->color = 1;
                    w->color = 0;
                    RIGHT_ROTATE(root,w);
                    ///把w再次设为x的右兄弟
                    w = parent->right;
                }

                ///case4 w的右孩子是红色的
                ///把w的颜色设为和x的父结点的颜色相同，再把父节点的颜色设为黑色 w的右孩子也设为黑色 进行左旋操作
                w->color = parent->color;
                parent->color = 1;
                if(w->right != NULL)
                w->right->color = 1;
                LEFT_ROTATE(root,parent);
                ///把x设为root结点
                x = root->node;
            }
        }
        ///x为右孩子
        else
        {
            w = parent->left;
            if(w != NULL && w->color == 0)
            {
                ///case1
                w->color = 1;
                parent->color = 0;
                RIGHT_ROTATE(root,parent);
                w = parent->left;
            }

            if((w->left == NULL || w->left->color == 1)&& (w->right == NULL || w->right->color == 1))
            {
                ///case2
                w->color = 0;
                x = parent;
                parent = x->parent;
            }

            ///case3
            else
            {if(w->left == NULL || w->left->color == 1)
                {
                    w->right->color = 1;
                    w->color = 0;
                    LEFT_ROTATE(root,w);
                    w = parent->left;
                }

                ///case4
                w->color = parent->color;
                parent->color = 1;
                w->left->color = 1;
                RIGHT_ROTATE(root,parent);
                x = root->node;
            }
        }
    }

    ///?????
    if(x != NULL)
    x->color = 1;
}

///创建红黑树
RBRoot* create_RBTree()
{
    ///动态分配内存
    RBRoot *root = (RBRoot*)malloc(sizeof(RBRoot));
    root->node = NULL;

    return root;
}
void Init_RBTree(rbNode *node)
{
	node = (rbNode*)malloc(sizeof(rbNode));
	node->left = (rbNode*)malloc(sizeof(rbNode));
	node->parent = (rbNode*)malloc(sizeof(rbNode));
	node->right = (rbNode*)malloc(sizeof(rbNode));
	node->word = (typestring *)malloc(40 * sizeof(typestring));
}

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
BNode* create_node(Tree *btree)
{
    int i;
    BNode *node = NULL;
    node = (BNode*)malloc(sizeof(BNode));

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
    node->child = (BNode**)calloc(btree->max + 2,sizeof(BNode));
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
void B_TREE_SPLIT(Tree *btree,BNode* parentNode,BNode* node,int pos)
{
    BNode* tempNode;
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

void btree_InsertNoneFull(Tree *btree,BNode *node,type key,char *word)
{
    int i;
    int j=0;///在找到熟悉度key后 再用j进行循环得到 根据单词序的插入位置
    BNode* child;


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

BNode* btree_insert(Tree *btree,BNode* node,type key,char* word)
{
    BNode* newNode;
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

BNode* btree_max(BNode* node)
{
    BNode* newNode = NULL;
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

BNode* btree_min(BNode* node)
{
    BNode* newNode = NULL;
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

void right_rotate(BNode* parentNode,BNode* currentNode,BNode* nearNode,int i)
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
void left_rotate(BNode* parentNode,BNode* currentNode,BNode* nearNode,int i)
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
	int BTree_merge_child(Tree* btree, BNode* parent, int loc)
	{
	    int i;
	    BNode* lNode;
	    BNode* rNode;

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

	}

	///删除主要分两种情况 一种是要删除key不在当前节点 一种是要删除key在当前结点

	void btree_delete_node(Tree *btree,BNode* node,type key,char *word)
	{
	    int i = 0;
	    int j;
	    type tempkey;
	    char *tempword;

	    ///找出删除位置 当当前节点遍历完 或者 key小于等于当前节点某个key时 跳出循环
	    while(i < node->keyNum && key > node->key[i] || (key == node->key[i] && strcmpi(word,node->word[i]) > 0))
        {
            i++;
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
         if(i < node->keyNum && key == node->key[i] && strcmpi(word,node->word[i]) == 0)
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
                     BNode* newNode;
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
                     BNode* newNode;
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

///根据单词 遍历B树 找到相应的结点
int inorder_Delete(BNode *node,char* word)
{
    int i;
    int a = -1;

    ///当没有找到相应单词时 进行查找

        if(!boolval && node != NULL)
      {
        for(i = node->keyNum;i > 0;i--)
        {
           //if(key == node->key[i])


           if(!boolval && node->child[i] != NULL)
           a = inorder_Delete(node->child[i],word);

           //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
           if(!boolval && strcmpi(node->word[i-1],word) == 0)
           {
               //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
               boolval = 1;
               //printf("== 要删除的单词为%s(%d)\n",node->word[i-1],node->key[i-1]);
               return node->key[i-1];
           }
        }
         if(!boolval && node->child[0] != NULL)
        a = inorder_Delete(node->child[0],word);
       }

     return a;

}

char* inorder_BTree(BNode *node,int num)
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
               //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
               //printf("== 熟悉度为第%d大的单词为%s(%d)\n",num,node->word[i-1],node->key[i-1]);
               printf("%s\n",node->word[i-1]);
               return node->word[i-1];
           }
        }
         if(node->child[0] != NULL)
        inorder_BTree(node->child[0],num);
    }

}

int Aggregate_BTree(BNode *node,int l,int r)
{
    int i;
    char* a;
    if(node != NULL)
    {
        for(i = node->keyNum;i > 0;i--)
        {
           //if(key == node->key[i])


           if(node->child[i] != NULL)
           Aggregate_BTree(node->child[i],l,r);

           m++;
           //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
           if(l<= m && m<= r)
           {
               //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
               //printf("== 熟悉度为第%d大的单词为%s\n",num,node->word[i-1]);
               //m++;
               sum += node->key[i-1];
           }
        }
         if(node->child[0] != NULL)
        Aggregate_BTree(node->child[0],l,r);
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
void BTREE_Print(BNode *node,int layer,int ith)
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


    int i = 0;
    //int ilen = sizeof(a)/sizeof(a[0]);



    rbroot = create_RBTree();
    Init_RBTree(rbroot->node);




    Tree *btree;
    //Tree *btree;
    //printf("====创建B树====\n");
    create_BTree(&btree,4);
    //printf("====向B树插入结点====\n");
   btree->root = create_node(btree);




   int op;
   char *str = (char*)malloc(sizeof(char));
   char *opstr[5]={"Find","Insert","Modify","Delete","Aggregate"};
   char *word;
   int N;
   int j = 0;
   int k;
   int l,r;
   //char word[40] = {NULL};


   int key;
   int deletekey = 0;

   FILE *fp;
   fp = fopen("pj2.txt","r");

   //printf("请输入操作次数: \n");
   fscanf(fp,"%d",&N);

   while(j++ < N)
   {

   //printf("请输入操作次数: \n");
   //scanf("%d",&N);
   fscanf(fp,"%s",str);
    for(i = 0;i<=4;i++)
    {
               if(strcmp(str,opstr[i])==0)
            {
                op = i+1;
                break;
            }
    }
    ///执行操作前先初始化数据
    ///sum:case5中的熟悉度综合
    ///m:从大到小遍历红黑树时的序号
    sum = 0;
    m = 1;
    //printf("1.查询\n2.插入\n3.修改\n4.删除\n5.计算\n请输入操作类型及相关参数: \n");

    //scanf("%d",&op);

    switch(op){
    ///Find
    case 1:
        //scanf("%d",&k);
        fscanf(fp,"%d",&k);
        inorder_BTree(btree->root,k);
        //printf("== 熟悉度为第%d大的单词为%s\n",k,word);
        break;
    ///Insert
    case 2:
        //scanf("%s",deleteWords);
        //printf("%s",deleteWords);

        boolval = 0;
        //scanf("%s %d",word,&key);
        word = (char*)malloc(sizeof(char));
        fscanf(fp,"%s",word);
        fscanf(fp,"%d",&key);
        //deletekey = inorder_Delete(btree->root,word);
        //if(deletekey>=0)
       // {
           // printf("该单词已存在\n");
        //}
        //printf("%s %d",deleteWords,deletekey);
        //else
        btree->root = btree_insert(btree,btree->root,key,word);
        RB_INSERT(rbroot,key,word);
        //printf("== 添加结点 树的详细信息: \n");
        //BTREE_Print(btree->root,1,1);
        break;
    ///Modify
    case 3:
        //inorder_DeleteNode(root->node,"click");
        //RB_DELETE(root,deleteNode->key,deleteNode->word);
        //RB_INSERT(root,10,"click");
        //scanf("%s %d",word,&key);
        boolval = 0;
        word = (char*)malloc(sizeof(char));
        fscanf(fp,"%s",word);
        fscanf(fp,"%d",&key);
        //deletekey = inorder_Delete(btree->root,word);
        deleteNode = RB_SEARCH(rbroot->node,word);
        btree_delete(btree,deleteNode->key,word);
        btree->root = btree_insert(btree,btree->root,key,word);
        ///红黑树插入

        RB_DELETE(rbroot,deleteNode->word);
        RB_INSERT(rbroot,key,word);

        //btree->root = btree_insert(btree,btree->root,17,"B");
        //printf("== 修改树的详细信息: \n");
       // BTREE_Print(btree->root,1,1);
        break;
    ///Delete
    case 4:
        //inorder_DeleteNode(root->node,"amuse");
        //RB_DELETE(root,deleteNode->key,deleteNode->word);
        boolval = 0;
        //scanf("%s",word);
        word = (char*)malloc(sizeof(char));
        fscanf(fp,"%s",word);
        deleteNode = RB_SEARCH(rbroot->node,word);
        btree_delete(btree,deleteNode->key,word);


        RB_DELETE(rbroot,deleteNode->word);
        //printf("== 删除树的详细信息: \n");
       // BTREE_Print(btree->root,1,1);
        break;
    ///Aggregate
    case 5:
        //m = 1;
       // inorder_Aggregate(root->node,l,r);
       //scanf("%d %d",&l,&r);
       fscanf(fp,"%d",&l);
       fscanf(fp,"%d",&r);

           m = 0;
           Aggregate_BTree(btree->root,l,r);

        //printf("== 从第%dth到第%dth的熟悉度总和为%d: \n",l,r,sum);
        printf("%.0f\n",sum);
        break;

    default:
        break;
   }

   }

   system("pause");

}
