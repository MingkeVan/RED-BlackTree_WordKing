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

//插入节点后  对红黑树的调整操作 其中当前节点为刚插入节点z
void RB_INSERT_FIXUP(Root *root,Node *z)
{
    Node *parent,*grandparent;//定义父结点 祖父节点
    Node *y;

    while(z->parent->color == 0)//父母是p[z],祖父是p[p[z]] y是z的uncle 0是红色   （！！！父结点是红色才继续进行 如果是黑色，则停止！！！）
    {
        if(z->parent == z->parent->parent->left)//如果z的父母是z的祖父的左孩子
        {
            y = z->parent->parent->right;//z的叔叔

            ///case1
            if(y->color == 0)//z的叔叔为红色
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;//当前节点切换为z的祖父
            }

            ///case2
            else
            {
                ///case2 z是右孩子 且它的叔叔是黑色 需要左旋
                if(z == z->parent->right)
                {
                    z = z->parent;//当前节点切换为z的父结点
                    LEFT_ROTATE(root,z);//左旋操作已经将当前节点z切换为z的左儿子

                    z->color = 0;
                    z->parent->color = 0;
                }
                else
            }

        }
    }
}

int main()
{
    printf("Hello world!\n");
    return 0;
}
