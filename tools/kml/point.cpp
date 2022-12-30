#ifndef __KML_POINT_CPP__
#define __KML_POINT_CPP__

#include "point.h"

Point::Point(double x_in, double y_in) {
    x = x_in;
    y = y_in;
}

// from string coordinate
/* ('lng-lat' decimal 'coorString' will be swapped) */
Point::Point(std::string coorString) {

    // overload space removement (only as separator allowed) //

    std::vector<int> spaceIndexes;
    bool isHasCommaSeparator = false;
    int CTR = 0;

    for (auto &ch : coorString) {
        if (ch == ' ') {
            spaceIndexes.push_back(CTR);
        }
        else if (ch == ',') {
            isHasCommaSeparator = true;
        }
        CTR++;
    }

    int nearestSeparatorSpaceIndex = 0;
    if (!isHasCommaSeparator) {
        for (auto &index : spaceIndexes) {
            if (std::abs(int(index - coorString.size() / 2))
                < std::abs(int(nearestSeparatorSpaceIndex - coorString.size() / 2))
            ) {
                nearestSeparatorSpaceIndex = index;
            }
        }
    }

    int spaceIndexReduceRate = 0;
    for (auto &index : spaceIndexes) {

        if (index - spaceIndexReduceRate
            != nearestSeparatorSpaceIndex - spaceIndexReduceRate
            ||
            isHasCommaSeparator
        ) {
            coorString.erase(
                coorString.begin() + index - spaceIndexReduceRate,
                coorString.begin() + index - spaceIndexReduceRate + 1
            );
            spaceIndexReduceRate++;
        }
    }

    // axis handle //

    bool isInvalidAxis = false;
    int coorStringCommaCount = mini_tool::getInStringCharCount(coorString, ',');
    int coorStringSpaceCount = mini_tool::getInStringCharCount(coorString, ' ');

    // cut more then 2 axis (>= 3D)
    if (coorStringCommaCount >= 2 || coorStringSpaceCount >= 2) {
        char chFind = ',';
        if (coorStringSpaceCount >= 2) chFind = ' ';

        // search the last separator and cut the rest
        size_t separatorDex = coorString.find(chFind);
        for (auto &ch : coorString) {
            size_t testDex = coorString.find(chFind, separatorDex + 1);
            if (testDex != std::string::npos) {
                separatorDex = testDex;
            }
            else break;
        }

        coorString.erase(
            coorString.begin() + separatorDex,
            coorString.end()
        );
    }
    // one axis error (1D)
    else if (coorStringCommaCount == 0 && coorStringSpaceCount == 0) {
        std::cerr << "KML-> Point warning. Coordinate has only one axis. Default set to zero point\n";
        isInvalidAxis = true;
    }

    // convert degree into decimal coordinate //

    if (!isInvalidAxis) {
        Converter converter;

        // separate strings coordinate into 'Point'
        std::vector<std::string> coorStringVec = (
            converter.separateCoordinate(coorString)
        );

        for (auto &coorStr : coorStringVec) {
            if (coorStr.front() == char(DEG_INT_CHAR_PT1) ||
                (!mini_tool::isANumber(coorStr.front()) &&
                coorStr.front() != '-')
            ) {
                coorStr.at(0) = '-';
            }
        }
        
        // compass rate for swapping purpose
        int compassRate = General().checkCompass(coorStringVec.at(X));
        if (std::abs(compassRate) == 1) { // horizontal
            coorStringVec = converter.convertCoor_degreeDecimal(
                coorStringVec,
                converter.LNG_LAT_SEPARATE_FLAG_IN,
                converter.LNG_LAT_SEPARATE_FLAG_OUT
            );
        }
        else if (std::abs(compassRate) == 2) { // vertical
            coorStringVec = converter.convertCoor_degreeDecimal(
                coorStringVec,
                converter.LAT_LNG_SEPARATE_FLAG_IN,
                converter.LNG_LAT_SEPARATE_FLAG_OUT
            );
        }
        /*
        *   '0' 'compassRate' considered by default as 'latitude' and 'longitude'
        *   (this should be a decimal coordinate)
        *   for 'cropper' input, there was notice message
        *   for inputting as format above
        */
        else {
            /* will be swapped back in node parameter constructor */
            converter.swapCoorStringVector(
                &coorStringVec,
                converter.LAT_LNG_SEPARATE_FLAG_IN,
                converter.LNG_LAT_SEPARATE_FLAG_OUT
            );
        }
        
        x = std::stod(coorStringVec.at(X));
        y = std::stod(coorStringVec.at(Y));
    }
}

// the node must has coordinate string (for single coordinate)
Point::Point(xml::Node *coorNode) {
    if (coorNode->getName() == "coordinates") {
        Point pt = Point(coorNode->getInnerText());
        /*
        *   invert the 'pt' because default 'latitude' and 'longitude' input format consideration
        *   (see 'compassRate' in string parameter constructor)
        */
        x = pt.y;
        y = pt.x;
    }
}

Point Point::operator+(Point addPt) {
    return Point(x + addPt.x, y + addPt.y);
}

Point Point::operator-(Point subtPt) {
    return Point(x - subtPt.x, y - subtPt.y);
}

Point Point::operator*(Point multPt) {
    return Point(x * multPt.x, y * multPt.y);
}

Point Point::operator/(Point divPt) {
    if (divPt.x == 0) {
        divPt = Point(0.000001, divPt.y);
    }

    if (divPt.y == 0) {
        divPt = Point(divPt.x, 0.000001);
    }

    return Point(x / divPt.x, y / divPt.y);
}

// expected input: '180,-90,0 -180,90,0'
std::vector<Point> Point::getPathPointsFromString(std::string coorStr) {
    
    std::vector<Point> retPoints;
    std::string coorStrBuff = "";
    int commaCount = 0;

    int ctr = 0;
    for (auto &ch : coorStr) {
        if (ch == ',') {
            commaCount++;
            coorStrBuff += ch;
        }
        else if (ch == ' ' && commaCount >= 2) {
            retPoints.push_back(Point(coorStrBuff));
            commaCount = 0;
            coorStrBuff = "";
        }
        else if (ctr == coorStr.size() - 1 && commaCount >= 2) {
            retPoints.push_back(Point(coorStrBuff));
        }
        else if (mini_tool::isANumber(ch) || ch == '.') coorStrBuff += ch;
        ctr++;
    }

    return retPoints;
}

std::string Point::stringify(bool isAddZeroAltitude) {
    return std::string(
        std::to_string(x) + "," + std::to_string(y) + (isAddZeroAltitude ? ",0" : "")
    );
}

#endif // __KML_POINT_CPP__