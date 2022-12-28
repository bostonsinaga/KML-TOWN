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
    std::vector<xml::Node*> &sortedPinNodes
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

                int accumulateDistance = 0;
                Point prevPoint = points.front();

                for (int i = 1; i < points.size(); i++) {

                    /* USING ‘HAVERSINE’ FORMULA */
                    
                    double
                        lat1 = prevPoint.x,
                        lat2 = points.at(i).x,
                        lon1 = prevPoint.y,
                        lon2 = points.at(i).y;

                    double R = 6371000.00; // metres
                    double φ1 = lat1 * M_PI/180; // φ, λ in radians
                    double φ2 = lat2 * M_PI/180;
                    double Δφ = (lat2-lat1) * M_PI/180;
                    double Δλ = (lon2-lon1) * M_PI/180;

                    double a = std::sin(Δφ/2) * std::sin(Δφ/2) +
                            std::cos(φ1) * std::cos(φ2) *
                            std::sin(Δλ/2) * std::sin(Δλ/2);
                    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));

                    double d = R * c; // in metres

                    accumulateDistance += std::round(d);
                    prevPoint = points.at(i);
                }

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
            << "-----------------------------\n"
            << "Total distance in document:  " << totalDistance << "m\n"
            << "-----------------------------\n";

        return true;
    }

    return false;
}

#endif // __KML_PLACEMARK_CPP__
