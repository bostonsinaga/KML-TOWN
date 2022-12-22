#ifndef __KML_TWINS_CHECKER_H__
#define __KML_TWINS_CHECKER_H__

class TwinsChecker {
    public:
        TwinsChecker() {}

        xml::Node *findPins(
            xml::Node *kmlNode,
            std::string meterRadiusRateString = ""
        );

        xml::Node *findPaths(
            xml::Node *kmlNode,
            std::string meterRadiusRateString = ""
        );

        xml::Node *insertFoundPlacemarks(
            xml::Node *kmlNode,
            std::vector<int> *matchedIndexes, // int vector array (has 2 members)
            std::vector<xml::Node*> &placemarkNodes,
            std::string placemarksType
        );
};

#endif // __KML_TWINS_CHECKER_H__