#ifndef __XML_NODE_H__
#define __XML_NODE_H__

class Attribute {
    public:
        Attribute(std::string name_in, std::string value_in) {
            name = name_in;
            value = value_in;
        }

        std::string getName() {return name;}
        std::string getValue() {return value;}

        void setName(std::string name_in) {
            name = name_in;
        }

        void setValue(std::string value_in) {
            value = value_in;
        }

    private:
        std::string name, value;
};

#define MALE_XMLGEN 0 // unable to own children
#define FEMALE_XMLGEN 1 // able to own children

class Node {
    public:
        Node(std::string name_in, Node *parent_in = nullptr) {
            name = name_in;
            if (parent_in) {
                parent = parent_in;
                parent->addChild(this);
            }
        }

        ~Node() {
            for (auto &child : children) {
                delete child;
            }
            parent = nullptr;
            children.clear();
            attributes.clear();
        }

        std::string getName() {return name;}
        std::string getInnerText() {return innerText;}

        Node *getParent() {return parent;}

        Node *getRoot() {
            Node *retNode = this;

            while (retNode->getParent()) {
                retNode = retNode->getParent();
            }

            return retNode;
        }
        
        std::vector<Node*> *getChildren() {return &children;}
        std::vector<Attribute> *getAttributes() {return &attributes;}

        Node *getFirstChildByName(
            std::string searchName,
            bool isPrintMessage = true
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

        std::vector<Node*> getChildrenByName(
            std::string searchName,
            bool isPrintMessage = true
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
        Node *getFirstDescendantByName(
            std::string searchName,
            bool isPrintMessage = true
        ) {
            for (auto &child : children) {
                if (child->getName() == searchName) {
                    return child;
                }
                else {
                    Node *retNode = child->getFirstDescendantByName(searchName, false);
                    if (retNode) return retNode;
                }
            }
            
            if (isPrintMessage) {
                printChildNotFoundMessage(searchName);
            }
            return nullptr;
        }

        // pull by name as children from upper to lower generations
        std::vector<Node*> getDescendantsByName(
            std::string searchName,
            bool isPrintMessage = true
        ) {
            std::vector<Node*> retNodes;

            for (auto &child : children) {

                if (child->getName() == searchName) {
                    retNodes.push_back(child);
                }

                std::vector<Node*> pulledNodes = child->getDescendantsByName(searchName, false);
                
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

        int getGender() {
            return genderFlag;
        }

        void setGender(int genderFlag_in) {
            genderFlag = genderFlag_in;
        }

        // NAME AND INNER TEXT //

        void setName(std::string name_in) {
            name = name_in;
        }

        void setInnerText(std::string text_in) {
            innerText = text_in;
        }

        // ATTRIBUTE //

        void setAttributes(std::vector<Attribute> attributes_in) {
            attributes = attributes_in;
        }

        void addAttribute(Attribute attribute_in) {
            attributes.push_back(attribute_in);
        }

        void setParent(Node *parent_in) {
            parent = parent_in;
        }

        void removeFromParent() {
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

        void addChild(Node *newChild, int order = -1) {

            if (order != -1) {
                if (order > children.size()) {
                    order = children.size();
                }
                children.insert(children.begin() + order, newChild);
            }
            else children.push_back(newChild);

            newChild->setParent(this);
        }

        void addChildren(std::vector<Node*> newChildren) {
            children.insert(
                children.end(),
                newChildren.begin(),
                newChildren.end()
            );
        }

        std::vector<Node*> replaceChildren(std::vector<Node*> newChildren) {
            std::vector<Node*> unusedChildren = releaseChildren();
            children = newChildren;
            return unusedChildren;
        }

        void swapChildren(int aDex, int bDex) {
            if (aDex != -1 && aDex != -1) {
                Node *buffer = children.at(aDex);
                children.at(aDex) = children.at(bDex);
                children.at(bDex) = buffer;
            }
        }

        void swapChildren(Node *childA, Node *childB) {
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

        void removeChild(Node *notChild) {
            int ctr = 0;
            for (auto &child : children) {
                if (child == notChild) {
                    children.erase(children.begin() + ctr, children.begin() + ctr + 1);
                    break;
                }
                ctr++;
            }
        }

        void removeChildren() {
            for (auto &child : children) {
                delete child;
            }
            children.clear();
        }

        std::vector<Node*> releaseChildren() {
            
            for (auto &child : children) {
                child->setParent(nullptr);
            }

            std::vector<Node*> retChildren = children;
            children.clear();
            return retChildren;
        }

    private:
        void printChildNotFoundMessage(std::string searchName) {
            std::cerr
                << "XML-> Node warning. No child named '"
                << searchName << "' in '"
                << name << "' node\n";
        }

        int genderFlag = FEMALE_XMLGEN;
        std::string name, innerText;
        Node *parent = nullptr;

        std::vector<Node*> children;
        std::vector<Attribute> attributes;
};

#endif // __XML_NODE_H__