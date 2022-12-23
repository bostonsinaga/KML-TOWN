#ifndef __KML_DATE_FOLDER_CPP__
#define __KML_DATE_FOLDER_CPP__

#include "date-folder.h"

/* called as 'folder-by-date' */

xml::Node *DateFolder::pack(xml::Node *kmlNode) {
    /* placemarks separated by type */

    std::vector<xml::Node*> pinNodes, pathNodes;

    for (int i = 0; i < 2; i++) {
        std::string typeIdentifierStr;
        if (i == 0) typeIdentifierStr = "Point";
        else typeIdentifierStr = "LineString";

        for (auto &node : kmlNode->getDescendantsByName("Placemark", true)) {
            xml::Node *typeIdentifierNode = (
                node->getFirstDescendantByName(typeIdentifierStr)
            );

            if (typeIdentifierNode) {
                if (i == 0) pinNodes.push_back(node);
                else pathNodes.push_back(node);
            }
        }
    }

    for (auto &node : pinNodes) {
        bool isContinue = false;
        std::string dateStr = "";

        xml::Node *nameNode = node->getFirstChildByName("name");        
        if (nameNode) {
            dateStr = testDateTemplate(nameNode->getInnerText());
            if (dateStr == "") isContinue = true;
        }
        else isContinue = true;

        if (isContinue) {
            xml::Node *descNode = node->getFirstChildByName("description");
            if (descNode) {
                dateStr = testDateTemplate(descNode->getInnerText());
            }
        }

        /* select forward or reverse numeral date format*/
        if (dateStr != "") {
            if (mini_tool::getInStringCharCount(dateStr, '/') == 2) {
                int dateInTrio[3];
                size_t found[2];

                found[0] = dateStr.find('/');
                dateInTrio[0] = std::stoi(dateStr.substr(0, found[0]));
                found[1] = dateStr.find('/', found[0] + 1);
                dateInTrio[1] = std::stoi(dateStr.substr(found[0] + 1, found[1]));
                dateInTrio[2] = std::stoi(dateStr.substr(found[1] + 1));

                if (dateInTrio[0] > 366) { // (yyyy/mm/dd)
                    // coming soon!!
                }
                else if (dateInTrio[0] <= 366) { // (dd/mm/yyyy)
                    // coming soon!!
                }
            }
        }

        /* EXPECTED RESULT:
        *   -2022
        *       -November
        *           -14th               --> has more than one
        *               -[placemarks] 
        *           -15th-21st          --> only has one each interval days
        *               -[placemarks]
        */
    }

    return nullptr;
}

/* only numeral template available yet! */
std::string DateFolder::testDateTemplate(std::string strIn) {
    std::string retStr = "";

    // numeral template (2022/11/14) //

    int slashCtr = 0;
    for (auto &ch : strIn) {
        if (mini_tool::isANumber(ch)) {
            retStr += ch;
        }
        else if (ch == '/') {
            retStr += ch;
            slashCtr++;
        }
        else if (slashCtr == 2) {
            break;
        }
        else {
            retStr = "";
            slashCtr = 0;
        }
    }

    // name template coming soon!! //

    return retStr;
}

#endif // __KML_DATE_FOLDER_CPP__