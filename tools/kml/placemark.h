#ifndef __KML_PLACEMARK_H__
#define __KML_PLACEMARK_H__

class Placemark {
    public:
        static void pinsPath(
            xml::Node *kmlNode,
            std::vector<xml::Node*> &sortedPinNodes
        );

        static void pinsPathSegments(
            xml::Node *kmlNode,
            std::vector<xml::Node*> &sortedPinNodes,
            bool isFolderize
        );

        template<typename TYPE_T>

        static LD getHaversineDistance(
            TYPE_T startPt,
            TYPE_T endPt
        );

        static int getPathDistance(std::vector<Point> &points);
        
        static bool setPathDistance(
            xml::Node *kmlNode,
            bool isOnlyGetInfo
        );

        // return 'true' if a path removed
        static bool removePathsByDistance(
            xml::Node *kmlNode,
            LD limitDistance,
            bool isOverDistance
        );

        // get placemark name or its folder name (closest parent)
        static std::string getName(xml::Node *placemarkNode);

        // print placemark name or its folder name (closest parent)
        static void logName(
            xml::Node *placemarkNode,
            bool isResetCtr = false
        );

        // get inner text of certain node data (alternative for 'getName' method)
        static std::string getDataText(
            xml::Node *placemarkNode,
            std::string dataNodeName
        );

        static void setDataText(
            xml::Node *placemarkNode,
            std::string dataNodeName,
            std::string valStr,
            bool isAddingOnly = false,  // [default] replace data
            bool isCreateNew = false    // [default] only process existing node
        );

        // used in method that need '--include-folders'
        static void includeFolder(
            xml::Node *placemarkNode,
            xml::Node *newFolderNode  // in 'classify' this will be a folder named 'type-style'
        );

        static Point convertDegreeToMeterPoint(Point &ptIn);
};

#endif // __KML_PLACEMARK_H__
