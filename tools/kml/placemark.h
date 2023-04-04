#ifndef __KML_PLACEMARK_H__
#define __KML_PLACEMARK_H__

class Placemark {
    public:
        Placemark() {}
        
        void pinsPath(
            xml::Node *kmlNode,
            std::vector<xml::Node*> &sortedPinNodes
        );

        void pinsPathSegments(
            xml::Node *kmlNode,
            std::vector<xml::Node*> &sortedPinNodes,
            bool isFolderize
        );

        int getPathDistance(std::vector<Point> &points);
        
        bool setPathDistance(
            xml::Node *kmlNode,
            bool isOnlyGetInfo
        );

        void removePathsByDistance(
            xml::Node *kmlNode,
            double limitDistance,
            bool isOverDistance
        );

        // read placemark name or its folder name (closest parent)
        static std::string getName(
            xml::Node *placemark,
            bool isWithParentName = true,
            std::string defaultEmptyString = "noname"
        );

        static void logName(xml::Node *placemark, bool isResetCtr = false);
        static std::string getDescription(xml::Node *placemark);
};

#endif // __KML_PLACEMARK_H__
