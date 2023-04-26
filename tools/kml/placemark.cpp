#ifndef __KML_PLACEMARK_CPP__
#define __KML_PLACEMARK_CPP__

#include "placemark.h"

void Placemark::pinsPath(
    xml::Node *kmlNode,
    std::vector<xml::Node*> &sortedPinNodes
) {
    Builder kmlBuilder;

    std::string styleMapId;
    kmlBuilder.insertStyleMap(
        kmlNode,
        kmlBuilder.createPathStyleMap(&styleMapId)
    );

    std::vector<std::string> coorStrVec;
    for (auto &pinNode : sortedPinNodes) {
        coorStrVec.push_back(
            pinNode->getFirstDescendantByName("coordinates")->getInnerText()
        );
    }

    std::vector<xml::Node*> pathNodes {kmlBuilder.createPath(
        kmlBuilder.COORSTR_NO_ADD_ALTITUDE,
        styleMapId,
        coorStrVec,
        "Path of Selected Pins",
        "Created with KML-TOWN"
    )};

    General kmlGeneral;

    kmlGeneral.insertEditedPlacemarksIntoFolder(
        kmlGeneral.searchMainFolder(kmlNode),
        Builder().createFolder(PINS_PATH_COMMAND_WORKING_FOLDER),
        pathNodes,
        {"Pins-path unifying", "Pins-path unify"},
        ""
    );
}

void Placemark::pinsPathSegments(
    xml::Node *kmlNode,
    std::vector<xml::Node*> &sortedPinNodes,
    bool isFolderize
) {
    Builder kmlBuilder;

    std::string styleMapId;
    kmlBuilder.insertStyleMap(
        kmlNode,
        kmlBuilder.createPathStyleMap(&styleMapId)
    );

    std::vector<std::string> coorStrVec, dateStrVec;
    for (auto &pinNode : sortedPinNodes) {
        coorStrVec.push_back(
            pinNode->getFirstDescendantByName("coordinates")->getInnerText()
        );
        dateStrVec.push_back(
            pinNode->getFirstDescendantByName("description")->getInnerText()
        );
    }

    std::vector<xml::Node*> pathNodes;

    for (int i = 0; i < coorStrVec.size() - 1; i++) {

        std::vector<std::string> coorStrPair {
            coorStrVec.at(i),
            coorStrVec.at(i + 1)
        };

        pathNodes.push_back(
            kmlBuilder.createPath(
                kmlBuilder.COORSTR_NO_ADD_ALTITUDE,
                styleMapId,
                coorStrPair,
                "Path " + std::to_string(i + 1),
                dateStrVec.at(i)
            )
        );
    }

    General kmlGeneral;

    kmlGeneral.insertEditedPlacemarksIntoFolder(
        kmlGeneral.searchMainFolder(kmlNode),
        Builder().createFolder(PINS_PATH_SEGMENTS_COMMAND_WORKING_FOLDER),
        pathNodes,
        {"Pins-paths segmenting", "Pins-paths segmentation"},
        ""
    );
}

template<typename TYPE_T>

// using 'haversine' formula (returns distance in meters)
LD Placemark::getHaversineDistance(
    TYPE_T startPt,
    TYPE_T endPt
) {
    LD lat1 = startPt.x,
       lat2 = endPt.x,
       lon1 = startPt.y,
       lon2 = endPt.y;

    LD radian1 = lat1 * M_PI / 180.0;
    LD radian2 = lat2 * M_PI / 180.0;
    LD deltaRadian1 = (lat2 - lat1) * M_PI / 180.0;
    LD deltaRadian2 = (lon2 - lon1) * M_PI / 180.0;

    LD a = std::sin(deltaRadian1 / 2) * std::sin(deltaRadian1 / 2) +
        std::cos(radian1) * std::cos(radian2) *
        std::sin(deltaRadian2 / 2) * std::sin(deltaRadian2 / 2);

    LD c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));

    // average earth radius times 'c'
    return 6371000.0 * c;
}

int Placemark::getPathDistance(std::vector<Point> &points) {
    LD accumulateDistance = 0;
    Point prevPoint = points.front();

    for (int i = 1; i < points.size(); i++) {
        accumulateDistance += getHaversineDistance<Point&>(prevPoint, points.at(i));
        prevPoint = points.at(i);
    }

    return int(std::round(accumulateDistance));
}

bool Placemark::setPathDistance(xml::Node *kmlNode, bool isOnlyGetInfo) {
    if (kmlNode) {
        int totalDistance = 0;

        for (auto &node : kmlNode->getDescendantsByName("Placemark", true)) {
            xml::Node *lineStringNode = node->getFirstDescendantByName("LineString");

            if (lineStringNode) {
                
                // certainly contains the coordinate string
                std::vector<Point> points = Point().getPathPointsFromString(
                    lineStringNode->getFirstDescendantByName("coordinates")->getInnerText()
                );

                int accumulateDistance = getPathDistance(points);

                if (!isOnlyGetInfo) {
                    xml::Node *descriptionNode = node->getFirstDescendantByName("description");

                    if (!descriptionNode) {
                        descriptionNode = new xml::Node("description", node);

                        if (node->getFirstDescendantByName("name")) {
                            if (node->getChildren()->size() >= 2) {
                                node->swapChildren(
                                    node->getChildren()->at(2),
                                    descriptionNode
                                );
                            }
                        }
                        else if (node->getChildren()->size() > 1) {
                            node->swapChildren(
                                node->getChildren()->front(),
                                descriptionNode
                            );
                        }
                    }

                    descriptionNode->setInnerText(
                        descriptionNode->getInnerText() + std::string("\n") +
                        std::to_string(accumulateDistance) + std::string("m")
                    );
                }

                totalDistance += accumulateDistance;
            }
        }

        std::cout
            << "-----------------------\n"
            << "Total paths distance:  " << totalDistance << "m\n"
            << "-----------------------\n";

        return true;
    }

    return false;
}

void Placemark::removePathsByDistance(
    xml::Node *kmlNode,
    LD limitDistance,
    bool isOverDistance
) {
    if (kmlNode) {
        int totalRemoved = 0;
        
        for (auto &node : kmlNode->getDescendantsByName("Placemark", true)) {
            xml::Node *lineStringNode = node->getFirstDescendantByName("LineString");

            if (lineStringNode) {
                xml::Node *coordinatesNode = (
                    lineStringNode->getFirstDescendantByName("coordinates")
                );

                if (coordinatesNode) {
                    std::vector<Point> points = Point().getPathPointsFromString(
                        coordinatesNode->getInnerText()
                    );

                    if ((!isOverDistance && getPathDistance(points) <= limitDistance) ||
                        (isOverDistance && getPathDistance(points) >= limitDistance)
                    ) {
                        node->removeFromParent();
                        delete node;
                        totalRemoved++;
                    }
                }
            }
        }

        std::cout
            << "----------------------\n"
            << "Total removed paths:  " << totalRemoved << "\n"
            << "----------------------\n";
    }
}

// read placemark name or its folder name (closest parent)
std::string Placemark::getName(xml::Node *placemarkNode) {
    std::string name = "noname";

    if (placemarkNode) {
        xml::Node *placemarkName = placemarkNode->getFirstDescendantByName("name");

        if (placemarkName) {
            name = placemarkName->getInnerText();
        }
        else {
            xml::Node *folderNode = placemarkNode->getParent();
            if (folderNode) {
                placemarkName = folderNode->getFirstDescendantByName("name");
                if (placemarkName) {
                    name = placemarkName->getInnerText();
                }
            }
        }
    }

    return name;
}

// read placemark name or its folder name (closest parent)
void Placemark::logName(xml::Node *placemarkNode, bool isResetCtr) {

    static int ctr = 1;
    if (isResetCtr) ctr = 1;

    if (placemarkNode) {
        xml::Node *placemarkName = placemarkNode->getFirstDescendantByName("name");

        if (placemarkName) {
            std::cout << ctr << ") Placemark name ---> " << placemarkName->getInnerText() << std::endl;
        }
        else {
            xml::Node *folderNode = placemarkNode->getParent();
            if (folderNode) {
                placemarkName = folderNode->getFirstDescendantByName("name");
                if (placemarkName) {
                    std::cout << ctr << ") Folder name ---> " << placemarkName->getInnerText() << std::endl;
                }
                else std::cout << ctr << ") NONAME.." << std::endl;
            }
        }
    }

    ctr++;
}

// get inner text of certain node data (alternative for 'getName' method)
std::string Placemark::getDataText(
    xml::Node *placemarkNode,
    std::string dataNodeName
) {
    std::string dataStr = "";

    if (placemarkNode) {
        xml::Node *placemarkData = placemarkNode->getFirstDescendantByName(dataNodeName);

        if (placemarkData) {
            dataStr = placemarkData->getInnerText();
        }
    }

    return dataStr;
}

// used in method that need '--include-folders'
void Placemark::includeFolder(
    xml::Node *placemarkNode,
    xml::Node *folderNode,
    int testIndex,
    bool isResetStatic
) {
    static std::vector<std::vector<std::string>> includedFolderNameVecVec;
    static std::vector<int> existIndexes;

    if (isResetStatic) {
        includedFolderNameVecVec.clear();
        existIndexes.clear();
    }

    xml::Node *includedNewFolder_node = nullptr;   // !nullptr -> not exist
    std::string includedExistFolder_name = "";     // !"" -> exist

    // include folders check
    if (placemarkNode->getParent()) {

        int includedFolderNameVecVec_foundDex = -1;
        includedExistFolder_name = getName(placemarkNode->getParent());
        int existIndex = mini_tool::isPrimitiveInsideVector<int>(existIndexes, testIndex);

        if (existIndexes.size() == 0 || existIndex == -1) {
            existIndexes.push_back(testIndex);
            includedFolderNameVecVec.push_back(std::vector<std::string>{});
        }
        else {
            includedFolderNameVecVec_foundDex = mini_tool::isPrimitiveInsideVector<std::string>(
                includedFolderNameVecVec.at(existIndex), includedExistFolder_name
            );
        }

        // there isn't any yet
        if (includedFolderNameVecVec_foundDex == -1) {
            includedNewFolder_node = Builder().createFolder(includedExistFolder_name);
            includedFolderNameVecVec.back().push_back(includedExistFolder_name);
            includedExistFolder_name = "";
        }
    }
    
    // add to new folder //

    placemarkNode->removeFromParent();

    if (includedNewFolder_node) {
        includedNewFolder_node->addChild(placemarkNode);
        folderNode->addChild(includedNewFolder_node);
    }
    else if (includedExistFolder_name != "") {
        
        for (auto &includedExistFolder_node : *folderNode->getChildren()) {
            if (includedExistFolder_name == getName(includedExistFolder_node)) {

                includedExistFolder_node->addChild(placemarkNode);
                break;
            }
        }
    }
    // 'placemarkNode' has no parent (rare)
    else {
        folderNode->addChild(placemarkNode);
    }
}

Point Placemark::convertDegreeToMeterPoint(Point &ptIn) {
    return Point(
        getHaversineDistance<Point>(
            Point(ptIn.x, 0),
            Point(0, 0)
        ),
        getHaversineDistance<Point>(
            Point(0, ptIn.y),
            Point(0, 0)
        )
    );
}

#endif // __KML_PLACEMARK_CPP__