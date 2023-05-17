#ifndef __KML_GENERAL_H__
#define __KML_GENERAL_H__

/* REUSABLE FUNCTIONS FOR ALL KML TOOLS */

class General {
    public:
        static int checkCompass(std::string axisStr);
        static xml::Node *searchMainFolder(xml::Node *kmlNode);
        static xml::Node *getRootDocument(xml::Node *kmlNode);
        static std::string getRootDocumentName(xml::Node *kmlNode);

        static void fillWithPlacemarks(
            xml::Node *containerNode,   // eg. 'Folder' or 'Document' node
            std::string coordinatesTypeName,    // eg. 'Point' or 'LineString'
            std::vector<xml::Node*> &pinVec,    // should be empty
            std::vector<xml::Node*> &pinsCoorNodeVec    // should be empty
        );

        static void putOnTopFolder(
            xml::Node *containerFolder,
            std::vector<xml::Node*> nodeVec,
            std::vector<std::string> additionalPriorityNameList = {}  // multiple appearance
        );

        static void insertEditedPlacemarksIntoFolder(
            xml::Node *existContainerNode,  // don't remove this node
            xml::Node *newContainerNode,
            std::vector<xml::Node*> &placemarks, // or any node
            const std::vector<std::string> &noticeFuncName,
            std::string typeStr
        );

        static bool logEditedPlacemarks(
            std::string typeStr,
            // (BELOW! set empty string to disable / using custom notification)
            std::vector<std::string> noticeFuncName,
            std::vector<xml::Node*> &placemarks,
            xml::Node *containerNode
        );

        static void setKMLDocumentName(xml::Node *kmlNode, std::string fileDir_out);
        
        static void cleanFolders(
            xml::Node *mainFolderNode,
            xml::Node *classifiedFolder,
            std::vector<xml::Node*> &newFolderVec
        );
};

#endif // __KML_GENERAL_H__