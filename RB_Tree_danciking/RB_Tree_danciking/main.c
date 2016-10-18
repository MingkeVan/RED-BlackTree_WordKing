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
///��Ӳ���  �����������һ����������� �������� ���������ɫΪ��ɫ����������Υ�������������5 ���ڸ߶Ȳ��䣩
void RB_INSERT(Root *root,Node *z)//nodeΪ����ڵ�
{
    Node *y = NULL;//�����²����㿴���ս��
    Node *x=root->node;//��x�������ڵ�

    ///�Ӹ��ڵ������ҳ�����ڵ�Ӧ�ò����λ��
    while(x != NULL)
    {
        y = x;
        if(z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;//���ｫx����node�ĸ�ĸ�Ƿ����   ������ x�Ѿ�ΪNULL ����y����x ����y��Ϊnode�ĸ����

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

//����ڵ��  �Ժ�����ĵ������� ���е�ǰ�ڵ�Ϊ�ղ���ڵ�z
void RB_INSERT_FIXUP(Root *root,Node *z)
{
    Node *parent,*grandparent;//���常��� �游�ڵ�
    Node *y;

    while(z->parent->color == 0)//��ĸ��p[z],�游��p[p[z]] y��z��uncle 0�Ǻ�ɫ   ��������������Ǻ�ɫ�ż������� ����Ǻ�ɫ����ֹͣ��������
    {
        if(z->parent == z->parent->parent->left)//���z�ĸ�ĸ��z���游������
        {
            y = z->parent->parent->right;//z������

            ///case1
            if(y->color == 0)//z������Ϊ��ɫ
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;//��ǰ�ڵ��л�Ϊz���游
            }

            ///case2
            else
            {
                ///case2 z���Һ��� �����������Ǻ�ɫ ��Ҫ����
                if(z == z->parent->right)
                {
                    z = z->parent;//��ǰ�ڵ��л�Ϊz�ĸ����
                    LEFT_ROTATE(root,z);//���������Ѿ�����ǰ�ڵ�z�л�Ϊz�������

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
