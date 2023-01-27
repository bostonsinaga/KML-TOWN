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
    std::string meterRadiusRateString
) {
    double meterRadius;

    /* limited from 0 to 100 meters */
    meterRadius = mini_tool::filterStringDecimal(meterRadiusRateString);
    meterRadius = std::abs(meterRadius);

    std::vector<xml::Node*> pinNodes;
    std::vector<Point> pinPoints;

    for (auto &node : kmlNode->getDescendantsByName("Placemark", true)) {
        xml::Node *pointNode = node->getFirstDescendantByName("Point");
        
        if (pointNode) {
            pinNodes.push_back(node);

            // get points as degree of decimal
            pinPoints.push_back(Point(
                pointNode->getFirstDescendantByName("coordinates")->getInnerText()
            ));

            Placemark kmlPlacemark;

            // SPECIAL! Convert point as 'degree' to point as 'meter' //

            std::vector<Point>
                ptHor_vec = { // X
                    Point(pinPoints.back().x, 0),
                    Point(0, 0)
                },
                ptVer_vec = { // Y
                    Point(0, pinPoints.back().y),
                    Point(0, 0)
                };

            pinPoints.back() = Point(
                kmlPlacemark.getPathDistance(ptHor_vec),
                kmlPlacemark.getPathDistance(ptVer_vec)
            );
        }
    }

    // CHECK WHETHER INSIDE 'pointB' //

    int ctrA = 0, ctrB = 0;
    std::vector<int> matchedIndexes[2]; // {originals, duplicates}
    std::vector<Point> pinPointsBuffer = pinPoints;

    for (auto &pointA : pinPoints) {
        bool isMatched = false;
        
        for (auto &pointB : pinPointsBuffer) {
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
        pinPointsBuffer.at(ctrA) = Point(999999999, 999999999);
        ctrA++;
        ctrB = 0;
    }

    return insertFoundPlacemarks(kmlNode, matchedIndexes, pinNodes, "pins");
}

xml::Node *TwinsChecker::findPaths(
    xml::Node *kmlNode,
    std::string meterRadiusRateString
) {
    std::cout << "KML-> Twins check for 'paths' is coming soon!!\n";
    return nullptr;
}

xml::Node *TwinsChecker::insertFoundPlacemarks(
    xml::Node *kmlNode,
    std::vector<int> *matchedIndexes, // int vector array (has 2 members)
    std::vector<xml::Node*> &placemarkNodes,
    std::string placemarksType
) {
    General kmlGeneral;
    std::string kmlNameString = kmlGeneral.getRootDocumentName(kmlNode);

    if (matchedIndexes[0].size() > 0) {
        Builder kmlBuilder;

        xml::Node *folder = kmlBuilder.getFolder(TWINS_CHECK_COMMAND_WORKING_FOLDER);
        kmlGeneral.getRootDocument(kmlNode)->addChild(folder);

        xml::Node *folderOri = kmlBuilder.getFolder(
            COMMAND_WORKING_FOLDER + std::string("ORIGINALS")
        );
        folder->addChild(folderOri);

        xml::Node *folderDupl = kmlBuilder.getFolder(
            COMMAND_WORKING_FOLDER + std::string("DUPLICATES")
        );
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
            << "Total matched:  " << matchedCtr << "\n"
            << "----------------\n"
            << "KML-> Twins check for '" << placemarksType
            << "' inside '"<< kmlNameString << "' document completed!\n";
        return folder;
    }
    else {
        std::cerr
            << "KML-> Twins checking warning. No '" << placemarksType << "' twins inside '"
            << kmlNameString << "' document\n";
        return nullptr;
    }
}

#endif // __KML_TWINS_CHECKER_CPP__