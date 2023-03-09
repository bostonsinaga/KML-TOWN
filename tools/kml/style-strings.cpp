#ifndef __KML_STYLE_STRINGS_CPP__
#define __KML_STYLE_STRINGS_CPP__

#include "style-strings.h"

int StyleStrings::getPinIconNamedIndex(std::string &pinIconNamed) {
    for (int i = 0;
        i < sizeof(pinIconUrlArray) / sizeof(std::string);
        i++
    ) {
        if (pinIconUrlArray[i].find(pinIconNamed) != std::string::npos) {
            return i;
        }
    }
    return -1;
}

std::string StyleStrings::getPinIconNamedUrl(std::string &pinIconNamed) {
    int index = getPinIconNamedIndex(pinIconNamed);
    if (index != -1) {
        return pinIconUrlArray[index];
    }
    return "";
}

int StyleStrings::getPinTyleFlag(std::string &pinIconNamed) {
    std::string url = getPinIconNamedUrl(pinIconNamed);

    if (url != "") {
        
        if (url.find("/pushpin/") != std::string::npos) {
            return PUSHPIN_PIN_TYPE_FLAG;
        }
        else if (url.find("/paddle/") != std::string::npos) {
            return PADDLE_PIN_TYPE_FLAG;
        }
        else if (url.find("/shapes/") != std::string::npos) {
            return SHAPES_PIN_TYPE_FLAG;
        }
    }
    return -1;
}

// identify if an icon
bool StyleStrings::isAnIconUrl(std::string &testStr) {
    for (int i = 0; i < pinIconUrlArray_count; i++) {
        if (mini_tool::cutFileDirName(pinIconUrlArray[i]) == testStr) {
            return true;
        }
    }
    return false;
}

bool StyleStrings::isAColorCode(std::string &testStr) {
    for (int i = 0; i < colorCodeArray_count; i++) {
        if (testStr == colorCodeArray[i]) {
            return true;
        }
    }
    return false;
}

// path's color codes from name
std::string StyleStrings::getPathColorCode(std::string pathColorNamed) {
    if (pathColorNamed == "red") {
        return colorCodeArray[0];
    }
    else if (pathColorNamed == "yellow") {
        return colorCodeArray[1];
    }
    else if (pathColorNamed == "magenta") {
        return colorCodeArray[2];
    }
    else if (pathColorNamed == "chocolate") {
        return colorCodeArray[3];
    }
    else if (pathColorNamed == "green") {
        return colorCodeArray[4];
    }
    else if (pathColorNamed == "orange") {
        return colorCodeArray[5];
    }
    else if (pathColorNamed == "purple") {
        return colorCodeArray[6];
    }
    else if (pathColorNamed == "white") {
        return colorCodeArray[7];
    }
    else if (pathColorNamed == "blue") {
        return colorCodeArray[8];
    }
    else if (pathColorNamed == "cyan") {
        return colorCodeArray[9];
    }
    else if (pathColorNamed == "black") {
        return colorCodeArray[10];
    }
    else if (pathColorNamed == "gray") {
        return colorCodeArray[11];
    }
    else return colorCodeArray[7];
    return "";
}

// get style string data (pin with 'href', path with 'color-code')
std::string StyleStrings::getPlacemarkStyleData(xml::Node *placemark) {

    xml::Node
        *kmlNode = nullptr,
        *styleUrlNode = nullptr;

    if (placemark) {
        kmlNode = placemark->getRoot();
        styleUrlNode = placemark->getFirstDescendantByName("styleUrl");
    }

    if (kmlNode && styleUrlNode) {
        std::string styleName = styleUrlNode->getInnerText();
        styleName = styleName.substr(1);

        for (auto &styleMapNode : kmlNode->getDescendantsByName("StyleMap", true)) {
            for (auto &att1 : *styleMapNode->getAttributes()) {
                
                if (att1.getName() == "id" && att1.getValue() == styleName) {

                    /* search only for normal style */

                    std::string normalPairStyleName = (
                        styleMapNode
                            ->getFirstDescendantByName("Pair")
                            ->getFirstDescendantByName("styleUrl")->getInnerText()
                    );

                    normalPairStyleName = normalPairStyleName.substr(1);

                    for (auto &styleNode : kmlNode->getDescendantsByName("Style", true)) {
                        for (auto &att2 : *styleNode->getAttributes()) {
                            if (att2.getName() == "id" && att2.getValue() == normalPairStyleName) {
                                
                                // pin
                                if (placemark->getFirstDescendantByName("Point")) {

                                    xml::Node *hrefNode = styleNode->getFirstDescendantByName("href");
                                    std::string pinUrlStr = (
                                        hrefNode ? hrefNode->getInnerText() : pinIconUrlArray[0]
                                    );

                                    std::string urlStr = mini_tool::cutFileDirName(pinUrlStr);

                                    size_t found = urlStr.find(".png");
                                    if (found != std::string::npos) {
                                        urlStr = urlStr.substr(0, found);
                                    }

                                    return urlStr;
                                }
                                //path 
                                else if (placemark->getFirstDescendantByName("LineString")) {
                                    xml::Node *colorNode =  styleNode->getFirstDescendantByName("color");
                                    if (colorNode) return colorNode->getInnerText();
                                    return "ffffffff";
                                }
                            }
                        }
                    }

                    break;
                }
            }
        }
    }
    // no 'styleUrl', set to default
    else if (placemark->getFirstDescendantByName("Point")) {
        // pin
        return "ylw-pushpin";
    }
    //path 
    return "ffffffff";
}

#endif // __KML_STYLE_STRINGS_CPP__