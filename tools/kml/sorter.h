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
        
        void printNotification(Menu &menu);

        std::vector<xml::Node*> orderPins(
            /*
            *   - basic (purely) use is just need single node for 'dualismVector'
            *   -'dualismVector' may contains nodes for command that involves 'sorter'
            */
            std::vector<xml::Node*> dualismVector, /* count = 1: container, count > 1: cropped pins */
            /******/
            Point startPt, // decimal coordinate
            bool isFolderInsertion // 'false' return nodes, 'true' return empty
        );

    private:
        std::vector<int> sortCoordinates(
            std::vector<xml::Node*> &pinCoorNodes,
            xml::Node *checkPointNode
        );
};

#endif // __KML_SORTER_H__