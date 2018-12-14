//
// Created by 史可 on 2018/9/20.
//
#include "node.h"

Node::Node(std::string str) {
    std::string value = str;
    terminal_ = false;
    if (str.length() > 3){
        std::string annotation = str.substr(0,3);
        if (annotation == "()("){
            value = str.substr(3,str.length());
            terminal_ = true;
        }
    };
    value_ = value;
    ptr_l_node_ = ptr_r_node_ = NULL;
    start_index_ = 0;
    end_index_ = 0;
}

Node* Node::GetLeftNode() {
    return ptr_l_node_;
}

Node* Node::GetRightNode() {
    return ptr_r_node_;
}

void Node::SetLeftNode(Node *ptr_node) {
    ptr_l_node_ = ptr_node;
}

void Node::SetRightNode(Node *ptr_node) {
    ptr_r_node_ = ptr_node;
}

std::string Node::GetData() {
    return value_;
}

bool Node::isTerminal(){
    return terminal_;
}

void Node::SetStartIndex(int start) {
    start_index_ = start;
}

void Node::SetEndIndex(int stop) {
    end_index_ = stop;
}

int Node::GetStartIndex() {
    return start_index_;
}

int Node::GetEndIndex() {
    return  end_index_;
}