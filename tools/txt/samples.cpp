#ifndef __TXT_SAMPLES_CPP__
#define __TXT_SAMPLES_CPP__

#include "samples.h"

std::string Samples::testDateTemplate(std::string &textLine) {

    int slashCtr = 0;
    std::string day = "", month = "", year = "", *timePtr = &day;

    /* the template is: (following WhatsApp '.txt' chat export format)
    *  dd / mm / 20yy (indonesia date format)
    */

    // only number and slash sign
    for (auto &CH : textLine) {

        // still searching
        if (CH == '/') {
            slashCtr++;
            if (slashCtr == 1) timePtr = &month;
            else if (slashCtr == 2) timePtr = &year;
        }
        else {
            bool isNumDetected = false;
            if (mini_tool::isANumber(CH)) {
                isNumDetected = true;
                timePtr->push_back(CH);
            }

            // stop searching
            if (year.size() == 2 && slashCtr == 2) {
                return day + "/" + month + "/20" + year;
            }
            // reset
            else if (!isNumDetected) {
                slashCtr = 0;
                day = "";
                month = "";
                year = "";
                timePtr = &day;
            }
        }
    }

    return "";
}

std::string Samples::testGoogleMapsTemplate(std::string &textLine) {

    std::string
        templateStr = "https://maps.google.com/?q=",
        retCoor = "";

    size_t found = textLine.find(templateStr);

    if (found != std::string::npos) {

        for (int chCtr = found + templateStr.size();
                chCtr < textLine.size();
                chCtr++
        ) {
            if (textLine.at(chCtr) != ' ' ||
                textLine.at(chCtr) != '\n'
            ) {
                retCoor += textLine.at(chCtr);
            }
        }
    }

    return retCoor;
}

// sample: [ 10°20'30.4"S 40°30'20.1"E ]
std::string Samples::testDegreeTemplateWithLetter(std::string &textLine) {
    enum {
        div_det,
        axis_det,
        secSg_det,
        sec_det,
        minSg_det,
        min_det,
        degSgNgt80_det,
        degSgNgt62_det,
        deg_det
    };

    bool isAllOff = true, isSearching = false;
    std::vector<bool> isDetected(9, false);
    std::string retCoor = "";

    for (auto &CH : textLine) {
        // end
        if (isDetected.at(div_det) &&
            isDetected.at(axis_det)
        ) {
            break;
        }
        // divide
        else if (
            isDetected.at(axis_det) &&
            (CH == ' ' || CH == ',' || mini_tool::isANumber(CH))
        ) {
            isDetected = std::vector<bool>(9, false);
            isDetected.at(div_det) = true;
            retCoor.push_back(CH);
            isAllOff = true;
        }
        // axis
        else if (
            isDetected.at(secSg_det) &&
            (mini_tool::isALetter(CH) || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(secSg_det) = false;
                isDetected.at(axis_det) = true;
                retCoor.push_back(CH);
            }
        }
        // second sign
        else if (
            isDetected.at(sec_det) &&
            (CH == '"' || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(sec_det) = false;
                isDetected.at(secSg_det) = true;
                retCoor.push_back(CH);
            }
        }
        // second value
        else if (
            isDetected.at(minSg_det) &&
            (mini_tool::isANumber(CH) || CH == '.' || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(minSg_det) = false;
                isDetected.at(sec_det) = true;
                retCoor.push_back(CH);
            }
        }
        // minute sign
        else if (
            isDetected.at(min_det) &&
            (CH == '\'' || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(min_det) = false;
                isDetected.at(minSg_det) = true;
                retCoor.push_back(CH);
            }
        }
        // minute value
        else if (
            isDetected.at(degSgNgt80_det) &&
            (mini_tool::isANumber(CH) || CH == '.' || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(degSgNgt80_det) = false;
                isDetected.at(min_det) = true;
                retCoor.push_back(CH);
            }
        }
        // degree sign DEG_INT_CHAR_PT2
        else if (
            isDetected.at(degSgNgt62_det) &&
            int(CH) == DEG_INT_CHAR_PT2
        ) {
            isDetected.at(degSgNgt62_det) = false;
            isDetected.at(degSgNgt80_det) = true;
            retCoor.push_back(CH);
        }
        // degree sign DEG_INT_CHAR_PT1
        else if (
            isDetected.at(deg_det) &&
            int(CH) == DEG_INT_CHAR_PT1
        ) {
            isDetected.at(deg_det) = false;
            isDetected.at(degSgNgt62_det) = true;
            retCoor.push_back(CH);
        }
        // degree value or the rest value 
        else if (mini_tool::isANumber(CH) || CH == '.') {
            if (isAllOff) {
                isAllOff = false;
                isSearching = true;
                isDetected.at(deg_det) = true;
            }
            retCoor.push_back(CH);
        }
        // reset if not follow the form
        else if (retCoor != "" || isSearching) {
            if (!(isDetected.at(div_det) && CH == ' ')) {
                isSearching = false;
                isDetected = std::vector<bool>(9, false);
                retCoor = "";
                isAllOff = true;
            }
        }
    }

    // verifying
    if (!isDetected.at(div_det)) {
        retCoor = "";
    }

    return retCoor;
}

// sample: [ -10°20'30.4,40°30'20.1 ]
std::string Samples::testDegreeTemplateWithSign(std::string &textLine) {
    enum {
        div_det,
        sec_det,
        minSg_det,
        min_det,
        degSgNgt80_det,
        degSgNgt62_det,
        deg_det
    };

    std::vector<bool> isDetected(7, false);
    std::string retCoor = "";
    bool isAllOff = true, isSearching = false;
    int chCtr = 0, secPtCtr = 0;

    for (auto &CH : textLine) {
        // end
        if (isDetected.at(div_det) &&
            isDetected.at(sec_det) &&
            !mini_tool::isANumber(CH) &&
            CH != '.' && secPtCtr < 1
        ) {
            break;
        }
        // divide
        else if (
            isDetected.at(sec_det) &&
            (CH == ' ' || CH == ',')
        ) {
            isDetected = std::vector<bool>(7, false);
            isDetected.at(div_det) = true;
            retCoor.push_back(CH);
            isAllOff = true;
            secPtCtr = 0;
        }
        // second value
        else if (
            isDetected.at(minSg_det) &&
            (mini_tool::isANumber(CH) || CH == '.' || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(minSg_det) = false;
                isDetected.at(sec_det) = true;
                retCoor.push_back(CH);
                if (CH == '.') secPtCtr++;
            }
        }
        // minute sign
        else if (
            isDetected.at(min_det) &&
            (CH == '\'' || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(min_det) = false;
                isDetected.at(minSg_det) = true;
                retCoor.push_back(CH);
            }
        }
        // minute value
        else if (
            isDetected.at(degSgNgt80_det) &&
            (mini_tool::isANumber(CH) || CH == '.' || CH == ' ')
        ) {
            if (CH != ' ') {
                isDetected.at(degSgNgt80_det) = false;
                isDetected.at(min_det) = true;
                retCoor.push_back(CH);
            }
        }
        // degree sign DEG_INT_CHAR_PT2
        else if (
            isDetected.at(degSgNgt62_det) &&
            int(CH) == DEG_INT_CHAR_PT2
        ) {
            isDetected.at(degSgNgt62_det) = false;
            isDetected.at(degSgNgt80_det) = true;
            retCoor.push_back(CH);
        }
        // degree sign DEG_INT_CHAR_PT1
        else if (
            isDetected.at(deg_det) &&
            int(CH) == DEG_INT_CHAR_PT1
        ) {
            isDetected.at(deg_det) = false;
            isDetected.at(degSgNgt62_det) = true;
            retCoor.push_back(CH);
        }
        // degree value or rest value
        else if (
            mini_tool::isANumber(CH) ||
            CH == '.' || CH == '-'
        ) {
            if (isAllOff) {
                isAllOff = false;
                isSearching = true;
                isDetected.at(deg_det) = true;
            }
            retCoor.push_back(CH);
        }
        // reset if not follow the form
        else if (retCoor != "" || isSearching) {
            if (!(isDetected.at(div_det) && CH == ' ')) {
                isSearching = false;
                isDetected = std::vector<bool>(7, false);
                retCoor = "";
                isAllOff = true;
                secPtCtr = 0;
            }
        }
    }

    // verifying
    if (!isDetected.at(div_det)) {
        retCoor = "";
    }

    return retCoor;
}

// sample: [ -1.234,5.678 ]
std::string Samples::testDecimalTemplate(std::string &textLine) {
    std::string retCoor;

    int stateCount = 9,
        expectedCount = 4,
        enumAxisLeapRate = 0; // will be 5 after divider detected

    enum {
        neg_det,
        val_det,
        pt_det,
        ptval_det,
        div_det
    };

    int expectedDetectionState[expectedCount][stateCount] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 0, 1, 1, 1},
        {0, 1, 1, 1, 1, 0, 1, 1, 1}
    };

    std::vector<bool> isDetected(stateCount, false);
    
    for (auto &CH : textLine) {
        bool isAddChar = true;

        if (CH == '-') {
            if (!isDetected.at(neg_det + enumAxisLeapRate) && !isDetected.at(val_det + enumAxisLeapRate)) {
                isDetected.at(neg_det + enumAxisLeapRate) = true;
            }
            else if (!isDetected.at(neg_det + enumAxisLeapRate)) {
                isDetected.at(neg_det + enumAxisLeapRate) = true;
            }
        }
        else if (
            mini_tool::isANumber(CH) &&
            !isDetected.at(val_det + enumAxisLeapRate)
        ) {
            isDetected.at(val_det + enumAxisLeapRate) = true;
        }
        else if (
            CH == '.' &&
            isDetected.at(val_det + enumAxisLeapRate) &&
            !isDetected.at(pt_det + enumAxisLeapRate)
        ) {
            isDetected.at(pt_det + enumAxisLeapRate) = true;
        }
        else if (mini_tool::isANumber(CH)) {
            if (isDetected.at(pt_det + enumAxisLeapRate) &&
                !isDetected.at(ptval_det + enumAxisLeapRate)
            ) {
                isDetected.at(ptval_det + enumAxisLeapRate) = true;
            }
        }
        else if (
            (CH == ' ' || CH == ',') &&
            isDetected.at(ptval_det) &&
            !isDetected.at(div_det) &&
            enumAxisLeapRate == 0
        ) {
            isDetected.at(div_det) = true;
            enumAxisLeapRate = 5;
        }
        else {
            // test the expected detection state
            bool isPass = false;

            for (int i = 0; i < expectedCount; i++) {
                for (int j = 0; j < stateCount; j++) {

                    if (expectedDetectionState[i][j] != isDetected.at(j)) {
                        isPass = false;
                        break;
                    }
                    else isPass = true;
                }

                if (isPass) break;
            }

            if (isPass) break;

            isDetected = std::vector<bool>(stateCount, false);
            isAddChar = false;
            retCoor = "";
            enumAxisLeapRate = 0;
        }

        if (isAddChar) retCoor.push_back(CH);
    }

    return retCoor;
}

#endif // __TXT_SAMPLES_CPP__