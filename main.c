#include <stdio.h>
#include <stdlib.h>
///projectĿ�ģ�һ�����������
///���ݽṹ�ú���� :������Ļ�����������ӡ�ɾ������ת��
/*����������ܣ�
1����ѯ��k�ߵ���Ϥ�ȷ�����Ӧ�ĵ���
2�����룺����һ����Ϥ��Ϊv�ĵ���w
3���޸ģ��޸ĵ���w����Ϥ��Ϊv
4��ɾ����ɾ������w
5���������Ϥ�ȴӵ�l�󵽵�r��ĵ�����Ϥ�ȵ��ܺ�
���ʳ���<=40,0<=v<=10000000
*/

#define RED 0
#define BLACK 1

typedef int type;
///������Ķ���:��������㣬���ӣ��Һ��� �Լ���ǰ�ڵ����ɫ��ֵ��key��
///���������Ľ��
typedef struct RBTree_Node
{
    unsigned char color;
    type key;//���ڵ��ֵ�����������ֻ����ַ���
    struct RBTree_Node *parent;
    struct RBTree_Node *left;
    struct RBTree_Node *right;
}Node,*RBTree;//Node��struct RBTree_Node�ı��� ������Ϊ*parent *left *right�����ڽṹ����� ���Բ����ñ�������

///���������ĸ�
typedef struct RB_RootNode
{
    Node *node;
}Root;

///����x�ڵ����������  ��ʵ����ÿ�����ĺ��ӣ����װ� ����������Ҫ�漰x���Һ��ӣ���y�� x�ĸ��� y������ y�ĸ��� �Լ�y�����ӵĸ���
void LEFT_ROTATE(Root *root,Node *xnode)
{
    Node *y;

    ///����yΪx���Һ���
    y = xnode->right;
    ///����x���Һ���Ϊy������
    xnode->right = y->left;
    ///����y�����ӵĸ���Ϊx�����y�����Ӳ�Ϊ��
    if(y->left != NULL)
    y->left->parent = x;

    ///����y�ĸ���Ϊx�ĸ���
    y->parent = xnode->parent;

    ///���x���ĸ���Ϊ�գ���xΪ���ڵ㣬��y��Ϊ���ڵ�
    if(xnode->parent == NULL)
        Root->node = y;
    ///���x���ĸ��ײ�Ϊ�գ���xΪ�丸�׽ڵ�����ӻ����Һ��ӣ��ͽ�y��Ϊ���ӻ��Һ���
    else
    {
        if(xnode == xnode->parent->left)
        xnode->parent->left = y;
        if(xnode == xnode->parent->right)
        xnode->parent->right = y;
    }

    ///ʣ��y��x֮��Ĺ�ϵû����ȷ ��x�ĸ�����Ϊy ��y��������Ϊx
    xnode->parent = y;
    y->left = xnode;



}
///�Խ��xΪ�� ��x�ĺ���y��Ϊ y��x�ĸ���
///����x�ڵ����������  ��ʵ����ÿ�����ĺ��ӣ����װ󶨡�
///����������Ҫ�漰x���Һ��ӣ���y�� x�ĸ��� y������ y�ĸ��� �Լ�y�����ӵĸ���
void RIGHT_ROTATE(Root *root,Node *xnode)
{
    Node *y;

    ///����yΪ��ǰ���x���Һ���
    y = xnode->left;
    ///����x���Һ���Ϊy������
    xnode->left = y->right;
    ///����y�����ӵĸ���Ϊx�����y�����Ӳ�Ϊ��
    if(y->right != NULL)
    y->right->parent = x;

    ///����y�ĸ���Ϊx�ĸ���
    y->parent = xnode->parent;

    ///���x���ĸ���Ϊ�գ���xΪ���ڵ㣬��y��Ϊ���ڵ�
    if(xnode->parent == NULL)
        Root->node = y;
    ///���x���ĸ��ײ�Ϊ�գ���xΪ�丸�׽ڵ�����ӻ����Һ��ӣ��ͽ�y��Ϊ���ӻ��Һ���
    else
    {
        if(xnode == xnode->parent->left)
        xnode->parent->left = y;
        if(xnode == xnode->parent->right)
        xnode->parent->right = y;
    }

    ///ʣ��y��x֮��Ĺ�ϵû����ȷ ��x�ĸ�����Ϊy ��y��������Ϊx
    xnode->parent = y;
    y->right = xnode;
}
void RB_INSERT(Root *root,Node *node)
{

}

void RB_INSERT_FIXUP(Root *root,Node *node)
{
    Node *parent,*grandparent;
    while(node->parent=="RED")//��ĸ��p[z],�游��p[p[z]] y��z��uncle
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
