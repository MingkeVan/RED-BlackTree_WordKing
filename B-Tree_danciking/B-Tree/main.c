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

typedef int type;

int m;///��¼�������
int sum;

/*���� b���Ľ��ṹ*/
typedef struct B_Tree_Node
{
    int keyNum; //���Ϊm-1
    type *key;//�ؼ�������
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

void btree_InsertNoneFull(Tree *btree,Node *node,type key,char *word)
{
    int i;
    int j=0;///���ҵ���Ϥ��key�� ����j����ѭ���õ� ���ݵ�����Ĳ���λ��
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

Node* btree_insert(Tree *btree,Node* node,type key,char* word)
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

void right_rotate(Node* parentNode,Node* currentNode,Node* nearNode,int i)
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
void left_rotate(Node* parentNode,Node* currentNode,Node* nearNode,int i)
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
	int BTree_merge_child(Tree* btree, Node* parent, int loc)
	{
	    int i;
	    Node* lNode;
	    Node* rNode;

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
/*	    ///��ǰ�ڵ��Ǹ���� ���غϲ�
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
        }*/
	}

	///ɾ����Ҫ��������� һ����Ҫɾ��key���ڵ�ǰ�ڵ� һ����Ҫɾ��key�ڵ�ǰ���

	void btree_delete_node(Tree *btree,Node* node,type key,char *word)
	{
	    int i = 0;
	    int j;
	    type tempkey;
	    char *tempword;

	    ///�ҳ�ɾ��λ�� ����ǰ�ڵ������ ���� keyС�ڵ��ڵ�ǰ�ڵ�ĳ��keyʱ ����ѭ��
	    while(i < node->keyNum && key > node->key[i])
        {
            i++;
        }

        ///��keyС�ڵ��ڵ�ǰkeyʱ ��Ҫ�ٸ����ֵ������ɾ��λ��
        if(key == node->key[i])
        {
            while(i < node->keyNum && strcmpi(word,node->word[i]) > 0)
                {
                    i++;
                }
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
         if(i < node->keyNum && key == node->key[i] && strcmp(word,node->word[i]) == 0)
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
                     Node* newNode;
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
                     Node* newNode;
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
        ///���ݽṹ��tree���max min�������if(i>0 && node->child[i-1]->keyNum > btree->min)
                      {
                        ///���оֲ����� ���ֵܸ�ԣ
                        right_rotate(node,node->child[i],node->child[i-1],i);
                      }/////////////////////////////////////////////////////////////////////////////////////////////////
                       else if(i < node->keyNum - 1 && node->child[i+1]->keyNum >btree->min)
                      {
                        ///���оֲ����� ���ֵܸ�ԣ
                        left_rotate(node,node->child[i],node->child[i+1],i);
                      }
                       else
                         i = BTree_merge_child(btree,node,i);
                       ///��Ҫ�ϲ�������� ����ִ��ɾ��
                       btree_delete_node(btree,node->child[i],key);
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

char* inorder_BTree(Node *node,int num)
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
               printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
               printf("== ��Ϥ��Ϊ��%d��ĵ���Ϊ%s\n",num,node->word[i-1]);
               return node->word[i-1];
           }
        }
         if(node->child[0] != NULL)
        inorder_BTree(node->child[0],num);
    }

}

int Aggregate_BTree(Node *node,int num)
{
    int i;
    char* a;
    if(node != NULL)
    {
        for(i = node->keyNum;i > 0;i--)
        {
           //if(key == node->key[i])


           if(node->child[i] != NULL)
           Aggregate_BTree(node->child[i],num);

           //printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
           if(m++ == num)
           {
               printf("%s(%d)\n",node->word[i-1],node->key[i-1]);
               printf("== ��Ϥ��Ϊ��%d��ĵ���Ϊ%s\n",num,node->word[i-1]);
               sum += node->key[i-1];
           }
        }
         if(node->child[0] != NULL)
        Aggregate_BTree(node->child[0],num);
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
void BTREE_Print(Node *node,int layer,int ith)
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
    //type a[] = {'F','S','Q','K','C','L','H','T','V','W','M','R','N','P','A','B','X','Y','D','Z','E','H','T','V','W','M','R','N','P','M','R','N','P','A','B','X','Y','D','Z','E'};
    //type c[] = {'Y','W','Q','X','K','B','H','P','T','F','M'};
    char b[][9] = {"B","abandon","avoid","amuse","bind","bike","beyond","clone","click","cling","clicking","J","J","j","j","N","A","Z","G","E","Q","k","k","l","k","k","k","l","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k","k"};
   static type a[]={12,12,4,5,7,3,5,0,0,0,0,66,45,34,435,3,435,4,56,867,54,7,97,565,3,2,2,23,465465,32,234,54654,634,6,46,4576,543,34,5,35,46,999,534,6,34,634,6,546345,52,5,43,64,6,4,56435,4,545};
   type c[]={12,3,4,5,7,3,5,0,3289,45,324,66,45,34,435,3,435,4,56,867,54,7,97,565,3,2,2,23,465465,32,234,54654,634,6,46,4576};
    int i, ilen=sizeof(a)/sizeof(a[0]);
    int clen = sizeof(c)/sizeof(a[0]);

    Tree *btree;
    //Tree *btree;
    printf("====����B��====\n");
    create_BTree(&btree,4);
    printf("====��B��������====\n");
    btree->root = create_node(btree);
//    btree_insert(btree,btree->root,30);
    for(i=0; i < ilen; i++)
     {
         //b[i][9] = "adf";
          btree->root = btree_insert(btree,btree->root,a[i],b[i]);
//#if CHECK_INSERT
         printf("== ��ӽڵ�: %d\n", a[i]);
         printf("== ������ϸ��Ϣ: \n");
         BTREE_Print(btree->root,1,1);
         printf("\n");
//#endif
     }
     inorder_BTree(btree->root,12);

    /* for(i=0; i<ilen; i++)
     {
         btree_delete(btree,a[i],b[i]);
//#if CHECK_DELETE
         printf("== ɾ���ڵ�: %d\n", a[i]);
         printf("== ������ϸ��Ϣ: \n");
         BTREE_Print(btree->root,1,1);
         printf("\n");
//#endif
     }*/

   int op;
   int N;
   int j = 0;
   int k;
   int l,r;
   char* word;

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
    //scanf("%d %d",&op,&k);
    scanf("%d %d %d",&op,&l,&r);
    switch(op){
    ///Find
    case 1:

        word = inorder_BTree(btree->root,k);
        //printf("== ��Ϥ��Ϊ��%d��ĵ���Ϊ%s\n",k,word);
        break;
    ///Insert
    case 2:
        btree->root = btree_insert(btree,btree->root,20,"storm");
        printf("== ��ӽ�� ������ϸ��Ϣ: \n");
        BTREE_Print(btree->root,1,1);
        break;
    ///Modify
    case 3:
        //inorder_DeleteNode(root->node,"click");
        //RB_DELETE(root,deleteNode->key,deleteNode->word);
        //RB_INSERT(root,10,"click");
        btree_delete(btree,12,"B");
        btree->root = btree_insert(btree,btree->root,17,"B");
        printf("== �޸�������ϸ��Ϣ: \n");
        BTREE_Print(btree->root,1,1);
        break;
    ///Delete
    case 4:
        //inorder_DeleteNode(root->node,"amuse");
        //RB_DELETE(root,deleteNode->key,deleteNode->word);
        btree_delete(btree,12,"B");
        printf("== ɾ��������ϸ��Ϣ: \n");
        BTREE_Print(btree->root,1,1);
        break;
    ///Aggregate
    case 5:
        //m = 1;
       // inorder_Aggregate(root->node,l,r);
       for(i = l;i<=r;i++)
       {
           m = 1;
           Aggregate_BTree(btree->root,i);
       }
        printf("== �ӵ�%dth����%dth����Ϥ���ܺ�Ϊ%d: \n",l,r,sum);
        break;

    default:
        break;
   }

   }

   system("pause");

}
