#include <iostream>
#include <list>
#include <assert.h>
#include <vector>
#include <algorithm>

using namespace std;
const int INF = 1000000007;
struct Vertex {
    int value, number;
    vector<int> childs;
};
class EmptySubTree {
private:
    Vertex* root;
    int parentValue, size;

public:
    EmptySubTree(int size, int parentValue, Vertex* root)
        : root(root)
        , parentValue(parentValue)
        , size(size)
    {
    }
    Vertex* getRoot() const
    {
        return root;
    }

    int getSize() const
    {
        return size;
    }

    int getParentValue() const
    {
        return parentValue;
    }
};
class AvailableValue {
private:
    int value;

public:
    int getValue() const
    {
        return value;
    }
    AvailableValue(int value)
        : value(value)
    {
    }
};
struct Tree {
    Vertex* tree;
    int root;
    int size;
};
Tree tree;
int getEmptySubTreesRecursively(Tree tree, int v, int parent, vector<EmptySubTree>& emptySubTrees)
{
    Vertex* t = tree.tree;
    //Vertex* vp = &tree.tree[v];
    int size = 1;
    for (vector<int>::iterator i = t[v].childs.begin(); i != t[v].childs.end(); i++) {
        size += getEmptySubTreesRecursively(tree, *i, v, emptySubTrees);
    }
    if (t[v].value == 0 && t[parent].value > 0) {
        emptySubTrees.push_back(EmptySubTree(size, t[parent].value, &t[v]));
    }
    if (t[v].value == 0 && v == parent && v == 0) {
        emptySubTrees.push_back(EmptySubTree(size, INF, &t[v]));
    }
    return size;
}
bool comp(EmptySubTree a, EmptySubTree b)
{
    return a.getParentValue() < b.getParentValue();
}
void parseInputAndPrepare(Tree& tree, vector<EmptySubTree>& emptySubTrees,
    list<AvailableValue>& availableValues)
{
    cin >> tree.size;
    int size = tree.size;
    tree.tree = new Vertex[size];
    bool* setValues = new bool[size + 1];

    for (int i = 0; i <= size; i++)
        setValues[i] = false;

    for (int i = 0; i < tree.size; i++) {
        tree.tree[i].number = i + 1;
        int parent;
        cin >> parent;
        parent--;
        if (parent == i)
            tree.root = i;
        else
            tree.tree[parent].childs.push_back(i);

        cin >> tree.tree[i].value;
        setValues[tree.tree[i].value] = true;
    }
    for (int i = 1; i <= tree.size; i++) {
        if (setValues[i] == false)
            availableValues.push_back(AvailableValue(i));
    }
    getEmptySubTreesRecursively(tree, tree.root, tree.root, emptySubTrees);
    sort(emptySubTrees.begin(), emptySubTrees.end(), comp);
    delete[] setValues;
}
bool decreaseIterator(list<AvailableValue>& availableValues, list<AvailableValue>::iterator& it)
{
    if (availableValues.empty())
        return false;
    if (it == availableValues.begin())
        return false;
    it--;
    return true;
}
void addVertexValues(EmptySubTree emptySubTree, list<AvailableValue>& availableValues,
    list<AvailableValue>::iterator it)
{
    Vertex* ptrTree = emptySubTree.getRoot();
    if (!decreaseIterator(availableValues, it))
        return;
    while (1) {
        ptrTree->value = it->getValue();
        if (ptrTree->childs.size() != 1)
            return;
        if (!decreaseIterator(availableValues, it))
            return;
        ptrTree = &tree.tree[ptrTree->childs.front()];
    }
}
void setValuesInFirstSubTree(EmptySubTree emptySubTree, list<AvailableValue>& availableValues, int& unclearlyValues)
{
    list<AvailableValue>::iterator it = availableValues.begin();
    int values = 0;
    while (it != availableValues.end() && it->getValue() < emptySubTree.getParentValue()) {
        it++;
        values++;
    }
    if (values + unclearlyValues == emptySubTree.getSize()) {
        addVertexValues(emptySubTree, availableValues, it);
        unclearlyValues = 0;
    }
    else if (values + unclearlyValues > emptySubTree.getSize()) {
        unclearlyValues = values + unclearlyValues;
        unclearlyValues -= emptySubTree.getSize();
    }
    else
        assert(false);
    for (list<AvailableValue>::iterator it2 = availableValues.begin(); it2 != it;) {
        list<AvailableValue>::iterator itTmp = it2;
        it2++;
        availableValues.erase(itTmp);
    }
}
void printTreeWithValues(Tree tree)
{
    Vertex* t = tree.tree;
    for (int i = 0; i < tree.size; i++) {
        cout << t[i].value << "\n";
    }
}

void calculateSalaries()
{
    list<AvailableValue> availableValues;
    vector<EmptySubTree> emptySubTrees;
    parseInputAndPrepare(tree, emptySubTrees, availableValues);

    int unclearlyValues = 0;
    for (vector<EmptySubTree>::iterator i = emptySubTrees.begin(); i != emptySubTrees.end(); i++) {
        setValuesInFirstSubTree(*i, availableValues, unclearlyValues);
    }
    printTreeWithValues(tree);
    delete[] tree.tree;
}
int main()
{
    ios_base::sync_with_stdio(0);
    calculateSalaries();
}
