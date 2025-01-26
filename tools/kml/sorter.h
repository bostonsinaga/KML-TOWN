#ifndef __KML_SORTER_H__
#define __KML_SORTER_H__

#include "placemark.h"

namespace kml {

/**
 * SORT LIKE A CHAINS
 * Started with nearest placemark from start point.
 */
class Sorter {
    public:
        Sorter() {}
        
        void printNotification(Menu &menu);

        std::vector<xml::Node*> orderApart(
            std::vector<xml::Node*> &croppedPlacemarks,
            Point &startPt,  // decimal coordinate
            int typeFlag,
            bool isFolderInsertion,  // 'false' returns nodes or empty, 'true' returns folder or empty
            bool isIncludeFolders
        );

        // assumed that 'isFolderInsertion' is true
        std::vector<xml::Node*> orderAll(
            std::vector<xml::Node*> *sortedPlacemarks_arr,  // consist of cropped pins and paths
            bool isIncludeFolders
        );

    private:
        std::vector<int> sortCoordinates(
            std::vector<xml::Node*> &pinCoorNodes,
            xml::Node *checkPointNode,
            int typeFlag
        );

        enum {PIN_TYPE, PATH_TYPE, ALL_TYPE};
};
}

#endif // __KML_SORTER_H__