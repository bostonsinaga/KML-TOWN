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
bool StyleStrings::isAnIconName(std::string &pinIconNamed) {
    for (int i = 0; i < pinIconUrlArray_count; i++) {
        if (pinIconNamed == pinIconUrlArray[i]) {
            return true;
        }
    }
    return false;
}

// path's color codes from name
std::string StyleStrings::getPathColorCode(std::string pathColorNamed) {
    if (pathColorNamed == "red") {
        return "ff0000ff";
    }
    else if (pathColorNamed == "yellow") {
        return "ff00ffff";
    }
    else if (pathColorNamed == "magenta") {
        return "ffff00ff";
    }
    else if (pathColorNamed == "chocolate") {
        return "ff385294";
    }
    else if (pathColorNamed == "green") {
        return "ff00ff00";
    }
    else if (pathColorNamed == "orange") {
        return "ff007fff";
    }
    else if (pathColorNamed == "purple") {
        return "ffbf007f";
    }
    else if (pathColorNamed == "white") {
        return "ffffffff";
    }
    else if (pathColorNamed == "blue") {
        return "ffff0000";
    }
    else if (pathColorNamed == "cyan") {
        return "ffffff00";
    }
    else if (pathColorNamed == "black") {
        return "ff000000";
    }
    else if (pathColorNamed == "gray") {
        return "ff7f7f7f";
    }
    else return "ffffffff";
    return "";
}

#endif // __KML_STYLE_STRINGS_CPP__