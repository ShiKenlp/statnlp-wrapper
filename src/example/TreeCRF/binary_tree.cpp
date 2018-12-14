//
// Created by shike on 2018/9/20.
//
#include "binary_tree.h"

BinaryTree::BinaryTree(std::string str) {
    ptr_sentence_vector_ = new std::vector<std::string>;
    ptr_str_vector_ = new std::vector<std::string>;
    ptr_x_vector_ = new std::vector<std::string>;
    ConvertToStringVector(str);
    index_offset_ = 0;
    ptr_root_ = BuildBinaryTree(str);
    //PrintTree(ptr_root_);
}

BinaryTree::~BinaryTree() {
    delete ptr_str_vector_;
    delete ptr_x_vector_;
    for(std::vector<Node *>::iterator it = node_vector_.begin(); it!=node_vector_.end();++it){
        delete(*it);
    }
}

void BinaryTree::ConvertToStringVector(std::string str) {
    std::stringstream ss(str);
    std::string str_tmp;
    while(ss >> str_tmp){   // split the string by space, convert string to vector<string>.
        ptr_str_vector_->push_back(str_tmp);
#ifdef IF_DEBUG__
        std::cout << "the vector is"<< str_tmp <<std::endl;
#endif
    }
}


Node *BinaryTree::BuildBinaryTree(std::string str) {
    if (0 == str.length()) {
        return NULL;
    }
    int space_index = str.find_first_of(SPACE_STRING);
    std::string root_str = str.substr(1, space_index - 1);
    //Node *ptr_node = new Node(str.substr(1, space_index - 1));  // Add each token to node.
    int index = -1;
    if (str[0] == LEFT_BRACKET_CHAR) {
        index = FindNextBracketIndex(str);
    }
    if (index != -1 && index != -2) {

        Node *ptr_node = new Node(root_str);
        std::string left_str = str.substr(space_index + 1, index - space_index);
        ptr_node->SetLeftNode(BuildBinaryTree(left_str));
        std::string rigth_str = str.substr(index + 1, str.length() - index - 2);
        ptr_node->SetRightNode(BuildBinaryTree(rigth_str));

        node_vector_.push_back(ptr_node);
        return ptr_node;

    }else if(index == -2){
        //std::cout<<str<<std::endl;
        // the terminator
        // annotate the terminal node.
        std::string terminal_str = "()(" + root_str;
        Node *ptr_node = new Node(terminal_str);
        std::string left_str = str.substr(space_index+1, str.length() - space_index -2);
        ptr_sentence_vector_->push_back(left_str);
        Node *ptr_last_node = new Node(left_str);
        ptr_node->SetLeftNode(ptr_last_node);

        node_vector_.push_back(ptr_node);
        return ptr_node;

    }
}

int BinaryTree::FindNextBracketIndex(std::string str) {
    if (str.length() <= 1) {
        return -1;
    }
    std::stack<char> bracket_stack;
    for (int i = 1; i < str.length(); ++i) {
        if (str[i] == LEFT_BRACKET_CHAR) {
            bracket_stack.push(LEFT_BRACKET_CHAR);
        } else if (str[i] == RIGHT_BRACKET_CHAR) {
            if (bracket_stack.empty()) {
                //the emission
                return -2;
            }
            if (bracket_stack.top() == LEFT_BRACKET_CHAR) {
                bracket_stack.pop();
                if (bracket_stack.empty()) {
                    //return the position of division
                    return i;
                }
            }
        }
    }
    //not found
    return -1;
}

bool BinaryTree::PrintTree(Node *ptr_node) {
    if (ptr_node == NULL) {
        return NULL;
    }
//    std::cout << ptr_node->GetData() << " : "<<ptr_node->GetStartIndex()<<","<<ptr_node->GetEndIndex()<< std::endl;
    PrintTree(ptr_node->GetLeftNode());
    PrintTree(ptr_node->GetRightNode());
}

Node *BinaryTree::GetRootNode() {
    return ptr_root_;
}

std::vector<std::string>* BinaryTree::GetXVector() {
    return ptr_x_vector_;
}

std::vector<std::string>* BinaryTree::GetSentenceVector() {
    return ptr_sentence_vector_;
}


