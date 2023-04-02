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

        void fillWithPlacemarks(
            xml::Node *containerNode,   // eg. 'Folder' or 'Document' node
            std::string coordinatesTypeName,    // eg. 'Point' or 'LineString'
            std::vector<xml::Node*> &pinVec,    // should be empty
            std::vector<xml::Node*> &pinsCoorNodeVec    // should be empty
        );

        void putOnTopFolder(
            xml::Node *containerFolder,
            std::vector<xml::Node*> nodeVec
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
            // (BELOW! set empty string to disable / using custom notification)
            std::vector<std::string> noticeFuncName,
            std::vector<xml::Node*> &placemarks,
            xml::Node *containerNode
        );

        void setKMLDocumentName(xml::Node *kmlNode, std::string fileDir_out);
};

#endif // __KML_GENERAL_H__