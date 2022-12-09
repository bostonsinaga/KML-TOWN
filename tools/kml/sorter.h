#ifndef __KML_SORTER_H__
#define __KML_SORTER_H__

/*
*   SORT LIKE A CHAINS
*   started with nearest placemark from start point
*   (this using cropper)
*/

class Sorter {
    public:
        Sorter() {}
        Sorter(bool isPrintNotification, Menu *menu) {
            if (isPrintNotification) {
                menu->setNotification(
                    std::string("KML-TOWN-> Make sure to put a start point\n") +
                    std::string("           closest to the first desired placemark!\n")
                );
            }
        }

        void orderPins(
            xml::Node *pinsContainerNode,
            Point startPt // decimal coordinate
        ) {
            std::vector<xml::Node*>
                pinNodes,     // parent // corresponding
                pinCoorNodes; // child  // corresponding

            fillWithPlacemarks(pinsContainerNode, &pinNodes, &pinCoorNodes);

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

            // insert into a different folder
            insertEditedPlacemarksIntoFolder(
                SORT_COMMAND_WORKING_FOLDER,
                pinsContainerNode,
                &sortedPinNodes,
                {"Sorting", "Sort"}
            );
        }

    private:
        std::vector<int> sortCoordinates(
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
};

#endif // __KML_SORTER_H__