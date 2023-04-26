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

        template<typename TYPE_T>

        LD getHaversineDistance(
            TYPE_T startPt,
            TYPE_T endPt
        );

        int getPathDistance(std::vector<Point> &points);
        
        bool setPathDistance(
            xml::Node *kmlNode,
            bool isOnlyGetInfo
        );

        void removePathsByDistance(
            xml::Node *kmlNode,
            LD limitDistance,
            bool isOverDistance
        );

        // read placemark name or its folder name (closest parent)
        static std::string getName(xml::Node *placemarkNode);
        static void logName(xml::Node *placemarkNode, bool isResetCtr = false);

        // get inner text of certain node data (alternative for 'getName' method)
        static std::string getDataText(
            xml::Node *placemarkNode,
            std::string dataNodeName
        );

        // used in method that need '--include-folders'
        void includeFolder(
            xml::Node *placemarkNode,
            xml::Node *folderNode,
            int testIndex = 0,
            bool isResetStatic = false
        );

        Point convertDegreeToMeterPoint(Point &ptIn);
};

#endif // __KML_PLACEMARK_H__
