#ifndef __KML_TWINS_CHECKER_CPP__
#define __KML_TWINS_CHECKER_CPP__

#include "twins-checker.h"

/* after process commands COMING SOON!*/

// return working folder node
xml::Node *TwinsChecker::findPins(
    xml::Node *kmlNode,
    std::string meterRadiusRateString
) {
    double meterRadius;

    /* limited from 0 to 100 meters */
    meterRadius = mini_tool::filterStringDecimal(meterRadiusRateString);
    if (meterRadius < 0.0) meterRadius = 0.0;
    else if (meterRadius > 100.0) meterRadius = 100.0;

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

            // convert points to meters distance
            /*
            *   'pinPoints' should not exceed degree limit
            *   ~EQUATOR_DISTANCE is -180° to 180°
            *   ~MERIDIAN_DISTANCE is -90° to 90°
            */
            pinPoints.back() = Point(
                pinPoints.back().x / 360 * EQUATOR_DISTANCE * 1000,
                pinPoints.back().y / 180 * MERIDIAN_DISTANCE * 1000
            );
        }
    }

    // CHECK WHETHER INSIDE 'pointB' //

    int ctrA = 0, ctrB = 0;
    std::vector<int> matchedIndexes[2]; // {originals, duplicates}
    std::vector<Point> pinPointsBuffer = pinPoints;

    for (auto &pointA : pinPoints) {
        for (auto &pointB : pinPointsBuffer) {
            if (ctrA != ctrB &&
                pointA.x >= pointB.x - meterRadius / 2 &&
                pointA.x <= pointB.x + meterRadius / 2 &&
                pointA.y >= pointB.y - meterRadius / 2 &&
                pointA.y <= pointB.y + meterRadius / 2
            ) {
                matchedIndexes[0].push_back(ctrA);

                for (int i = 0; i < matchedIndexes[1].size(); i++) {
                    if (matchedIndexes[1].at(i) == ctrA) {
                        matchedIndexes[1].erase(
                            matchedIndexes[1].begin() + i,
                            matchedIndexes[1].begin() + i + 1
                        );
                        break;
                    }
                }                    

                matchedIndexes[1].push_back(ctrB);

                pointB = Point(
                    2000 * EQUATOR_DISTANCE,
                    2000 * MERIDIAN_DISTANCE
                );

                break;
            }
            ctrB++;
        }
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

        for (int i = 0; i < 2; i++) {
            for (auto &index : matchedIndexes[i]) {
                placemarkNodes.at(index)->removeFromParent();
                if (i == 0) {
                    folderOri->addChild(placemarkNodes.at(index));
                }
                else folderDupl->addChild(placemarkNodes.at(index)); 
            }
        }

        std::cout
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