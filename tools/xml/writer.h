#ifndef __XML_WRITER_H__
#define __XML_WRITER_H__

class Writer {
    public:

        void stringify(
            std::string fileDir_out,
            Node *root,
            bool isWithRoot = true
        ) {
            /* note: the 'root' is not include to write */

            std::cout << "XML-TOOL-> Writing '" + fileDir_out + "'...\n";

            Node *includedRoot;

            if (isWithRoot) {
                includedRoot = root;
                root = new Node("SuperRoot");
                root->addChild(includedRoot);
            }
            
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

            std::cout << "XML-TOOL-> Stringify into '" + fileDir_out + "' completed!\n";
        }

    private:

        std::string stringifyAttributes(std::vector<Attribute> *attributes) {
            std::string retTxt = "";
            for (auto &att : *attributes) {
                retTxt += " " + att.getName() + "=\"" + att.getValue() + "\"";
            }
            return retTxt;
        }

        std::string pullChildrenStrings(std::vector<Node*> *children) {

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

                // no children (single)
                if (child->isSterileElement()) {
                    retTxt += (
                        indentSpaces[0] + "<" + child->getName()
                        + stringifyAttributes(child->getAttributes()) + "/>\n"
                    );

                    for (auto &att : *child->getAttributes()) {
                        std::cout << att.getName() << " - " << att.getValue() << std::endl;
                    }
                }
                else { // closable element
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
};

#endif // __XML_WRITER_H__