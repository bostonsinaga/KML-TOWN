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
    std::string &containerStr,
    std::string testStr,
    bool isIgnoreCaseSensitive = false
  );

  bool isStringEquals(
    std::string &containerStr,
    std::string testStr,
    bool isIgnoreCaseSensitive = false
  );

  enum {
    LOWER_CASE_FLAG,
    UPPER_CASE_FLAG
  };

  void changeStringCase(
    std::string *string_ptr,
    int caseFlag,
    int onlyCharDex = -1 // index of single character change
  );

  std::string changeStringCase(
    std::string string_in,
    int caseFlag,
    int onlyCharDex = -1 // index of single character change
  );

  void completeDegreeCoordinateSecondsSign(std::string &coorStr);

  // 'TYPE_T' is a primitive data type
  template<typename TYPE_T>

  // return index (-1 not found)
  int isPrimitiveInsideVector(
    std::vector<TYPE_T> &dataVec,
    TYPE_T dataTest
  );

  template<typename TYPE_T>

  /**
   * Returns intersect indexes of 'testVec'
   * in 2 kind of vector (duplicate and single[second dimension is 1 member]).
   * 
   * If not found at 'containerVec' index will set second dimension empty.
   * If all not found will return emty vector.
   * 
   * Empty first dimension will be deleted.
   */
  std::vector<std::vector<int>> vectorIntersectsVector(   // pass 1D vector only
    std::vector<TYPE_T> &containerVec,
    std::vector<TYPE_T> &testVec,
    bool isTestDuplicate = false
  );

  template<typename TYPE_T>
  
  // set first member of second dimension to be a member of new 1D vector
  std::vector<TYPE_T> collapse2DTo1DVector(std::vector<std::vector<TYPE_T>> &vec_in);
}

#endif // __MINI_TOOL__H__