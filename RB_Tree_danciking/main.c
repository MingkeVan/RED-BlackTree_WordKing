#include <stdio.h>
#include <stdlib.h>
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
///红黑树的定义:包含父结点，左孩子，右孩子 以及当前节点的颜色和值（key）
///定义红黑树的结点
typedef struct RBTree_Node
{
    unsigned char color;
    type key;//即节点的值，可能是数字或者字符串
    struct RBTree_Node *parent;
    struct RBTree_Node *left;
    struct RBTree_Node *right;
}Node,*RBTree;//Node是struct RBTree_Node的别名 但是因为*parent *left *right都是在结构体里边 所以不能用别名定义

///定义红黑树的根
typedef struct RB_RootNode
{
    Node *node;
}Root;

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
    y->left->parent = x;

    ///设置y的父亲为x的父亲
    y->parent = xnode->parent;

    ///如果x结点的父亲为空，即x为根节点，则将y设为根节点
    if(xnode->parent == NULL)
        Root->node = y;
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
    y->right->parent = x;

    ///设置y的父亲为x的父亲
    y->parent = xnode->parent;

    ///如果x结点的父亲为空，即x为根节点，则将y设为根节点
    if(xnode->parent == NULL)
        Root->node = y;
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
///添加操作  将红黑树当做一个二叉查找树 将结点插入 并将结点着色为红色（这样不会违背红黑树的特性5 即黑高度不变）
void RB_INSERT(Root *root,Node *z)//node为插入节点
{
    Node *y = NULL;//把握新插入结点看做空结点
    Node *x=root->node;//把x当做根节点

    ///从根节点往下找出插入节点应该插入的位置
    while(x != NULL)
    {
        y = x;
        if(z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;//这里将x赋给node的父母是否合适   不合适 x已经为NULL 故用y接受x 并将y作为node的父结点

    if(y == NULL)
    {
        root->node = z;
    }
    else
    {
        if(z->key < y->key)
        y->left = z;
        else
        y->right = z;
    }
    z->color = 0;

    RB_INSERT_FIXUP(root,z);
}

///插入节点后  对红黑树的调整操作 其中当前节点为刚插入节点z  当前节点肯定是红的？
void RB_INSERT_FIXUP(Root *root,Node *z)
{
    Node *parent,*grandparent;//定义父结点 祖父节点
    Node *y;

    ///父母是p[z],祖父是p[p[z]] y是z的uncle 0是红色   （！！！父结点是红色才继续进行 如果是黑色，则停止！！！）
    while(z->parent->color == 0)
    {
        ///如果z的父亲是z的祖父的左孩子
        if(z->parent == z->parent->parent->left)
        {
            ///z的叔叔
            y = z->parent->parent->right;

            ///case1 z的叔叔为红色
            if(y->color == 0)
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
                    //z = z->parent->parent;
                }
            }

        }

       ///z的父结点是z的祖父节点的右孩子
        else
        {
            ///z的叔叔是z的祖父的左孩子
            y = z->parent->parent->left;

            ///case1 z的叔叔为红色
            if(y->color == 0)
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
    root->node->color = 1;
}

Node* Tree_Successor(Node *x)
{
    Node *y;

    ///如果x的右子树不为空 则右子树中最小的即为x的后继
    if(x->right != NULL)
    {
        return TreeMinimum(x->right);
    }

    ///如果x的右子树为空，则x的后继只能是从x网上找父母 第一个拐点的父结点 因为既然x没有右子树 则x是当前这个分支上最大的数 它的后继只能是统领这个分支的结点
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

///z为插入节点 也是当前节点
Node* RB_DELETE(Root *root,Node *z)
{
    Node *y;///用于接受 待删除节点
    Node *x;///用于接受 待删除节点的儿子

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

    ///用于z有两个儿子的情况  z的后继与z肯定不相同  此时将z的值用后继的值覆盖掉 同时删除z的后继
    if(y != z)
    {
        z->key = y->key;
    }

    ///case1、case2:y是待删除结点 待删除结点为红色 则不用调整；若待删除结点为黑色 则需要调整
    ///case3:y是待删除结点的后继 等于说删掉了后继 （替换y后 需要调整）
    if(y->color = 1)
        RB_DELETE_FIXUP(root,x);
    ///这里为什么return y？？？
    return y;
}

void RB_DELETE_FIXUP(Root *root,Node *x)
{

    Node *w;

    ///替换后的结点x为黑色 并且x不是root结点 循环进行
    while(x != root->node && x->color == 1)
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
            if(w->color == 0)
            {
                ///case1
                w->color = 1;
                x->parent->color = 0;
                RIGHT_ROTATE(root,x->parent);
                w = x->parent->left;
            }
            if(w->left->color == 1 && w->right->color == 1)
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
    x->color = 1;
}
int main()
{
    printf("Hello world!\n");
    return 0;
}
