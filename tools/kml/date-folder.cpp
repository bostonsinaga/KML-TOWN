#ifndef __KML_DATE_FOLDER_CPP__
#define __KML_DATE_FOLDER_CPP__

#include "date-folder.h"

/* called as 'folder-by-date' */

/* for now the pattern is 'dd/mm/yyyy' */

void DateFolder::packNumeral(xml::Node *kmlNode) {

    int CTR = 0;
    General kmlGeneral;
    std::vector<std::vector<int>> dateVector;
    xml::Node *mainFolderNode = kmlGeneral.searchMainFolder(kmlNode);

    std::vector<xml::Node*> placemarkNodes = (
        mainFolderNode->getDescendantsByName("Placemark", true)
    );

    // search dates //

    for (auto &node : placemarkNodes) {

        xml::Node
            *nameNode = node->getFirstDescendantByName("name"),
            *descNode = node->getFirstDescendantByName("description");

        bool isContinue = true;
        std::string testStr;
        std::vector<int> dmyInt;

        // in name
        if (nameNode) {
            testStr = nameNode->getInnerText();
            dmyInt = testNumeralDate(testStr);

            if (dmyInt.size() > 0) {
                dateVector.push_back(dmyInt);
                isContinue = false;
            }
        }
        
        // in description
        if (isContinue && descNode) {
            testStr = descNode->getInnerText();
            dmyInt = testNumeralDate(testStr);

            if (dmyInt.size() > 0) {
                dateVector.push_back(dmyInt);
                isContinue = false;
            }
        }

        // not found
        if (isContinue) {
            dateVector.push_back(std::vector<int>{31, 12, 9999});
        }
    }

    // sort dates //

    std::vector<int> dateRates;

    // assumed the format is 'dd/mm/yyyy'
    for (auto &dmyInt : dateVector) { // convert to total days count
        dateRates.push_back(
            dmyInt.at(0) +
            convertMonthInDaysCount(dmyInt.at(1)) +
            dmyInt.at(2) * 365 + dmyInt.at(2) / 4 // regard to leap years
        );
    }

    // smaller-bigger (descending) bubble sort
    CTR = 0;
    for (int i = 0; i < dateRates.size(); i++) {
        for (int j = 1; j < dateRates.size(); j++) {
            if (dateRates.at(CTR) <= dateRates.at(j)) {
                CTR = j;
            }
            else { // swapping
                int bufferRate = dateRates.at(CTR);
                dateRates.at(CTR) = dateRates.at(j);
                dateRates.at(j) = bufferRate;

                std::vector<int> bufferDmyInt = dateVector.at(CTR);
                dateVector.at(CTR) = dateVector.at(j);
                dateVector.at(j) = bufferDmyInt;

                xml::Node *bufferNode = placemarkNodes.at(CTR);
                placemarkNodes.at(CTR) = placemarkNodes.at(j);
                placemarkNodes.at(j) = bufferNode;
            }
        }
        CTR = 0;
    }

    // create date folders //

    std::vector<xml::Node*> newFolderNodes;
    std::vector<std::string> dateStrVec;

    int bufferItemsCount = 0;

    auto setFolderNameWithCount = [=](xml::Node *node, int count) {
        xml::Node *nameNode = node->getFirstDescendantByName("name");
        if (nameNode) {
            nameNode->setInnerText(
                nameNode->getInnerText() +
                std::string(" (") +
                std::to_string(count) +
                std::string(" pcs)")
            );
        }
    };

    CTR = 0;
    for (auto &dmyInt : dateVector) {

        std::string dayMonthZeroPrefix[2] = {
            std::log10(dmyInt.at(0)) < 1 ? "0" : "",
            std::log10(dmyInt.at(1)) < 1 ? "0" : ""
        };

        std::string dateStr = (
            dayMonthZeroPrefix[0] +
            std::to_string(dmyInt.at(0)) + std::string("/") +
            dayMonthZeroPrefix[1] +
            std::to_string(dmyInt.at(1)) + std::string("/") +
            std::to_string(dmyInt.at(2))
        );

        if (dateStr == "31/12/9999") {
            dateStr = "NO DATE";
        }

        int insideDex = mini_tool::isPrimitiveInsideVector<std::string>(dateStrVec, dateStr);

        if (insideDex == -1) {
            if (newFolderNodes.size() > 0) {
                setFolderNameWithCount(newFolderNodes.back(), bufferItemsCount);
            }

            dateStrVec.push_back(dateStr);
            newFolderNodes.push_back(Builder().createFolder(dateStr, false));
            insideDex = dateStrVec.size() - 1;
            bufferItemsCount = 0;
        }

        placemarkNodes.at(CTR)->removeFromParent();
        newFolderNodes.at(insideDex)->addChild(placemarkNodes.at(CTR));
        bufferItemsCount++;
        CTR++;
    }

    setFolderNameWithCount(newFolderNodes.back(), bufferItemsCount);

    // finishing //

    kmlGeneral.insertEditedPlacemarksIntoFolder(
        mainFolderNode,
        Builder().createFolder(FOLDER_BY_DATE_COMMAND_WORKING_FOLDER),
        newFolderNodes,
        {"Date foldering", "Folder by date"},
        "placemark"
    );
}

std::vector<int> DateFolder::testNumeralDate(std::string testStr) {

    std::vector<std::string> dmyStr(3, "");
    int slashCount = 0;

    for (auto &ch : testStr) {
        if (mini_tool::isANumber(ch)) {
            dmyStr.at(slashCount).push_back(ch);
        }
        else if (ch == '/') {
            slashCount++;
        }
        else if (slashCount == 2 && !mini_tool::isANumber(ch)) {
            break;
        }
        else {
            dmyStr = std::vector<std::string>(3, "");
            slashCount = 0;
        }
    }

    std::vector<int> retDmyInt;
    int blankCount = 0;

    for (auto &str : dmyStr) {
        if (str == "") blankCount++;
        else {
            retDmyInt.push_back(
                int(mini_tool::filterStringDecimal(str))
            );
        }
    }

    if (blankCount > 0) {
        return std::vector<int>{};
    }

    // handle year
    if (int(std::log10(retDmyInt.back())) == 1) {
        retDmyInt.back() += 2000;
    }

    return retDmyInt;
}

int DateFolder::convertMonthInDaysCount(int month) {
    int retCount = 0;
    
    int daysCountArr[12] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    for (int i = 0; i < month; i++) {
        retCount += daysCountArr[i];
    }

    return retCount;
}

#endif // __KML_DATE_FOLDER_CPP__