#ifndef __KML_CROPPER_CPP__
#define __KML_CROPPER_CPP__

#include "cropper.h"

// CROPPING BY RECTANGULAR AREA FROM START TO END POINT //

/* ONLY AVAILABLE FOR PINS YET */

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
    bool isFolderInsertion,
    bool isIncludeFolders,
    bool isParentFolderNamedAType
) {
    std::vector<xml::Node*>
        pinNodes,
        pinCoorNodes;

    General kmlGeneral;

    kmlGeneral.fillWithPlacemarks(
        pinsContainerNode,
        "Point",
        pinNodes,
        pinCoorNodes
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
            Builder().createFolder(CROP_COMMAND_WORKING_FOLDER),
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

std::vector<xml::Node*> Cropper::cutPaths(
    xml::Node *pathsContainerNode,
    Point startPt,  // decimal coordinate
    Point endPt,    // decimal coordinate
    bool isFolderInsertion,
    bool isIncludeFolders,
    bool isParentFolderNamedAType
) {
    std::vector<xml::Node*>
        pathNodes,
        pathCoorNodes;

    General kmlGeneral;
    Builder kmlBuilder;

    kmlGeneral.fillWithPlacemarks(
        pathsContainerNode,
        "LineString",
        pathNodes,
        pathCoorNodes
    );

    // new paths from cropped (inside rectangular)
    std::vector<xml::Node*> croppedPathNodes;

    // [1] is the cropped
    std::vector<Point> segmentPathPoints[3];
    bool isCroppedSegment = false;
    int ctr = 0;

    for (auto &pathCoor : pathCoorNodes) {
        std::vector<Point> pathPoints = Point::getPathPointsFromString(pathCoor->getInnerText());

        for (auto &pathPt : pathPoints) {
            if ((pathPt.x >= startPt.x && pathPt.x <= endPt.x && // BL - TR
                pathPt.y >= startPt.y && pathPt.y <= endPt.y)
                ||
                (pathPt.x >= startPt.x && pathPt.x <= endPt.x && // TL - BR
                pathPt.y >= endPt.y && pathPt.y <= startPt.y)
                ||
                (pathPt.x >= endPt.x && pathPt.x <= startPt.x && // TR - BL
                pathPt.y >= endPt.y && pathPt.y <= startPt.y)
                ||
                (pathPt.x >= endPt.x && pathPt.x <= startPt.x && // BR - TL
                pathPt.y >= startPt.y && pathPt.y <= endPt.y)
            ) {
                segmentPathPoints[1].push_back(pathPt);
                if (!isCroppedSegment) isCroppedSegment = true;
            }
            else {
                segmentPathPoints[isCroppedSegment ? 2 : 0].push_back(pathPt);
            }
        }

        if (isCroppedSegment) {
            xml::Node *segmentPaths[3];

            for (int i = 0; i < 3; i++) {
                segmentPaths[i] = kmlBuilder.createPath(
                    kmlBuilder.COORSTR_AUTO_ADD_ALTITUDE,
                    Placemark::getDataText(pathNodes.at(ctr), "styleUrl"),
                    Point::stringifyVector(segmentPathPoints[i]),
                    Placemark::getDataText(pathNodes.at(ctr), "name"),
                    Placemark::getDataText(pathNodes.at(ctr), "description")
                );

                // exclude
                if (i != 1) {
                    segmentPaths[i]->setParent(pathNodes.at(ctr)->getParent(), true);
                }
                // include
                else {
                    croppedPathNodes.push_back(segmentPaths[i]);
                }
            }

            pathNodes.at(ctr)->removeFromParent(true);
        }

        ctr++;
    }

    if (isFolderInsertion) { // insert into a different folder
        kmlGeneral.insertEditedPlacemarksIntoFolder(
            pathsContainerNode,
            Builder().createFolder(CROP_COMMAND_WORKING_FOLDER),
            croppedPathNodes,
            {"Cropping", "Crop"},
            "pin"
        );
        
        if (croppedPathNodes.size() > 0) {
            return std::vector<xml::Node*>{croppedPathNodes.front()->getParent()};
        }
        else return std::vector<xml::Node*>{pathsContainerNode};
    }
    else { // logging
        if (kmlGeneral.logEditedPlacemarks(
            "pin",
            {"Cropping", "Crop"},
            croppedPathNodes,
            pathsContainerNode
        )) { // succeeded
            return croppedPathNodes;
        }
        else { // failed
            return std::vector<xml::Node*>{};
        }
    }

    return std::vector<xml::Node*>{};
}

std::vector<xml::Node*> Cropper::cutAll(
    xml::Node *pathsContainerNode,
    Point startPt,  // decimal coordinate
    Point endPt,    // decimal coordinate
    bool isFolderInsertion,
    bool isIncludeFolders
) {

    return std::vector<xml::Node*>{};
}

#endif // __KML_CROPPER_CPP__