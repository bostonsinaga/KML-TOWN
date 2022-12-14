#ifndef __KML_H__
#define __KML_H__

/*
*   there's only be one element as child of 'kml' element
*   otherwise, the Google Earth will reject it 
*/

namespace kml {
    int checkCompass(std::string axisStr);
    xml::Node *searchMainFolder(xml::Node *kmlNode);
    xml::Node *getRootDocument(xml::Node *kmlNode);
    
    #include "style-strings.h"
    #include "converter.h"
    #include "point.h"
    #include "builder.h"

    void fillWithPlacemarks(
        xml::Node *containerNode,
        std::vector<xml::Node*> &placemarkVec,           // should be empty
        std::vector<xml::Node*> &placemarksCoorNodeVec,  // should be empty
        bool isProcessingPin
    );

    void insertEditedPlacemarksIntoFolder(
        std::string folderName,
        xml::Node *containerNode,
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

    #include "cropper.h"
    #include "sorter.h"
    #include "placemark.h"
}

#endif // __KML_H__