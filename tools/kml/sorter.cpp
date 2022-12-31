#ifndef __KML_SORTER_CPP__
#define __KML_SORTER_CPP__

#include "sorter.h"

/*
*   SORT LIKE A CHAINS
*   started with nearest placemark from start point
*   (this using cropper)
*/

/* ONLY AVAILABLE FOR PINS YET */

void Sorter::printNotification(Menu &menu) {
    menu.setNotification(
        std::string("KML-TOWN-> Make sure to put a start point\n") +
        std::string("           closest to the first desired placemark!\n")
    );
}

std::vector<xml::Node*> Sorter::orderPins(
    /*
    *   - basic (purely) use is just need single node for 'dualismVector'
    *   -'dualismVector' may contains nodes for command that involves 'sorter'
    */
    std::vector<xml::Node*> dualismVector, /* count = 1: container, count > 1: cropped pins */
    /******/
    Point startPt, // decimal coordinate
    bool isFolderInsertion // 'false' return nodes, 'true' return empty
) {
    std::vector<xml::Node*>
        pinNodes,     // parent // corresponding
        pinCoorNodes; // child  // corresponding

    xml::Node *pinsContainerNode = nullptr;
    General kmlGeneral;

    if (dualismVector.size() == 1) {
        pinsContainerNode = dualismVector.front();
        
        kmlGeneral.fillWithPins(
            pinsContainerNode,
            pinNodes,
            pinCoorNodes
        );
    }
    else {
        // contains vector of pin 'Placemark'
        pinNodes = dualismVector;
        for (auto &node : dualismVector) {
            pinCoorNodes.push_back(
                node->getFirstDescendantByName("coordinates")
            );
        }
    }

    /*
    *   sorting pins chaining from start point to nearest one
    *   inside start to end point formed rectangular
    */

    xml::Node
        *checkPointNode = new xml::Node("coordinates"),
        *firstCheckPointNode = checkPointNode;

    // set 'point' into string form
    checkPointNode->setInnerText(startPt.stringify());

    // used in new sorted pins folder (in sorted composing)
    std::vector<xml::Node*> sortedPinNodes;
    
    // search for the nearest coordinate to start point
    int sortCtr = pinCoorNodes.size();
    for (int i = 0; i < sortCtr; i++) {

        // get sorted index orders of nearest to longest coordinates from checkpoint
        std::vector<int> sortIndexes = sortCoordinates(pinCoorNodes, checkPointNode);

        // new checkpoint from the nearest point as the next checkpoint too
        checkPointNode = pinCoorNodes.at(sortIndexes.front());

        // entered into new sorted pins folder
        sortedPinNodes.push_back(pinNodes.at(sortIndexes.front()));

        pinNodes.erase(
            pinNodes.begin() + sortIndexes.front(),
            pinNodes.begin() + sortIndexes.front() + 1
        );

        pinCoorNodes.erase(
            pinCoorNodes.begin() + sortIndexes.front(),
            pinCoorNodes.begin() + sortIndexes.front() + 1
        );
    }

    delete firstCheckPointNode;

    if (isFolderInsertion) {
        // insert into a different folder
        kmlGeneral.insertEditedPlacemarksIntoFolder(
            pinsContainerNode,
            Builder().getFolder(SORT_COMMAND_WORKING_FOLDER),
            sortedPinNodes,
            {"Sorting", "Sort"},
            "pin"
        );
        
        if (sortedPinNodes.size() > 0) {
            return std::vector<xml::Node*>{sortedPinNodes.front()->getParent()};
        }
        else return std::vector<xml::Node*>{pinsContainerNode};
    }
    else { // logging
        /* 'dualismVector' has multiple nodes */

        if (kmlGeneral.logEditedPlacemarks(
            "pin",
            {"Sorting", "Sort"},
            sortedPinNodes,
            pinsContainerNode
        )) { // succeeded
            return sortedPinNodes;
        }
        else { // failed
            return std::vector<xml::Node*>{};
        }
    }

    return std::vector<xml::Node*>{};
}

std::vector<int> Sorter::sortCoordinates(
    std::vector<xml::Node*> &pinCoorNodes,
    xml::Node *checkPointNode
) {
    Point cpPoint = Point(checkPointNode);
    std::vector<double> hypotenuses;

    for (auto &pinCoor : pinCoorNodes) {
        Point delta = Point(pinCoor) - cpPoint;
        hypotenuses.push_back(
            std::sqrt(std::pow(delta.x, 2) + std::pow(delta.y, 2))
        );
    }

    // changable numbers
    std::vector<int> retIndexes;
    for (int i = 0; i < hypotenuses.size(); i++) {
        retIndexes.push_back(i);
    }

    // sorting lowest to highest
    for (int i = 0; i < hypotenuses.size(); i++) {
        for (int j = 0; j < hypotenuses.size(); j++) {
            if (hypotenuses.at(i) < hypotenuses.at(j)) {

                double bufferHypo = hypotenuses.at(i);
                hypotenuses.at(i) = hypotenuses.at(j);
                hypotenuses.at(j) = bufferHypo;

                int bufferIndex = retIndexes.at(i);
                retIndexes.at(i) = retIndexes.at(j);
                retIndexes.at(j) = bufferIndex;
            }
        }
    }

    return retIndexes;
}

#endif // __KML_SORTER_CPP__