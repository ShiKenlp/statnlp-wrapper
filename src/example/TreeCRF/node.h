//
// Created by shike on 2018/9/20.
//

#ifndef STATNLP_NODE_H
#define STATNLP_NODE_H

#include "binary_tree_common.h"
class Node{
public:
    Node(std::string value);
    std::string GetData();
    void SetLeftNode(Node *ptr_node);
    void SetRightNode(Node *ptr_node);
    Node * GetLeftNode();
    Node * GetRightNode();
    void SetStartIndex(int start);
    void SetEndIndex(int stop);
    int GetStartIndex();
    int GetEndIndex();
    bool isTerminal();
private:
    std::string value_;
    bool terminal_;
    Node *ptr_l_node_;
    Node *ptr_r_node_;
    int start_index_;
    int end_index_;
};

#endif //STATNLP_NODE_H
