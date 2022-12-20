#ifndef __KML_CROPPER_H__
#define __KML_CROPPER_H__

// CROPPING BY RECTANGULAR AREA FROM START TO END POINT //

class Cropper {
    public:
        Cropper() {}
        Cropper(bool isPrintNotification, Menu *menu);

        std::vector<xml::Node*> cutPins(
            xml::Node *pinsContainerNode,
            Point startPt,  // decimal coordinate
            Point endPt,    // decimal coordinate
            bool isFolderInsertion
        );
};

#endif // __KML_CROPPER_H__