#ifndef __KML_TWINS_CHECKER_CPP__
#define __KML_TWINS_CHECKER_CPP__

#include "twins-checker.h"

/* NOTES:
*   -after process commands COMING SOON!
*   -the radius measurement is deprecated (not using 'haversine' formula)
*/

// return working folder node
xml::Node *TwinsChecker::findPins(
    xml::Node *kmlNode,
    std::string meterRadiusRateString,
    bool isParentFolderNamedAType
) {
    double meterRadius = getLimitedMeterRadius(meterRadiusRateString);
    std::vector<xml::Node*> nodes;
    std::vector<Point> pointVec;

    for (auto &node : kmlNode->getDescendantsByName("Placemark", true)) {
        xml::Node *pointNode = node->getFirstDescendantByName("Point");
        
        if (pointNode) {
            nodes.push_back(node);

            // get points as degree of decimal
            pointVec.push_back(Point(
                pointNode->getFirstDescendantByName("coordinates")->getInnerText()
            ));

            Placemark kmlPlacemark;

            // SPECIAL! Convert point as 'degree' to point as 'meter' //

            std::vector<Point>
                ptHor_vec = { // X
                    Point(pointVec.back().x, 0),
                    Point(0, 0)
                },
                ptVer_vec = { // Y
                    Point(0, pointVec.back().y),
                    Point(0, 0)
                };

            pointVec.back() = Point(
                kmlPlacemark.getPathDistance(ptHor_vec),
                kmlPlacemark.getPathDistance(ptVer_vec)
            );
        }
    }

    // CHECK WHETHER INSIDE 'pointB' //

    int ctrA = 0, ctrB = 0;
    std::vector<int> matchedIndexes[2]; // {originals, duplicates}
    std::vector<Point> pointVec_buffer = pointVec;

    for (auto &pointA : pointVec) {
        bool isMatched = false;
        
        for (auto &pointB : pointVec_buffer) {
            if (ctrA != ctrB &&
                pointA.x >= pointB.x - meterRadius &&
                pointA.x <= pointB.x + meterRadius &&
                pointA.y >= pointB.y - meterRadius &&
                pointA.y <= pointB.y + meterRadius
            ) {
                isMatched = true;
                matchedIndexes[1].push_back(ctrB);
            }
            ctrB++;
        }

        if (isMatched) matchedIndexes[0].push_back(ctrA);
        pointVec_buffer.at(ctrA) = Point(999999999, 999999999);
        ctrA++;
        ctrB = 0;
    }

    return insertFoundPlacemarks(
        kmlNode,
        matchedIndexes,
        nodes,
        "pin",
        isParentFolderNamedAType
    );
}

xml::Node *TwinsChecker::findPaths(
    xml::Node *kmlNode,
    std::string meterRadiusRateString,
    bool isParentFolderNamedAType
) {
    double meterRadius = getLimitedMeterRadius(meterRadiusRateString);
    std::vector<xml::Node*> nodes;

    // may contains empty point vector
    std::vector<std::vector<Point>> pointVecVec;

    for (auto &placemark : kmlNode->getDescendantsByName("Placemark", true)) {
        
        xml::Node *lineString = placemark->getFirstDescendantByName("LineString");
        
        if (lineString) {

            nodes.push_back(placemark);
            xml::Node *coorNode = lineString->getFirstDescendantByName("coordinates");

            // get vector points as degree of decimal
            pointVecVec.push_back(Point::getPathPointsFromString(
                coorNode ? coorNode->getInnerText() : ""
            ));

            Placemark kmlPlacemark;
            std::vector<Point> pointVec_buffer;

            for (auto &point : pointVecVec.back()) {

                /* '0,0' point in near central Africa */

                std::vector<Point> 
                ptHor_vec = std::vector<Point> { // X
                    Point(point.x, 0),
                    Point(0, 0)
                },
                ptVer_vec = std::vector<Point> { // Y
                    Point(0, point.y),
                    Point(0, 0)
                };

                // convert 'degree' point to 'meter' point
                pointVec_buffer.push_back(Point(
                    kmlPlacemark.getPathDistance(ptHor_vec),
                    kmlPlacemark.getPathDistance(ptVer_vec)
                ));
            }

            pointVecVec.back() = pointVec_buffer;
        }
    }

    // CHECK WHETHER INSIDE VECTOR OF 'pointB' //

    int ctrA = 0, ctrB = 0;
    std::vector<int> matchedIndexes[2]; // {originals, duplicates}
    std::vector<std::vector<Point>> pointVecVec_buffer = pointVecVec;
    
    for (auto &pointVec_A : pointVecVec) {
        
        bool isMatched = false;
        int whoIsOriginal; // 'ctrA' or 'ctrB'

        for (auto &pointVec_B : pointVecVec_buffer) {
            if (ctrA != ctrB && pointVec_B.size() > 0) {

                std::vector<Point> *longestPointVec = (
                    pointVec_A.size() >= pointVec_B.size() ?
                    &pointVec_A : &pointVec_B
                );
                
                int equalCount = 0;

                for (int i = 0; i < longestPointVec->size(); i++) {

                    if (i == pointVec_A.size() - 1 ||
                        i == pointVec_B.size() - 1
                    ) {
                        if (equalCount == pointVec_A.size() - 1 ||
                            equalCount == pointVec_B.size() - 1
                        ) {
                            isMatched = true;

                            if (pointVec_A.size() < pointVec_B.size()) {
                                matchedIndexes[1].push_back(ctrA);
                                whoIsOriginal = ctrB;
                            }
                            else { // normal
                                matchedIndexes[1].push_back(ctrB);
                                whoIsOriginal = ctrA;
                            }
                        }

                        break;
                    }

                    if (pointVec_A.at(i).x >= pointVec_B.at(i).x - meterRadius &&
                        pointVec_A.at(i).x <= pointVec_B.at(i).x + meterRadius &&
                        pointVec_A.at(i).y >= pointVec_B.at(i).y - meterRadius &&
                        pointVec_A.at(i).y <= pointVec_B.at(i).y + meterRadius
                    ) {
                        equalCount++;
                    }
                }
            }

            if (isMatched) break;
            ctrB++;
        }

        if (isMatched) matchedIndexes[0].push_back(whoIsOriginal);
        pointVecVec_buffer.at(whoIsOriginal) = std::vector<Point>{};
        ctrA++;
        ctrB = 0;
    }

    return insertFoundPlacemarks(
        kmlNode,
        matchedIndexes,
        nodes,
        "path",
        isParentFolderNamedAType
    );
}

xml::Node *TwinsChecker::findAll(
    xml::Node *kmlNode,
    std::string meterRadiusRateString
) {
    xml::Node *pinsFolder = findPins(kmlNode, meterRadiusRateString, true);
    xml::Node *pathsFolder = findPaths(kmlNode, meterRadiusRateString, true);

    xml::Node *workingFolder = Builder().getFolder(TWINS_CHECK_COMMAND_WORKING_FOLDER);

    if (pinsFolder) workingFolder->addChild(pinsFolder);
    if (pathsFolder) workingFolder->addChild(pathsFolder);

    // connect to 'kmlNode'
    if (workingFolder->getChildren()->size() > 0) {
        General kmlGeneral;
        kmlGeneral.putOnTopFolder(
            kmlGeneral.getRootDocument(kmlNode),
            {workingFolder}
        );
        return workingFolder;
    }
    else {
        delete workingFolder;
        return nullptr;
    }
}

xml::Node *TwinsChecker::insertFoundPlacemarks(
    xml::Node *kmlNode,
    std::vector<int> *matchedIndexes, // int vector array (has 2 members)
    std::vector<xml::Node*> &placemarkNodes,
    std::string placemarksType,
    bool isParentFolderNamedAType
) {
    General kmlGeneral;
    std::string kmlNameString = kmlGeneral.getRootDocumentName(kmlNode);

    if (matchedIndexes[0].size() > 0) {
        Builder kmlBuilder;

        xml::Node *folder = kmlBuilder.getFolder(
            isParentFolderNamedAType ?
            mini_tool::changeStringCase(placemarksType, mini_tool::UPPER_CASE_FLAG) :
            TWINS_CHECK_COMMAND_WORKING_FOLDER
        );

        xml::Node *folderOri = kmlBuilder.getFolder(std::string("ORIGINALS"));
        folder->addChild(folderOri);

        xml::Node *folderDupl = kmlBuilder.getFolder(std::string("DUPLICATES"));
        folder->addChild(folderDupl);

        int matchedCtr = 0;

        for (int i = 0; i < 2; i++) {
            for (auto &index : matchedIndexes[i]) {

                placemarkNodes.at(index)->removeFromParent();

                if (i == 0) { // originals
                    folderOri->addChild(placemarkNodes.at(index));
                    matchedCtr++;
                }
                // duplicates
                else folderDupl->addChild(placemarkNodes.at(index)); 
            }
        }

        std::cout
            << "----------------\n"
            << "Total matched:  " << matchedCtr
            << (isParentFolderNamedAType ?
                std::string("  (") + placemarksType + std::string(")") :
                "")
            << std::endl
            << "----------------\n"
            << "KML-> Twins check for '" << placemarksType
            << "' inside '"<< kmlNameString << "' document completed!\n";

        if (isParentFolderNamedAType) return folder;
        else {
            // connect to 'kmlNode'
            xml::Node *rootDoc = kmlGeneral.getRootDocument(kmlNode);
            kmlGeneral.putOnTopFolder(rootDoc, {folder});
            return folder;
        }
    }
    else {
        std::cerr
            << "KML-> Twins checking warning. No '" << placemarksType << "' twins inside '"
            << kmlNameString << "' document\n";
        return nullptr;
    }
}

/* limited from 0 to 100 meters */
double TwinsChecker::getLimitedMeterRadius(std::string meterRadiusRateString) {

    double meterRadius = mini_tool::filterStringDecimal(meterRadiusRateString);
    meterRadius = std::abs(meterRadius);

    if (meterRadius > 100.0) {
        std::cerr << "KML-> Twins checking warning. The given radius exceeds 100 meters.\n"
                  << "      Radius is set to limit.\n";
        meterRadius = 100.0;
    }

    return meterRadius;
}

#endif // __KML_TWINS_CHECKER_CPP__