#ifndef __CLASSIFIER_CPP__
#define __CLASSIFIER_CPP__

#include "classifier.h"

void Classifier::rearrange(xml::Node *kmlNode, bool isClean) {
    if (kmlNode) {

        std::vector<std::string> styleDataStrVec;
        std::vector<xml::Node*> newFolderNodes;

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

#endif // __CLASSIFIER_CPP__