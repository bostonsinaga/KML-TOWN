#ifndef __KML_CROPPER_H__
#define __KML_CROPPER_H__

// CROPPING BY RECTANGULAR GENERATED FROM START TO END POINT //

class Cropper {
    public:
        Cropper() {}
        Cropper(bool isPrintNotification, Menu *menu) {
            if (isPrintNotification) {
                menu->setNotification(
                    std::string("KML-TOWN-> Make sure to enter only 2D coordinate\n") +
                    std::string("           of 'latitude' and 'longitude' sequentially!\n")
                );
            }
        }

        void cutPins(
            xml::Node *pinsContainerNode,
            Point startPt, // decimal coordinate
            Point endPt    // decimal coordinate
        ) {
            std::vector<xml::Node*>
                pinNodes,
                pinCoorNodes;

            fillWithPlacemarks(pinsContainerNode, &pinNodes, &pinCoorNodes);
 
            // new pins from cropped (inside rectangular)
            std::vector<xml::Node*> croppedPins;

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
                    croppedPins.push_back(pinNodes.at(ctr));
                }

                ctr++;
            }

            // insert into a different folder
            insertEditedPlacemarksIntoFolder(
                CROP_COMMAND_WORKING_FOLDER,
                pinsContainerNode,
                &croppedPins,
                {"Cropping", "Crop"}
            );
        }
};

#endif // __KML_CROPPER_H__