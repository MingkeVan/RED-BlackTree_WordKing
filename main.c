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
void RB_INSERT(Root *root,Node *node)
{

}

void RB_INSERT_FIXUP(Root *root,Node *node)
{
    Node *parent,*grandparent;
    while(node->parent=="RED")//父母是p[z],祖父是p[p[z]] y是z的uncle
    {
        if(p[z]==left[p[p[z]]])
        {
            y = right[p[p[z]]];
        }
    }
}

int main()
{
    printf("Hello world!\n");
    return 0;
}
