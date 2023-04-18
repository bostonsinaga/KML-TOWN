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

        if (Point::isBetween(pinPoint, startPt, endPt)) {
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
    setupSelectionRect(startPt, endPt);

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

    // new paths from cropped (inside selection rect)
    std::vector<xml::Node*> croppedPathNodes;

    /*
        part has 3 segments
        [0] is before cropped
        [1] is being cropped
        [2] is after cropped
    */
    std::vector<Point> segmentPathPoints[3];

    // topper for loop counter
    int mainCtr = 0;

    auto reloadSegmentsPart = [&]() {
        for (int i = 0; i < 3; i++) {
            segmentPathPoints[i].clear();
        }
    };

    // create segments part as 3 separated paths //
    auto createPathsFromParts = [&](xml::Node *pathNode, std::string &styleString) {
        xml::Node *segmentPaths[3];

        for (int i = 0; i < 3; i++) {
            if (segmentPathPoints[i].size() > 0) {

                segmentPaths[i] = kmlBuilder.createPath(
                    kmlBuilder.COORSTR_AUTO_ADD_ALTITUDE,
                    styleString.size() > 1 ? styleString.substr(1) : styleString,
                    Point::stringifyVector(segmentPathPoints[i], false),
                    Placemark::getDataText(pathNode, "name"),
                    Placemark::getDataText(pathNode, "description")
                );

                // exclude
                if (i != 1) {
                    segmentPaths[i]->setParent(pathNode->getParent(), true);
                }
                // include
                else {
                    croppedPathNodes.push_back(segmentPaths[i]);
                }
            }
        }
    };

    for (auto &pathCoor : pathCoorNodes) {

        // empty segments part
        reloadSegmentsPart();

        std::vector<Point> pathPoints = (
            Point::getPathPointsFromString(pathCoor->getInnerText())
        );

        bool isPreviousOutsider = false,
             isDetectedInsider = false;

        std::string styleString = Placemark::getDataText(pathNodes.at(mainCtr), "styleUrl");

        for (int i = 0; i < pathPoints.size(); i++) {

            // 'pathPoints.at(i)' axis inverted from xml form (x,y -> y,x)
            Point pathPt_rev = Point(pathPoints.at(i).y, pathPoints.at(i).x);

            // points inside selection rect
            if (Point::isBetween(pathPt_rev, startPt, endPt)) {

                // multiple insiders
                if (isDetectedInsider && isPreviousOutsider) {
                    std::vector<Point> segmentBuffer = {pathPt_rev};

                    keepPathEdge(
                        segmentPathPoints[2],
                        segmentBuffer
                    );

                    createPathsFromParts(pathNodes.at(mainCtr), styleString);
                    reloadSegmentsPart();

                    segmentPathPoints[1] = segmentBuffer;
                }
                // first insider intersection with outsider
                else {
                    segmentPathPoints[1].push_back(pathPt_rev);
                    
                    if (!isDetectedInsider) {
                        isDetectedInsider = true;

                        if (isPreviousOutsider) {
                            keepPathEdge(
                                segmentPathPoints[0],
                                segmentPathPoints[1]
                            );
                        }
                    }
                }

                isPreviousOutsider = false;
            }
            // points outside selection rect
            else {
                segmentPathPoints[isDetectedInsider ? 2 : 0].push_back(pathPt_rev);
                
                // previous point at inside rect and current at outside rect
                if (isDetectedInsider && !isPreviousOutsider) {
                    keepPathEdge(
                        segmentPathPoints[2],
                        segmentPathPoints[1]
                    );
                }
                /*
                    the path may intersects selection rect
                    where the segment of path outside the rect
                */
                else {
                    LineEquation linEq_AB_hook;
                    bool isPathPointsLoopContinue = false;

                    for (int j = 0; j < 3; j += 2) {

                        // set cropped path (insider) first edge point
                        if (segmentPathPoints[j].size() > 1 &&
                            isSegmentIntersectsSelectionRect(
                                segmentPathPoints[j].at(segmentPathPoints[j].size() - 2),
                                segmentPathPoints[j].back(),
                                &linEq_AB_hook
                            )
                        ) {
                            // multiple insiders from outsider intersection with selection rect
                            if (j == 2) {
                                Point ptBuffer[2] = {
                                    segmentPathPoints[j].at(segmentPathPoints[j].size() - 2),
                                    segmentPathPoints[j].back()
                                };

                                segmentPathPoints[j].pop_back();
                                createPathsFromParts(pathNodes.at(mainCtr), styleString);
                                reloadSegmentsPart();

                                segmentPathPoints[0].push_back(ptBuffer[0]);
                                segmentPathPoints[0].push_back(ptBuffer[1]);
                            }

                            i--;

                            // 'segmentPathPoints[1]' is empty
                            keepPathEdge(
                                segmentPathPoints[0],
                                segmentPathPoints[1],
                                &linEq_AB_hook
                            );

                            isDetectedInsider = true;
                            isPreviousOutsider = false;
                            isPathPointsLoopContinue = true;
                            break;
                        }
                    }

                    if (isPathPointsLoopContinue) continue;
                }

                isPreviousOutsider = true;
            }
        }

        // remove previous path if croppable
        if (isDetectedInsider) {
            createPathsFromParts(pathNodes.at(mainCtr), styleString);            
            pathNodes.at(mainCtr)->removeFromParent(true);
        }
        
        mainCtr++;
    }

    if (isFolderInsertion) { // insert into a different folder
        kmlGeneral.insertEditedPlacemarksIntoFolder(
            pathsContainerNode,
            Builder().createFolder(CROP_COMMAND_WORKING_FOLDER),
            croppedPathNodes,
            {"Cropping", "Crop"},
            "path"
        );
        
        if (croppedPathNodes.size() > 0) {
            return std::vector<xml::Node*>{croppedPathNodes.front()->getParent()};
        }
        else return std::vector<xml::Node*>{pathsContainerNode};
    }
    else { // logging
        if (kmlGeneral.logEditedPlacemarks(
            "path",
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

// 'm' is gradient and 'c' is constant
Cropper::LineEquation Cropper::produceLineEquation(Point &ptA, Point &ptB) {
    LineEquation retLinEq;
    retLinEq.m = (ptA.y - ptB.y) / (ptA.x - ptB.x);
    retLinEq.c = ptA.y - retLinEq.m * ptA.x;
    return retLinEq;
}

/*
    choose what selection rect 'intersect' function will be used
    and get the intersection point
*/
void Cropper::obtainNewPointFromIntersection(
    Point &ptA,
    Point &ptB,
    LineEquation &linEq_AB,
    const std::function<void(Point&)> &callback
) {
    int relCtr = 0;

    // 'ptA' and 'ptB' are path segment sequentially directed
    Point ptDifferent = ptA - ptB;

    // set to first probability intersection index
    if (ptDifferent.x < 0 && ptDifferent.y < 0) { // BL->TR
        relCtr = 3;
    }
    else if (ptDifferent.x < 0 && ptDifferent.y >= 0) { // TL->BR
        relCtr = 0;
    }
    else if (ptDifferent.x >= 0 && ptDifferent.y >= 0) { // TR->BL
        relCtr = 1;
    }
    else if (ptDifferent.x >= 0 && ptDifferent.y < 0) { // BR->TL
        relCtr = 2;
    }

    int absCtr = 0;
    Point ptNew;

    while (absCtr < 4) {
        if (selRect.intersect[relCtr](linEq_AB, &ptNew)) {
            callback(ptNew);
            break;
        }

        absCtr++;
        relCtr++;
        if (relCtr == 4) relCtr = 0;
    }
}

// add additional point at selection rect intersection (must intersected)
void Cropper::keepPathEdge(
    std::vector<Point> &outsiderPtVec,  // this two vectors should be filled except,
    std::vector<Point> &insiderPtVec,   // 'insiderPtVec' can be empty if only path beyonds selection rect but intersects it
    LineEquation *linEq_AB_hooked
) {
    int outsiderVecSz = outsiderPtVec.size();
    if (outsiderVecSz == 0) return;

    LineEquation linEq_AB;

    if (insiderPtVec.size() > 0) {

        if (linEq_AB_hooked) linEq_AB = *linEq_AB_hooked;
        else linEq_AB = produceLineEquation(
            outsiderPtVec.back(),
            insiderPtVec.front()
        );

        obtainNewPointFromIntersection(
            outsiderPtVec.back(),
            insiderPtVec.front(),
            linEq_AB,
            [&](Point &ptNew) {
                Point ptBuffer = insiderPtVec.front();
                insiderPtVec.at(0) = ptNew;
                insiderPtVec.push_back(ptBuffer);

                ptBuffer = outsiderPtVec.back();
                outsiderPtVec.pop_back();
                outsiderPtVec.push_back(ptNew);
                outsiderPtVec.push_back(ptBuffer);
            }
        );
    }
    // when path beyonds selection rect but intersects it
    else {
        if (outsiderVecSz == 1) return;

        if (linEq_AB_hooked) linEq_AB = *linEq_AB_hooked;
        else linEq_AB = produceLineEquation(
            outsiderPtVec.at(outsiderVecSz - 2),
            outsiderPtVec.back()
        );

        obtainNewPointFromIntersection(
            outsiderPtVec.at(outsiderVecSz - 2),
            outsiderPtVec.back(),
            linEq_AB,
            [&](Point &ptNew) {
                insiderPtVec.push_back(ptNew);
                outsiderPtVec.pop_back();
                outsiderPtVec.push_back(ptNew);
            }
        );
    }
}

// set selection rect points with statictly 'BL, TL, TR, BR' formation
void Cropper::setupSelectionRect(Point &startPt, Point &endPt) {
    Point ptDifferent = startPt - endPt;

    if (ptDifferent.x < 0 && ptDifferent.y < 0) { // BL->TR
        selRect.ptArr[0] = startPt;
        selRect.ptArr[1] = Point(startPt.x, endPt.y);
        selRect.ptArr[2] = endPt;
        selRect.ptArr[3] = Point(endPt.x, startPt.y);
    }
    else if (ptDifferent.x < 0 && ptDifferent.y >= 0) { // TL->BR
        selRect.ptArr[0] = Point(startPt.x, endPt.y);
        selRect.ptArr[1] = startPt;
        selRect.ptArr[2] = Point(endPt.x, startPt.y);
        selRect.ptArr[3] = endPt;
    }
    else if (ptDifferent.x >= 0 && ptDifferent.y >= 0) { // TR->BL
        selRect.ptArr[0] = endPt;
        selRect.ptArr[1] = Point(endPt.x, startPt.y);
        selRect.ptArr[2] = startPt;
        selRect.ptArr[3] = Point(startPt.x, endPt.y);
    }
    else if (ptDifferent.x >= 0 && ptDifferent.y < 0) { // BR->TL
        selRect.ptArr[0] = Point(endPt.x, startPt.y);
        selRect.ptArr[1] = endPt;
        selRect.ptArr[2] = Point(startPt.x, endPt.y);
        selRect.ptArr[3] = startPt;
    }

    // line equation each side
    for (int i = 0; i < 4; i++) {
        selRect.linEqArr[i] = produceLineEquation(
            selRect.ptArr[i],
            selRect.ptArr[i < 3 ? i+1 : 0]
        );
    }

    // intersection lambdas //

    selRect.intersect[0] = [&](LineEquation &linEq_AB, Point *newPt_hook)->bool {
        double y = linEq_AB.m * selRect.ptArr[0].x + linEq_AB.c;

        if (y >= selRect.ptArr[0].y &&
            y < selRect.ptArr[1].y
        ) {
            *newPt_hook = Point(selRect.ptArr[0].x, y);
            return true;
        }

        return false;
    };

    selRect.intersect[1] = [&](LineEquation &linEq_AB, Point *newPt_hook)->bool {
        double x = (selRect.ptArr[1].y - linEq_AB.c) / linEq_AB.m;

        if (x >= selRect.ptArr[1].x &&
            x < selRect.ptArr[2].x
        ) {
            *newPt_hook = Point(x, selRect.ptArr[1].y);
            return true;
        }

        return false;
    };

    selRect.intersect[2] = [&](LineEquation &linEq_AB, Point *newPt_hook)->bool {
        double y = linEq_AB.m * selRect.ptArr[2].x + linEq_AB.c;

        if (y <= selRect.ptArr[2].y &&
            y > selRect.ptArr[3].y
        ) {
            *newPt_hook = Point(selRect.ptArr[2].x, y);
            return true;
        }

        return false;
    };

    selRect.intersect[3] = [&](LineEquation &linEq_AB, Point *newPt_hook)->bool {
        double x = (selRect.ptArr[3].y - linEq_AB.c) / linEq_AB.m;

        if (x <= selRect.ptArr[3].x &&
            x > selRect.ptArr[0].x
        ) {
            *newPt_hook = Point(x, selRect.ptArr[3].y);
            return true;
        }

        return false;
    };
}

// 'ptA' and 'ptB' are path segment/couple points that beyond selection rect
bool Cropper::isSegmentIntersectsSelectionRect(
    Point &ptA,
    Point &ptB,
    LineEquation *linEq_AB_hook
) {
    LineEquation
        linEq_AB = produceLineEquation(ptA, ptB),
        linEq_oppRect; // opposite to AB gradient

    if (linEq_AB_hook) *linEq_AB_hook = linEq_AB;

    int oppDex = linEq_AB.m < 0 ? 0 : 1;
    linEq_oppRect = produceLineEquation(selRect.ptArr[oppDex], selRect.ptArr[oppDex + 2]);

    /*  compute intersection
        f(x)1 = f(x)2
        x = (c2 - c1) / (m1 - m2)
    */
    double mDiff = linEq_AB.m - linEq_oppRect.m;
    
    if (mDiff != 0.0) {
        double
            x = (linEq_oppRect.c - linEq_AB.c) / mDiff,
            y = linEq_AB.m * x + linEq_AB.c;
        
        Point testPt = Point(x, y);

        if (Point::isBetween(testPt, selRect.ptArr[0], selRect.ptArr[2])) {
            return true;
        }
        
        return false;
    }
    
    return false;
}

#endif // __KML_CROPPER_CPP__