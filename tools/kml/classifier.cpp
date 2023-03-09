#ifndef __CLASSIFIER_CPP__
#define __CLASSIFIER_CPP__

#include "classifier.h"

void Classifier::rearrange(xml::Node *kmlNode, bool isClean) {
    if (kmlNode) {

        std::vector<std::string> styleDataStrVec;
        std::vector<xml::Node*> newFolderNodes;

        int testCtr = 1;

        for (auto &placemark : kmlNode->getDescendantsByName("Placemark", true)) {

            StyleStrings kmlStyleStrings;
            std::string styleDataStr = kmlStyleStrings.getPlacemarkStyleData(placemark);
            int foundDex = mini_tool::isInsideVectorString(styleDataStrVec, styleDataStr);

            placemark->removeFromParent();

            if (foundDex == -1) { // there isn't any yet
                styleDataStrVec.push_back(styleDataStr);
                std::string folderName;
                
                if (kmlStyleStrings.isAColorCode(styleDataStr)) {
                    folderName = "PATHS " + styleDataStr;
                }
                else {
                    folderName = "PINS " + styleDataStr;
                }

                newFolderNodes.push_back(
                    Builder().getFolder(folderName)
                );
                newFolderNodes.back()->addChild(placemark);
            }
            // already available
            else newFolderNodes.at(foundDex)->addChild(placemark);

            testCtr++;
        }

        General kmlGeneral = General();

        xml::Node
            *mainFolderNode = kmlGeneral.searchMainFolder(kmlNode),
            *classifyFolder;

        if (isClean) {
            std::vector<xml::Node*>
                *mainFolderChildrenPtr = mainFolderNode->getChildren(),
                savedNodes;

            for (auto &node : *mainFolderChildrenPtr) {
                std::string nodeName = node->getName();

                if (nodeName != "StyleMap" &&
                    nodeName != "Style" &&
                    nodeName != "name" &&
                    nodeName != "description" &&
                    nodeName != "open"
                ) {
                    delete node;
                    node = nullptr;
                }
                else savedNodes.push_back(node);
            }

            // alternative for 'removeFromParent()'
            mainFolderChildrenPtr->clear();
            *mainFolderChildrenPtr = savedNodes;

            for (auto &folderNode : newFolderNodes) {
                mainFolderNode->addChild(folderNode);
            }
        }
        else {
            classifyFolder = Builder().getFolder(CLASSIFY_COMMAND_WORKING_FOLDER);

            kmlGeneral.insertEditedPlacemarksIntoFolder(
                mainFolderNode,
                classifyFolder,
                newFolderNodes,
                {"Classifying placemarks", "Classify placemarks"},
                ""
            );
        }
    }
}

bool Classifier::folderByText(xml::Node *kmlNode, std::string searchStr) {
    if (kmlNode) {

        std::vector<xml::Node*> placemarks;
        General kmlGeneral = General();

        for (auto &placemark : kmlNode->getDescendantsByName("Placemark", true)) {

            xml::Node *plmk_stringNode;
            bool isAble = false;

            plmk_stringNode = placemark->getFirstDescendantByName("name");
            if (plmk_stringNode) {
                if (plmk_stringNode->getInnerText().find(searchStr) != std::string::npos) {
                    placemark->removeFromParent();
                    placemarks.push_back(placemark);
                    isAble = true;
                }
            }

            if (!isAble) {
                plmk_stringNode = placemark->getFirstDescendantByName("description");
                if (plmk_stringNode) {
                    if (plmk_stringNode->getInnerText().find(searchStr) != std::string::npos) {
                        placemark->removeFromParent();
                        placemarks.push_back(placemark);
                    }
                }
            }
        }

        if (placemarks.size() > 0) {
            std::string folderName = FOLDERBYTEXT_COMMAND_WORKING_FOLDER + std::string("  ") + searchStr;

            xml::Node *mainFolderNode = kmlGeneral.searchMainFolder(kmlNode);
            std::cout << "KML-> Found: " << placemarks.size() << "\n"
                      << "      Placed to folder named '" << folderName << "'\n"
                      << "      Folder by text completed!\n";

            kmlGeneral.insertEditedPlacemarksIntoFolder(
                mainFolderNode,
                Builder().getFolder(folderName),
                placemarks,
                {"", ""},
                ""
            );
            return true;
        }
        else {
            std::cerr << "KML-> Folder by text error. "
                      << "No placemark's name or description contains '" << searchStr << "'\n";
            return false;
        }
    }

    return false;
}

#endif // __CLASSIFIER_CPP__