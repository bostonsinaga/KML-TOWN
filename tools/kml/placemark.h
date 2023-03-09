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

        // log placemark name or its folder name
        static void logName(xml::Node *placemark);
};

#endif // __KML_PLACEMARK_H__
