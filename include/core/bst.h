#ifndef BST_H
#define BST_H

#include "core/scholarship.h"
#include <functional>
using namespace std;

struct BSTNode
{
    Scholarship data;
    BSTNode *left, *right;
    BSTNode(const Scholarship &s);
};

struct IDListNode
{
    int scholarshipID;
    IDListNode *next;
    IDListNode(int id);
};

struct ResultList
{
    IDListNode *head;
    ResultList();
    void pushFront(int id);
    void clear();
    ~ResultList();
};

extern BSTNode *root;
extern int nextScholarshipID;

BSTNode *insertBST(BSTNode *node, const Scholarship &s);
void inorderTraverse(BSTNode *node, function<void(const Scholarship &)> f);
BSTNode *findByID(BSTNode *node, int id);
BSTNode *deleteByKey(BSTNode *node, double amount, int id);

void collectIf(BSTNode *node, ResultList &res, function<bool(const Scholarship &)> pred);
void printResultList(ResultList &r);

void cleanupBST(BSTNode *node);

#endif