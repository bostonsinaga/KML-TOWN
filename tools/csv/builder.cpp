#ifndef __TXT_BUILDER_CPP__
#define __TXT_BUILDER_CPP__

#include "builder.h"

/* NOTE:
*  the columns are the first list of folders from main folder
*  the folders inside each of them are ignored
*  (directly to the placemarks)
*/

bool Builder::compose(
    std::string fileDir_out,
    xml::Node *mainFolderNode,
    std::string separatorSign
) {
    if (!mainFolderNode) return false;

    std::cout << "CSV-> Composing '" + fileDir_out + "'...\n";

    int CTR = 0;
    std::vector<xml::Node*>
        columnNodes,
        outsiderNodes;

    // filter subfolders and outsider placemarks (create the columns)
    for (auto &node : *mainFolderNode->getChildren()) {
        if (node->getName() == "Document") {
            // folders of document
            std::vector<xml::Node*> innerDocFolVec = node->getChildrenByName("Folder");
            columnNodes.insert(
                columnNodes.end(),
                innerDocFolVec.begin(),
                innerDocFolVec.end()
            );

            // documents of document
            std::vector<xml::Node*> innerDocDocVec = node->getChildrenByName("Document");
            outsiderNodes.insert(
                outsiderNodes.end(),
                innerDocDocVec.begin(),
                innerDocDocVec.end()
            );
        }
        else if (node->getName() == "Folder") {
            columnNodes.push_back(node);
        }
        else if (node->getName() == "Placemark") {
            outsiderNodes.push_back(node);
        }
    }

    // initialize with nullptr (the size equals to 'columnNode')
    std::vector<std::vector<xml::Node*>> rowNodesVector(
        columnNodes.size(),
        std::vector<xml::Node*>()
    );
    
    bool isHasPlacemarks = false;

    // pull all folders placemarks
    for (auto &node : columnNodes) {
        rowNodesVector.at(CTR) = pullRowNodes(node);

        if (!isHasPlacemarks) {
            if (rowNodesVector.at(CTR).size() > 0) {
                isHasPlacemarks =  true;
            }
        }

        CTR++;
    }

    /*
    *   placemarks outside of folders
    *   (directly to the main folder)
    *   placed in latest column
    */

    if (outsiderNodes.size() > 0) {
        isHasPlacemarks = true;
        rowNodesVector.push_back(outsiderNodes);

        kml::Builder kmlBuilder;
        columnNodes.push_back(kmlBuilder.createFolder("Others"));
    }

    if (columnNodes.size() == 0) {
        std::cerr << "CSV-> Builder error. Empty document\n";
        return false;
    }
    else if (!isHasPlacemarks) {
        std::cerr << "CSV-> Builder warning. No 'Placemarks' found\n";
    }

    ///////////
    // WRITE //
    ///////////

    /* FORMAT:
    *  |             TITLE               |  -> PINS
    *  | NAME | DESCRIPTION | COORDINATE |
    *  ----------------------------------------------------------
    *  |                          TITLE                         |  -> PATHS
    *  | NAME | DESCRIPTION | START COORDINATE | END COORDINATE |
    */

    // the size equals to 'rowNodesVector' first dimension
    std::vector<int> subColumnsCounts;

    std::ofstream writeFile(fileDir_out);

    /* name is use as order names (row list) as well */
    std::vector<std::string> colNodeNames;

    CTR = 0;
    for (auto &node : columnNodes) {

        // get folder name
        xml::Node *nameNode = node->getFirstChildByName("name");
        if (nameNode) {
            colNodeNames.push_back(nameNode->getInnerText());
        }
        else colNodeNames.push_back("PLACEMARKS");

        // write column title
        writeFile << colNodeNames.back();

        // classify column type //

        // using paths column
        if (mini_tool::isStringContains(colNodeNames.back(), "PATH", true)  ||
            mini_tool::isStringContains(colNodeNames.back(), "PATHS", true) ||
            mini_tool::isStringContains(colNodeNames.back(), "JALUR", true)
        ) {
            subColumnsCounts.push_back(5);
        }
        // using pins column
        else subColumnsCounts.push_back(3);        

        // space from sub columns
        for (int i = 0; i < subColumnsCounts.back() - 1; i++) {
            writeFile << separatorSign;
        }

        // barrier
        if (CTR != columnNodes.size() - 1) {
            writeFile << separatorSign;
            writeFile << separatorSign;
        }
    }

    // column titles end
    writeFile << '\n';

    // sub columns
    CTR = 0;
    for (auto &subCt : subColumnsCounts) {

        if (subCt == 5) { // path
            writeFile << "Name";
            writeFile << separatorSign;
            writeFile << "Description";
            writeFile << separatorSign;
            writeFile << "Distance";
            writeFile << separatorSign;
            writeFile << "Start Coordinate";
            writeFile << separatorSign;
            writeFile << "End Coordinate";
        }
        else { // 3 sub folder as pins
            writeFile << "Name";
            writeFile << separatorSign;
            writeFile << "Description";
            writeFile << separatorSign;
            writeFile << "Coordinate";
        }

        if (CTR != subColumnsCounts.size() - 1) {
            writeFile << separatorSign;
            writeFile << separatorSign;
        }

        CTR++;
    }

    // sub column titles end
    writeFile << '\n';

    /* note: 'rowNodesVector' size equals to 'columnNodes' size */

    int maxRowsCount;
    std::vector<int> rowsCounts;

    /* sort and get the most row nodes count as 'maxRowsCount' */

    for (auto &rowNodes : rowNodesVector) {
        rowsCounts.push_back(rowNodes.size());
    }

    std::sort(rowsCounts.begin(), rowsCounts.end());
    maxRowsCount = rowsCounts.back();

    /* compose all the data (name, description and coordinate) */

    /**************/
    /* MINI FUNCS */
    /**************/

    auto getFrontCoordinate = [=](std::string coorStr_lmd)->std::string {
        int commaCount = 0;
        std::string retStr;

        for (auto &ch : coorStr_lmd) {
            if (ch == ',') {
                commaCount++;
            }

            retStr.push_back(ch);

            if (commaCount == 2 && mini_tool::isANumber(ch)) {

                // set default altitude to '0' (conservation)
                if (ch != '0') {
                    retStr.pop_back();
                    retStr.push_back('0');
                }

                return retStr;
            }
        }

        return "";
    };

    auto getBackCoordinate = [=](std::string coorStr_lmd)->std::string {
        int commaCount = 0;
        std::string retStr;

        for (int i = coorStr_lmd.size() - 1; i >= 0; i--) {
            if (coorStr_lmd.at(i) == ',') {
                commaCount++;
            }

            retStr.push_back(coorStr_lmd.at(i));

            if (i == 0 ||
                (commaCount == 2 && coorStr_lmd.at(i-1) == ' ')
            ) {
                // reverse the string against the loop direction
                return mini_tool::reverseString(retStr);
            }
        }

        return "";
    };

    std::function<void(std::string&)> convertIntoDegreeCoordinate = [=](std::string &coorStr_lmd) {
        kml::Converter kmlConverter;

        for (int i = coorStr_lmd.size() - 1; i >= 0; i--) {
            if (coorStr_lmd.at(i) == ',') {
                coorStr_lmd = coorStr_lmd.substr(0, i);
                break;
            }
        }

        // separate decimal string coordinate
        std::vector<std::string> decimalAxisStrVec = (
            kmlConverter.separateCoordinate(coorStr_lmd)
        );

        // convert decimal (in string form) to degree coordinate
        std::vector<std::string> axisDegreeStrVec = kmlConverter.convertCoor_decimalDegree(
            decimalAxisStrVec,
            kmlConverter.LNG_LAT_SEPARATE_FLAG_IN,
            kmlConverter.LAT_LNG_SEPARATE_FLAG_OUT
        );

        // stringify degree coordinate vector
        coorStr_lmd = axisDegreeStrVec.at(0);
        coorStr_lmd += " " + axisDegreeStrVec.at(1);
    };

    /* MINI FUNCS END -------------------- */

    CTR = 0;
    for (int i = 0; i < maxRowsCount; i++) {
        for (auto &rowNodes : rowNodesVector) { // follows the column count

            if (rowNodes.size() > 0 && i <= rowNodes.size() - 1) {

                std::function<void()> printNoCoorWarningMessage = [=]() {
                    std::cerr << "CSV-> Builder warning. No coordinate value found in 'Placemarks'\n";
                };

                // coordinate //

                xml::Node *coorNode = rowNodes.at(i)->getFirstDescendantByName("coordinates");
                std::string coorStr = "";

                if (coorNode) {
                    coorStr = coorNode->getInnerText();
                    if (coorStr == "") printNoCoorWarningMessage();
                }
                else printNoCoorWarningMessage();

                // name and description //

                xml::Node
                    *rowNameNode = rowNodes.at(i)->getFirstChildByName("name"),
                    *rowDescNode = rowNodes.at(i)->getFirstChildByName("description");

                std::string rowNameStr, rowDescStr = "";

                if (rowNameNode) {
                    rowNameStr = rowNameNode->getInnerText();
                }
                else {
                    if (colNodeNames.at(CTR) == "PLACEMARKS") {
                        rowNameStr = "Placemark " + std::to_string(i+1);
                    }
                    else {
                        rowNameStr = colNodeNames.at(CTR) + " " + std::to_string(i+1);
                    }
                }

                if (rowDescNode) {
                    rowDescStr = rowDescNode->getInnerText();
                }

                // handle multi lines row covered with double quotes sign ("") //

                // name
                if (rowNameStr.find('\n') != std::string::npos) {
                    rowNameStr.insert(rowNameStr.begin(), '"');
                    rowNameStr.push_back('"');
                }

                // description
                if (rowDescStr.find('\n') != std::string::npos) {
                    rowDescStr.insert(rowDescStr.begin(), '"');
                    rowDescStr.push_back('"');
                }

                // WRITE THE ROWS //

                // PATHS
                if (subColumnsCounts.at(CTR) == 5) {
                    std::string frontCoorStr = getFrontCoordinate(coorStr);
                    std::string backCoorStr = getBackCoordinate(coorStr);

                    convertIntoDegreeCoordinate(frontCoorStr);
                    convertIntoDegreeCoordinate(backCoorStr);

                    std::vector<kml::Point> points = kml::Point().getPathPointsFromString(coorStr);

                    std::string distanceStr = (
                        std::to_string(kml::Placemark().getPathDistance(points))
                    );

                    writeFile << rowNameStr;
                    writeFile << separatorSign;
                    writeFile << rowDescStr;
                    writeFile << separatorSign;
                    writeFile << distanceStr;
                    writeFile << separatorSign;
                    writeFile << frontCoorStr;
                    writeFile << separatorSign;
                    writeFile << backCoorStr;
                }
                // 3 sub folder as PINS
                else {
                    std::string frontCoorStr = getFrontCoordinate(coorStr);
                    convertIntoDegreeCoordinate(frontCoorStr);

                    writeFile << rowNameStr;
                    writeFile << separatorSign;
                    writeFile << rowDescStr;
                    writeFile << separatorSign;
                    writeFile << frontCoorStr;
                }
            }
            else { // keep the blanks
                int blankSeparatorCount;
                
                if (subColumnsCounts.at(CTR) == 5) {
                    blankSeparatorCount = 4;
                }
                else blankSeparatorCount = 2;

                for (int j = 0; j < blankSeparatorCount; j++) {
                    writeFile << separatorSign;
                }
            }

            if (CTR != rowNodesVector.size() - 1) {
                writeFile << separatorSign;
                writeFile << separatorSign;
            }

            CTR++;
        }

        writeFile << '\n';
        CTR = 0;
    }

    writeFile.close();
    std::cout << "CSV-> Compose into '" << fileDir_out << "' completed!\n";
    return true;
}

std::vector<xml::Node*> Builder::pullRowNodes(xml::Node *parent) {
    std::vector<xml::Node*> retNodes;
    
    for (auto &child : *parent->getChildren()) {
        if (
            child->getName() == "Folder" ||
            child->getName() == "Document"
        ) {
            std::vector<xml::Node*> pullNodes = pullRowNodes(child);
            retNodes.insert(retNodes.end(), pullNodes.begin(), pullNodes.end());
        }
        else if (child->getName() == "Placemark") {
            retNodes.push_back(child);
        }
    }

    return retNodes;
}

#endif // __TXT_BUILDER_CPP__