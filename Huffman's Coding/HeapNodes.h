#ifndef HUFFMAN_S_CODING_HEAPNODES_H
#define HUFFMAN_S_CODING_HEAPNODES_H
struct Node
{
    int frequency=0;
    // Constructor
    Node(int f)
    {
        frequency = f;
    }
    virtual Node* getLeft(){ return NULL; }
    virtual Node* getRight(){ return NULL; }
    virtual unsigned char getCharater(){ return NULL; }
};

struct LeafNode: public Node
{

    unsigned char character;
    // Constructor
    LeafNode(unsigned char c) : Node(1) {
        character = c;
    }

    unsigned char getCharater() override{ return character; }
};

struct SubTreeNode: public Node
{

    Node* left;
    Node* right;
    // Constructor
    SubTreeNode(int f, Node *l, Node *r ): Node(f) {
        left = l;
        right = r;
    }
    Node* getLeft() override{ return left; }
    Node* getRight() override{ return right; }

};


#endif //HUFFMAN_S_CODING_HEAPNODES_H
/*
 * Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.
Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
 */
