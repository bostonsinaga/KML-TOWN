#ifndef __KML_BUILDER_CPP__
#define __KML_BUILDER_CPP__

#include "builder.h"

xml::Node *Builder::getFolder(
    std::string name,
    bool isOpen
) {
    if (name == "") name = "Folder";

    std::stringstream placemark_strStream;
    placemark_strStream
        << "<Folder>"
        << "<name>" << name  << "</name>"
        << "<open>" << std::to_string(isOpen) << "</open>"
        << "</Folder>";

    return xmlReader.parse(
        placemark_strStream.str(),
        "Folder [in runtime element]"
    );
}

// return one style map set of pins
xml::Node *Builder::getPinStyleMap(
    std::string *styleMapId_hook,
    std::string pinIconNamed,
    std::string pinIconScaleStr
) {
    std::string
        normalId, highlightId,
        normalScale, highlightScale,
        styleMapId, hotspotPos[2];

    std::stringstream styleMap_strStream;

    std::string pinIconUrl;
    int pinIconTypeFlag;

    // using default 'ylw-pushpin'
    if (pinIconNamed == "" ||
        !styleStrings.isAnIconName(pinIconNamed)
    ) {
        pinIconUrl = styleStrings.pinIconUrlArray[0];
        pinIconNamed = mini_tool::cutFileDirName(pinIconUrl);
    }
    // using custom
    else {
        pinIconUrl = styleStrings.getPinIconNamedUrl(pinIconNamed);
    }

    // remove '.png' extension string
    size_t containsPNGDex = pinIconNamed.find(".png");
    if (containsPNGDex != std::string::npos) {
        pinIconNamed = pinIconNamed.substr(0, containsPNGDex);
    }

    normalId = "sn_" + pinIconNamed;
    highlightId = "sh_" + pinIconNamed;
    styleMapId = "msn_" + pinIconNamed;
    *styleMapId_hook =  styleMapId;

    pinIconTypeFlag = styleStrings.getPinTyleFlag(pinIconNamed);

    switch (pinIconTypeFlag) {
        case styleStrings.PUSHPIN_PIN_TYPE_FLAG: {
            normalScale = pushpinScaleNormal;
            highlightScale = pushpinScaleHighlight;
            hotspotPos[0] = pushpinHotspotPos[0];
            hotspotPos[1] = pushpinHotspotPos[1];
        break;}
        case styleStrings.PADDLE_PIN_TYPE_FLAG: {
            normalScale = paddleScaleNormal;
            highlightScale = paddleScaleHighlight;
            hotspotPos[0] = paddleHotspotPos[0];
            hotspotPos[1] = paddleHotspotPos[1];
        break;}
        case styleStrings.SHAPES_PIN_TYPE_FLAG: {
            normalScale = shapesScaleNormal;
            highlightScale = shapesScaleHighlight;
            hotspotPos[0] = shapesHotspotPos[0];
            hotspotPos[1] = shapesHotspotPos[1];
        break;}
    }

    // mutliply string's float value
    auto multStrFlt = [=](
        std::string *defSclStr,
        std::string *cusSclStr
    ) {
        *defSclStr = std::to_string(
            std::stof(*defSclStr) *
            std::stof(*cusSclStr)
        );
        
        *defSclStr = defSclStr->substr(
            0, defSclStr->find('.') + 3 // takes only two digits behind decimal point
        );
    };

    multStrFlt(&normalScale, &pinIconScaleStr);
    multStrFlt(&highlightScale, &pinIconScaleStr);
    
    styleMap_strStream
        << "<StyleSet>"
        // normal style
        << "<Style id=\"" << normalId << "\">"
        << "<IconStyle>"
        << "<scale>" << normalScale << "</scale>"
        << "<Icon>"
        << "<href>" << pinIconUrl << "</href>"
        << "</Icon>"
        << "<hotSpot x=\""
            << hotspotPos[0] << "\" y=\""
            << hotspotPos[1] << "\" xunits=\"pixels\" yunits=\"pixels\"/>"
        << "</IconStyle>"
        << "</Style>"
        // highlight style
        << "<Style id=\"" << highlightId << "\">"
        << "<IconStyle>"
        << "<scale>" << highlightScale << "</scale>"
        << "<Icon>"
        << "<href>" << pinIconUrl << "</href>"
        << "</Icon>"
        << "<hotSpot x=\""
            << hotspotPos[0] << "\" y=\""
            << hotspotPos[1] << "\" xunits=\"pixels\" yunits=\"pixels\"/>"
        << "</IconStyle>"
        << "</Style>"
        // style map
        << "<StyleMap id=\"" << styleMapId << "\">"
        << "<Pair>"
        << "<key>normal</key>"
        << "<styleUrl>#" << normalId << "</styleUrl>"
        << "</Pair>"
        << "<Pair>"
        << "<key>highlight</key>"
        << "<styleUrl>#" << highlightId << "</styleUrl>"
        << "</Pair>"
        << "</StyleMap>"
        << "</StyleSet>";

    return xmlReader.parse(
        styleMap_strStream.str(),
        "StyleSet [in runtime element]"
    );
}

// return one style map set of path
xml::Node *Builder::getPathStyleMap(
    std::string *styleMapId_hook,
    std::string pathColorNamed,
    std::string pathThicknessStr
) {
    std::string
        normalId,
        highlightId,
        styleMapId;
        
    std::stringstream styleMap_strStream;

    if (pathColorNamed == "") {
        pathColorNamed = "white";
    }

    std::string pathColorCode = (
        styleStrings.getPathColorCode(pathColorNamed)
    );

    /*
    *   if the name no matched pre-defined color's (but not empty)
    *   (eg. 'foo')
    *   the result id would be 'foo-path'
    */
    pathColorNamed += "-path";

    normalId = "sn_" + pathColorNamed;
    highlightId = "sh_" + pathColorNamed;
    styleMapId = "msn_" + pathColorNamed;
    *styleMapId_hook =  styleMapId;
    
    styleMap_strStream
        << "<StyleSet>"
        // normal style
        << "<Style id=\"" << normalId << "\">"
        << "<LineStyle>"
        << "<color>" << pathColorCode << "</color>"
        << "<width>" << pathThicknessStr << "</width>"
        << "</LineStyle>"
        << "</Style>"
        // highlight style
        << "<Style id=\"" << highlightId << "\">"
        << "<LineStyle>"
        << "<color>" << pathColorCode << "</color>"
        << "<width>" << pathThicknessStr << "</width>"
        << "</LineStyle>"
        << "</Style>"
        // style map
        << "<StyleMap id=\"" << styleMapId << "\">"
        << "<Pair>"
        << "<key>normal</key>"
        << "<styleUrl>#" << normalId << "</styleUrl>"
        << "</Pair>"
        << "<Pair>"
        << "<key>highlight</key>"
        << "<styleUrl>#" << highlightId << "</styleUrl>"
        << "</Pair>"
        << "</StyleMap>"
        << "</StyleSet>";
    
    return xmlReader.parse(
        styleMap_strStream.str(),
        "StyleSet [in runtime element]"
    );
}

xml::Node *Builder::getPin(
    int altitudeAdditionFlag,
    std::string styleMapId,
    std::string decimalCoorStr,
    std::string name,
    std::string description
) {
    if (altitudeAdditionFlag == COORSTR_AUTO_ADD_ALTITUDE) {
        decimalCoorStr += getAltitudeAddition(
            altitudeAdditionFlag,
            decimalCoorStr
        );
    }
    else decimalCoorStr += getAltitudeAddition(altitudeAdditionFlag);

    std::stringstream placemark_strStream;
    placemark_strStream 
        << "<Placemark>"
        << "<name>" << name << "</name>"
        << "<description>" << description << "</description>"
        << "<styleUrl>#" << styleMapId << "</styleUrl>"
        << "<Point>"
        << "<coordinates>" << decimalCoorStr << "</coordinates>"
        << "</Point>"
        <<" </Placemark>";

    return xmlReader.parse(
        placemark_strStream.str(),
        "Placemark [in runtime element]"
    );
}

xml::Node *Builder::getPath(
    int altitudeAdditionFlag,
    std::string styleMapId,
    std::vector<std::string> &coorStrVec,
    std::string name,
    std::string description
) {
    std::string
        decimalCoorStrVec_joinStr = "",
        altAddStr;

    bool isAltAddStrUpdate = false;
    
    if (altitudeAdditionFlag == COORSTR_AUTO_ADD_ALTITUDE) {
        isAltAddStrUpdate = true;
    }
    else {
        altAddStr = getAltitudeAddition(altitudeAdditionFlag);
    }

    for (auto &coorStr : coorStrVec) {

        /* if unsure of comma count (coordinat form) consistency */
        if (isAltAddStrUpdate) {
            altAddStr = getAltitudeAddition(
                altitudeAdditionFlag,
                coorStr
            );
        }

        decimalCoorStrVec_joinStr += coorStr + altAddStr + " ";
    }

    // remove last space
    if (decimalCoorStrVec_joinStr != "") {
        decimalCoorStrVec_joinStr.pop_back();
    }

    std::stringstream placemark_strStream;
    placemark_strStream 
        << "<Placemark>"
        << "<name>" << name << "</name>"
        << "<description>" << description << "</description>"
        << "<styleUrl>#" << styleMapId << "</styleUrl>"
        << "<LineString>"
        << "<tessellate>1</tessellate>"
        << "<coordinates>" << decimalCoorStrVec_joinStr << "</coordinates>"
        << "</LineString>"
        <<" </Placemark>";

    return xmlReader.parse(
        placemark_strStream.str(),
        "Placemark [in runtime element]"
    );
}

xml::Node *Builder::getSkeleton(std::string &docName) {
    if (docName == "") docName = "KML-TOWN";

    std::stringstream placemark_strStream;
    placemark_strStream
        // kml doc
        << "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" "
        << "xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">"
        << "<Document>"
        << "<name>" << docName << "</name>"

            // main folder
            << "<Folder>"
            << "<name>" << docName << "</name>"
            << "<open>1</open>"
            // all stuff branches from here
            << "</Folder>"

        << "</Document>"
        << "</kml>";
    
    return xmlReader.parse(
        placemark_strStream.str(),
        "kml [in runtime element]"
    );
}

void Builder::setTitle(xml::Node *kmlNode, std::string &docName) {
    xml::Node *docNode = getRootDocument(kmlNode);

    if (docNode) {
        xml::Node *nameNode = docNode->getFirstChildByName("name", false);
        if (!nameNode) {
            nameNode = new xml::Node("name", docNode);
        }
        nameNode->setInnerText(docName);
    }

    xml::Node *mainFolderNode = searchMainFolder(kmlNode);

    if (mainFolderNode) {
        xml::Node *nameNode = mainFolderNode->getFirstChildByName("name", false);
        if (!nameNode) {
            nameNode = new xml::Node("name", mainFolderNode);
        }
        nameNode->setInnerText(docName);
    }
}

void Builder::insertStyleMap(xml::Node *kmlNode, xml::Node *styleSetNode) {

    xml::Node *docNode = getRootDocument(kmlNode);
    int childCtr = 0;

    for (auto &child : *docNode->getChildren()) {
        if (child->getName() == "Folder") break;
        childCtr++;
    }

    std::vector<xml::Node*> styleSetNodeVec;

    if (styleSetNode) {
        styleSetNodeVec = styleSetNode->releaseChildren();
        delete styleSetNode;

        for (auto &node : styleSetNodeVec) {
            docNode->addChild(node, childCtr);
            childCtr++; // still pointing to named 'Folder'
        }
    }
}

std::string Builder::getAltitudeAddition(
    int altitudeAdditionFlag,
    std::string coorStrAuto // must be filled if using 'COORSTR_AUTO_ADD_ALTITUDE'
) {
    switch (altitudeAdditionFlag) {

        case COORSTR_AUTO_ADD_ALTITUDE: {
            int commaCount = mini_tool::getInStringCharCount(coorStrAuto, ',');
            if (commaCount == 1) {
                return ",0";
            }
        }
        case COORSTR_ZERO_ADD_ALTITUDE: {
            return ",0";
        }
        case COORSTR_NO_ADD_ALTITUDE: {
            return "";
        }
    }

    return "";
}

#endif // __KML_BUILDER_CPP__