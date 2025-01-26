#ifndef __KML_CONVERTER_H__
#define __KML_CONVERTER_H__

#include "style-strings.h"

/**
 * Note:
 * This class is not a '.txt' to '.kml' converter
 * please use 'txt/scanner' and 'kml/builder' instead
 * this class just for coordinate converter from 'degree' to 'decimal' and vice versa
 */

namespace kml {

class Converter {
    public:
        /* note: the tool functions take only two stringified axis */

        enum {
            LAT_LNG_SEPARATE_FLAG_IN,
            LNG_LAT_SEPARATE_FLAG_IN,
            LAT_LNG_SEPARATE_FLAG_OUT,
            LNG_LAT_SEPARATE_FLAG_OUT
        };

        std::vector<std::string> separateCoordinate(
            std::string coorStr,
            int separateFlagIn = -1, // '-1' means no position swap
            int separateFlagOut = -1
        );

        // return value e.g: '{1°10'100"S,100°10'100"E}' (no minus sign '-')
        std::vector<std::string> convertCoor_decimalDegree(
            std::vector<std::string> coorStrCouple,
            int separateFlagIn,
            int separateFlagOut
        );

        std::vector<std::string> convertCoor_degreeDecimal(
            std::vector<std::string> coorStrCouple, // can be partly empty
            int separateFlagIn,
            int separateFlagOut
        );

        void swapCoorStringVector(
            std::vector<std::string> *coorStrCouple_ptr,
            int separateFlagIn,
            int separateFlagOut
        );        
};
}

#endif // __KML_CONVERTER_H__