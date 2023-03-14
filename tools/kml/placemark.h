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
            bool isAbove
        );

        // read placemark name or its folder name (closest parent)
        static std::string getName(xml::Node *placemark);
        static void logName(xml::Node *placemark);
};

#endif // __KML_PLACEMARK_H__
