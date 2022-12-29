#ifndef __CLASSIFIER_CPP__
#define __CLASSIFIER_CPP__

#include "classifier.h"

void Classifier::rearrange(xml::Node *kmlNode) {
    if (kmlNode) {

        std::vector<std::string> styleDataStrVec;
        std::vector<xml::Node*> newFolderNodes;

        for (auto &placemark : kmlNode->getDescendantsByName("Placemark", true)) {

            std::string styleDataStr = StyleStrings().getPlacemarkStyleData(placemark);
            int foundDex = mini_tool::isInsideVectorString(styleDataStrVec, styleDataStr);

            placemark->removeFromParent();

            if (foundDex == -1) {
                styleDataStrVec.push_back(styleDataStr);
                newFolderNodes.push_back(
                    Builder().getFolder(styleDataStr)
                );
                newFolderNodes.back()->addChild(placemark);
            }
            else newFolderNodes.at(foundDex)->addChild(placemark);
        }

        General kmlGeneral = General();

        xml::Node
            *mainFolderNode = kmlGeneral.searchMainFolder(kmlNode),
            *classifyFolder = Builder().getFolder(CLASSIFY_COMMAND_WORKING_FOLDER);

        kmlGeneral.insertEditedPlacemarksIntoFolder(
            mainFolderNode,
            classifyFolder,
            newFolderNodes,
            {"Classifying placemarks", "Classify placemarks"},
            ""
        );
    }
}

#endif // __CLASSIFIER_CPP__