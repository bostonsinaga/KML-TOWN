#ifndef __MINI_TOOL_CPP__
#define __MINI_TOOL_CPP__

#include "mini-tool.h"

namespace mini_tool {

    // acually remove the string in front of '/' with the sign itself
    std::string cutFileDirName(std::string &fileDir) {
        std::string cutName = fileDir;

        for (int i = 0; i < fileDir.size(); i++) {
            size_t foundDex = cutName.find("/");
            
            if (foundDex != std::string::npos) {
                cutName = cutName.substr(foundDex + 1);
            }
            else break;
        }

        return cutName;
    }

    bool isANumber(char ch) {
        if (int(ch) >= NM_BEGIN_CODE &&
            int(ch) <= NM_END_CODE
        ) {
            return true;
        }
        return false;
    }

    bool isALetter(char ch) {
        if ((int(ch) >= CH_A_UPPER_CODE && int(ch) <= CH_Z_UPPER_CODE) ||
            (int(ch) >= CH_A_LOWER_CODE && int(ch) <= CH_Z_LOWER_CODE)
        ) {
            return true;
        }
        return false;
    }

    std::string reverseString(std::string &str_in) {
        
        std::string str_out = str_in;
        int out_ctr = str_out.size() - 1;

        for (int in_ctr = 0; in_ctr < str_in.size(); in_ctr++) {
            str_out.at(out_ctr) = str_in.at(in_ctr);
            out_ctr--;
        }

        return str_out;
    }

    bool isMatchButIgnoreCase(std::string strA, std::string strB) {
        if (strA.size() == strB.size()) {

            int aToADifference = CH_A_LOWER_CODE - CH_A_UPPER_CODE; // positive
            int ctrB = 0, equalCount = 0;

            for (auto &chA : strA) {
                if (chA == strB.at(ctrB) ||
                    std::abs(int(chA) - int(strB.at(ctrB))) == aToADifference
                ) {
                    equalCount++;
                }
                ctrB++;
            }

            if (equalCount == strA.size()) {
                return true;
            }
        }
        
        return false;
    }

    int getInStringCharCount(std::string str, char ch) {
        int count = 0;

        for (int i = 0; i < str.size(); i++) {
            size_t found =  str.find(ch);

            if (found != std::string::npos) {
                count++;
                str.erase(str.begin() + found, str.begin() + found + 1);
            }
            else break;
        }

        return count;
    }

    void changeStringCase(
        std::string *string_in,
        int caseFlag,
        int onlyCharDex
    ) {
        int aToADifference = CH_A_LOWER_CODE - CH_A_UPPER_CODE, // positive
            startDex, borderDex;

        if (onlyCharDex != -1) {
            if (onlyCharDex <= string_in->size() - 1 &&
                isALetter(string_in->at(onlyCharDex))
            ) {
                startDex = onlyCharDex;
                borderDex = onlyCharDex + 1;
            }
        }
        else {
            startDex = 0;
            borderDex = string_in->size();
        }

        for (int i = startDex; i < borderDex; i++) {
            
            int chCode = int(string_in->at(startDex)),
                diffSign = 0;
            
            // upper detected
            if (chCode < CH_A_LOWER_CODE &&
                caseFlag == LOWER_CASE_FLAG
            ) {
                diffSign = 1;
            }
            // lower detected
            else if (
                chCode > CH_Z_UPPER_CODE &&
                caseFlag == UPPER_CASE_FLAG
            ) {
                diffSign = -1;
            }

            if (diffSign != 0) {
                string_in->at(startDex) = char(chCode + diffSign * aToADifference);
            }
        }
    }

    void completeDegreeCoordinateSecondsSign(std::string *coorStr) {
        int ctr = 0,
            letterCtr = 0,
            letterDex[2];
        
        for (auto &ch : *coorStr) {
            if (isALetter(ch)) {

                letterDex[letterCtr] = ctr;
                letterCtr++;

                if (letterCtr == 2) {
                    coorStr->insert(coorStr->begin() + letterDex[0], '\"');
                    letterDex[1]++;
                    coorStr->insert(coorStr->begin() + letterDex[1], '\"');
                    break;
                }
            }
            ctr++;
        }
    }

    double filterStringDecimal(std::string strIn) {
        int ctr = 0,
            dcPtCtr = 0; // decimal point counter

        std::string strVal = "";
        bool isHasNumber = false;

        for (auto &ch : strIn) {
            if (isANumber(ch)) {
                strVal += ch;
                isHasNumber = true;
            }
            else if (dcPtCtr == 0 && ch == '.') {
                if (ctr = 0) strVal += "0";
                dcPtCtr++;
            }
            else if (ctr == 0 && ch == '-') {
                strVal += ch;
            }
            ctr++;
        }

        if (!isHasNumber) return 0.000;
        return std::stod(strVal);
    }
}

#endif // __MINI_TOOL_CPP__