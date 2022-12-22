#ifndef __KML_CROPPER_CPP__
#define __KML_CROPPER_CPP__

#include "cropper.h"

// CROPPING BY RECTANGULAR AREA FROM START TO END POINT //

void Cropper::printNotification(Menu &menu) {
    menu.setNotification(
        std::string("KML-TOWN-> Make sure to enter only 2D coordinate\n") +
        std::string("           of 'latitude' and 'longitude' sequentially!\n")
    );
}

std::vector<xml::Node*> Cropper::cutPins(
    xml::Node *pinsContainerNode,
    Point startPt,  // decimal coordinate
    Point endPt,    // decimal coordinate
    bool isFolderInsertion
) {
    std::vector<xml::Node*>
        pinNodes,
        pinCoorNodes;

    General kmlGeneral;

    kmlGeneral.fillWithPlacemarks(
        pinsContainerNode,
        pinNodes,
        pinCoorNodes,
        true
    );

    // new pins from cropped (inside rectangular)
    std::vector<xml::Node*> croppedPinNodes;

    int ctr = 0;
    for (auto &pinCoor : pinCoorNodes) {
        Point pinPoint = Point(pinCoor);

        if ((pinPoint.x >= startPt.x && pinPoint.x <= endPt.x && // BL - TR
            pinPoint.y >= startPt.y && pinPoint.y <= endPt.y)
            ||
            (pinPoint.x >= startPt.x && pinPoint.x <= endPt.x && // TL - BR
            pinPoint.y >= endPt.y && pinPoint.y <= startPt.y)
            ||
            (pinPoint.x >= endPt.x && pinPoint.x <= startPt.x && // TR - BL
            pinPoint.y >= endPt.y && pinPoint.y <= startPt.y)
            ||
            (pinPoint.x >= endPt.x && pinPoint.x <= startPt.x && // BR - TL
            pinPoint.y >= startPt.y && pinPoint.y <= endPt.y)
        ) {
            croppedPinNodes.push_back(pinNodes.at(ctr));
        }

        ctr++;
    }

    if (isFolderInsertion) { // insert into a different folder
        kmlGeneral.insertEditedPlacemarksIntoFolder(
            pinsContainerNode,
            Builder().getFolder(CROP_COMMAND_WORKING_FOLDER),
            croppedPinNodes,
            {"Cropping", "Crop"},
            "pin"
        );
        
        if (croppedPinNodes.size() > 0) {
            return std::vector<xml::Node*>{croppedPinNodes.front()->getParent()};
        }
        else return std::vector<xml::Node*>{pinsContainerNode};
    }
    else { // logging
        if (kmlGeneral.logEditedPlacemarks(
            "pin",
            {"Cropping", "Crop"},
            croppedPinNodes,
            pinsContainerNode
        )) { // succeeded
            return croppedPinNodes;
        }
        else { // failed
            return std::vector<xml::Node*>{};
        }
    }

    return std::vector<xml::Node*>{};
}

#endif // __KML_CROPPER_CPP__