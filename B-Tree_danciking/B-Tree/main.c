#include <stdio.h>
#include <stdlib.h>
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


/*���� b���Ľ��ṹ*/
typedef struct B_Tree_Node
{
    int keyNum; //���Ϊm-1
    int *key;//�ؼ�������
    char **word;
    struct B_Tree_Node **child;//����ָ������  ָ����ӵ�ָ����ָ������ĳһ������
    struct B_Tree_Node *parent;//ָ�򸸽���ָ��

}Node;

///B���ṹ
typedef struct B_Tree
{
    int max;
    int min;
    int index;//�������� = ��max+1��/2

    Node *root;  //�����

}Tree;


///����m��b�� �ؼ��ֵĸ��� ���Ϊm-1 ����Ϊceil��m/2�� ����ȡm/2�����ޣ�  �Ҳ�����������
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
    ///����йؼ��ֵĸ���min >= ��(m/2)������-1��  ���� m=3 ��ؼ�������Ϊ1 ���Ϊ2  m=4 ��ؼ�������Ϊ2 ���Ϊ3
    bt->min --;

    ///��Ƿָ��
    bt->index = m / 2;
    bt->root = NULL;

    *btree = bt;
    return 0;
}

///�������
Node* create_node(Tree *btree)
{
    Node *node = NULL;
    node = (Node*)malloc(sizeof(Node));

    if(node == NULL)
    {
        return NULL;
    }

    ///��ʼ��������ǵ�ǰnode�йؼ��ָ�����keyNum �����
    node->keyNum = 0;
    ///calloc����ͨ������Ϊ���������ڴ�ռ� ����key������
    node->key = (int*)calloc(btree->max + 1,sizeof(int));
    node->word = (char*)calloc(btree->max + 1,sizeof(char));

    if(node->word == NULL || node->key == NULL)
    {
        free(node);
        node = NULL;
        return NULL;
    }

    ///��max+1����Ϊ���ƶ����������� ���������Ϊmax+1=m
    node->child = (Node**)calloc(btree->max + 2,sizeof(Node));
    if(node->child == NULL)
    {
        free(node->key);
        free(node);
        node == NULL;
        return NULL;
    }

    return node;

}
/*
  1) ���node��sidx�ؼ���Ϊ�ָ�㣬����(0 ~ sidx-1)�ؼ��ּ������ڽ��node�У�����(sidx+1 ~ num-1)�ؼ��ַ����½��node2��
  2) ������sidx�ؼ��������node->parent�У��ٽ��½��node2��Ϊ������²���ؼ��ֵ��Һ��ӽ��
  3) �жϲ���node��sidx�ؼ��ֺ�node->parent�Ĺؼ��ָ���num�Ƿ񳬹�max���������������parentΪ�����������1)�Ĵ������򣬴������
*/
/*int _btree_insert(B_Tree *btree,Node *node,int i,int k,char* word)
{
    ///����Ԥ���� ���Խ��в�����
    int i = 0;

    for(int i=node->keyNum; i > 0 ; i--)
    {
        if(k < node->key[i-1])
        {
            ///�ؼ��ֻ���
            node->key[i] = node->key[i-1];
            continue;
        }
            break;
    }
    node->key[i-1] = k;
    node->keyNum++;
    return 0;
}*/

///�����Ѵ���
Node* B_TREE_SPLIT(Tree *btree,Node* node)
{
    ///���ڽ��ܷ��Ѻ�����ĸ����
    Node* parentNode = NULL;
    ///���ڽ��ܷ��Ѻ�Ľ��
    Node* tempNode = NULL;

    while(node->keyNum >= btree->max)
    {
        ///�Է��ѽ���һϵ�и�ֵ
        tempNode->key = node->key + (btree->index);
        tempNode->child = node->child + (btree->index);
        tempNode->keyNum = btree->max - (btree->index);
        tempNode->parent = node->parent;

        ///���������Ľ�����ԭ�����
        parentNode = node->parent;
        ///��ԭ�����Ϊ�� ����ǰ�ڵ�Ϊ�����
        if(parentNode == NULL)
        {
            parentNode == create_node(btree);
            if(parentNode == NULL)
            {
                return -1;
            }
            ///���ڵ����������� һ���ؼ���
            parentNode->child[0] = node;
            parentNode->key[0] = node->key[btree->index - 1];
            parentNode->keyNum = 1;
            parentNode->child[0] = tempNode;

            ///�����ѵ������ڵ�ĸ������Ϊ���ڵ�
            node->parent = parentNode;
            tempNode->parent = parentNode;
        }
        else
        {
            ///����ǿո���� ����������бȽ� ����Ƚ���������
            int i;
            for(i=parentNode->keyNum; i > 0 ; i++)
            {
                if(node->key[btree->index - 1] < parentNode->key[i-1])
                {
                    ///�ؼ��ֺͺ��Ӷ�����
                    parentNode->key[i] = parentNode->key[i-1];
                    parentNode->child[i+1] = parentNode->child[i];
                    continue;
                }
                break;
            }
            ///��ʱ������ȥ��key��ָ��child��ָ�����
            parentNode->key[i-1] = node->key[btree->index - 1];
            parentNode->child[i] = tempNode;
            tempNode->parent = parentNode;
            parentNode->keyNum ++;
        }

        ///��ԭ�н��Ķ��Ӻ͹ؼ��ֽ��в������
        node->keyNum = btree->index;
        int i;
        for(i = btree->index - 1 ;i< btree->max ; i++)
        {
            ///Ӧ�ø�ֵ���٣�����
            node->key[i] = NULL;
        }
        for(i = btree->index ;i< btree->max + 1 ; i++)
        {
            ///Ӧ�ø�ֵ���٣�����
            node->child[i] = NULL;
        }
    }
    ///�����Ҫ�ı�ԭ���ĺ��ӵĸ��׽�� ��Ϊԭ��������һ������� ���ڷֿ�

   /*for(int i = 0 ;i< btree->index ; i++)
    {
        if(node->child[i] != NULL)
        node->child[i]->parent = node;
    }*/
    int i;
   for(i = 0 ;i < tempNode->keyNum ; i++)
    {
        if(tempNode->child[i] != NULL)
        tempNode->child[i]->parent = tempNode;
    }
    ///���˷��ѽ��� ������������ ��������Ƿ�Ϊ��  ���ص�ǰ�������ɵĸ����
    return parentNode;
}

int B_TREE_INSERT(Tree *btree,int k,char* word)
{
    int i = 0;
    Node* node = btree->root;
    Node* tempNode = create_node(btree);
    ///������һ��node
    if(node == NULL)
    {
        ///���ݽṹ��tree���max min�������
        node = create_node(btree);
        if(node == NULL)
        {
            return -1;
        }
        node->keyNum = 1;
        node->key[0] = k;
        node->word[0] = word;

        ///�Ѵ����ĵ�һ��������¸��������
        btree->root = node;
        return 0;
    }

    ///���Ҳ����� ͬʱ�����ж� ��ǰ����ǲ������� �����������Ҫ���м�Ĺؼ��ֶ���ȥ
    while(node != NULL)
    {

        if(node->keyNum == btree->max)
        {
            ///����Բ��ԣ�����������Ԥ����
            node = B_TREE_SPLIT(btree,node);
            break;
        }

            for(i = 0;i < node->keyNum;i--)
            {
               if(k == node->key && word == node->word)
               {
                printf("����Ѵ���\n");
                return 0;
               }
                else if(k < node->key[i])
               {
                break;
               }
            }

        ///����ϱ߶���ִ�еĻ� i��ʱΪnode->keyNum
        if(node->child[i] != NULL)
        {
            node == node->child[i];
        }
        ///ֻ��Ҷ���ĺ��ӽ��ΪNULL
        else
            break;
    }

    ///�ҵ�����λ�ú� ���в������
     for(i=node->keyNum; i > 0 ; i--)
    {
        if(k < node->key[i-1])
        {
            ///�ؼ��ֻ���
            node->key[i] = node->key[i-1];
            continue;
        }
            break;
    }
    node->key[i-1] = k;
    node->keyNum++;
    return 0;
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
/*void BTREE_Print(Tree *tree,char* word,int key,int direction)
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
void Print_BTREE(Tree *tree)
{
    if(tree != NULL && tree->root != NULL)
    {
        BTREE_Print(root->root);
    }
}
*/
int main()
{
    char array[] = {
        'G','G', 'M', 'P', 'X', 'A', 'C', 'D', 'E', 'J', 'K',
        'N', 'O', 'R', 'S', 'T', 'U', 'V', 'Y', 'Z', 'F', 'X'
    };
    const int length = sizeof(array)/sizeof(char);
    Tree *btree;
    printf("====����B��====\n");
    create_BTree(&btree,3);
    printf("====��B��������====\n");
    B_TREE_INSERT(btree,10,"w");

    printf("");

}
