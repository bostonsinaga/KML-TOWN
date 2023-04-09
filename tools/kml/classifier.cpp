#ifndef __CLASSIFIER_CPP__
#define __CLASSIFIER_CPP__

#include "classifier.h"

void Classifier::rearrange(
    xml::Node *kmlNode,
    bool isCleanFolders,
    bool isIncludeFolders
) {
    if (kmlNode) {
        std::vector<std::string> styleDataStrVec;
        std::vector<xml::Node*> newFolderNodes;

        General kmlGeneral;
        Placemark kmlPlacemark;
        Builder kmlBuilder;
        StyleStrings kmlStyleStrings;

        bool isFirstPlcItr = true;
        std::string styleDataStr;
        int ctr = 0;

        for (auto &placemarkNode : kmlNode->getDescendantsByName("Placemark", true)) {

            if (isFirstPlcItr) {
                styleDataStr = kmlStyleStrings.getPlacemarkStyleData(placemarkNode, true);
                isFirstPlcItr = false;
            }
            else styleDataStr = kmlStyleStrings.getPlacemarkStyleData(placemarkNode, false);

            int styleDataStrVec_foundDex = mini_tool::isPrimitiveInsideVector<std::string>(
                styleDataStrVec, styleDataStr
            );

            // remove from parent
            if (!isIncludeFolders) {
                placemarkNode->removeFromParent();
            }

            // there isn't any yet
            if (styleDataStrVec_foundDex == -1) {

                styleDataStrVec.push_back(styleDataStr);
                std::string folderName;
                
                if (kmlStyleStrings.isAColorCode(styleDataStr)) {
                    folderName = "PATHS " + styleDataStr;
                }
                else {
                    folderName = "PINS " + styleDataStr;
                }

                newFolderNodes.push_back(
                    kmlBuilder.createFolder(folderName)
                );

                if (isIncludeFolders) {
                    kmlPlacemark.includeFolder(
                        placemarkNode,
                        newFolderNodes.back(),
                        styleDataStrVec.size() - 1, true
                    );
                }
                else {
                    newFolderNodes.back()->addChild(placemarkNode);
                }
            }
            // already available
            else {
                xml::Node *folderNode = newFolderNodes.at(styleDataStrVec_foundDex);

                if (isIncludeFolders) {
                    kmlPlacemark.includeFolder(
                        placemarkNode,
                        folderNode,
                        styleDataStrVec_foundDex, false
                    );
                }
                else folderNode->addChild(placemarkNode);
            }

            ctr++;
        }

        // add the folders to a working folder //

        xml::Node
            *mainFolderNode = kmlGeneral.searchMainFolder(kmlNode),
            *classifiedFolderNode = kmlBuilder.createFolder(CLASSIFY_COMMAND_WORKING_FOLDER);

        if (isCleanFolders) {
            kmlGeneral.cleanFolders(
                mainFolderNode,
                classifiedFolderNode,
                newFolderNodes
            );
        }
        else {
            kmlGeneral.insertEditedPlacemarksIntoFolder(
                mainFolderNode,
                classifiedFolderNode,
                newFolderNodes,
                {"Classifying placemarks", "Classify placemarks"},
                ""
            );
        }
    }
}

bool Classifier::filterString(xml::Node *kmlNode, std::string searchStr) {
    if (kmlNode) {

        std::vector<xml::Node*> placemarks;
        General kmlGeneral = General();

        for (auto &placemarkNode : kmlNode->getDescendantsByName("Placemark", true)) {

            xml::Node *plmk_stringNode;
            bool isAble = false;

            plmk_stringNode = placemarkNode->getFirstDescendantByName("name");
            if (plmk_stringNode) {
                if (plmk_stringNode->getInnerText().find(searchStr) != std::string::npos) {
                    placemarkNode->removeFromParent();
                    placemarks.push_back(placemarkNode);
                    isAble = true;
                }
            }

            if (!isAble) {
                plmk_stringNode = placemarkNode->getFirstDescendantByName("description");
                if (plmk_stringNode) {
                    if (plmk_stringNode->getInnerText().find(searchStr) != std::string::npos) {
                        placemarkNode->removeFromParent();
                        placemarks.push_back(placemarkNode);
                    }
                }
            }
        }

        std::string frontWord = "";

        for (auto &searchCh : searchStr) {
            if (searchCh == ' ' && frontWord != "") {
                std::cerr << "KML-> Filter string warning. The search string has more than one word.\n"
                          << "      Folder name will use '" << frontWord << "' from the first word\n";
                break;
            }
            else if (searchCh != ' ') {
                frontWord += searchCh;
            }
        }

        if (placemarks.size() > 0) {
            std::string folderName = FILTER_STRING_COMMAND_WORKING_FOLDER + std::string("  ") + frontWord;

            xml::Node *mainFolderNode = kmlGeneral.searchMainFolder(kmlNode);
            std::cout << "KML-> Found: " << placemarks.size() << "\n"
                      << "      Placed to folder named '" << folderName << "'\n"
                      << "      Filter string completed!\n";

            kmlGeneral.insertEditedPlacemarksIntoFolder(
                mainFolderNode,
                Builder().createFolder(folderName),
                placemarks,
                {"", ""},
                ""
            );
            return true;
        }
        else {
            std::cerr << "KML-> Filter string error. "
                      << "No placemark's name or description contains '" << frontWord << "'\n";
            return false;
        }
    }

    return false;
}

#endif // __CLASSIFIER_CPP__