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

///����ڵ��  �Ժ�����ĵ������� ���е�ǰ�ڵ�Ϊ�ղ���ڵ�z  ��ǰ�ڵ�϶��Ǻ�ģ�
void RB_INSERT_FIXUP(Root *root,Node *z)
{
    Node *parent,*grandparent;//���常��� �游�ڵ�
    Node *y;

    ///��ĸ��p[z],�游��p[p[z]] y��z��uncle 0�Ǻ�ɫ   ��������������Ǻ�ɫ�ż������� ����Ǻ�ɫ����ֹͣ��������
    while(z->parent->color == 0)
    {
        ///���z�ĸ�����z���游������
        if(z->parent == z->parent->parent->left)
        {
            ///z������
            y = z->parent->parent->right;

            ///case1 z������Ϊ��ɫ
            if(y->color == 0)
            {
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;//��ǰ�ڵ��л�Ϊz���游
            }

            ///case2 z������Ϊ��ɫ  (z���������Ϊ�սڵ� ������else �����Ǻ�ڵ��Ĭ��Ϊ��  ֻ����������ִ�н������Ĳ�������������)
            else
            {
                ///case2 z���Һ��� �����������Ǻ�ɫ ��Ҫ����
                if(z == z->parent->right)
                {
                    z = z->parent;//��ǰ�ڵ��л�Ϊz�ĸ����
                    LEFT_ROTATE(root,z);//���������Ѿ�����ǰ�ڵ�z�л�Ϊz�������

                    ///��ǰ���z�ĸ����Ǻ�ɫ �䱾��Ҳ�Ǻ�ɫ ������ɫ���õ���
                    //z->color = 0;///???
                    //z->parent->color = 1;///???
                }
                else
                {
                    ///case3 z������ �����������Ǻ�ɫ ������  ��ǰ�ڵ㲻�ü������� ��Ϊ��ǰ�ڵ�ĸ�����Ѿ�����Ϊ��ɫ
                    //z = z->parent->parent;
                    ///ʵ�ֽ�z�ĸ������Ϊ��ɫ �游�ڵ���Ϊ��ɫ Ȼ������������� ������滻���游�ڵ�
                    z->parent->color = 1;
                    z->parent->parent->color = 0;

                    ///��z���游Ϊ���������
                    RIGHT_ROTATE(root,z->parent->parent);
                    //z = z->parent->parent;
                }
            }

        }

       ///z�ĸ������z���游�ڵ���Һ���
        else
        {
            ///z��������z���游������
            y = z->parent->parent->left;

            ///case1 z������Ϊ��ɫ
            if(y->color == 0)
            {
                ///������ɫ ���ڵ������ڵ㶼��� �游�ڵ���
                z->parent->color = 1;
                y->color = 1;
                z->parent->parent->color = 0;

                ///��ǰ�ڵ��л�Ϊz���游
                z = z->parent->parent;
            }

            ///case2 z������Ϊ��ɫ
            else
            {
                ///case2 z������ �����������Ǻ�ɫ ��Ҫ����
                if(z == z->parent->left)
                {
                    ///��ǰ�ڵ��л�Ϊz�ĸ����
                    z = z->parent;

                    ///���������Ѿ�����ǰ�ڵ�z�л�Ϊz�������
                    RIGHT_ROTATE(root,z);

                    //��ǰ���z�ĸ����Ǻ�ɫ �䱾��Ҳ�Ǻ�ɫ ������ɫ���õ���
                    //z->parent->color = 1;///???
                    //z->parent->parent->color = 0;///??? ��ǰ�ڵ�z�ĸ��׵���Ϊ��ɫ �游����Ϊ��ɫ��������
                }
                else
                {
                    ///case3 z���Һ��� �����������Ǻ�ɫ ������  ��ǰ�ڵ㲻�ü������� ��Ϊ��ǰ�ڵ�ĸ�����Ѿ�����Ϊ��ɫ
                    //z = z->parent->parent;
                    ///ʵ�ֽ�z�ĸ������Ϊ��ɫ �游�ڵ���Ϊ��ɫ Ȼ������������� ������滻���游�ڵ�
                    z->parent->color = 1;
                    z->parent->parent->color = 0;

                    ///��z���游Ϊ�������
                    LEFT_ROTATE(root,z->parent->parent);
                    //z = z->parent->parent;
                }
            }
        }

    }
    ///���ڵ�����Ϊ��ɫ  ��whileѭ�����ִ�б���
    root->node->color = 1;
}

Node* Tree_Successor(Node *x)
{
    Node *y;

    ///���x����������Ϊ�� ������������С�ļ�Ϊx�ĺ��
    if(x->right != NULL)
    {
        return TreeMinimum(x->right);
    }

    ///���x��������Ϊ�գ���x�ĺ��ֻ���Ǵ�x�����Ҹ�ĸ ��һ���յ�ĸ���� ��Ϊ��Ȼxû�������� ��x�ǵ�ǰ�����֧�������� ���ĺ��ֻ����ͳ�������֧�Ľ��
    y = x->parent;

    ///��x����y���Һ���ʱ����ѭ��  �����y==nullʱ����ѭ�� ��˵����֧�����ڹյ� �������ں��
    while(y != NULL && x == y->right)
    {
        ///��������
        x = y;
        y = x->parent;
    }
    return y;

}

///zΪ����ڵ� Ҳ�ǵ�ǰ�ڵ�
Node* RB_DELETE(Root *root,Node *z)
{
    Node *y;///���ڽ��� ��ɾ���ڵ�
    Node *x;///���ڽ��� ��ɾ���ڵ�Ķ���

    ///�����ɾ�����ֻ��һ�����ӻ���û�ж���
    if(z->left == NULL || z->right == NULL)
    {
        ///���丳��y ��y���в��� ��yĿǰ�Ǵ�ɾ���ڵ�
        y = z;
    }
    else
    {
        ///��ɾ��������������� �� �ô�ɾ�����ĺ���滻��ɾ����� Ȼ��ɾȥ��̼���
        y = Tree_Successor(z);
    }

    ///case1��zû�ж��� �򸳸�x����NULL
    ///case2: zֻ��һ������ �� ����x����z������ӻ��Ҷ���
    ///case3: z���������� ��ʱy����z ����z�ĺ�� �ж�z�ĺ����û������ӣ� ��������ӣ�����x����� ��������� ����xΪNULL��
    if(y->left != NULL)
    {
        x = y->left;
    }
    else
    {
        x = y->right;
    }

    ///case1: ��ִ��
    ///case2: x����ɾ���ڵ�Ķ��ӵĸ������Ϊ��ɾ���ڵ�ĸ����
    ///case3: ��x������� �򽫸������Ϊԭ��̵ĸ���� ��������� ��ִ��
    if(x != NULL)
    {
       ///ֱ�ӰѴ�ɾ�����ĸ�������ɾ�����Ķ�������  ����y�Ƿ��Ǹ��ڵ� ���Ƚ�y�ĸ���㸳��x�ĸ����
       x->parent = y->parent;
    }

    ///case1: �����ɾ�����Ϊ���ڵ� �򽫸��ڵ���Ϊx ��ʱ���ڵ㱻ɾ��Ϊ�� complete
    ///case2: �����ɾ�����Ϊ���ڵ� �򽫸��ڵ���Ϊ��ɾ���ڵ�Ķ��� �������ɾ����㲻�Ǹ���� ���ɾ���ڵ�ĸ���������ӻ��Ҷ��� ��Ϊ��ɾ���ڵ������ӻ��Ҷ��� complete
    ///case3: y��ʱ�Ǻ�� y�ض���z���������� ����y��Ϊ�Ҷ��� �����һ������ y�ĸ���㲻Ϊ�գ�������y������� ���������Ϊ�丸�����ж��ӣ����� ��Ϊ��
    if(y->parent == NULL)
    {
        ///���y�Ǹ��ڵ� ��x����Ϊ���ڵ�
        root->node = x;
    }
    else
    {
        ///y������
        if(y == y->parent->left)
        {
            ///��ɾ���ڵ�ĸ��ڵ�������Ϊx
            y->parent->left = x;
        }
        else
            y->parent->right = x;///case3:���y�ĸ������Ҷ�����y��������滻  ���ú��y�Ķ����滻��y

    ///����z���������ӵ����  z�ĺ����z�϶�����ͬ  ��ʱ��z��ֵ�ú�̵�ֵ���ǵ� ͬʱɾ��z�ĺ��
    if(y != z)
    {
        z->key = y->key;
    }

    ///case1��case2:y�Ǵ�ɾ����� ��ɾ�����Ϊ��ɫ ���õ���������ɾ�����Ϊ��ɫ ����Ҫ����
    ///case3:y�Ǵ�ɾ�����ĺ�� ����˵ɾ���˺�� ���滻y�� ��Ҫ������
    if(y->color = 1)
        RB_DELETE_FIXUP(root,x);
    ///����Ϊʲôreturn y������
    return y;
}

void RB_DELETE_FIXUP(Root *root,Node *x)
{

    Node *w;

    ///�滻��Ľ��xΪ��ɫ ����x����root��� ѭ������
    while(x != root->node && x->color == 1)
    {
        ///xΪ�����
        if(x == x->parent->left)
        {
            ///��w����x���ֵ�
            w = x->parent->right;
            ///case1�����x���ֵ��Ǻ�ɫ
            if(w->color == 0)
            {
                ///case1�� ��w����ɫ��Ϊ��ɫ w��x�ĸ������ɫ��Ϊ��ɫ Ȼ�����������������x�ĸ����Ϊ�� Ȼ���
                w->color = 1;
                x->parent->color = 0;
                LEFT_ROTATE(root,x->parent);

                ///�ٰ�x���ֵܸ���w ��ʵ����w��ԭw������� �϶��Ǻڵ� ��Ϊ��������������
                w = x->parent->right;
            }

            ///case2: ������w�Ǻڵģ� ͬʱw������������ɫ��Ϊ��
            if(w->left->color == 1 && w->right->color == 1)
            {
                ///case2 ��w����ɫ���ɺ�ɫ ��ǰ�ڵ�x�Ƶ�x�ĸ����
                w->color = 0;
                x = x->parent;
            }

            ///case3��w���Һ����Ǻ�ɫ��
            else if(w->right->color == 1)
                {
                    ///��w������Ҳ��Ϊ��ɫ w�������ɫ��Ϊ��ɫ ������wΪ�����������
                    w->left->color = 1;
                    w->color = 0;
                    RIGHT_ROTATE(root,w);
                    ///��w�ٴ���Ϊx�����ֵ�
                    w = x->parent->right;
                }

                ///case4 x���Һ����Ǻ�ɫ��
                ///��w����ɫ��Ϊ��x�ĸ�������ɫ��ͬ���ٰѸ��ڵ����ɫ��Ϊ��ɫ w���Һ���Ҳ��Ϊ��ɫ ������������
                w->color = x->parent->color;
                x->parent->color = 1;
                w->right->color = 1;
                LEFT_ROTATE(root,x->parent);
                ///��x��Ϊroot���
                x = root->node;
        }
        ///xΪ�Һ���
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
