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
        kmlBuilder.getPathStyleMap(&styleMapId)
    );

    std::vector<std::string> coorStrVec;
    for (auto &pinNode : sortedPinNodes) {
        coorStrVec.push_back(
            pinNode->getFirstDescendantByName("coordinates")->getInnerText()
        );
    }

    std::vector<xml::Node*> pathNodes {kmlBuilder.getPath(
        kmlBuilder.COORSTR_NO_ADD_ALTITUDE,
        styleMapId,
        coorStrVec,
        "Path of Selected Pins",
        "Created with KML-TOWN"
    )};

    General kmlGeneral;

    kmlGeneral.insertEditedPlacemarksIntoFolder(
        kmlGeneral.searchMainFolder(kmlNode),
        Builder().getFolder(PINS_PATH_COMMAND_WORKING_FOLDER),
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
        kmlBuilder.getPathStyleMap(&styleMapId)
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
            kmlBuilder.getPath(
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
        Builder().getFolder(PINS_PATH_SEGMENTS_COMMAND_WORKING_FOLDER),
        pathNodes,
        {"Pins-paths segmenting", "Pins-paths segmentation"},
        ""
    );
}

int Placemark::getPathDistance(std::vector<Point> &points) {

    double accumulateDistance = 0;
    Point prevPoint = points.front();

    for (int i = 1; i < points.size(); i++) {

        ///////////////////////////////
        // USING ‘HAVERSINE’ FORMULA //
        ///////////////////////////////
        
        double
            lat1 = prevPoint.x,
            lat2 = points.at(i).x,
            lon1 = prevPoint.y,
            lon2 = points.at(i).y;

        double RADIUS = 6371000.00; // in meter
        double radian1 = lat1 * M_PI/180;
        double radian2 = lat2 * M_PI/180;
        double deltaRadian1 = (lat2-lat1) * M_PI/180;
        double deltaRadian2 = (lon2-lon1) * M_PI/180;

        double a = std::sin(deltaRadian1/2) * std::sin(deltaRadian1/2) +
                std::cos(radian1) * std::cos(radian2) *
                std::sin(deltaRadian2/2) * std::sin(deltaRadian2/2);
        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));

        double d = RADIUS * c; // in meter

        accumulateDistance += d;
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
    double limitDistance,
    bool isAbove
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

                    if ((!isAbove && getPathDistance(points) <= limitDistance) ||
                        (isAbove && getPathDistance(points) >= limitDistance)
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

void Placemark::logName(xml::Node *placemark) {
    if (placemark) {

        xml::Node *placemarkName = placemark->getFirstDescendantByName("name");

        if (placemarkName) {
            std::cout << "Placemark name ---> " << placemarkName->getInnerText() << std::endl;
        }
        else {
            xml::Node *folderNode = placemark->getParent();
            if (folderNode) {
                placemarkName = folderNode->getFirstDescendantByName("name");
                if (placemarkName) {
                    std::cout << "Folder name ---> " << placemarkName->getInnerText() << std::endl;
                }
                else std::cout << "NONAME.." << std::endl;
            }
        }
    }
}

#endif // __KML_PLACEMARK_CPP__
