#ifndef __XML_WRITER_CPP__
#define __XML_WRITER_CPP__

#include "writer.h"

void Writer::stringify(
    std::string fileDir_out,
    Node *root,
    bool isWithRoot
) {
    /* the 'root' is not include to write */

    std::cout << "XML-> Writing '" + fileDir_out + "'...\n";

    Node *includedRoot;

    if (isWithRoot) {
        includedRoot = root;
        root = new Node("SuperRoot");
        root->addChild(includedRoot);
    }

    /* write into 'fileDir_out' */
    
    std::ofstream writeFile(fileDir_out);

    std::string text = (
        XML_TOPPER + std::string("\n") + pullChildrenStrings(root->getChildren())
    );
    
    writeFile << text;
    writeFile.close();

    if (isWithRoot) {
        root->releaseChildren();
        delete root; // delete 'SuperRoot'
    }

    std::cout << "XML-> Stringify into '" + fileDir_out + "' completed!\n";
}

std::string Writer::stringifyAttributes(std::vector<Attribute> *attributes) {
    std::string retTxt = "";
    for (auto &att : *attributes) {
        retTxt += " " + att.getName() + "=\"" + att.getValue() + "\"";
    }
    return retTxt;
}

std::string Writer::pullChildrenStrings(std::vector<Node*> *children) {

    std::string
        retTxt = "",
        indentSpaces[2] = {"", ""};

    for (auto &child: *children) {

        // produce indent spaces
        int indentCtr = -1;
        Node *parent = child->getParent();

        while (parent) {
            indentCtr++;
            parent = parent->getParent();
        }

        for (int i = 0; i < indentCtr; i++) {
            indentSpaces[0] += "    ";
        }

        // never has children (MALE)
        if (child->getGender() == MALE_XMLGEN) {
            retTxt += (
                indentSpaces[0] + "<" + child->getName()
                + stringifyAttributes(child->getAttributes()) + "/>\n"
            );
        }
        else { // FEMALE element
            std::string pulledStrings = pullChildrenStrings(child->getChildren());

            if (pulledStrings != "") { // has children
                pulledStrings = "\n" + pulledStrings;
                indentSpaces[1] = indentSpaces[0];
            }
            else { // no children
                pulledStrings = child->getInnerText();
                indentSpaces[1] = "";
            }

            retTxt += (
                indentSpaces[0] + "<" + child->getName()
                    + stringifyAttributes(child->getAttributes()) + ">"
                        + pulledStrings
                + indentSpaces[1] + "</" + child->getName() + ">\n"
            );
        }

        indentSpaces[0] = "";
        indentSpaces[0] = "";
    }

    return retTxt;
}

#endif // __XML_WRITER_CPP__