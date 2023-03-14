#ifndef __XML_NODE_CPP__
#define __XML_NODE_CPP__

#include "node.h"

Node::Node(std::string name_in, Node *parent_in) {
    name = name_in;
    if (parent_in) {
        parent = parent_in;
        parent->addChild(this);
    }
}

Node::~Node() {    
    if (parent) {
        parent->removeChild(this);
        parent = nullptr;
    }

    for (auto &child : children) {
        delete child;
    }

    children.clear();
    attributes.clear();
}

std::string Node::getName() {return name;}
std::string Node::getInnerText() {return innerText;}

Node *Node::getParent() {return parent;}

Node *Node::getRoot() {
    Node *retNode = this;

    while (retNode->getParent()) {
        retNode = retNode->getParent();
    }

    return retNode;
}

std::vector<Node*> *Node::getChildren() {return &children;}
std::vector<Attribute> *Node::getAttributes() {return &attributes;}

Node *Node::getFirstChildByName(
    std::string searchName,
    bool isPrintMessage
) {    
    for (auto &child : children) {
        if (child->getName() == searchName) {
            return child;
        }
    }

    if (isPrintMessage) {
        printChildNotFoundMessage(searchName);
    }
    
    return nullptr;
}

std::vector<Node*> Node::getChildrenByName(
    std::string searchName,
    bool isPrintMessage
) {
    std::vector<Node*> retNodes;

    for (auto &child : children) {
        if (child->getName() == searchName) {
            retNodes.push_back(child);
        }
    }

    if (retNodes.size() == 0 && isPrintMessage) {
        printChildNotFoundMessage(searchName);
    }

    return retNodes;
}

// pull by name as first child from upper to lower generations
Node *Node::getFirstDescendantByName(
    std::string searchName,
    bool isPrintMessage
) {
    for (auto &child : children) {
        if (child->getName() == searchName) {
            return child;
        }
        else {
            Node *retNode = child->getFirstDescendantByName(searchName);
            if (retNode) return retNode;
        }
    }
    
    if (isPrintMessage) {
        printChildNotFoundMessage(searchName);
    }

    return nullptr;
}

// pull by name as children from upper to lower generations
std::vector<Node*> Node::getDescendantsByName(
    std::string searchName,
    bool isPrintMessage
) {
    std::vector<Node*> retNodes;

    for (auto &child : children) {

        if (child->getName() == searchName) {
            retNodes.push_back(child);
        }

        std::vector<Node*> pulledNodes = child->getDescendantsByName(searchName);
        
        retNodes.insert(
            retNodes.end(),
            pulledNodes.begin(),
            pulledNodes.end()
        );
    }

    if (retNodes.size() == 0 && isPrintMessage) {
        printChildNotFoundMessage(searchName);
    }

    return retNodes;
}

int Node::getGender() {
    return genderFlag;
}

void Node::setGender(int genderFlag_in) {
    genderFlag = genderFlag_in;
}

// NAME AND INNER TEXT //

void Node::setName(std::string name_in) {
    name = name_in;
}

void Node::setInnerText(std::string text_in) {
    innerText = text_in;
}

// ATTRIBUTE //

void Node::setAttributes(std::vector<Attribute> attributes_in) {
    attributes = attributes_in;
}

void Node::addAttribute(Attribute attribute_in) {
    attributes.push_back(attribute_in);
}

void Node::setParent(Node *parent_in) {
    parent = parent_in;
}

// node still exist (take care)
void Node::removeFromParent() {
    if (parent) {
        int ctr = 0;
        for (auto &child : *parent->getChildren()) {
            if (child == this) {
                parent->getChildren()->erase(
                    parent->getChildren()->begin() + ctr,
                    parent->getChildren()->begin() + ctr + 1
                );
                break;
            }
            ctr++;
        }
        parent = nullptr;
    }
}

void Node::addChild(Node *newChild, int order) {

    if (order != -1) {
        if (order > children.size()) {
            order = children.size();
        }
        children.insert(children.begin() + order, newChild);
    }
    else children.push_back(newChild);

    newChild->setParent(this);
}

void Node::addChildren(std::vector<Node*> newChildren) {
    children.insert(
        children.end(),
        newChildren.begin(),
        newChildren.end()
    );
}

std::vector<Node*> Node::replaceChildren(std::vector<Node*> newChildren) {
    std::vector<Node*> unusedChildren = releaseChildren();
    children = newChildren;
    return unusedChildren;
}

void Node::swapChildren(int aDex, int bDex) {
    if (aDex != -1 && aDex != -1) {
        Node *buffer = children.at(aDex);
        children.at(aDex) = children.at(bDex);
        children.at(bDex) = buffer;
    }
}

void Node::swapChildren(Node *childA, Node *childB) {
    if (childA != childB) {

        int swapIndexes[2] = {-1, -1}, ctr = 0;

        for (auto &child : children) {

            if (child == childA) swapIndexes[0] = ctr;
            else if (child == childB) swapIndexes[1] = ctr;

            if (swapIndexes[0] != -1 && swapIndexes[1] != -1) {
                break;
            }

            ctr++;
        }

        swapChildren(swapIndexes[0], swapIndexes[1]);
    }
}

/*  WARNING!
*   use 'isClean' if only the child has not been deleted
*   (don't use 'isClean' from node destructor)
*/
void Node::removeChild(Node *exChild, bool isClean) {
    
    for (int i = 0; i < children.size(); i++) {
        if (children.at(i) == exChild) {

            if (isClean) delete children.at(i);

            children.erase(children.begin() + i, children.begin() + i + 1);
            break;
        }
    }
}

void Node::removeChildren() {
    for (auto &child : children) {
        delete child;
    }
    children.clear();
}

std::vector<Node*> Node::releaseChildren() {
    
    for (auto &child : children) {
        child->setParent(nullptr);
    }

    std::vector<Node*> retChildren = children;
    children.clear();
    return retChildren;
}

void Node::printChildNotFoundMessage(std::string searchName) {
    std::cerr
        << "XML-> Node warning. No child named '"
        << searchName << "' in '"
        << name << "' node\n";
}

#endif // __XML_NODE_CPP__