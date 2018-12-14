//
// Created by shi ke on 2018/9/20.
//

#ifndef STATNLP_BINARYTREE_H
#define STATNLP_BINARYTREE_H

#include "binary_tree_common.h"
#include "node.h"

class BinaryTree{
public:
    BinaryTree(std::string str);
    ~BinaryTree();
    void ConvertToStringVector(std::string str);
    Node * BuildBinaryTree(std::string str);
    int FindNextBracketIndex(std::string str);
    bool PrintTree(Node *ptr_node);
    Node * GetRootNode();
    std::vector<std::string> *GetXVector();
    std::vector<std::string> *GetSentenceVector();
private:
    Node *ptr_root_;
    std::vector<Node *> node_vector_;
    std::vector<std::string> *ptr_sentence_vector_;
    std::vector<std::string> *ptr_str_vector_;
    std::vector<std::string> *ptr_x_vector_;
    int index_offset_;
};

#endif //STATNLP_BINARYTREE_H
