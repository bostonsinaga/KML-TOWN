#ifndef __KML_CPP__
#define __KML_CPP__

#include "kml.h"

namespace kml {

    int checkCompass(std::string axisStr) {

        if (axisStr.find('N') != std::string::npos ||
            axisStr.find('n') != std::string::npos ||
            axisStr.find('U') != std::string::npos ||
            axisStr.find('u') != std::string::npos
        ) {
            return 2; // NORTH
        }
        else if (
            axisStr.find('S') != std::string::npos ||
            axisStr.find('s') != std::string::npos
        ) {
            return -2; // SOUTH
        }
        else if (
            axisStr.find('E') != std::string::npos ||
            axisStr.find('e') != std::string::npos ||
            axisStr.find('T') != std::string::npos ||
            axisStr.find('t') != std::string::npos
        ) {
            return 1; // EAST
        }
        else if (
            axisStr.find('W') != std::string::npos ||
            axisStr.find('B') != std::string::npos ||
            axisStr.find('w') != std::string::npos ||
            axisStr.find('b') != std::string::npos
        ) {
            return -1; // WEST
        }

        return 0;
    }

    xml::Node *searchMainFolder(xml::Node *kmlNode) {

        /* NOTE:
        *   the main folder only caught on 3 generations
        *   from kml root (documents and folders)
        *   Below after 3 generations will be ignored
        */

        if (!kmlNode) return nullptr;

        std::vector<xml::Node*> documentNodes = (
            kmlNode->getChildrenByName("Document")
        );

        if (documentNodes.size() > 0) {

            if (documentNodes.size() > 1) {
                // 1st generation
                return kmlNode;
            }
            else {
                std::vector<xml::Node*> folderNodes = (
                    documentNodes.front()->getChildrenByName("Folder")
                );

                if (folderNodes.size() > 0) {

                    if (folderNodes.size() > 1) {
                        // 2nd generation
                        return documentNodes.front();
                    }
                    else {
                        // 3rd generation
                        return folderNodes.front();
                    }
                }
                else if (documentNodes.front()->getFirstChildByName("Placemark")) {
                    // document as main folder (empty folder but may has placemarks)
                    return documentNodes.front();
                }
            }
        }

        std::cerr << "KML-> Error. Empty document\n";
        return nullptr;
    }

    xml::Node *getRootDocument(xml::Node *kmlNode) {
        xml::Node *docNode = kmlNode->getFirstChildByName("Document");
        if (!docNode) {
            docNode =  kmlNode->getFirstChildByName("Folder");
        }
        return docNode;
    }

    std::string getRootDocumentName(xml::Node *kmlNode) {
        xml::Node *docNode = getRootDocument(kmlNode);
        if (docNode) {
            xml::Node *nameNode = docNode->getFirstChildByName("name");
            if (nameNode) {
                std::string retStr = nameNode->getInnerText();
                if (retStr != "") return retStr;
                return "noname";
            }
        }
        return "noname";
    }

    void fillWithPlacemarks(
        xml::Node *containerNode,
        std::vector<xml::Node*> &placemarkVec,           // should be empty
        std::vector<xml::Node*> &placemarksCoorNodeVec,  // should be empty
        bool isProcessingPin
    ) {
        // collect pins
        placemarkVec = containerNode->getDescendantsByName("Placemark", true);

        // collect pins coordinate //

        std::vector<int> noCoorIndexes;

        int ctr = 0;
        for (auto &pin : placemarkVec) {
            xml::Node *pinCoor;
            pinCoor = pin->getFirstDescendantByName("coordinates");

            if (pinCoor) {
                if ((isProcessingPin && pinCoor->getParent()->getName() == "Point") || // pins
                    (!isProcessingPin && pinCoor->getParent()->getName() != "Point")   // paths
                ) {
                    if (isProcessingPin) {
                        if (mini_tool::getInStringCharCount(pinCoor->getInnerText(), ',') == 2) {
                            placemarksCoorNodeVec.push_back(pinCoor);
                        }
                        else noCoorIndexes.push_back(ctr);
                    }
                    else {
                        placemarksCoorNodeVec.push_back(pinCoor);
                    }
                }
            }
            else noCoorIndexes.push_back(ctr);

            ctr++;
        }

        // remove no coordinate nodes
        int idxShrinkRate = 0;
        for (auto &idx : noCoorIndexes) {
            placemarkVec.erase(
                placemarkVec.begin() + idx + idxShrinkRate,
                placemarkVec.begin() + idx + idxShrinkRate + 1
            );
            idxShrinkRate--;
        }
    }

    void insertEditedPlacemarksIntoFolder(
        std::string newFolderName,
        xml::Node *containerNode,
        std::vector<xml::Node*> &placemarks, // or any node
        const std::vector<std::string> &noticeFuncName,
        std::string typeStr
    ) {
        // previous container name for noticing purpose //

        std::string prevFolderName = containerNode->getName();
        xml::Node *prevFolderNameNode = containerNode->getFirstDescendantByName("name");

        if (prevFolderNameNode) {
            prevFolderName = prevFolderNameNode->getInnerText();
        }

        // succeeded
        if (placemarks.size() > 0) {
            
            // create new pins folder
            xml::Node *folderNode = Builder().getFolder(newFolderName);
            containerNode->addChild(folderNode);

            // move up the folder //

            int priorityFolderDataNode = 0;
            if (containerNode->getFirstChildByName("name")) {
                priorityFolderDataNode++;
            }
            if (containerNode->getFirstChildByName("visibility")) {
                priorityFolderDataNode++;
            }
            if (containerNode->getFirstChildByName("open")) {
                priorityFolderDataNode++;
            }

            containerNode->swapChildren(
                containerNode->getChildren()->at(priorityFolderDataNode),
                folderNode
            );

            // insert pins into the folder
            for (auto &plmrk : placemarks) {
                plmrk->removeFromParent();
                folderNode->addChild(plmrk);
            }
        }

        // logging
        logEditedPlacemarks(
            typeStr,
            noticeFuncName,
            placemarks,
            containerNode
        );
    }

    bool logEditedPlacemarks(
        std::string typeStr,
        std::vector<std::string> noticeFuncName,
        std::vector<xml::Node*> &placemarks,
        xml::Node *containerNode
    ) {
        // set folder name //
        std::string folderName = "noname";
        bool isFolderNaming = true;

        xml::Node *groupParentNameNode;
        if (!containerNode) {
            if (placemarks.size() > 0) {
                containerNode = placemarks.front()->getParent();
            }
            else isFolderNaming = false;
        }

        if (isFolderNaming) {
            groupParentNameNode = (
                containerNode->getFirstChildByName("name")
            );

            if (groupParentNameNode) {
                folderName = groupParentNameNode->getInnerText();
            }
        }

        // evaluate //

        if (placemarks.size() == 0) { // failed
            std::string taskName = noticeFuncName.at(1);

            // task name lower case
            mini_tool::changeStringCase(
                &taskName,
                mini_tool::LOWER_CASE_FLAG,
                0
            );

            std::cerr
                << "KML-> "
                << noticeFuncName.at(0) << " error. No "
                << typeStr << " to "
                << taskName << " inside '"
                << (containerNode ? containerNode->getName() : "unknown") << "' named '"
                << folderName << "'\n";

            return false;
        }
        else { // succeeded
            std::cout
                << "KML-> "
                << noticeFuncName.at(1) << " "
                << typeStr << (typeStr != "" ? "s " : "") << "inside '"
                << (containerNode ? containerNode->getName() : "unknown") << "' named '"
                << folderName << "' completed!\n";

            return true;
        }
    }

    //set document name as 'fileDir_out' file name
    void setKMLDocumentName(xml::Node *kmlNode, std::string fileDir_out) {
        xml::Node *mainFolderNode = searchMainFolder(kmlNode);

        while (mainFolderNode) {

            xml::Node *nameNode = kmlNode->getFirstChildByName("name");
            if (!nameNode) {
                nameNode = new xml::Node("name", mainFolderNode);
            }
            nameNode->setInnerText(mini_tool::cutFileDirName(fileDir_out));

            xml::Node *possibleParentNode = mainFolderNode->getParent();
            if (possibleParentNode &&
                possibleParentNode->getName() != "kml"
            ) {
                mainFolderNode = possibleParentNode;
            }
            else break;
        }
    }
}

#endif // __KML_CPP__