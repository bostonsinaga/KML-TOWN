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
            kmlNode->getChildrenByName("Document", false)
        );

        if (documentNodes.size() > 0) {

            if (documentNodes.size() > 1) {
                // 1st generation
                return kmlNode;
            }
            else {
                std::vector<xml::Node*> folderNodes = (
                    documentNodes.front()->getChildrenByName("Folder", false)
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
                else if (documentNodes.front()->getFirstChildByName("Placemark", false)) {
                    // document as main folder (empty folder but may has placemarks)
                    return documentNodes.front();
                }
            }
        }

        std::cerr << "KML-> Error. Empty document\n";
        return nullptr;
    }

    xml::Node *getRootDocument(xml::Node *kmlNode) {
        xml::Node *docNode = kmlNode->getFirstChildByName("Document", false);
        if (!docNode) {
            docNode =  kmlNode->getFirstChildByName("Folder", false);
        }
        return docNode;
    }

    void fillWithPlacemarks(
        xml::Node *containerNode,
        std::vector<xml::Node*> *placemarkVec,           // should be empty
        std::vector<xml::Node*> *placemarksCoorNodeVec,  // should be empty
        bool isPin
    ) {
        // collect pins
        *placemarkVec = containerNode->getDescendantsByName("Placemark");

        // collect pins coordinate //

        std::vector<int> noCoorIndexes;

        int ctr = 0;
        for (auto &pin : *placemarkVec) {
            xml::Node *pinCoor;
            pinCoor = pin->getFirstDescendantByName("coordinates", false);

            if (pinCoor) { 
                if ((isPin && pinCoor->getParent()->getName() == "Point") || // pins
                    (!isPin && pinCoor->getParent()->getName() != "Point")   // paths
                ) {
                    if (isPin) {
                        if (mini_tool::getInStringCharCount(pinCoor->getInnerText(), ',') == 2) {
                            if (placemarksCoorNodeVec) placemarksCoorNodeVec->push_back(pinCoor);
                        }
                        else noCoorIndexes.push_back(ctr);
                    }
                    else if (placemarksCoorNodeVec) {
                        placemarksCoorNodeVec->push_back(pinCoor);
                    }
                }
            }
            else noCoorIndexes.push_back(ctr);

            ctr++;
        }

        // remove no coordinate nodes
        int idxShrinkRate = 0;
        for (auto &idx : noCoorIndexes) {
            placemarkVec->erase(
                placemarkVec->begin() + idx + idxShrinkRate,
                placemarkVec->begin() + idx + idxShrinkRate + 1
            );
            idxShrinkRate--;
        }
    }

    void insertEditedPlacemarksIntoFolder(
        std::string newFolderName,
        xml::Node *containerNode,
        std::vector<xml::Node*> *placemarks, // or any node
        const std::vector<std::string> &noticeFuncName
    ) {
        // previous container name for noticing purpose //

        std::string prevFolderName = containerNode->getName();
        xml::Node *prevFolderNameNode = containerNode->getFirstDescendantByName("name");

        if (prevFolderNameNode) {
            prevFolderName = prevFolderNameNode->getInnerText();
        }

        // failed stop
        if (placemarks->size() == 0) {
            std::string taskName = noticeFuncName.at(1);

            // task name lower case
            mini_tool::changeStringCase(
                &taskName,
                mini_tool::LOWER_CASE_FLAG,
                0
            );

            std::cerr
                << "KML-> "
                << noticeFuncName.at(0) << " error. No pin to "
                << taskName << " inside '"
                << prevFolderName << "' node\n";
            return;
        }

        // create new pins folder
        Builder kmlBuilder;
        xml::Node *folderNode = kmlBuilder.getFolder(newFolderName);
        containerNode->addChild(folderNode);

        // move up the folder //

        int priorityFolderDataNode = 0;
        if (containerNode->getFirstChildByName("name", false)) {
            priorityFolderDataNode++;
        }
        if (containerNode->getFirstChildByName("visibility", false)) {
            priorityFolderDataNode++;
        }
        if (containerNode->getFirstChildByName("open", false)) {
            priorityFolderDataNode++;
        }

        containerNode->swapChildren(
            containerNode->getChildren()->at(priorityFolderDataNode),
            folderNode
        );

        // insert pins into the folder

        for (auto &plmrk : *placemarks) {
            plmrk->removeFromParent();
            folderNode->addChild(plmrk);
        }

        // success notice
        std::cout
            << "KML-> "
            << noticeFuncName.at(1) << " pins inside '"
            << prevFolderName << "' node completed!\n";
    }

    //set document name as 'fileDir_out' file name
    void setKMLDocumentName(xml::Node *kmlNode, std::string fileDir_out) {
        xml::Node *mainFolderNode = searchMainFolder(kmlNode);

        while (mainFolderNode) {

            xml::Node *nameNode = kmlNode->getFirstChildByName("name", false);
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