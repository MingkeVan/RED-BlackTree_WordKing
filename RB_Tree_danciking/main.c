#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
typedef char typestring;
///������Ķ���:��������㣬���ӣ��Һ��� �Լ���ǰ�ڵ����ɫ��ֵ��key��
///���������Ľ��
typedef struct RBTree_Node
{
    unsigned char color;
    ///����wordΪ����
    typestring *word;
    ///������key����Ϊ���ʵ���Ϥ�� 0<=key<=100
    type key;
    struct RBTree_Node *parent;
    struct RBTree_Node *left;
    struct RBTree_Node *right;
}Node,*RBTree;//Node��struct RBTree_Node�ı��� ������Ϊ*parent *left *right�����ڽṹ����� ���Բ����ñ�������

static Node *kthnode;
static Node *deleteNode;
static Node *ArrNode[];
static int sum;
///���ڱ�Ǳ�Ǳ�����ȡ����ֵ����� ��1��ʼ
static int m = 1;

///���������ĸ�
typedef struct RB_RootNode
{
    Node *node;
}Root;

///ȫ�ָ����
Root *root = NULL;

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
    y->left->parent = xnode;

    ///����y�ĸ���Ϊx�ĸ���
    y->parent = xnode->parent;

    ///���x���ĸ���Ϊ�գ���xΪ���ڵ㣬��y��Ϊ���ڵ�
    if(xnode->parent == NULL)
        root->node = y;
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
    y->right->parent = xnode;

    ///����y�ĸ���Ϊx�ĸ���
    y->parent = xnode->parent;

    ///���x���ĸ���Ϊ�գ���xΪ���ڵ㣬��y��Ϊ���ڵ�
    if(xnode->parent == NULL)
        root->node = y;
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
///��Ӳ���  �����������һ����������� �������� ���������ɫΪ��ɫ����������Υ�������������5 ���ڸ߶Ȳ��䣩fan �����Ӧ���Ǽ�ֵkey
void RB_INSERT(Root *root,type key,typestring *word)//nodeΪ����ڵ�
{
    ///����һ���µĽڵ� ��ֵΪ�����key
    Node *z = (Node*)malloc(sizeof(Node));
    z->key = key;
    z->word = word;


    Node *y = NULL;//���²����㿴���ս��
    Node *x=root->node;//��x�������ڵ�

   // if(x == NULL)
    //{
    //  root->node = z;
    //}
    ///�Ӹ��ڵ������ҳ�����ڵ�Ӧ�ò����λ��
    while(x != NULL)
    {
        y = x;
        if(z->key < x->key)
            x = x->left;
        else if(z->key > x->key)
            x = x->right;

        ///��Ϥ����ͬʱ ����ĸ˳�����
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
                   ///����� continue
                    continue;



            }
            ///������뵥���뵱ǰ�ڵ㵥��ÿһλ����� ��˵���õ��ʰ���ĸ˳��С�ڵ�ǰ���� �����
            if(i == strlen(z->word))
                x = x->left;
        }
    }
    z->parent = y;//���ｫx����node�ĸ�ĸ�Ƿ����   ������ x�Ѿ�ΪNULL ����y����x ����y��Ϊnode�ĸ����

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

        ///��Ϥ����ͬʱ ����ĸ˳�����
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
            ///������뵥���뵱ǰ�ڵ㵥��ÿһλ����� ��˵���õ��ʰ���ĸ˳��С�ڵ�ǰ���� �����
            if(i == strlen(z->word))
                y->left = z;
        }
    }

    z->left = NULL;
    z->right = NULL;
    z->color = RED;//////////////////////////////

    RB_INSERT_FIXUP(root,z);
}

///����ڵ��  �Ժ�����ĵ������� ���е�ǰ�ڵ�Ϊ�ղ���ڵ�z  ��ǰ�ڵ�϶��Ǻ�ģ�
void RB_INSERT_FIXUP(Root *root,Node *z)
{
    Node *parent = (Node*)malloc(sizeof(Node));
    Node *grandparent;//���常��� �游�ڵ�
    Node *y = (Node*)malloc(sizeof(Node));

    //if(z->parent != NULL)
    //{parent = z->parent;
    //if(z->parent->parent != NULL)
    //grandparent = z->parent->parent;
    //}
    ///��ĸ��p[z],�游��p[p[z]] y��z��uncle 0�Ǻ�ɫ   ��������������Ǻ�ɫ�ż������� ����Ǻ�ɫ����ֹͣ��������

        while( z ->parent != NULL && z->parent->color == RED )
    {
        ///���z�ĸ�����z���游������
        if(z->parent == z->parent->parent->left)
        {
            ///z������
            y = z->parent->parent->right;

            ///case1 z������Ϊ��ɫ
            if(y != NULL && y->color == 0)
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
                    //z = z->parent->parent;RBTREE_Print
                }
            }

        }

       ///z�ĸ������z���游�ڵ���Һ���
        else
        {
            ///z��������z���游������
            y = z->parent->parent->left;

            ///case1 z������Ϊ��ɫ
            if(y != NULL && y->color == 0)
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
    root->node->color = BLACK;
}

///�ҳ���nodeΪ���ڵ����������Сֵ
Node* TreeMinimum(Node *node)
{
    if(node->left != NULL)
        return TreeMinimum(node->left);
    else
        return node;
}

///�ҳ����x�ĺ��
Node* Tree_Successor(Node *x)
{
    Node *y;

    ///���x����������Ϊ�� ������������С�ļ�Ϊx�ĺ��
    if(x->right != NULL)
    {
        return TreeMinimum(x->right);
    }

    ///���x��������Ϊ�գ���x�ĺ��ֻ���Ǵ������Ҹ�ĸ ��һ���յ�ĸ���� ��Ϊ��Ȼxû�������� ��x�ǵ�ǰ�����֧�������� ���ĺ��ֻ����ͳ�������֧�Ľ��
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

///���ݵ��ʺ���Ϥ��������
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


///�����ɾ�����
///zΪ����ڵ� Ҳ�ǵ�ǰ�ڵ�
void RB_DELETE(Root *root,type key,typestring *word)
{
    Node *z;
    Node *y;///���ڽ��� ��ɾ���ڵ�
    Node *x;///���ڽ��� ��ɾ���ڵ�Ķ���

    ///Ҫɾ���Ľ��
    z = RB_SEARCH(root->node,key,word);
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
    }
    ///����z���������ӵ����  z�ĺ����z�϶�����ͬ  ��ʱ��z��ֵ�ú�̵�ֵ���ǵ� ͬʱɾ��z�ĺ��
    if(y != z)
    {
        z->key = y->key;
    }

    ///case1��case2:y�Ǵ�ɾ����� ��ɾ�����Ϊ��ɫ ���õ���������ɾ�����Ϊ��ɫ ����Ҫ����
    ///case3:y�Ǵ�ɾ�����ĺ�� ����˵ɾ���˺�� ���滻y�� ��Ҫ������
    if(y->color == 1)
        RB_DELETE_FIXUP(root,x);
    ///����Ϊʲôreturn y������
   // return y;
}


///�����ɾ������������
void RB_DELETE_FIXUP(Root *root,Node *x)
{

    Node *w;

    ///�滻��Ľ��xΪ��ɫ ����x����root��� ѭ������
    while(x != NULL && x != root->node && x->color == 1)
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


///������� ��������Ҫ��ɾ���ĵ����ҵ���Ӧ�Ľ��
Node* inorder_DeleteNode(Node *node,typestring* deleteword)
{

    //static char *a[10];
   int i;
   //�������
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
///������� �����ҵ�����Ϥ����kth�ĵ���
void inorder_tree(Node *node,int k)
{
    //static int m = 1;
    static char *a[10];

   //�������
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
    ///�ҳ���k�����ֵ
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

///������� �����ҵ�����Ϥ����lth��kth�ĵ���
void inorder_Aggregate(Node *node,int l,int r)
{
    ///���ڱ�Ǳ�Ǳ�����ȡ����ֵ�����
    //static int m = 1;
   // static char *a[10];

    if(m >= r+1)
    {

        return;
    }
   //�������
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

///�����������Ϥ��Ҳ����key ����(��ʵ�����������
void RB_QUERY(Root *root,int k)
{
    if(root)
    {
        return inorder_tree(root->node,k);
    }

}
/*
* ��ӡ"�����"
*
* tree       -- ������Ľڵ�
* key        -- �ڵ�ļ�ֵ
* direction  --  0����ʾ�ýڵ��Ǹ��ڵ�;
*               -1����ʾ�ýڵ������ĸ���������;
*                1����ʾ�ýڵ������ĸ������Һ��ӡ�
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

///��ӡ�������� ����RBTREE_Print����
void Print_RBTREE(Root *root)
{
    if(root != NULL && root->node != NULL)
    {
        RBTREE_Print(root->node,root->node->word,root->node->key,0);
    }
}

///���������
Root* create_RBTree()
{
    ///��̬�����ڴ�
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
    ///strlen()�����ַ������ȵ� ���±ߺ����ó����鳤��
    int i, ilen=sizeof(a)/sizeof(a[0]);


    root = create_RBTree();
    Init_RBTree(root->node);

    printf("== ԭʼ����: ");
    for(i=0; i<ilen; i++)
      printf("%d ", a[i]);
    printf("\n");

    for(i=0; i<ilen; i++)
     {
         RB_INSERT(root, a[i],b[i]);
//#if CHECK_INSERT
         printf("== ��ӽڵ�: %d\n", a[i]);
         printf("== ������ϸ��Ϣ: \n");
         Print_RBTREE(root);
         printf("\n");
//#endif
     }
/*
for(i=0; i<ilen; i++)
     {
         RB_DELETE(root, a[i],b[i]);

         printf("== ɾ���ڵ�: %d\n", a[i]);
        if (root)
         {
             printf("== ������ϸ��Ϣ: \n");
             Print_RBTREE(root);
            printf("\n");
         }
    }*/

   int op;
   int N;
   int j = 0;
   int k;
   int l,r;
   printf("�������������: \n");
   scanf("%d",&N);

   while(j++ < N)
   {
    ///ִ�в���ǰ�ȳ�ʼ������
    ///sum:case5�е���Ϥ���ۺ�
    ///m:�Ӵ�С���������ʱ�����
    sum = 0;
    m = 1;
    printf("������������ͺ͵ڼ�����: \n");
    scanf("%d %d",&op,&k);
    //scanf("%d %d %d",&op,&l,&r);
    switch(op){
    ///Find
    case 1:

        RB_QUERY(root,k);
        printf("== ��Ϥ��Ϊ��%d��ĵ���Ϊ%s\n",k,kthnode->word);
        break;
    ///Insert
    case 2:
        RB_INSERT(root,10,"amazing");
        printf("== ��ӽ�� ������ϸ��Ϣ: \n");
        Print_RBTREE(root);
        break;
    ///Modify
    case 3:
        inorder_DeleteNode(root->node,"click");
        RB_DELETE(root,deleteNode->key,deleteNode->word);
        RB_INSERT(root,10,"click");
        printf("== �޸�������ϸ��Ϣ: \n");
        Print_RBTREE(root);
        break;
    ///Delete
    case 4:
        inorder_DeleteNode(root->node,"amuse");
        RB_DELETE(root,deleteNode->key,deleteNode->word);
        printf("== ɾ��������ϸ��Ϣ: \n");
        Print_RBTREE(root);
        break;
    ///Aggregate
    case 5:
        m = 1;
        inorder_Aggregate(root->node,l,r);
        printf("== �ӵ�%dth����%dth����Ϥ���ܺ�Ϊ%d: \n",l,r,sum);
        break;

    default:
        break;
   }

   }

   system("pause");
    return 0;
}
