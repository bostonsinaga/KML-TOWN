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

#endif // __KML_PLACEMARK_CPP__
