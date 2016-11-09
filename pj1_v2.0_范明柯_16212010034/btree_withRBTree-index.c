#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*m��b��
����ÿ�������ຬ��m�����ӣ�m>=2����
��������Ҷ�ӽ���⣬����ÿ�����������[ceil(m / 2)]�����ӣ�����ceil(x)��һ��ȡ���޵ĺ�������
�����������2�����ӣ�����B��ֻ����һ����㣺����㣩��
����Ҷ�ӽ�㶼������ͬһ�㣬Ҷ�ӽ�㲻�����κιؼ�����Ϣ(���Կ������ⲿ�����ѯʧ�ܵĽ�㣬ָ����Щ����ָ�붼Ϊnull)����ע��Ҷ�ӽڵ�ֻ��û�к��Ӻ�ָ���ӵ�ָ�룬��Щ�ڵ�Ҳ���ڣ�Ҳ��Ԫ�ء����ƺ�����У�ÿһ��NULLָ�뼴����Ҷ�ӽ�㣬ֻ��û���������ѣ���
ÿ�����ն˽���а�����n���ؼ�����Ϣ�� (n��P0��K1��P1��K2��P2��......��Kn��Pn)�����У�
a) Ki (i=1...n)Ϊ�ؼ��֣��ҹؼ��ְ�˳����������K(i-1)< Ki��
b) PiΪָ���������Ľ�㣬��ָ��P(i-1)ָ�����������н��Ĺؼ��־�С��Ki����������K(i-1)��
c) �ؼ��ֵĸ���n�������㣺 [ceil(m / 2)-1]<= n <= m-1��������j�����ӵķ�Ҷ���ǡ����j-1���ؼ��롣
*/
#define RED 0
#define BLACK 1

typedef int type;
typedef char typestring;
typedef double LL;

int m;///��¼�������
LL sum;
int boolval = 0;///����Ƿ��ҵ�Ҫ�ҵ��ĵ���

/*���� b���Ľ��ṹ*/
typedef struct B_Tree_Node
{
    int keyNum; //���Ϊm-1
    type *key;//�ؼ�������
    char **word;
    struct B_Tree_Node **child;//����ָ������  ָ����ӵ�ָ����ָ������ĳһ������
    struct B_Tree_Node *parent;//ָ�򸸽���ָ��

}BNode;

///B���ṹ
typedef struct B_Tree
{
    int max;
    int min;
    int index;//�������� = ��max+1��/2

    BNode *root;  //�����

}Tree;

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
}rbNode,*RBTree;//Node��struct RBTree_Node�ı��� ������Ϊ*parent *left *right�����ڽṹ����� ���Բ����ñ�������

///���������ĸ�
typedef struct RB_RootNode
{
    rbNode *node;
}RBRoot;

///ȫ�ָ����
RBRoot *rbroot = NULL;
rbNode *deleteNode;

///����x�ڵ����������  ��ʵ����ÿ�����ĺ��ӣ����װ� ����������Ҫ�漰x���Һ��ӣ���y�� x�ĸ��� y������ y�ĸ��� �Լ�y�����ӵĸ���
void LEFT_ROTATE(RBRoot *root,rbNode *xnode)
{
    rbNode *y;

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
void RIGHT_ROTATE(RBRoot *root,rbNode *xnode)
{
    rbNode *y;

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
void RB_INSERT(RBRoot *root,type key,typestring *word)//nodeΪ����ڵ�
{
    ///����һ���µĽڵ� ��ֵΪ�����key
    rbNode *z = (rbNode*)malloc(sizeof(rbNode));
    z->key = key;
    z->word = word;


    rbNode *y = NULL;//���²����㿴���ս��
    rbNode *x=root->node;//��x�������ڵ�

   // if(x == NULL)
    //{
    //  root->node = z;
    //}
    ///�Ӹ��ڵ������ҳ�����ڵ�Ӧ�ò����λ��
    while(x != NULL)
    {
        y = x;
        ///���ֵ������
        if(strcmp(z->word,x->word)<0)
            x= x->left;
        else
            x= x->right;


    }
    z->parent = y;//���ｫx����node�ĸ�ĸ�Ƿ����   ������ x�Ѿ�ΪNULL ����y����x ����y��Ϊnode�ĸ����

    if(y == NULL)
    {
        root->node = z;
        //z->parent = NULL;
    }
    else
    {
        ///���ֵ������
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

///����ڵ��  �Ժ�����ĵ������� ���е�ǰ�ڵ�Ϊ�ղ���ڵ�z  ��ǰ�ڵ�϶��Ǻ�ģ�
void RB_INSERT_FIXUP(RBRoot *root,rbNode *z)
{
    rbNode *parent = (rbNode*)malloc(sizeof(rbNode));
    rbNode *grandparent;//���常��� �游�ڵ�
    rbNode *y = (rbNode*)malloc(sizeof(rbNode));

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
                //else

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

    ///���ڵ�����Ϊ��ɫ  ��whileѭ�����ִ�б���
    root->node->color = BLACK;
}

///�ҳ���nodeΪ���ڵ����������Сֵ
rbNode* TreeMinimum(rbNode *node)
{
    if(node->left != NULL)
        return TreeMinimum(node->left);
    else
        return node;
}

///�ҳ����x�ĺ��
rbNode* RBTree_Successor(rbNode *x)
{
    rbNode *y;

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


///�����ɾ�����
///zΪ����ڵ� Ҳ�ǵ�ǰ�ڵ�
void RB_DELETE(RBRoot *root,typestring *word)
{
    rbNode *z;
    rbNode *y;///���ڽ��� ��ɾ���ڵ�
    rbNode *x;///���ڽ��� ��ɾ���ڵ�Ķ���

    ///Ҫɾ���Ľ��
    z = RB_SEARCH(root->node,word);
    ///�����ɾ�����ֻ��һ�����ӻ���û�ж���
    if(z->left == NULL || z->right == NULL)
    {
        ///���丳��y ��y���в��� ��yĿǰ�Ǵ�ɾ���ڵ�
        y = z;
    }
    else
    {
        ///��ɾ��������������� �� �ô�ɾ�����ĺ���滻��ɾ����� Ȼ��ɾȥ��̼���
        y = RBTree_Successor(z);
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
        if(y->right != NULL)
            x = y->right;
        else
            x = NULL;
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
        z->word = y->word;
    }

    ///case1��case2:y�Ǵ�ɾ����� ��ɾ�����Ϊ��ɫ ���õ���������ɾ�����Ϊ��ɫ ����Ҫ����
    ///case3:y�Ǵ�ɾ�����ĺ�� ����˵ɾ���˺�� ���滻y�� ��Ҫ������
    if(y->color == 1)
        RB_DELETE_FIXUP(root,x,y->parent);
    ///����Ϊʲôreturn y������
   // return y;
}


///�����ɾ������������
void RB_DELETE_FIXUP(RBRoot *root,rbNode *x,rbNode *parent)
{

    rbNode *w;

    ///�滻��Ľ��xΪ��ɫ ����x����root��� ѭ������
    while(x== NULL ||(x != root->node && x->color == 1))
    {
        if(x == NULL && x == root->node)
            break;
        ///xΪ�����
        if(x == parent->left)
        {
            ///��w����x���ֵ�
            w = parent->right;
            ///case1�����x���ֵ��Ǻ�ɫ
            if(w != NULL && w->color == 0)
            {
                ///case1�� ��w����ɫ��Ϊ��ɫ w��x�ĸ������ɫ��Ϊ��ɫ Ȼ�����������������x�ĸ����Ϊ�� Ȼ���
                w->color = 1;
                parent->color = 0;
                LEFT_ROTATE(root,parent);

                ///�ٰ�x���ֵܸ���w ��ʵ����w��ԭw������� �϶��Ǻڵ� ��Ϊ��������������
                w = parent->right;
            }


            ///case2: ������w�Ǻڵģ� ͬʱw������������ɫ��Ϊ��
            //if(w != NULL && w->left!=NULL && w->right!=NULL)
            //{
                if((w->left == NULL || w->left->color == 1)&& (w->right == NULL || w->right->color == 1))
            {
                ///case2 ��w����ɫ���ɺ�ɫ ��ǰ�ڵ�x�Ƶ�x�ĸ����
                w->color = 0;
                x = parent;
                parent = x->parent;
            }
            //}

            ///case3��w���Һ����Ǻ�ɫ��
            else
            {


                if(w->right == NULL ||w->right->color == 1)
                {
                    ///��w������Ҳ��Ϊ��ɫ w�������ɫ��Ϊ��ɫ ������wΪ�����������
                    w->left->color = 1;
                    w->color = 0;
                    RIGHT_ROTATE(root,w);
                    ///��w�ٴ���Ϊx�����ֵ�
                    w = parent->right;
                }

                ///case4 w���Һ����Ǻ�ɫ��
                ///��w����ɫ��Ϊ��x�ĸ�������ɫ��ͬ���ٰѸ��ڵ����ɫ��Ϊ��ɫ w���Һ���Ҳ��Ϊ��ɫ ������������
                w->color = parent->color;
                parent->color = 1;
                if(w->right != NULL)
                w->right->color = 1;
                LEFT_ROTATE(root,parent);
                ///��x��Ϊroot���
                x = root->node;
            }
        }
        ///xΪ�Һ���
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

///���������
RBRoot* create_RBTree()
{
    ///��̬�����ڴ�
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

///����m��b�� �ؼ��ֵĸ��� ���Ϊm-1 ����Ϊceil��m/2��-1 ����ȡm/2�����ޣ�  �Ҳ�����������
int create_BTree(Tree **btree,int m)
{
    Tree *bt = NULL;

    if(m < 3)
    {
       printf("b���Ľ����������2./n");
       return -1;
    }
    bt = (Tree*)malloc(sizeof(Tree));
    if(bt == NULL)
    {
       return -1;
    }

    //bt->max = m-1;
    //bt->min = m/2;
    ///���½����ĳ�ʼ�� ������֧��������
    bt->max = m - 1;
    bt->min = m/2;
    ///mΪ����
    if(m % 2 != 0)
    {
        bt->min ++;
    }
    ///����йؼ��ֵĸ���min >= ��(m/2)������-1��  ���� m=3 ��ؼ�������Ϊ1 ���Ϊ2  m=4 ��ؼ�������Ϊ1 ���Ϊ3
    bt->min --;

    ///��Ƿָ��
    bt->index = m / 2;
    bt->root = NULL;

    *btree = bt;
    return 0;
}

///�������
BNode* create_node(Tree *btree)
{
    int i;
    BNode *node = NULL;
    node = (BNode*)malloc(sizeof(BNode));

    if(node == NULL)
    {
        return NULL;
    } //tempNode->child = node->child + (btree->index);

    ///��ʼ��������ǵ�ǰnode�йؼ��ָ�����keyNum �����
    node->keyNum = 0;
    ///calloc����ͨ������Ϊ���������ڴ�ռ� ����key������
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

    ///��max+1����Ϊ���ƶ����������� ���������Ϊmax+1=m
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

///�����Ѵ��� ��node���ѳ�tempnode  pos�ǲ���λ��
void B_TREE_SPLIT(Tree *btree,BNode* parentNode,BNode* node,int pos)
{
    BNode* tempNode;
    int i;
    ///Ϊ�·��ѳ��Ľ�������ڴ�
    tempNode = create_node(btree);
        ///�Է��ѽ���һϵ�и�ֵ
        for (i = 0; i<btree->index - 1; i++)
			{
			    tempNode->key[i] = node->key[i + btree->index];
			    tempNode->word[i] = node->word[i + btree->index];
			}
        //tempNode->key = node->key + (btree->index);
        tempNode->keyNum = btree->max - (btree->index);
        ///�����ǰ��㲻��Ҷ��� ���ƺ���ָ��
        if(node->child[0] != NULL)
            //tempNode->child = node->child + (btree->index);
           {
             for (i = 0; i<btree->index; i++)
			tempNode->child[i] = node->child[i + btree->index];
           }
        node->keyNum = btree->index - 1;

        //���м�����Ϊ�������뵽˫�׽ڵ���
        //��������Ĺؼ��ֺ�ָ�붼�����ƶ�һ��λ��
         for(i=parentNode->keyNum; i > pos ; i--)
            {
                    ///�ؼ��ֺͺ��Ӷ�����
                    parentNode->key[i] = parentNode->key[i-1];
                    parentNode->word[i] = parentNode->word[i-1];
                    parentNode->child[i+1] = parentNode->child[i];

            }
            ///��ʱ������ȥ��key��ָ��child��ָ�����
            parentNode->key[pos] = node->key[btree->index - 1];
            parentNode->word[pos] = node->word[btree->index - 1];
            parentNode->child[pos+1] = tempNode;
            tempNode->parent = parentNode;
            parentNode->keyNum ++;

            ///��ԭ�н��Ķ��Ӻ͹ؼ��ֽ��в������
       // node->keyNum = btree->index - 1;

        for(i = btree->index - 1 ;i< btree->max ; i++)
        {
            ///Ӧ�ø�ֵ���٣�����
            node->key[i] = NULL;
            node->word[i] = NULL;
        }
        for(i = btree->index ;i< btree->max + 1 ; i++)
        {
            ///Ӧ�ø�ֵ���٣�����
            node->child[i] = NULL;
        }

        for(i = 0 ;i < tempNode->keyNum ; i++)
        {
           if(tempNode->child[i] != NULL)
              tempNode->child[i]->parent = tempNode;
        }
        for(i = btree->index - 1 ;i< btree->max ; i++)
        {
            ///Ӧ�ø�ֵ���٣�����
            node->key[i] = NULL;
            node->word[i] = NULL;
        }
}

void btree_InsertNoneFull(Tree *btree,BNode *node,type key,char *word)
{
    int i;
    int j=0;///���ҵ���Ϥ��key�� ����j����ѭ���õ� ���ݵ�����Ĳ���λ��
    BNode* child;


    i = node->keyNum;

    ///�����Ҷ��� ֱ�Ӳ���
    if(node->child[0] == NULL)
    {
        for(i=node->keyNum; i > 0 ; i--)
      {
        if(key < node->key[i-1])
        {
            ///�ؼ��ֻ���
            node->key[i] = node->key[i-1];
            node->word[i] = node->word[i-1];
            continue;
        }
        else
            if(key == node->key[i-1])
            {
                if(strcmpi(word,node->word[i-1]) <0)
                {
                    ///�ؼ��ֻ���
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
        ///�˴�i�Ƕ��� �����
      node->key[i] = key;
      node->word[i] = word;
      node->keyNum++;
      }
    }
    else
   {
            ///�ҳ�Ӧ�ò���ĺ��ӽڵ�
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

        ///�� key���˶�������ֵ�� �� ������Ӧ�������Һ��� ��i++
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
    ///����������ӽڵ�
    child = node->child[i];
    btree_InsertNoneFull(btree,child,key,word);
    }
}

BNode* btree_insert(Tree *btree,BNode* node,type key,char* word)
{
    BNode* newNode;
    /* ����Ƿ���ڵ�������������������Ѳ������µĸ��ڵ� */
    if(node->keyNum == btree->max)
        {
            ///����Բ��ԣ�����������Ԥ���� newnode�����Ѻ�����ĸ���� �˴�Ϊ�µĸ��ڵ�
            newNode =create_node(btree);
            newNode->keyNum = 0;
            newNode->child[0] = node;
            B_TREE_SPLIT(btree,newNode,node,0);

            btree_InsertNoneFull(btree,newNode,key,word);
            return newNode;
            //node = btree->root;
            //continue;
        }
        else///δ��
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
    // �� tree �еĽڵ� node ���кϲ����ӽڵ㴦��.
	// ע�⣺���ӽڵ�� keynum ������Ѵﵽ���ޣ��������� BTree_D - 1
    // �� tree ������Ϊ index �� key ���������ӽ���У�
	// �� node ������Ϊ index + 1 �ĺ��ӽڵ�ϲ�������Ϊ index �ĺ��ӽڵ��У��Һ��Ӻϲ������ӽ���С�
	// ������ص� key ��ָ�롣
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

	///ɾ����Ҫ��������� һ����Ҫɾ��key���ڵ�ǰ�ڵ� һ����Ҫɾ��key�ڵ�ǰ���

	void btree_delete_node(Tree *btree,BNode* node,type key,char *word)
	{
	    int i = 0;
	    int j;
	    type tempkey;
	    char *tempword;

	    ///�ҳ�ɾ��λ�� ����ǰ�ڵ������ ���� keyС�ڵ��ڵ�ǰ�ڵ�ĳ��keyʱ ����ѭ��
	    while(i < node->keyNum && key > node->key[i] || (key == node->key[i] && strcmpi(word,node->word[i]) > 0))
        {
            i++;
        }





	/*======================����key�ĵ�ǰ���ʱ�����========node->key[i]============
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
         ///�ڸò� �� �����ҵ���ֵ
         if(i < node->keyNum && key == node->key[i] && strcmpi(word,node->word[i]) == 0)
         {


             /// case1:�����Ҷ�ӽ�� ֱ��ɾ��
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
                  /*ɾ����ֵ�ڵ�ǰ�ڵ� ������ǰ�����̣�����Ҷ�ڵ㣩 ������ɾ��Ҷ�ڵ�
	              * ��ǰ�ڸ�λ�õĹؼ��ֵ����ӽ��ؼ��ָ������ڵ���Tʱ��
	              * Ѱ�Ҹ�λ�õĹؼ���ǰ�������ӽ������ؼ��֣�
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
                     ///������  �ҵ���Ӧ��i ִ��ɾ������
                     i++;
                 }
                 ///�������Ӷ����� ֻ�ܺ�����
                 else
                 {
                     i = BTree_merge_child(btree,node,i);
                 }
                 ///������  �ҵ���Ӧ��i ִ��ɾ������ �ݹ���� һ��һ������
                 btree_delete_node(btree,node->child[i],key,word);
             }
         }

         ///��ǰ�ڵ��Ҳ���key ������һ������ҵ�key ��Ҫɾ����ֵ���ڵ�ǰ���
         else
         {
             //if(i == node->keyNum)
      // {
          //  i--;
        //}
             if(node->child[0] == NULL)
             {
                 printf("***������Ҫɾ����ֵ\n");
             }
             else
             {
                 ///���Ҫɾ����ֵ�ڵ�ǰ���ĺ��ӷ�֧�� ִ�еݹ�ɾ�� ////////////////���ж��ǵ��ڻ��Ǵ���
                 if(node->child[i]->keyNum > btree->min)
                    btree_delete_node(btree,node->child[i],key,word);
                ///���������ӷ�֧̫�� �Ǿ�Ҫ�������ֵ� �ֵܿ��Խ�ͽ�
                 else
                 {
                      if(i>0 && node->child[i-1]->keyNum > btree->min)
                      {
                        ///���оֲ����� ���ֵܸ�ԣ
                        right_rotate(node,node->child[i],node->child[i-1],i);
                      }/////////////////////////////////////////////////////////////////////////////////////////////////
                       else if(i < node->keyNum  && node->child[i+1]->keyNum >btree->min)
                      {
                        ///���оֲ����� ���ֵܸ�ԣ
                        left_rotate(node,node->child[i],node->child[i+1],i);
                      }
                       else
                         i = BTree_merge_child(btree,node,i);
                       ///��Ҫ�ϲ�������� ����ִ��ɾ��
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

///���ݵ��� ����B�� �ҵ���Ӧ�Ľ��
int inorder_Delete(BNode *node,char* word)
{
    int i;
    int a = -1;

    ///��û���ҵ���Ӧ����ʱ ���в���

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
               //printf("== Ҫɾ���ĵ���Ϊ%s(%d)\n",node->word[i-1],node->key[i-1]);
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
               //printf("== ��Ϥ��Ϊ��%d��ĵ���Ϊ%s(%d)\n",num,node->word[i-1],node->key[i-1]);
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
               //printf("== ��Ϥ��Ϊ��%d��ĵ���Ϊ%s\n",num,node->word[i-1]);
               //m++;
               sum += node->key[i-1];
           }
        }
         if(node->child[0] != NULL)
        Aggregate_BTree(node->child[0],l,r);
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
void BTREE_Print(BNode *node,int layer,int ith)
{
    int i;
    if(node != NULL)
    {
        printf("===��%d��====��%d�����:",layer,ith);

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
        printf("==��Ϊ��\n");
}


int main()
{


    int i = 0;
    //int ilen = sizeof(a)/sizeof(a[0]);



    rbroot = create_RBTree();
    Init_RBTree(rbroot->node);




    Tree *btree;
    //Tree *btree;
    //printf("====����B��====\n");
    create_BTree(&btree,4);
    //printf("====��B��������====\n");
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

   //printf("�������������: \n");
   fscanf(fp,"%d",&N);

   while(j++ < N)
   {

   //printf("�������������: \n");
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
    ///ִ�в���ǰ�ȳ�ʼ������
    ///sum:case5�е���Ϥ���ۺ�
    ///m:�Ӵ�С���������ʱ�����
    sum = 0;
    m = 1;
    //printf("1.��ѯ\n2.����\n3.�޸�\n4.ɾ��\n5.����\n������������ͼ���ز���: \n");

    //scanf("%d",&op);

    switch(op){
    ///Find
    case 1:
        //scanf("%d",&k);
        fscanf(fp,"%d",&k);
        inorder_BTree(btree->root,k);
        //printf("== ��Ϥ��Ϊ��%d��ĵ���Ϊ%s\n",k,word);
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
           // printf("�õ����Ѵ���\n");
        //}
        //printf("%s %d",deleteWords,deletekey);
        //else
        btree->root = btree_insert(btree,btree->root,key,word);
        RB_INSERT(rbroot,key,word);
        //printf("== ��ӽ�� ������ϸ��Ϣ: \n");
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
        ///���������

        RB_DELETE(rbroot,deleteNode->word);
        RB_INSERT(rbroot,key,word);

        //btree->root = btree_insert(btree,btree->root,17,"B");
        //printf("== �޸�������ϸ��Ϣ: \n");
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
        //printf("== ɾ��������ϸ��Ϣ: \n");
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

        //printf("== �ӵ�%dth����%dth����Ϥ���ܺ�Ϊ%d: \n",l,r,sum);
        printf("%.0f\n",sum);
        break;

    default:
        break;
   }

   }

   system("pause");

}
