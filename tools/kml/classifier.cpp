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

        // include folders stuff
        std::vector<std::vector<std::string>> includedFolderNameVecVec;

        General kmlGeneral;
        Placemark kmlPlacemark;
        Builder kmlBuilder;
        StyleStrings kmlStyleStrings;

        bool isFirstPlcItr = true;
        std::string styleDataStr;

        for (auto &placemark : kmlNode->getDescendantsByName("Placemark", true)) {

            if (isFirstPlcItr) {
                styleDataStr = kmlStyleStrings.getPlacemarkStyleData(placemark, true);
                isFirstPlcItr = false;
            }
            else styleDataStr = kmlStyleStrings.getPlacemarkStyleData(placemark, false);

            int styleDataStrVec_foundDex = mini_tool::isPrimitiveInsideVector<std::string>(
                styleDataStrVec, styleDataStr
            );

            // INCLUDE FOLDERS STUFF //

            xml::Node *includedNewFolder_node = nullptr;   // !nullptr -> not exist
            std::string includedExistFolder_name = "";     // !"" -> exist

            // include folders check
            if (isIncludeFolders && placemark->getParent()) {

                int includedFolderNameVecVec_foundDex = -1,
                    styleDataStrVec_foundDex_buffer;

                includedExistFolder_name = kmlPlacemark.getName(placemark->getParent());

                if (styleDataStrVec_foundDex == -1) {
                    includedFolderNameVecVec.push_back(std::vector<std::string>{});
                    styleDataStrVec_foundDex_buffer = includedFolderNameVecVec.size() - 1;
                }
                else {
                    includedFolderNameVecVec_foundDex = mini_tool::isPrimitiveInsideVector<std::string>(
                        includedFolderNameVecVec.at(styleDataStrVec_foundDex), includedExistFolder_name
                    );
                    styleDataStrVec_foundDex_buffer = styleDataStrVec_foundDex;
                }

                // there isn't any yet
                if (includedFolderNameVecVec_foundDex == -1) {
                    includedNewFolder_node = kmlBuilder.createFolder(includedExistFolder_name);
                    includedFolderNameVecVec.at(styleDataStrVec_foundDex_buffer).push_back(includedExistFolder_name);
                    includedExistFolder_name = "";
                }
            }

            // ********* INCLUDE FOLDERS STUFF //

            // remove from parent
            placemark->removeFromParent();

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

                // INCLUDE FOLDERS STUFF //
                if (isIncludeFolders) {
                    includedNewFolder_node->addChild(placemark);
                    newFolderNodes.back()->addChild(includedNewFolder_node);
                }   // ********* INCLUDE FOLDERS STUFF //
                else {
                    newFolderNodes.back()->addChild(placemark);
                }
            }
            // already available
            else {
                xml::Node *folderNode = newFolderNodes.at(styleDataStrVec_foundDex);

                // INCLUDE FOLDERS STUFF //
                if (isIncludeFolders) {
                    if (includedNewFolder_node) {
                        includedNewFolder_node->addChild(placemark);
                        folderNode->addChild(includedNewFolder_node);
                    }
                    else if (includedExistFolder_name != "") {
                        
                        for (auto &includedExistFolder_node : *folderNode->getChildren()) {
                            if (includedExistFolder_name == kmlPlacemark.getName(includedExistFolder_node)) {

                                includedExistFolder_node->addChild(placemark);
                                break;
                            }
                        }
                    }
                }
                // ********* INCLUDE FOLDERS STUFF //
                else folderNode->addChild(placemark);
            }
        }

        // add the folders to a working folder //

        xml::Node
            *mainFolderNode = kmlGeneral.searchMainFolder(kmlNode),
            *classifyFolder = kmlBuilder.createFolder(CLASSIFY_COMMAND_WORKING_FOLDER);

        if (isCleanFolders) {

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
                classifyFolder->addChild(folderNode);
            }

            mainFolderNode->addChild(classifyFolder);
        }
        else {
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

bool Classifier::filterString(xml::Node *kmlNode, std::string searchStr) {
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