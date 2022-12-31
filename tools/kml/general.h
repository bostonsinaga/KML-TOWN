#ifndef __KML_GENERAL_H__
#define __KML_GENERAL_H__

/* REUSABLE FUNCTIONS FOR ALL KML TOOLS */

class General {
    public:
        General() {}

        int checkCompass(std::string axisStr);
        xml::Node *searchMainFolder(xml::Node *kmlNode);
        xml::Node *getRootDocument(xml::Node *kmlNode);
        std::string getRootDocumentName(xml::Node *kmlNode);

        void fillWithPins(
            xml::Node *containerNode,
            std::vector<xml::Node*> &placemarkVec,          // should be empty
            std::vector<xml::Node*> &placemarksCoorNodeVec  // should be empty
        );

        void insertEditedPlacemarksIntoFolder(
            xml::Node *prevContainerNode,
            xml::Node *newContainerNode,
            std::vector<xml::Node*> &placemarks, // or any node
            const std::vector<std::string> &noticeFuncName,
            std::string typeStr
        );

        bool logEditedPlacemarks(
            std::string typeStr,
            std::vector<std::string> noticeFuncName,
            std::vector<xml::Node*> &placemarks,
            xml::Node *containerNode
        );

        void setKMLDocumentName(xml::Node *kmlNode, std::string fileDir_out);
};

#endif // __KML_GENERAL_H__