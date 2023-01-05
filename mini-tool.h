#ifndef __MINI_TOOL__H__
#define __MINI_TOOL__H__

#define NM_BEGIN_CODE 48
#define NM_END_CODE 57

#define CH_A_UPPER_CODE 65
#define CH_Z_UPPER_CODE 90

#define CH_A_LOWER_CODE 97
#define CH_Z_LOWER_CODE 122

namespace mini_tool {

    const std::string DEGREE_SIGN = std::string{
        char(DEG_INT_CHAR_PT1), 
        char(DEG_INT_CHAR_PT2)
    };

    std::string cutFileDirName(std::string &fileDir);
    bool isANumber(char ch);
    bool isALetter(char ch);
    std::string reverseString(std::string &str_in);
    int getInStringCharCount(std::string str, char ch);
    double filterStringDecimal(std::string strIn);

    bool isStringContains(
        std::string containerStr,
        std::string testStr,
        bool isIgnoreCaseSensitive = false
    );

    enum {
        LOWER_CASE_FLAG,
        UPPER_CASE_FLAG
    };

    void changeStringCase(
        std::string *string_in,
        int caseFlag,
        int onlyCharDex = -1
    );

    void completeDegreeCoordinateSecondsSign(std::string *coorStr);

    // return counter (-1 not found)
    int isInsideVectorString(
        std::vector<std::string> &strVec,
        std::string strTest
    );

    // return counter (-1 not found)
    int isInsideVectorInteger(
        std::vector<int> &dataVec,
        int dataTest
    );
}

#endif // __MINI_TOOL__H__