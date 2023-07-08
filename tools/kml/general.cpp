#ifndef __KML_GENERAL_CPP__
#define __KML_GENERAL_CPP__

#include "general.h"

/* REUSABLE FUNCTIONS FOR ALL KML TOOLS */

int General::checkCompass(std::string axisStr) {

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

xml::Node *General::searchMainFolder(xml::Node *kmlNode) {

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

xml::Node *General::getRootDocument(xml::Node *kmlNode) {
    xml::Node *docNode = kmlNode->getFirstChildByName("Document");
    if (!docNode) {
        docNode =  kmlNode->getFirstChildByName("Folder");
    }
    return docNode;
}

std::string General::getRootDocumentName(xml::Node *kmlNode) {
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

void General::fillWithPlacemarks(
    xml::Node *containerNode,   // eg. 'Folder' or 'Document' node
    std::string coordinatesTypeName,    // eg. 'Point' or 'LineString'
    std::vector<xml::Node*> &pinVec,    // should be empty
    std::vector<xml::Node*> &pinsCoorNodeVec    // should be empty
) {
    pinVec = containerNode->getDescendantsByName(coordinatesTypeName, true);
    for (auto &pin : pinVec) {
        pinsCoorNodeVec.push_back(pin->getFirstDescendantByName("coordinates"));
        pin = pin->getParent(); // to be node named 'Placemark'
    }
}

int General::putOnTopFolder_getPriorityFolderCount(
    xml::Node *containerFolder,
    std::vector<std::string> &additionalPriorityNameList  // multiple appearance
) {
    /* 'priorityFolderCount' index as a limit to top placement */
    int priorityFolderCount = 0;

    std::vector<std::string> priorityNameList {
        // single appearance
        "name", "visibility", "open",
        // multiple appearance
        "Style", "StyleMap" // if parent a document
    };

    priorityNameList.insert(
        priorityNameList.end(),
        additionalPriorityNameList.begin(),
        additionalPriorityNameList.end()
    );

    // single appearance
    for (int i = 0; i < 3; i++) {
        if (containerFolder->getFirstChildByName(priorityNameList.at(i))) {
            priorityFolderCount++;
        }
    }

    // multiple appearance
    if (containerFolder->getName() == "Document") {
        for (int i = 3; i < priorityNameList.size(); i++) {

            std::vector<xml::Node*> multiAppearanceVec = {
                containerFolder->getChildrenByName(priorityNameList.at(i))
            };

            priorityFolderCount += multiAppearanceVec.size();
        }
    }

    return priorityFolderCount;
}

void General::putOnTopFolder(
    xml::Node *containerFolder,
    xml::Node *node_in,
    std::vector<std::string> additionalPriorityNameList  // multiple appearance
) {
    int priorityFolderCount = putOnTopFolder_getPriorityFolderCount(
        containerFolder, additionalPriorityNameList
    );

    // firstly append new folder
    containerFolder->addChild(node_in);
    
    for (int i = containerFolder->getChildren()->size() - 2;
        i >= priorityFolderCount;
        i--
    ) {
        containerFolder->swapChildren(
            containerFolder->getChildren()->at(i),
            node_in
        );
    }
}

void General::putOnTopFolder(
    xml::Node *containerFolder,
    std::vector<xml::Node*> nodeVec,
    std::vector<std::string> additionalPriorityNameList  // multiple appearance
) {
    int priorityFolderCount = putOnTopFolder_getPriorityFolderCount(
        containerFolder, additionalPriorityNameList
    );

    for (auto &node : nodeVec) {
        
        // firstly append new folder
        containerFolder->addChild(node);
        
        for (int i = containerFolder->getChildren()->size() - 2;
             i >= priorityFolderCount;
             i--
        ) {
            containerFolder->swapChildren(
                containerFolder->getChildren()->at(i),
                node
            );
        }

        priorityFolderCount++;
    }
}

void General::insertEditedPlacemarksIntoFolder(
    xml::Node *existContainerNode,  // don't remove this node
    xml::Node *newContainerNode,
    std::vector<xml::Node*> &placemarks, // or any node
    const std::vector<std::string> &noticeFuncName,
    std::string typeStr
) {
    // succeeded
    if (placemarks.size() > 0) {
        putOnTopFolder(existContainerNode, newContainerNode);
        
        // insert pins into the folder
        for (auto &plmrk : placemarks) {
            plmrk->removeFromParent();
            newContainerNode->addChild(plmrk);
        }
    }

    // logging
    if (!(noticeFuncName[0] == "" && noticeFuncName[1] == "")) {
        logEditedPlacemarks(
            typeStr,
            noticeFuncName,
            placemarks,
            existContainerNode
        );
    }
}

bool General::logEditedPlacemarks(
    std::string typeStr,
    // (BELOW! set empty string to disable / using custom notification)
    std::vector<std::string> noticeFuncName, // [error, correct]
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

    // failed
    if (placemarks.size() == 0) {
        std::string taskName = noticeFuncName.at(1);

        // task name lower case
        taskName = mini_tool::changeStringCase(
            taskName,
            mini_tool::LOWER_CASE_FLAG,
            0
        );

        if (taskName != "") {
            std::cerr
                << "KML-> "
                << noticeFuncName.at(0) << " error. No "
                << typeStr << " to "
                << taskName << " inside '"
                << (containerNode ? containerNode->getName() : "unknown") << "' named '"
                << folderName << "'\n";
        }
        else {
            std::cerr
                << "KML-> " << noticeFuncName.at(0)
                << " error. Nothing to process inside '"
                << (containerNode ? containerNode->getName() : "unknown") << "' named '"
                << folderName << "'\n";
        }

        return false;
    }
    // succeeded
    else {
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
void General::setKMLDocumentName(xml::Node *kmlNode, std::string fileDir_out) {
    xml::Node *mainFolder = searchMainFolder(kmlNode);

    while (mainFolder) {

        xml::Node *nameNode = mainFolder->getFirstChildByName("name");
        if (!nameNode) {
            nameNode = new xml::Node("name", mainFolder);
            putOnTopFolder(mainFolder, nameNode);
        }
        nameNode->setInnerText(mini_tool::cutFileDirName(fileDir_out));

        xml::Node *possibleParentNode = mainFolder->getParent();
        if (possibleParentNode &&
            possibleParentNode->getName() != "kml"
        ) {
            mainFolder = possibleParentNode;
        }
        else break;
    }
}

// folders expected to be empty and all items has released and moved to 'newFolderVec'
void General::cleanFolders(
    xml::Node *mainFolder,
    xml::Node *classifiedFolder,
    std::vector<xml::Node*> &newFolderVec
) {
    // get main folder 'name', 'description' and 'open' node //

    xml::Node
        *mainNameNode = mainFolder->getFirstChildByName("name"),
        *mainDescNode = mainFolder->getFirstChildByName("description"),
        *mainOpenNode = mainFolder->getFirstChildByName("open");

    std::vector<xml::Node*> mainInfoNodes;
    if (mainNameNode) mainInfoNodes.push_back(mainNameNode);
    if (mainDescNode) mainInfoNodes.push_back(mainDescNode);
    if (mainOpenNode) mainInfoNodes.push_back(mainOpenNode);

    // release nodes from parent
    for (auto &node : mainInfoNodes) {
        node->removeFromParent();
    }

    // 'StyleMap' and 'Style' node
    std::vector<xml::Node*> stylingNodeArr[2] = {
        mainFolder->getDescendantsByName("StyleMap"),
        mainFolder->getDescendantsByName("Style")
    };

    // style nodes switching parent (lifted up as document children)
    for (int i = 0; i < 2; i++) {
        for (auto &node : stylingNodeArr[i]) {
            node->removeFromParent();
        }
        putOnTopFolder(mainFolder->getParent(), stylingNodeArr[i]);
    }

    // release memory of the rest nodes
    for (auto &node : *mainFolder->getChildren()) {
        if (node) delete node;
    }

    mainFolder->getChildren()->clear();
    mainFolder->addChildren(mainInfoNodes);

    // set 'newFolderVec' nodes as children of classifiedFolder
    for (auto &folderNode : newFolderVec) {
        classifiedFolder->addChild(folderNode);
    }

    mainFolder->addChild(classifiedFolder);
}

#endif // __KML_GENERAL_CPP__