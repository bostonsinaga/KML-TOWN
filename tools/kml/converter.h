#ifndef __KML_CONVERTER_H__
#define __KML_CONVERTER_H__

class Converter {
    public:
        /* note: the tool functions take only two stringified axis */

        enum {
            LAT_LNG_SEPARATE_FLAG_IN,
            LNG_LAT_SEPARATE_FLAG_IN,
            LAT_LNG_SEPARATE_FLAG_OUT,
            LNG_LAT_SEPARATE_FLAG_OUT
        };

        std::vector<std::string> separateCoor(
            std::string coorStr,
            int separateFlagIn,
            int separateFlagOut
        ) {
            bool isComma = false;
            std::string latStr = "", lngStr = "";

            for (auto &ch : coorStr) {
                if (ch == ',' || ch == ' ') isComma = true;
                else {
                    if (separateFlagIn == LAT_LNG_SEPARATE_FLAG_IN) {
                        if (isComma) {
                            lngStr += ch;
                        }
                        else latStr += ch;
                    }
                    else if (separateFlagIn == LNG_LAT_SEPARATE_FLAG_IN) {
                        if (isComma) {
                            latStr += ch;
                        }
                        else lngStr += ch;
                    }
                }
            }

            if (separateFlagOut == LAT_LNG_SEPARATE_FLAG_OUT) {
                return std::vector<std::string>{latStr, lngStr};
            }
            else if (separateFlagOut == LNG_LAT_SEPARATE_FLAG_OUT) {
                return std::vector<std::string>{lngStr, latStr};
            }

            return std::vector<std::string>{"", ""};
        }

        std::vector<std::string> convertCoor_decimalDegree(
            std::vector<std::string> coorStrVec,
            int separateFlagIn,
            int separateFlagOut
        ) {
            int ctr = 0;
            double coor, minNum;
            std::string deg, min, sec;

            for (auto &str : coorStrVec) {

                coor = std::stod(str);
                deg = std::to_string(std::abs(coor)) + "\xB0";
                minNum = (coor - std::abs(coor)) * 60.0;
                sec = std::to_string((minNum - std::abs(minNum)) * 60.0) + "\"";
                min = std::to_string(std::abs(minNum)) + "'";
                str = deg + min + sec;

                if (separateFlagIn == LAT_LNG_SEPARATE_FLAG_IN) {
                    if (ctr == 0) { // latitude
                        if (coor < 0) str += "S";
                        else str += "N";
                    }
                    else { // longitude
                        if (coor < 0) str += "W";
                        else str += "E";
                    }
                }
                else if (separateFlagIn == LNG_LAT_SEPARATE_FLAG_IN) {
                    if (ctr == 0) { // longitude
                        if (coor < 0) str += "W";
                        else str += "E";
                    }
                    else { // latitude
                        if (coor < 0) str += "S";
                        else str += "N";
                    }
                }
                
                ctr++;
            }

            swapCoorStringVector(&coorStrVec, separateFlagIn, separateFlagOut);
            return coorStrVec;
        }

        std::vector<std::string> convertCoor_degreeDecimal(
            std::vector<std::string> coorStrVec,
            int separateFlagIn,
            int separateFlagOut
        ) {
            int ctr = 0;
            double coor;

            for (auto &str : coorStrVec) {

                size_t deg_startCt = str.find("\xB0"),
                    min_startCt = str.find("'"),
                    sec_startCt = str.find("\"");

                std::string formErrorMessage = "TXT-TOOL-> form error of degree coordinate\n";

                if (deg_startCt == std::string::npos ||
                    min_startCt == std::string::npos ||
                    sec_startCt == std::string::npos
                ) {
                    std::cerr << formErrorMessage;
                    break;
                }
                else {
                    std::string degStr = "", minStr = "", secStr = "";

                    for (int i = deg_startCt - 1; i >= 0; i--) {
                        degStr += str.at(i);
                    }

                    for (int i = min_startCt - 1; i > deg_startCt; i--) {
                        minStr += str.at(i);
                    }

                    for (int i = sec_startCt - 1; i > min_startCt; i--) {
                        secStr += str.at(i);
                    }

                    // must be at 'int' converted 'char' of numbers interval
                    if (int(degStr.front()) >= 48 && int(degStr.front()) <= 57 &&
                        int(minStr.front()) >= 48 && int(minStr.front()) <= 57 &&
                        int(secStr.front()) >= 48 && int(secStr.front()) <= 57
                    ) {
                        int deg = std::stoi(degStr),
                            min = std::stoi(minStr);
                        
                        double sec = std::stod(secStr);

                        // PRODUCT
                        coor += (sec / 60.0) + min;
                        coor += (min / 60.0) + deg;
                    }
                    else {
                        std::cerr << formErrorMessage;
                        break;
                    }
                }

                if (separateFlagIn == LAT_LNG_SEPARATE_FLAG_IN) {
                    if (ctr == 0) { // latitude
                        if (str.back() == 'S' || str.back() == 's') {
                            coor *= -1;
                        }
                    }
                    else { // longitude
                        if (str.back() == 'W' || str.back() == 'B' ||
                            str.back() == 'w' || str.back() == 'b'
                        ) {
                            coor *= -1;
                        }
                    }
                    str = std::to_string(coor);
                }
                else if (separateFlagIn == LNG_LAT_SEPARATE_FLAG_IN) {
                    if (ctr == 0) { // latitude
                        if (str.back() == 'W' || str.back() == 'B' ||
                            str.back() == 'w' || str.back() == 'b'
                        ) {
                            coor *= -1;
                        }
                    }
                    else { // longitude
                        if (str.back() == 'S' || str.back() == 's') {
                            coor *= -1;
                        }
                    }
                    str = std::to_string(coor);
                }

                ctr++;
            }

            swapCoorStringVector(&coorStrVec, separateFlagIn, separateFlagOut);
            return coorStrVec;
        }

    private:
        void swapCoorStringVector(
            std::vector<std::string> *coorStrVec_ptr,
            int separateFlagIn,
            int separateFlagOut
        ) {
            if ((separateFlagOut == LAT_LNG_SEPARATE_FLAG_OUT &&
                separateFlagIn == LNG_LAT_SEPARATE_FLAG_IN) ||
                (separateFlagOut == LNG_LAT_SEPARATE_FLAG_OUT &&
                separateFlagIn == LAT_LNG_SEPARATE_FLAG_IN)
            ) {
                std::string buffer = coorStrVec_ptr->at(0);
                coorStrVec_ptr->at(0) = coorStrVec_ptr->at(1);
                coorStrVec_ptr->at(1) = buffer;
            }
        }
};

#endif // __KML_CONVERTER_H__