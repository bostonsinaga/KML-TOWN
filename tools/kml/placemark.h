#ifndef __KML_PLACEMARK_H__
#define __KML_PLACEMARK_H__

class Placemark {
    public:
        void pinsPath(
            xml::Node *kmlNode,
            std::vector<xml::Node*> &sortedPinNodes
        );
};

#endif // __KML_PLACEMARK_H__
