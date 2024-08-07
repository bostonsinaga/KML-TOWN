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
    ) { return true; }
    return false;
  }

  bool isALetter(char ch) {
    if ((int(ch) >= CH_A_UPPER_CODE && int(ch) <= CH_Z_UPPER_CODE) ||
      (int(ch) >= CH_A_LOWER_CODE && int(ch) <= CH_Z_LOWER_CODE)
    ) { return true; }
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

  bool isStringContains(
    std::string &containerStr,
    std::string testStr,
    bool isIgnoreCaseSensitive
  ) {
    if (isIgnoreCaseSensitive) {
      changeStringCase(&containerStr, UPPER_CASE_FLAG);
      changeStringCase(&testStr, UPPER_CASE_FLAG);
    }

    if (containerStr.find(testStr) != std::string::npos) return true;
    return false;
  }

  bool isStringEquals(
    std::string &containerStr,
    std::string testStr,
    bool isIgnoreCaseSensitive
  ) {
    if (isIgnoreCaseSensitive) {
      changeStringCase(&containerStr, UPPER_CASE_FLAG);
      changeStringCase(&testStr, UPPER_CASE_FLAG);
    }

    if (containerStr == testStr) return true;
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
    std::string *string_ptr,
    int caseFlag,
    int onlyCharDex // index of single character change
  ) {
    if (string_ptr->size() == 0) return;

    int aToADifference = CH_A_LOWER_CODE - CH_A_UPPER_CODE, // positive
      startDex, borderDex;

    if (onlyCharDex != -1) {
      if (onlyCharDex <= string_ptr->size() - 1 &&
        isALetter(string_ptr->at(onlyCharDex))
      ) {
        startDex = onlyCharDex;
        borderDex = onlyCharDex + 1;
      }
    }
    else {
      startDex = 0;
      borderDex = string_ptr->size();
    }

    for (int i = startDex; i < borderDex; i++) {

      int chCode = std::abs(int(string_ptr->at(i))),
        diffSign = 0;

      // upper detected
      if (chCode < CH_A_LOWER_CODE &&
        caseFlag == LOWER_CASE_FLAG
      ) { diffSign = 1; }
      // lower detected
      else if (
        chCode > CH_Z_UPPER_CODE &&
        caseFlag == UPPER_CASE_FLAG
      ) { diffSign = -1; }

      if (diffSign != 0) {
        string_ptr->at(i) = char(chCode + diffSign * aToADifference);
      }
    }
  }

  std::string changeStringCase(
    std::string string_in,
    int caseFlag,
    int onlyCharDex // index of single character change
  ) {
    changeStringCase(&string_in, caseFlag, onlyCharDex);
    return string_in;
  }

  void completeDegreeCoordinateSecondsSign(std::string &coorStr) {
    int ctr = 0,
      letterCtr = 0,
      letterDex[2];
    
    for (auto &ch : coorStr) {
      if (isALetter(ch)) {

        letterDex[letterCtr] = ctr;
        letterCtr++;

        if (letterCtr == 2) {
          coorStr.insert(coorStr.begin() + letterDex[0], '\"');
          letterDex[1]++;
          coorStr.insert(coorStr.begin() + letterDex[1], '\"');
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

  template<typename TYPE_T>

  // return index (-1 not found)
  int isPrimitiveInsideVector(
    std::vector<TYPE_T> &dataVec,
    TYPE_T dataTest
  ) {
    int ctr = 0;
    for (auto &data : dataVec) {
      if (data == dataTest) return ctr;
      ctr++;
    }
    return -1;
  }

  template<typename TYPE_T>

  /**
   * Return intersect indexes of 'testVec' in 2 kind of vector
   * (duplicate and single[second dimension is 1 member]).
   *
   * If not found at 'containerVec' index will set second dimension empty.
   * If all not found will return emty vector.
   *
   * Empty first dimension will be deleted.
   *
   * Return examples:
   *
   * {
   *   {0, 1, 2}  -> duplicate on
   *   {3, 4}        container[2] and test[5]
   * }
   *
   * {
   *   {0}        -> duplicate off
   *   {3}           container[2] and test[5]
   * }
   */
  std::vector<std::vector<int>> vectorIntersectsVector(  // pass 1D vector only
    std::vector<TYPE_T> &containerVec,
    std::vector<TYPE_T> &testVec,
    bool isTestDuplicate
  ) {
    bool isIntersected = false;
    std::vector<std::vector<int>> intersectIndexesVec(containerVec.size(), std::vector<int>{});

    // test vector //

    int containerCtr = 0;
    for (auto &containerVal : containerVec) {

      int testCtr = 0;
      for (auto &testVal : testVec) {
        if (containerVal == testVal) {

          if (!isIntersected) isIntersected = true;
          intersectIndexesVec.at(containerCtr).push_back(testCtr);
          if (!isTestDuplicate) break;
        }
        testCtr++;
      }
      containerCtr++;
    }

    // cleaning empty first dimension
    if (isIntersected) {
      std::vector<std::vector<int>> intersectIndexesVec_buffer;
      
      for (auto &indexes : intersectIndexesVec) {
        if (indexes.size() > 0) {
          intersectIndexesVec_buffer.push_back(indexes);
        }
      }

      intersectIndexesVec = intersectIndexesVec_buffer;
    }
    // return check
    else return std::vector<std::vector<int>>{};
    return intersectIndexesVec;
  }

  template<typename TYPE_T>

  // set first member of second dimension to be a member of new 1D vector
  std::vector<TYPE_T> collapse2DTo1DVector(std::vector<std::vector<TYPE_T>> &vec_in) {
    std::vector<TYPE_T> newVec;

    for (auto &vec1D : vec_in) {
      if (vec1D.size() > 0) {
        newVec.push_back(vec1D.front());
      }
    }

    return newVec;
  }
}

#endif // __MINI_TOOL_CPP__