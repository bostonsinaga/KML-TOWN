#ifndef __CALL_BRIEFER_H__
#define __CALL_BRIEFER_H__

class call_briefer {
public:
  static std::string checkOverwrite(
    Menu &menu,
    std::string &fileDir_in,
    std::string &fileDir_out
  );

  static std::vector<xml::Node*> cropPlacemarkFunc(
    xml::Node *kmlNode,
    std::vector<std::string> &axisStrVec,
    std::string type_paramStr,
    bool isFolderInsertion,  // 'false' return nodes, 'true' return empty
    bool isIncludeFolders    // if true the 'isFolderInsertion' must be true
  );

  static std::vector<xml::Node*> sortPlacemarksFunc(
    xml::Node *kmlNode,
    std::vector<std::string> &axisStrVec,
    std::string type_paramStr,
    bool isFolderInsertion,  // 'false' return nodes, 'true' return empty
    bool isIncludeFolders    // if true the 'isFolderInsertion' must be true
  );

  // write file (end of process)
  static void writeFileFunc(
    xml::Node *kmlNode,
    std::string &fileDir_out
  );

  // return command working folder
  static xml::Node *selectFunctionByType(
    std::string inputType,
    const std::vector<std::string> &seqTypeStrVec,
    const std::vector<std::function<xml::Node*()>> &seqFuncVec
  );

private:
  static void repairAnomalyDegreeSign(std::vector<std::string> &axisStrVec);
  static void repairAnomalyDegreeSign(std::string &axisStr);
};

#endif // __CALL_BRIEFER_H__