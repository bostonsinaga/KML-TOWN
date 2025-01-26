#ifndef __KML_SORTER_CPP__
#define __KML_SORTER_CPP__

#include "sorter.h"

namespace kml {

void Sorter::printNotification(Menu &menu) {
    menu.setNotification(
        std::string("KML-TOWN-> Make sure to put a start point\n") +
        std::string("           closest to the first desired placemark!\n")
    );
}

std::vector<xml::Node*> Sorter::orderApart(
    std::vector<xml::Node*> &croppedPlacemarks,
    Point &startPt,  // decimal coordinate
    int typeFlag,
    bool isFolderInsertion,  // 'false' returns nodes or empty, 'true' returns folder or empty
    bool isIncludeFolders
) {
    std::vector<xml::Node*> placemarkCoorNodes;

    xml::Node *placemarksContainerNode = General::searchMainFolder(
        croppedPlacemarks.front()->getRoot()
    );

    for (auto &node : croppedPlacemarks) {
        placemarkCoorNodes.push_back(
            node->getFirstDescendantByName("coordinates")
        );
    }

    /*
    *   sorting placemarks chaining from start point to nearest one
    *   inside start to end point (selection rect)
    */

    xml::Node
        *checkPointNode = new xml::Node("coordinates"),
        *firstCheckPointNode = checkPointNode;

    // set 'point' into string form
    checkPointNode->setInnerText(startPt.stringify());

    // used in new sorted placemarks folder (in sorted composing)
    std::vector<xml::Node*> sortedPlacemarkNodes;
    
    // search for the nearest coordinate to start point
    int sortCtr = placemarkCoorNodes.size();

    for (int i = 0; i < sortCtr; i++) {

        // get sorted index orders of nearest to longest coordinates from checkpoint
        std::vector<int> sortIndexes = sortCoordinates(
            placemarkCoorNodes,
            checkPointNode,
            typeFlag
        );

        // new checkpoint from the nearest point as the next checkpoint too
        checkPointNode = placemarkCoorNodes.at(sortIndexes.front());

        // entered into new sorted placemarks folder
        sortedPlacemarkNodes.push_back(croppedPlacemarks.at(sortIndexes.front()));

        croppedPlacemarks.erase(
            croppedPlacemarks.begin() + sortIndexes.front(),
            croppedPlacemarks.begin() + sortIndexes.front() + 1
        );

        placemarkCoorNodes.erase(
            placemarkCoorNodes.begin() + sortIndexes.front(),
            placemarkCoorNodes.begin() + sortIndexes.front() + 1
        );
    }

    delete firstCheckPointNode;

    // insert into a special folder
    if (isFolderInsertion) {

        // succeeded
        if (sortedPlacemarkNodes.size() > 0) {

            xml::Node *workingFolder = Builder::createFolder(
              definitions::SORT_COMMAND_WORKING_FOLDER
            );

            if (isIncludeFolders) {
                General::putOnTopFolder(
                    placemarksContainerNode,
                    workingFolder
                );

                for (int i = 0; i < sortedPlacemarkNodes.size(); i++) {
                    Placemark::includeFolder(
                        sortedPlacemarkNodes.at(i),
                        workingFolder
                    );
                }
            }
            else {
                General::insertEditedPlacemarksIntoFolder(
                    placemarksContainerNode,
                    workingFolder,
                    sortedPlacemarkNodes,
                    {"Sorting", "Sort"},
                    typeFlag == PIN_TYPE ? "pin" : "path"
                );
            }

            if (isIncludeFolders) {
                std::string typeStr = typeFlag == PIN_TYPE ? "pin" : "path";
                std::cout << "KML-> Sort " << typeStr << "s inside '" << placemarksContainerNode->getName()
                          << "' named '" << Placemark::getDataText(placemarksContainerNode, "name") << "' completed!\n";
            }

            return std::vector<xml::Node*>{workingFolder};
        }
        // failed
        else {
            General::logEditedPlacemarks(
                typeFlag == PIN_TYPE ? "pin" : "path",
                {"Sorting", "Sort"},
                sortedPlacemarkNodes,
                placemarksContainerNode
            );

            return std::vector<xml::Node*>{};
        }
    }
    // logging
    else {
        if (General::logEditedPlacemarks(
            typeFlag == PIN_TYPE ? "pin" : "path",
            {"Sorting", "Sort"},
            sortedPlacemarkNodes,
            placemarksContainerNode
        )) { // succeeded
            return sortedPlacemarkNodes;
        }
        else { // failed
            return std::vector<xml::Node*>{};
        }
    }

    return std::vector<xml::Node*>{};
}

// assumed that 'isFolderInsertion' is true
std::vector<xml::Node*> Sorter::orderAll(
    std::vector<xml::Node*> *sortedPlacemarks_arr,  // consist of cropped pins and paths
    bool isIncludeFolders
) {
    // succeeded
    if (sortedPlacemarks_arr[0].size() > 0 ||
        sortedPlacemarks_arr[1].size() > 0
    ) {
        std::string typeNamesArr[2] = {"PINS", "PATHS"};

        int startCt = sortedPlacemarks_arr[0].size() > 0 ? 0 : 1,
            totalCt = sortedPlacemarks_arr[1].size() > 0 ? 2 : 1;

        for (int i = startCt; i < totalCt; i++) {
            xml::Node *plcFolder = Builder::createFolder(typeNamesArr[i]);

            std::vector<xml::Node*> plcVec = sortedPlacemarks_arr[i].front()->getChildrenByName(
                isIncludeFolders ? "Folder" : "Placemark"
            );

            General::insertEditedPlacemarksIntoFolder(
                sortedPlacemarks_arr[startCt].front(),
                plcFolder,
                plcVec,
                {"", ""},
                ""
            );

            if (i == 1 && startCt == 0) {
                sortedPlacemarks_arr[1].front()->removeFromParent(true);
            }
        }

        if (totalCt == 2) {
            int memberCount = sortedPlacemarks_arr[startCt].front()->getChildren()->size();
            sortedPlacemarks_arr[startCt].front()->swapChildren(
                memberCount - 1,
                memberCount - 2
            );
        }

        return sortedPlacemarks_arr[startCt];
    }
    // failed
    else return std::vector<xml::Node*>{};
}

std::vector<int> Sorter::sortCoordinates(
    std::vector<xml::Node*> &placemarkCoorNodes,
    xml::Node *checkPointNode,
    int typeFlag
) {
    Point cpPoint = Point(checkPointNode);
    std::vector<double> hypotenuses;

    for (auto &placemarkCoor : placemarkCoorNodes) {
        Point point;

        if (typeFlag == PIN_TYPE) {
            point = Point(placemarkCoor);
        }
        else if (typeFlag == PATH_TYPE) {
            point = Point::getPathPointsFromString(
                placemarkCoor->getInnerText()
            ).front();
        }

        Point delta = point - cpPoint;

        hypotenuses.push_back(
            std::sqrt(std::pow(delta.x, 2) + std::pow(delta.y, 2))
        );
    }

    // changable numbers
    std::vector<int> retIndexes;
    for (int i = 0; i < hypotenuses.size(); i++) {
        retIndexes.push_back(i);
    }

    // // sorting lowest to highest
    // for (int i = 0; i < hypotenuses.size(); i++) {
    //     for (int j = 0; j < hypotenuses.size(); j++) {

    //         if (hypotenuses.at(i) < hypotenuses.at(j)) {

    //             double bufferHypo = hypotenuses.at(i);
    //             hypotenuses.at(i) = hypotenuses.at(j);
    //             hypotenuses.at(j) = bufferHypo;

    //             int bufferIndex = retIndexes.at(i);
    //             retIndexes.at(i) = retIndexes.at(j);
    //             retIndexes.at(j) = bufferIndex;
    //         }
    //     }
    // }

    // COMB SORT //

    int gap = hypotenuses.size();
    bool swapped = true, ascending = true;

    while (gap != 1 || swapped) {
        gap = (gap * 10) / 13;

        if (gap < 1) gap = 1;
        swapped = false;

        for (int i = 0; i < hypotenuses.size() - gap; i++) {
            if ((ascending && hypotenuses.at(i) > hypotenuses.at(i + gap)) ||
                (!ascending && hypotenuses.at(i) < hypotenuses.at(i + gap))
            ) {
                std::swap(hypotenuses.at(i), hypotenuses.at(i + gap));
                std::swap(retIndexes.at(i), retIndexes.at(i + gap));
                swapped = true;
            }
        }
    }

    return retIndexes;
}
}

#endif // __KML_SORTER_CPP__