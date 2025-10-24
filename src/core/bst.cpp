#include "core/bst.h"
#include <iostream>
using namespace std;

BSTNode *root = nullptr;
int nextScholarshipID = 1;

BSTNode::BSTNode(const Scholarship &s) : data(s), left(nullptr), right(nullptr) {}
IDListNode::IDListNode(int id) : scholarshipID(id), next(nullptr) {}
ResultList::ResultList() : head(nullptr) {}

void ResultList::pushFront(int id)
{
    IDListNode *n = new IDListNode(id);
    n->next = head;
    head = n;
}
void ResultList::clear()
{
    IDListNode *cur = head;
    while (cur)
    {
        IDListNode *nx = cur->next;
        delete cur;
        cur = nx;
    }
    head = nullptr;
}
ResultList::~ResultList() { clear(); }

BSTNode *insertBST(BSTNode *node, const Scholarship &s)
{
    if (!node)
        return new BSTNode(s);
    int c = cmpScholarship(s, node->data);
    if (c < 0)
        node->left = insertBST(node->left, s);
    else
        node->right = insertBST(node->right, s);
    return node;
}

void inorderTraverse(BSTNode *node, function<void(const Scholarship &)> f)
{
    if (!node)
        return;
    inorderTraverse(node->left, f),f(node->data),inorderTraverse(node->right, f);
}

BSTNode *findByID(BSTNode *node, int id)
{
    if (!node)
    {
        return nullptr;
    }
    if (node->data.id == id)
        return node;
    BSTNode *left = findByID(node->left, id);
    if (left)
        return left;
    return findByID(node->right, id);
}

BSTNode *deleteByKey(BSTNode *node, double amount, int id)
{
    if (!node)
        return nullptr;
    int c = cmpScholarshipKey(amount, id, node->data);
    if (c < 0)
        node->left = deleteByKey(node->left, amount, id);
    else if (c > 0)
        node->right = deleteByKey(node->right, amount, id);
    else
    {
        if (!node->left)
        {
            BSTNode *r = node->right;
            delete node;
            return r;
        }
        else if (!node->right)
        {
            BSTNode *l = node->left;
            delete node;
            return l;
        }
        else
        {
            BSTNode *succParent = node;
            BSTNode *succ = node->right;
            while (succ->left)
            {
                succParent = succ;
                succ = succ->left;
            }
            node->data = succ->data;
            if (succParent == node)
                succParent->right = deleteByKey(succParent->right, succ->data.amount, succ->data.id);
            else
                succParent->left = deleteByKey(succParent->left, succ->data.amount, succ->data.id);
        }
    }
    return node;
}
void collectIf(BSTNode *node, ResultList &res, function<bool(const Scholarship &)> pred)
{
    if (!node)
        return;
    collectIf(node->left, res, pred);
    if (pred(node->data))
        res.pushFront(node->data.id);
    collectIf(node->right, res, pred);
}

void printResultList(ResultList &r)
{
    if (!r.head)
    {
        cout << "No results found.\n";
        return;
    }
    cout << "=== Results ===\n";
    IDListNode *cur = r.head;
    while (cur)
    {
        BSTNode *n = findByID(root, cur->scholarshipID);
        if (n)
            printScholarshipDetail(n->data);
        cur = cur->next;
    }
}
void cleanupBST(BSTNode *node)
{
    if (!node)
        return;
    cleanupBST(node->left),cleanupBST(node->right);
    delete node;
}
