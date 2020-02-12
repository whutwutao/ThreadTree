//
//  ThreadTree.h
//  线索二叉树
//
//  Created by WuTao on 2020/2/8.
//  Copyright © 2020 WuTao. All rights reserved.
//

#ifndef ThreadTree_h
#define ThreadTree_h
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

typedef int ElemType;
typedef struct ThreadNode
{
    ElemType data;
    struct ThreadNode *lchild, *rchild;
    bool ltag, rtag;//线索标志
    // false表示该结点的对应指针指向其孩子结点，
    // true表示该结点的对应指针指向该结点的前驱或后继
}ThreadNode, *ThreadTree;

ThreadTree PreInCreateTree(ElemType Pre[],ElemType In[],int s1,int e1,int s2,int e2)
{
    ThreadTree T = (ThreadTree)malloc(sizeof(ThreadNode));
    T->data = Pre[s1];
    int i;
    for (i = s2; T->data != In[i]; i++);
    int len1 = i - s2;//左子树序列长度
    int len2 = e2 - i;//右子树序列长度
    if (len1 > 0)
    {
        T->lchild = PreInCreateTree(Pre,In,s1+1,s1+len1,s2,s2+len1-1);
    }
    else
    {
        T->lchild = NULL;
    }
    if (len2 > 0)
    {
        T->rchild = PreInCreateTree(Pre,In,s1+1+len1,s1+len1+len2,s2+len1+1,s2+len1+len2);
    }
    else
    {
        T->rchild = NULL;
    }
    return T;
}


//释放以T为根结点的二叉树所占存储空间
//void destroyBiTree(ThreadTree T)
//{
//    if (T != NULL)
//    {
//        destroyBiTree(T->lchild);
//        destroyBiTree(T->rchild);
//        //        printf("结点%d已释放\n",T->data);
//        free(T);
//    }
//}


//释放前序线索二叉树
void destroyPreThreadTree(ThreadTree T)
{
    if (T)
    {
        ThreadNode *p = T, *q = T;
        while (p)
        {
            if (!p->ltag)
            {
                q = p;
                p = p->lchild;
                free(q);
            }
            else
            {
                q = p;
                p = p->rchild;
                free(q);
            }
        }
        
    }
}

//访问结点
void visit(ThreadNode *node)
{
    if (node != NULL)
    {
        cout << node->data << " ";
    }
}


//将二叉树转化为前序线索二叉树
void PreOrderThreading(ThreadTree T)
{
    if (T)
    {
        int MaxSize = 50;
        ThreadNode * Stack[MaxSize];
        int top = -1;
        ThreadNode *p = T, *pre = NULL;
        while (p || top != -1)
        {
            //从根结点开始依次考察左结点，直到找到最左子孙结点
            while (p)
            {
                Stack[++top] = p;//结点进栈
                //沿路根据右子树的有无确定rtag的值
                if (p->rchild)
                {
                    p->rtag = false;
                }
                else
                {
                    p->rtag = true;
                }
                //若不是最左子孙结点，则判断其是否有右子树，若无，则将其左孩子设为其后继结点
                if (p->lchild)
                {
                    if (!p->rchild)
                    {
                        p->rchild = p->lchild;
                    }
                    p->ltag = false;
                    pre = p;
                    p = p->lchild;
                }
                //若找到最左子孙结点,则将其p->lchild指向pre所指的其前驱结点,pre指向p,退出循环
                else
                {
                    p->ltag = true;
                    p->lchild = pre;
                    pre = p;
                    break;
                }
            }
            //从栈中取出结点做回溯
            if (top != -1)
            {
                p = Stack[top--];
                //栈顶结点若有右子树，则p指向右子树，进行下一轮的考察
                if (!p->rtag)
                {
                    pre->rchild = p->rchild;
                    p = p->rchild;
                }
                //栈顶结点若无右子树，则下一步仍要做回溯，所以将p设为NULL
                else
                {
                    p = NULL;
                }
            }
        }
        //最后将前序遍历 最后 一个结点的 后继 设为空
        pre->rtag = true;
        pre->rchild = NULL;
    }
}


//前序线索二叉树的前序遍历
void PreOrderThreadTree(ThreadTree T)
{
    if (T)
    {
        ThreadNode *p = T;
        while (p)
        {
            visit(p);//访问根结点
            //若根结点有左子树，则下一次访问左孩子
            if (!p->ltag)
            {
                p = p->lchild;
            }
            //若根结点无左子树，则下一次访问右孩子(rtag = 0)或后继结点(rtag = 1)
            else
            {
                p = p->rchild;
            }
        }
    }
}


//通过中序遍历对二叉树线索化的递归算法
void InThread(ThreadTree &p, ThreadTree &pre)//pre为中序遍历下p的前驱结点
{
    if (p != NULL)
    {
        InThread(p->lchild, pre);//递归地将左子树线索化
        if (p->lchild == NULL)
        {
            p->lchild = pre;
            p->ltag = 1;
        }
        if (pre != NULL && pre->rchild == NULL)
        {
            pre->rchild = p;
            pre->rtag = 1;
        }
        pre = p;
        InThread(p->rchild, pre);//递归地将右子树线索化
    }
}

void CreateInThread(ThreadTree T)
{
    ThreadTree pre = NULL;
    if (T != NULL)
    {
        InThread(T,pre);
        pre->rchild = NULL;//线索化最后p指向NULL,pre指向中序下的最后一个结点,将其后继置空
        pre->rtag = true;
    }
}


//构造中序线索二叉树的非递归算法
void InOrderThreading(ThreadNode *T)
{
    int MaxSize = 50;
    ThreadNode * Stack[MaxSize];
    int top = -1;
    ThreadNode *p = T, *pre = NULL;
    while (p || top != -1)
    {
        //从根结点开始一直向左考察左子结点，直到到达最左子孙结点为止
        while (p)
        {
            Stack[++top] = p;
            //如果有左孩子，则将ltag置为0，继续向左孩子考察
            if (p->lchild)
            {
                p->ltag = false;
                p = p->lchild;
            }
            //如果没有左孩子，说明找到了最左子孙结点
            else
            {
                p->lchild = pre;
                p->ltag = true;
                pre = p;
                p = NULL;//p置为空，为强制退栈作准备
            }
        }
        if (top != -1)
        {
            p = Stack[top--];
            p->rtag = false;
            if (!pre->rchild && pre != p)
            {
                pre->rchild = p;
                pre->rtag = true;
                pre = p;
            }
            p = p->rchild;
        }
    }
    pre->rtag = true;
}


//中序线索二叉树的遍历
void InOrderThreadTree(ThreadTree T)
{
    ThreadNode *p = T;
    bool flag;
    cout << "中序线索二叉树的中序遍历序列:";
    while (p)
    {
        while (!p->ltag)
        {
            p = p->lchild;
        }
        flag = true;
        while (flag && p)
        {
            visit(p);
            if (!p->rtag)
            {
                flag = p->rtag;
            }
            p = p->rchild;
        }
    }
}


//中序线索二叉树的释放
void destroyInThreadTree(ThreadTree T)
{
    ThreadNode *p = T;
    bool flag;
    while (p)
    {
        while (!p->ltag)
        {
            p = p->lchild;
        }
        flag = true;
        while (flag && p)
        {
            ThreadNode *q = p;
            if (!p->rtag)
            {
                flag = p->rtag;
            }
            p = p->rchild;
            free(q);
        }
    }
}
#endif /* ThreadTree_h */
