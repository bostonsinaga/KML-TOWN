#ifndef __KML_BUILDER_H__
#define __KML_BUILDER_H__

class Builder {
    public:
        Builder() {}
        
        xml::Node *getFolder(
            std::string name = "",
            bool isOpen = false
        );

        // return one style map set of pins
        xml::Node *getPinStyleMap(
            std::string *styleMapId_hook,
            std::string pinIconNamed = "",
            std::string pinIconScaleStr = "1.0"
        );

        // return one style map set of path
        xml::Node *getPathStyleMap(
            std::string *styleMapId_hook,
            std::string pathColorNamed = "",
            std::string pathThicknessStr = "2.0"
        );

        enum {
            COORSTR_AUTO_ADD_ALTITUDE,
            COORSTR_ZERO_ADD_ALTITUDE,
            COORSTR_NO_ADD_ALTITUDE
        };

        xml::Node *getPin(
            int altitudeAdditionFlag,
            std::string styleMapId,
            std::string decimalCoorStr,
            std::string name = "",
            std::string description = ""
        );

        xml::Node *getPath(
            int altitudeAdditionFlag,
            std::string styleMapId,
            std::vector<std::string> &coorStrVec,
            std::string name = "",
            std::string description = ""
        );

        xml::Node *getSkeleton(std::string &docName);
        void setTitle(xml::Node *kmlNode, std::string &docName);
        void insertStyleMap(xml::Node *kmlNode, xml::Node *styleSetNode);

    private:
        std::string getAltitudeAddition(
            int altitudeAdditionFlag,
            std::string coorStrAuto = ""
        );

        xml::Reader xmlReader;
        StyleStrings styleStrings;

        std::string
            pushpinHotspotPos[2] = {"20", "2"},
            paddleHotspotPos[2] = {"32", "1"},
            shapesHotspotPos[2] = {"0.5", "0"};

        std::string
            pushpinScaleNormal = "1.1", pushpinScaleHighlight = "1.3",
            paddleScaleNormal = "1.1", paddleScaleHighlight = "1.3",
            shapesScaleNormal = "1.2", shapesScaleHighlight = "1.4";
};

#endif // __KML_BUILDER_H__