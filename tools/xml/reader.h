#ifndef __XML_READER_H__
#define __XML_READER_H__

class Reader {
    public:
        Node *fileParse(std::string fileName) {

            std::ifstream readFile(fileName);
            std::string text, stringBuffer;

            while (std::getline(readFile, stringBuffer)) {
                text += stringBuffer + "\n";
            }

            readFile.close();
            size_t foundTopper = text.find(XML_TOPPER);

            if (foundTopper != std::string::npos) {
                text = text.substr(foundTopper + XML_TOPPER_COUNT);
                return parse(text, fileName);
            }

            std::cerr
                << "XML-TOOL-> Parsing error. '"
                << fileName
                << "' is missing the version and encoding value at its top\n";
                
            return nullptr;
        }

        // input must only has one root in text form
        Node *parse(std::string text, std::string fileName = ".xml") {

            std::cout << "XML-TOOL-> Reading '" << fileName << "'...\n";

            std::function<bool(char)> isALetter = [=](char ch)->bool {
                if (int(ch) >= 97 && int(ch) <= 122) {
                    return true;
                }
                return false;
            };

            // multi root error message
            std::function<void(std::string)> multiRootErrorMessage = [=](std::string name) {
                std::cerr
                    << "XML-TOOL-> Parsing error. "
                    << "Cannot parse multiple root the one named '"
                    << name << "'\n";
            };

            enum {
                void_rdflag,
                inner_text_rdflag,
                mark_name_rdflag,
                tail_name_rdflag,
                att_name_rdflag,
                att_val_rdflag
            };

            int readingFlag = void_rdflag;
            bool isRooted = false;

            Node *retNode = nullptr;
            std::vector<Node*> queueNodes;
            std::vector<Attribute> readAttributes;

            int qnSz = queueNodes.size();

            std::string
                readMarkName = "",
                readAttName = "",
                readAttVal = "",
                readInnerText = "";

            int ctr = 0;
            char preCH = '\n';

            for (auto &CH : text) {

                // MARK OPENING
                if (preCH == '<' && CH != '/') {
                    readingFlag = mark_name_rdflag;
                    readMarkName += CH;
                    readInnerText = "";
                }
                // FEMALE TAIL OPENING
                else if (preCH == '<' && CH == '/') {
                    readingFlag = tail_name_rdflag;
                }
                // INSIDE MARK READING
                else if (ctr > 0) {
                    // MARK NAME
                    if (readingFlag == mark_name_rdflag) {
                        bool isSkip = false;

                        // to ATT NAME
                        if (isALetter(preCH) && CH == ' ') {
                            readingFlag = att_name_rdflag;
                            isSkip = true;
                        }
                        // get MALE
                        else if (preCH == '/' && CH == '>') {
                            readingFlag = void_rdflag;
                            isSkip = true;

                            if (readAttributes.size() == 0) {
                                // cut '/' at back of 'readMarkName'
                                readMarkName = readMarkName.substr(
                                    0, readMarkName.size() - 1
                                );
                            }

                            Node *node = new Node(readMarkName);
                            node->setGender(MALE_XMLGEN);
                            
                            if (qnSz > 0) {
                                queueNodes.back()->addChild(node);
                            }
                            else if (qnSz == 0) {
                                if (!isRooted) {
                                    isRooted = true;
                                    retNode = node;
                                }
                                else { // error message
                                    multiRootErrorMessage(node->getName());
                                    delete node;
                                    if (retNode) delete retNode;
                                    return nullptr;
                                }
                            }

                            // MALE with ATT
                            if (readAttributes.size() > 0) {
                                node->setAttributes(readAttributes);
                                readAttributes.clear();
                            }

                            readMarkName = "";
                        }
                        // get FEMALE without ATT
                        else if (isALetter(preCH) && CH == '>') {
                            readingFlag = inner_text_rdflag;
                            isSkip = true;

                            Node *node = new Node(readMarkName);
                            queueNodes.push_back(node);
                            qnSz = queueNodes.size();
                            readMarkName = "";
                        }

                        if (!isSkip) readMarkName += CH;
                    }
                    // FEMALE TAIL
                    else if (readingFlag == tail_name_rdflag) {
                        bool isSkip = false;

                        // verify MARK
                        if (isALetter(preCH) && CH == '>') {
                            readingFlag = void_rdflag;
                            isSkip = true;

                            if (queueNodes.back()->getName() == readMarkName) {

                                if (qnSz == 1) {
                                    if (!isRooted) {
                                        isRooted = true;
                                        retNode = queueNodes.back();
                                    }
                                    else { // error message
                                        multiRootErrorMessage(queueNodes.back()->getName());
                                        delete queueNodes.back();
                                        if (retNode) delete retNode;
                                        return nullptr;
                                    }
                                }
                                else if (qnSz > 1) {
                                    int index = qnSz - 2;
                                    if (queueNodes.at(index)) {
                                        queueNodes.at(index)->addChild(queueNodes.back());
                                    }
                                }

                                // set inner text if no children
                                if (queueNodes.back()->getChildren()->size() == 0) {

                                    // cut '<' at back of 'readInnerText'
                                    readInnerText = readInnerText.substr(
                                        0, readInnerText.size() - 1
                                    );

                                    queueNodes.back()->setInnerText(readInnerText);
                                }

                                queueNodes.pop_back();
                                qnSz = queueNodes.size();
                                readMarkName = "";
                            }
                            else { // error message
                                std::cerr
                                    << "XML-TOOL-> Parsing error. The element named '"
                                    << queueNodes.back()->getName()
                                    << "' has wrong structure\n";

                                delete queueNodes.back();
                                if (retNode) delete retNode;
                                return nullptr;
                            }
                        }

                        if (!isSkip) readMarkName += CH;
                    }
                    // ATT NAME
                    else if (readingFlag == att_name_rdflag) {
                        bool isSkip = false;

                        // to ATT VAL
                        if (isALetter(preCH) && CH == '=') {
                            readingFlag = att_val_rdflag;
                            isSkip = true;
                        }

                        if (!isSkip) readAttName += CH;
                    }
                    // ATT VAL
                    else if (readingFlag == att_val_rdflag) {
                        bool isSkip = false;

                        // push ATT
                        if ((preCH == '"' && CH == ' ') ||
                            (preCH == '"' && CH == '>') ||
                            (preCH == '"' && CH == '/')
                        ) {
                            if (readAttVal.at(0) == '"') {
                                readAttVal = readAttVal.substr(1);
                            }

                            if (readAttVal.at(readAttVal.size() - 1) == '"') {
                                readAttVal = readAttVal.substr(0, readAttVal.size() - 1);
                            }

                            readAttributes.push_back(
                                Attribute(readAttName, readAttVal)
                            );

                            readAttName = "";
                            readAttVal = "";
                        }

                        // to ATT NAME
                        if (preCH == '"' && CH == ' ') {
                            readingFlag = att_name_rdflag;
                            isSkip = true;
                        }
                        // approach the closing of MALE ELEMENT
                        else if (preCH == '"' && CH == '/') {
                            readingFlag = mark_name_rdflag;
                            isSkip = true;
                        }
                        // close FEMALE HEAD with ATT
                        else if (preCH == '"' && CH == '>') {
                            readingFlag = inner_text_rdflag;
                            isSkip = true;

                            Node *node = new Node(readMarkName);
                            queueNodes.push_back(node);
                            qnSz = queueNodes.size();
                            node->setAttributes(readAttributes);
                            readAttributes.clear();
                            readMarkName = "";
                        }

                        if (!isSkip) readAttVal += CH;
                    }
                    else if (readingFlag == inner_text_rdflag) {
                        readInnerText += CH;
                    }
                }
                else if (CH != '<' && CH != ' ' && CH != '\n') {
                    std::cerr
                        << "XML-TOOL-> Parsing error. Unexpected character at beginning of '"
                        << fileName
                        << "'\n";

                    return nullptr;
                }

                // counting
                preCH = CH;
                ctr++;
            }

            if (retNode) {
                if (fileName == ".xml") {
                    fileName = retNode->getName() + " [in runtime element]";
                }
                std::cout << "XML-TOOL-> Parse from '" << fileName << "' completed!\n";
            }

            return retNode;
        }
};

#endif // __XML_READER_H__