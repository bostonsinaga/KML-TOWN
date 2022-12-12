#ifndef __KML_H__
#define __KML_H__

/*
*   there's only be one element as child of 'kml' element
*   otherwise, the Google Earth will reject it 
*/

namespace kml {
    int checkCompass(std::string axisStr);
    
    #include "style-strings.h"
    #include "converter.h"
    #include "point.h"
    #include "builder.h"

    xml::Node *searchMainFolder(xml::Node *kmlNode);

    void fillWithPlacemarks(
        xml::Node *containerNode,
        std::vector<xml::Node*> *placemarkVec,                     // should be empty
        std::vector<xml::Node*> *placemarksCoorNodeVec = nullptr,  // should be empty
        bool isPin = true
    );

    void insertEditedPlacemarksIntoFolder(
        std::string folderName,
        xml::Node *containerNode,
        std::vector<xml::Node*> *placemarks, // or any node
        const std::vector<std::string> &noticeFuncName
    );

    void setKMLDocumentName(xml::Node *kmlNode, std::string fileDir_out);

    #include "cropper.h"
    #include "sorter.h"
}

#endif // __KML_H__