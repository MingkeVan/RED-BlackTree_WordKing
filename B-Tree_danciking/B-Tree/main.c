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
Node* create_node(Tree *btree)
{
    int i;
    Node *node = NULL;
    node = (Node*)malloc(sizeof(Node));

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
    node->child = (Node**)calloc(btree->max + 2,sizeof(Node));
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
void B_TREE_SPLIT(Tree *btree,Node* parentNode,Node* node,int pos)
{
    Node* tempNode;
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

void btree_InsertNoneFull(Tree *btree,Node *node,int key,char *word)
{
    int i;
    Node* child;

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
            break;
      }
      ///�˴�i�Ƕ��� �����
      node->key[i] = key;
      node->word[i] = word;
      node->keyNum++;
    }
    else{
            ///�ҳ�Ӧ�ò���ĺ��ӽڵ�
         for(i=node->keyNum; i > 0 ; i--)
      {
        if(key < node->key[i-1])
        {
            continue;
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
    }
    ///����������ӽڵ�
    child = node->child[i];
    btree_InsertNoneFull(btree,child,key,word);
    }
}

Node* btree_insert(Tree *btree,Node* node,int key,char* word)
{
    Node* newNode;
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

Node* btree_max(Node* node)
{
    Node* newNode = NULL;
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

Node* btree_min(Node* node)
{
    Node* newNode = NULL;
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

    //remove
    // �� tree �еĽڵ� node ���кϲ����ӽڵ㴦��.
	// ע�⣺���ӽڵ�� keynum ������Ѵﵽ���ޣ��������� BTree_D - 1
    // �� tree ������Ϊ index �� key ���������ӽ���У�
	// �� node ������Ϊ index + 1 �ĺ��ӽڵ�ϲ�������Ϊ index �ĺ��ӽڵ��У��Һ��Ӻϲ������ӽ���С�
	// ������ص� key ��ָ�롣
	void BTree_merge_child(Tree* btree, Node* node,Node* parent, int key)
	{
	    int i;
	    ///��ǰ�ڵ��Ǹ���� ���غϲ�
	    if(parent == NULL)
        {
            if(node->keyNum == 0)
            {
                if(node->child[0] != NULL)
                 {
                    btree->root = node->child[0];
                    node->child[0]->parent = NULL;
                 }

                else
                {
                 btree->root = NULL;
                }
                free(node);
            }
        }

        for(i = 0;i <= parent->keyNum;i++)
        {
            if(node == parent->child[i])
            {
                break;
            }
        }
        ///��ɾ��key�����һ�����ӽ����
        if(i == parent->keyNum)
        {
            int m = parent->child[i-1]->keyNum;
            ///����ֵܲ����� �ϲ�
            if(m <= btree->min)
            {
            parent->child[i-1]->key[m] = parent->key[i-1];
            for(i = 1;i<node->keyNum;i++)
            parent->child[i-1]->key[m + i] = node->key[i];

            parent->key[i-1] = NULL;
            parent->word[i-1] = NULL;
            parent->child[i] = NULL;
            free(node);
            }
            else
            {///����ֵܹ��� �Ǿͽ�һ�� ���оֲ�����
                node->key[node->keyNum] = parent->key[parent->keyNum - 1];
                node->keyNum ++;
                parent->key[parent->keyNum - 1] = parent->child[i - 1]->key[m-1];
                parent->child[i - 1]->key[m-1] = NULL;
                parent->child[i - 1]->keyNum --;
                ///ת��case1 ɾ��
                btree_delete(btree,node,key);
            }
        }
        else///������м�ĺ��ӽ����
        {
            if()
        }
	}

	///ɾ����Ҫ��������� һ����Ҫɾ��key���ڵ�ǰ�ڵ� һ����Ҫɾ��key�ڵ�ǰ���

	void btree_delete(Tree *btree,Node* node,int key)
	{
	    int i = 0;
	    int temp;

	    while(i < node->keyNum)
        {
            if(key <= node->key[i])
            {
             break;
            }
            i++;
        }



	/*======================����key�ĵ�ǰ���ʱ�����====================
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
         if(i < node->keyNum && key == node->key[i])
         {
             /// case1:�����Ҷ�ӽ�� ֱ��ɾ��
             if(node->child[0] == NULL)
             {

                     for(j = i;j < node->keyNum - 1;j++)
                     {
                         node->key[j]=node->key[j+1];
                     }
                 node->key[node->keyNum - 1] = 0;
                 node->keyNum --;

                 if(node->keyNum < btree->min)
                 {
                     BTree_merge_child(btree,node,node->parent);
                 }
                // for(j = )
             }
             else
             {
                 /*ɾ����ֵ�ڵ�ǰ�ڵ� ������ǰ�����̣�����Ҷ�ڵ㣩 ������ɾ��Ҷ�ڵ�
	              * ��ǰ�ڸ�λ�õĹؼ��ֵ����ӽ��ؼ��ָ������ڵ���Tʱ��
	              * Ѱ�Ҹ�λ�õĹؼ���ǰ�������ӽ������ؼ��֣�
	              */
                 if(node->child[i]->keyNum >= btree->min)
                 {
                     Node* newNode;
                     newNode = btree_max(node->child[i]);
                     temp = node->key[i];
                     node->key[i] = newNode->key[newNode->keyNum - 1];
                     newNode->key[newNode->keyNum - 1] = temp;
                 }
                 else if(node->child[i+1]->keyNum >= btree->min)
                 {
                     Node* newNode;
                     newNode = btree_min(node->child[i+1]);
                     temp = node->key[i];
                     node->key[i] = newNode->key[0];
                     newNode->key[0] = temp;
                     ///������  �ҵ���Ӧ��i ִ��ɾ������
                     i++;
                 }
                 ///�������Ӷ����� ֻ�ܺ�����
                 else
                 {
                     i = BTree_merge_child(node,i);
                 }
                 ///������  �ҵ���Ӧ��i ִ��ɾ������ �ݹ���� һ��һ������
                 btree_delete(btree,node->child[i],key);
             }
         }
         ///��ǰ�ڵ��Ҳ���key ������һ������ҵ�key ��Ҫɾ����ֵ���ڵ�ǰ���
         else
         {
             if(node->child[0] == NULL)
             {
                 printf("***������Ҫɾ����ֵ\n");
             }
             else
             {
                 ///���Ҫɾ����ֵ�ڵ�ǰ���ĺ��ӷ�֧�� ִ�еݹ�ɾ��
                 if(node->child[i]->keyNum >= btree->min)
                    btree_delete(btree,node->child[i],key);
                ///���������ӷ�֧̫�� �Ǿ�Ҫ�������ֵ� �ֵܿ��Խ�ͽ�
                 else if(i>0 && node->child[i-1]->keyNum >btree->min)
                 {
                     ///���оֲ����� ���ֵܸ�ԣ
                     for(j = node->child[i]->keyNum - 1;j>=0;j--)
                     node->child[i]->key[j+1] = node->child[i]->key[j];

                     node->child[i]->key[0] = node->key[i];
                     node->key[i] = node->child[i-1]->key[node->child[i-1]->keyNum - 1];

                     if(node->child[i]->child[0] != NULL)
                     {
                         for(j = node->child[i]->keyNum;j>=0;j--)
                          node->child[i]->child[j+1] = node->child[i]->child[j];

                          node->child[i]->child[0] = node->child[i-1]->child[node->child[i-1]->keyNum];
                     }

                     node->child[i]->keyNum ++;
                     node->child[i-1]->keyNum --;
                 }
                 else if(i>0 && node->child[i+1]->keyNum >btree->min)
                 {
                     ///���оֲ����� ���ֵܸ�ԣ
                     node->child[i]->key[node->child[i]->keyNum] = node->key[i];
                     node->key[i] = node->child[i+1]->key[0];

                     for(j = 0;j<node->child[i+1]->keyNum - 1;j++)
                     node->child[i+1]->key[j] = node->child[i+1]->key[j+1];

                     node->child[i+1]->key[node->child[i+1]->keyNum - 1] = NULL;


                     if(node->child[i]->child[0] != NULL)
                     {
                         node->child[i]->child[node->child[i]->keyNum +1] = node->child[i-1]->child[0];

                         for(j = 0;j<node->child[i+1]->keyNum;j++)
                          node->child[i+1]->child[j] = node->child[i+1]->child[j+1];
                     }

                     node->child[i]->keyNum ++;
                     node->child[i+1]->keyNum --;
                 }
                 else
                    i = BTree_merge_child(btree,node,i);

                 ///��Ҫ�ϲ�������� ����ִ��ɾ��
                 btree_delete(btree,node->child[i],key);
             }
         }



	}



/*
int B_TREE_INSERT(Tree *btree,int k,char* word)
{
    int i = 0;
    Node* node = btree->root;
    Node* newnode = NULL;
    //Node* tempNode = create_node(btree);
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
        node->parent = NULL;

        ///�Ѵ����ĵ�һ��������¸��������
        btree->root = node;
        return 0;
    }

    ///���Ҳ����� ͬʱ�����ж� ��ǰ����ǲ������� �����������Ҫ���м�Ĺؼ��ֶ���ȥ
    while(node != NULL)
    {

        if(btree->root->keyNum == btree->max)
        {
            ///����Բ��ԣ�����������Ԥ����
            newnode =create_node(btree);
            newnode->keyNum = 0;
            newnode->child[0] = node;
            B_TREE_SPLIT(btree,node,newnode);

            //node = btree->root;
            //continue;
        }

             for(i = 0;i < node->keyNum;i++)
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
            node = node->child[i];
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
            node->word[i] = node->word[i-1];
            continue;
        }
            break;
    }
    ///Ӧ���Ƕ�key[i]��ֵ
    node->key[i] = k;
    node->word[i] = word;
    node->keyNum++;
    ///���¸�ֵ����������
   // btree->root = node;
    return 0;
}
*/


/*
* ��ӡ"�����"
*
* tree       -- ������Ľڵ�
* key        -- �ڵ�ļ�ֵ
* direction  --  0����ʾ�ýڵ��Ǹ��ڵ�;
*               -1����ʾ�ýڵ������ĸ���������;
*                1����ʾ�ýڵ������ĸ������Һ��ӡ�
*/
void BTREE_Print(Node *node,int layer,int max,int ith)
{
    int i;
    if(node != NULL)
    {
        printf("===��%d��====��%d�����:",layer,ith);

        for(i=0;i< max;i++)
        {
            if(node->key[i] != NULL)
            printf("(%d)%s ",node->key[i],node->word[i]);
        }
        printf("\n");

        layer++;
        for(i = 0;i < node->keyNum + 1;i++)
        {
            if(node->child[i] != NULL)
                BTREE_Print(node->child[i],layer,max,i+1);
        }
    }
    else
        printf("==��Ϊ��\n");
}


int main()
{
    int a[] = {20, 10, 15, 14, 11, 19, 25, 51, 80};
    char b[9][9] = {"a","abandon","avoid","amuse","bind","bike","beyond","clone","click"};
    int i, ilen=sizeof(a)/sizeof(a[0]);

    Tree *btree;
    //Tree *btree;
    printf("====����B��====\n");
    create_BTree(&btree,4);
    printf("====��B��������====\n");
    btree->root = create_node(btree);
//    btree_insert(btree,btree->root,30);
    for(i=0; i<ilen; i++)
     {
         btree->root = btree_insert(btree,btree->root,a[i],b[i]);
//#if CHECK_INSERT
         printf("== ��ӽڵ�: %s\n", b[i]);
         printf("== ������ϸ��Ϣ: \n");
         BTREE_Print(btree->root,1,btree->max,1);
         printf("\n");
//#endif
     }

   // Print_BTREE(btree);

}
