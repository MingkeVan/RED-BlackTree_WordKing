#include <stdio.h>
#include <stdlib.h>
#include <string.h>
///project目的：一个背单词软件
///数据结构用红黑树 :红黑树的基本操作是添加、删除和旋转。
/*包含五个功能：
1、查询第k高的熟悉度分数对应的单词
2、插入：插入一个熟悉度为v的单词w
3、修改：修改单词w的熟悉度为v
4、删除：删除单词w
5、计算从熟悉度从第l大到第r大的单词熟悉度的总和
单词长度<=40,0<=v<=10000000
*/

#define RED 0
#define BLACK 1




typedef int type;
typedef char typestring;
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
}Node,*RBTree;//Node是struct RBTree_Node的别名 但是因为*parent *left *right都是在结构体里边 所以不能用别名定义

static Node *kthnode;
static Node *deleteNode;
static Node *ArrNode[];
static int sum;
///用于标记标记遍历所取到的值的序号 从1开始
static int m = 1;

///定义红黑树的根
typedef struct RB_RootNode
{
    Node *node;
}Root;

///全局根结点
Root *root = NULL;

///定义x节点的左旋操作  其实就是每个结点的孩子，父亲绑定 左旋操作主要涉及x的右孩子（即y） x的父亲 y的左孩子 y的父亲 以及y的左孩子的父亲
void LEFT_ROTATE(Root *root,Node *xnode)
{
    Node *y;

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
void RIGHT_ROTATE(Root *root,Node *xnode)
{
    Node *y;

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
void RB_INSERT(Root *root,type key,typestring *word)//node为插入节点
{
    ///定义一个新的节点 键值为插入的key
    Node *z = (Node*)malloc(sizeof(Node));
    z->key = key;
    z->word = word;


    Node *y = NULL;//把新插入结点看做空结点
    Node *x=root->node;//把x当做根节点

   // if(x == NULL)
    //{
    //  root->node = z;
    //}
    ///从根节点往下找出插入节点应该插入的位置
    while(x != NULL)
    {
        y = x;
        if(z->key < x->key)
            x = x->left;
        else if(z->key > x->key)
            x = x->right;

        ///熟悉度相同时 按字母顺序插入
        else if(z->key == x->key)
        {
            int i=0;
            for(i=0;i < strlen(z->word);i++)
            {
                if(z->word[i] < x->word[i])
                {
                    x = x->left;
                    break;
                }
                else if(z->word[i] > x->word[i])
                {
                     x = x->right;
                     break;
                }
                   ///如相等 continue
                    continue;



            }
            ///如果插入单词与当前节点单词每一位都相等 则说明该单词按字母顺序小于当前单词 插左边
            if(i == strlen(z->word))
                x = x->left;
        }
    }
    z->parent = y;//这里将x赋给node的父母是否合适   不合适 x已经为NULL 故用y接受x 并将y作为node的父结点

    if(y == NULL)
    {
        root->node = z;
        //z->parent = NULL;
    }
    else
    {
        if(z->key < y->key)
        y->left = z;
        //else
        //y->right = z;
       else if(z->key > y->key)
            y->right = z;

        ///熟悉度相同时 按字母顺序插入
       else if(z->key == y->key)
        {
            int i=0;
            for(i = 0;i < strlen(z->word); i++)
            {
                if(z->word[i] < y->word[i])
                {
                    y->left = z;
                    break;
                }
                else if(z->word[i] > y->word[i])
                    {
                        y->right = z;
                        break;
                    }

                    continue;
            }
            ///如果插入单词与当前节点单词每一位都相等 则说明该单词按字母顺序小于当前单词 插左边
            if(i == strlen(z->word))
                y->left = z;
        }
    }

    z->left = NULL;
    z->right = NULL;
    z->color = RED;//////////////////////////////

    RB_INSERT_FIXUP(root,z);
}

///插入节点后  对红黑树的调整操作 其中当前节点为刚插入节点z  当前节点肯定是红的？
void RB_INSERT_FIXUP(Root *root,Node *z)
{
    Node *parent = (Node*)malloc(sizeof(Node));
    Node *grandparent;//定义父结点 祖父节点
    Node *y = (Node*)malloc(sizeof(Node));

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
                else
                {
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
                else
                {
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

    }

    ///根节点设置为黑色  在while循环外边执行本句
    root->node->color = BLACK;
}

///找出以node为根节点的子树的最小值
Node* TreeMinimum(Node *node)
{
    if(node->left != NULL)
        return TreeMinimum(node->left);
    else
        return node;
}

///找出结点x的后继
Node* Tree_Successor(Node *x)
{
    Node *y;

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
Node* RB_SEARCH(Node *node,type key,typestring *word)
{

    if(node == NULL || ((node->key == key)&&(node->word == word)))
        return node;

    if(node != NULL)
    {
        if( key < node->key )
       {
         return RB_SEARCH(node->left,key,word);
       }
       if( key > node->key )
         return RB_SEARCH(node->right,key,word);
       if(key == node->key)
       {
           int i=0;
            for(i = 0;i < strlen(node->word); i++)
            {
                if(word[i] < node->word[i])
                {
                    return RB_SEARCH(node->left,key,word);
                    break;
                }
                else if(word[i] > node->word[i])
                    {
                        return RB_SEARCH(node->right,key,word);
                        break;
                    }
                if(i == strlen(node->word)-1)
                    {
                        return node;
                    }
                    continue;
            }
       }
    }
}


///红黑树删除结点
///z为插入节点 也是当前节点
void RB_DELETE(Root *root,type key,typestring *word)
{
    Node *z;
    Node *y;///用于接受 待删除节点
    Node *x;///用于接受 待删除节点的儿子

    ///要删除的结点
    z = RB_SEARCH(root->node,key,word);
    ///如果待删除结点只有一个儿子或者没有儿子
    if(z->left == NULL || z->right == NULL)
    {
        ///将其赋给y 对y进行操作 即y目前是待删除节点
        y = z;
    }
    else
    {
        ///待删除结点有两个儿子 则 用待删除结点的后继替换待删除结点 然后删去后继即可
        y = Tree_Successor(z);
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
        x = y->right;
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
    }

    ///case1、case2:y是待删除结点 待删除结点为红色 则不用调整；若待删除结点为黑色 则需要调整
    ///case3:y是待删除结点的后继 等于说删掉了后继 （替换y后 需要调整）
    if(y->color == 1)
        RB_DELETE_FIXUP(root,x);
    ///这里为什么return y？？？
   // return y;
}


///红黑树删除结点调整函数
void RB_DELETE_FIXUP(Root *root,Node *x)
{

    Node *w;

    ///替换后的结点x为黑色 并且x不是root结点 循环进行
    while(x != NULL && x != root->node && x->color == 1)
    {
        ///x为左儿子
        if(x == x->parent->left)
        {
            ///将w看做x的兄弟
            w = x->parent->right;
            ///case1：如果x的兄弟是红色
            if(w->color == 0)
            {
                ///case1： 把w的颜色换为黑色 w和x的父结点颜色改为红色 然后进行左旋操作，以x的父结点为轴 然后把
                w->color = 1;
                x->parent->color = 0;
                LEFT_ROTATE(root,x->parent);

                ///再把x的兄弟赋给w 其实现在w是原w的左儿子 肯定是黑的 因为经历了左旋操作
                w = x->parent->right;
            }

            ///case2: （现在w是黑的） 同时w的两个孩子颜色都为黑
            if(w->left->color == 1 && w->right->color == 1)
            {
                ///case2 把w的颜色换成红色 当前节点x移到x的父结点
                w->color = 0;
                x = x->parent;
            }

            ///case3：w的右孩子是黑色的
            else if(w->right->color == 1)
                {
                    ///把w的左孩子也设为黑色 w本身的颜色设为红色 进行以w为轴的右旋操作
                    w->left->color = 1;
                    w->color = 0;
                    RIGHT_ROTATE(root,w);
                    ///把w再次设为x的右兄弟
                    w = x->parent->right;
                }

                ///case4 x的右孩子是红色的
                ///把w的颜色设为和x的父结点的颜色相同，再把父节点的颜色设为黑色 w的右孩子也设为黑色 进行左旋操作
                w->color = x->parent->color;
                x->parent->color = 1;
                w->right->color = 1;
                LEFT_ROTATE(root,x->parent);
                ///把x设为root结点
                x = root->node;
        }
        ///x为右孩子
        else
        {
            w = x->parent->left;
            if(w != NULL && w->color == 0)
            {
                ///case1
                w->color = 1;
                x->parent->color = 0;
                RIGHT_ROTATE(root,x->parent);
                w = x->parent->left;
            }
            if(w != NULL && w->left->color == 1 && w->right->color == 1)
            {
                ///case2
                w->color = 0;
                x = x->parent;
            }

            ///case3
            else if(x->left->color == 1)
                {
                    w->right->color = 1;
                    w->color = 0;
                    LEFT_ROTATE(root,w);
                    w = x->parent->left;
                }

                ///case4
                w->color = x->parent->color;
                x->parent->color = 1;
                w->left->color = 1;
                RIGHT_ROTATE(root,x->parent);
                x = root;
        }
    }

    ///?????
    if(x != NULL)
    x->color = 1;
}


///中序遍历 用来根据要被删除的单词找到相应的结点
Node* inorder_DeleteNode(Node *node,typestring* deleteword)
{

    //static char *a[10];
   int i;
   //中序遍历
   if(deleteNode != NULL)
   {
       return;
   }
   if(node != NULL)
    {
        inorder_DeleteNode(node->right,deleteword);
        //a[m] = node->word;
        for(i = 0;i < strlen(deleteword);i++)
        {
            if(node->word[i] != deleteword[i])
                break;
            //printf("%s ",a[i]);
            if(i == strlen(deleteword)-1)
            {
                deleteNode = node;
                break;
            }
        }
        inorder_DeleteNode(node->left,deleteword);
    }
}
///中序遍历 用来找到按熟悉度排kth的单词
void inorder_tree(Node *node,int k)
{
    //static int m = 1;
    static char *a[10];

   //中序遍历
   if(node != NULL)
    {
        inorder_tree(node->right,k);
        a[m] = node->word;
        if(m == k)
        {
            //printf("%s ",a[i]);
            kthnode = node;
        }
        m++;
        inorder_tree(node->left,k);
    }

/*
    ///找出第k个最大值
    while(node != NULL)
    {

        if(node->right != NULL)
            node = node->right;
        else
        {
            kth ++;


            if(node->left != NULL)
              {

                node = node->left;

              }
              else
                {
                    kth ++;

                }
            if(kth == k)
            return node->word;
        }
    }*/
}

///中序遍历 用来找到按熟悉度排lth到kth的单词
void inorder_Aggregate(Node *node,int l,int r)
{
    ///用于标记标记遍历所取到的值的序号
    //static int m = 1;
   // static char *a[10];

    if(m >= r+1)
    {

        return;
    }
   //中序遍历
   if(node != NULL)
    {
        inorder_Aggregate(node->right,l,r);
        //a[m] = node->word;
        if(l <= m && m<= r)
        {
            //printf("%s ",a[i]);
            //kthnode = node;
            sum += node->key;

        }
        m++;
        inorder_Aggregate(node->left,l,r);
    }


}

///红黑树根据熟悉度也就是key 排序(其实就是中序遍历
void RB_QUERY(Root *root,int k)
{
    if(root)
    {
        return inorder_tree(root->node,k);
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
void RBTREE_Print(RBTree tree,typestring* word,type key,int direction)
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
void Print_RBTREE(Root *root)
{
    if(root != NULL && root->node != NULL)
    {
        RBTREE_Print(root->node,root->node->word,root->node->key,0);
    }
}

///创建红黑树
Root* create_RBTree()
{
    ///动态分配内存
    Root *root = (Root*)malloc(sizeof(Root));
    root->node = NULL;

    return root;
}
void Init_RBTree(Node *node)
{
	node = (Node*)malloc(sizeof(Node));
	node->left = (Node*)malloc(sizeof(Node));
	node->parent = (Node*)malloc(sizeof(Node));
	node->right = (Node*)malloc(sizeof(Node));
	node->word = (typestring *)malloc(40 * sizeof(typestring));
}
int main()
{
    int a[] = {10, 10, 10, 10, 10, 10, 20, 50, 80};
    char b[9][9] = {"a","abandon","avoid","amuse","bind","bike","beyond","clone","click"};
    ///strlen()是求字符串长度的 用下边函数得出数组长度
    int i, ilen=sizeof(a)/sizeof(a[0]);


    root = create_RBTree();
    Init_RBTree(root->node);

    printf("== 原始数据: ");
    for(i=0; i<ilen; i++)
      printf("%d ", a[i]);
    printf("\n");

    for(i=0; i<ilen; i++)
     {
         RB_INSERT(root, a[i],b[i]);
//#if CHECK_INSERT
         printf("== 添加节点: %d\n", a[i]);
         printf("== 树的详细信息: \n");
         Print_RBTREE(root);
         printf("\n");
//#endif
     }
/*
for(i=0; i<ilen; i++)
     {
         RB_DELETE(root, a[i],b[i]);

         printf("== 删除节点: %d\n", a[i]);
        if (root)
         {
             printf("== 树的详细信息: \n");
             Print_RBTREE(root);
            printf("\n");
         }
    }*/

   int op;
   int N;
   int j = 0;
   int k;
   int l,r;
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
    scanf("%d %d",&op,&k);
    //scanf("%d %d %d",&op,&l,&r);
    switch(op){
    ///Find
    case 1:

        RB_QUERY(root,k);
        printf("== 熟悉度为第%d大的单词为%s\n",k,kthnode->word);
        break;
    ///Insert
    case 2:
        RB_INSERT(root,10,"amazing");
        printf("== 添加结点 树的详细信息: \n");
        Print_RBTREE(root);
        break;
    ///Modify
    case 3:
        inorder_DeleteNode(root->node,"click");
        RB_DELETE(root,deleteNode->key,deleteNode->word);
        RB_INSERT(root,10,"click");
        printf("== 修改树的详细信息: \n");
        Print_RBTREE(root);
        break;
    ///Delete
    case 4:
        inorder_DeleteNode(root->node,"amuse");
        RB_DELETE(root,deleteNode->key,deleteNode->word);
        printf("== 删除树的详细信息: \n");
        Print_RBTREE(root);
        break;
    ///Aggregate
    case 5:
        m = 1;
        inorder_Aggregate(root->node,l,r);
        printf("== 从第%dth到第%dth的熟悉度总和为%d: \n",l,r,sum);
        break;

    default:
        break;
   }

   }

   system("pause");
    return 0;
}
