#ifndef __KML_CROPPER_H__
#define __KML_CROPPER_H__

// CROPPING BY RECTANGULAR AREA FROM START TO END POINT //

class Cropper {
    public:
        Cropper() {}

        void printNotification(Menu &menu);

        std::vector<xml::Node*> cutPins(
            xml::Node *pinsContainerNode,
            Point startPt,  // decimal coordinate
            Point endPt,    // decimal coordinate
            bool isFolderInsertion
        );

        std::vector<xml::Node*> cutPaths(
            xml::Node *pathsContainerNode,
            Point startPt,  // decimal coordinate
            Point endPt,    // decimal coordinate
            bool isFolderInsertion
        );
};

#endif // __KML_CROPPER_H__