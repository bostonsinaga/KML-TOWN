#ifndef __CALL_BRIEFER_CPP__
#define __CALL_BRIEFER_CPP__

#include "call-briefer.h"

std::string call_briefer::checkOverwrite(
  Menu &menu,
  std::string &fileDir_in,
  std::string &fileDir_out
) {
  if (fileDir_in == fileDir_out || fileDir_out == "") {

    if (menu.setAlert(
      std::string("KML-TOWN-> No '--out [FILE_NAME]'. Are you sure to overwrite the '")
      + fileDir_in + std::string("'?\n")
    )) {
      return fileDir_in;
    }
    else return ""; // command canceled
  }

  return fileDir_out;
}

std::vector<xml::Node*> call_briefer::cropPlacemarkFunc(
  xml::Node *kmlNode,
  std::vector<std::string> &axisStrVec,
  std::string type_paramStr,
  bool isFolderInsertion,  // 'false' return nodes, 'true' return empty
  bool isIncludeFolders    // if true the 'isFolderInsertion' must be true
) {
  xml::Node *retContainerNode = nullptr;
  std::vector<xml::Node*> croppedPlacemarkNodes;
  bool isSucceeded = true;
  
  if (kmlNode) {
    xml::Node *mainFolderNode = kml::General::searchMainFolder(kmlNode);

    if (mainFolderNode) {
      repairAnomalyDegreeSign(axisStrVec);

      /**
       * Cropper:
       *   May returns nodes if 'isFolderInsertion' false
       *   (actually if there were placemarks in selection
       *   otherwise return empty as on 'isFolderInsertion' true).
       */
      kml::Cropper kmlCropper;

      int typeFlag;
      enum {PIN_TYPE, PATH_TYPE, ALL_TYPE};

      if (mini_tool::isStringContains(type_paramStr, "pin", true)) {
        typeFlag = PIN_TYPE;
      }
      else if (mini_tool::isStringContains(type_paramStr, "path", true)) {
        typeFlag = PATH_TYPE;
      }
      else if (mini_tool::isStringContains(type_paramStr, "all", true)) {
        typeFlag = ALL_TYPE;
      }
      // prevent wrong type string
      else {
        std::cerr
          << "KML-TOWN-> Placemarks type input warning. Unknown type of '"
          << type_paramStr << "'. Default set to 'pin'\n";

        typeFlag = PIN_TYPE;
      }

      if (typeFlag == PIN_TYPE) {
        croppedPlacemarkNodes = kmlCropper.cutPins(
          mainFolderNode,
          kml::Point(axisStrVec.at(0)),
          kml::Point(axisStrVec.at(1)),
          isFolderInsertion,
          isIncludeFolders,
          false
        );
      }
      else if (typeFlag == PATH_TYPE) {
        croppedPlacemarkNodes = kmlCropper.cutPaths(
          mainFolderNode,
          kml::Point(axisStrVec.at(0)),
          kml::Point(axisStrVec.at(1)),
          isFolderInsertion,
          isIncludeFolders,
          false
        );
      }
      else if (typeFlag == ALL_TYPE) {
        croppedPlacemarkNodes = kmlCropper.cutAll(
          mainFolderNode,
          kml::Point(axisStrVec.at(0)),
          kml::Point(axisStrVec.at(1)),
          isIncludeFolders
        );
      }

      if (isFolderInsertion) {

        // test the cropped folder
        xml::Node *croppedFolderNode = (
          mainFolderNode->getFirstChildByName("Folder")
        );

        if (croppedFolderNode) {
          if (kml::Placemark::getDataText(croppedFolderNode, "name")
            == CROP_COMMAND_WORKING_FOLDER
          ) {
            retContainerNode = croppedFolderNode; // 'croppedPlacemarkNodes' is single
          }
          else isSucceeded = false;
        }
        else isSucceeded = false;
      }
    }
    else isSucceeded = false;
  }
  else isSucceeded = false;

  if (isSucceeded) return croppedPlacemarkNodes;
  else return std::vector<xml::Node*>{};
}

std::vector<xml::Node*> call_briefer::sortPlacemarksFunc(
  xml::Node *kmlNode,
  std::vector<std::string> &axisStrVec,
  std::string type_paramStr,
  bool isFolderInsertion,  // 'false' return nodes, 'true' return empty
  bool isIncludeFolders    // if true the 'isFolderInsertion' must be true
) {
  int typeFlag;
  enum {PIN_TYPE, PATH_TYPE, ALL_TYPE};

  if (mini_tool::isStringContains(type_paramStr, "pin", true)) {
    typeFlag = PIN_TYPE;
  }
  else if (mini_tool::isStringContains(type_paramStr, "path", true)) {
    typeFlag = PATH_TYPE;
  }
  else if (mini_tool::isStringContains(type_paramStr, "all", true)) {
    typeFlag = ALL_TYPE;
  }

  // separately repair 'axisStrVec.at(0)' by copy the value
  std::string firstAxisStr = axisStrVec.at(0);
  repairAnomalyDegreeSign(firstAxisStr);
  kml::Point startPt = kml::Point(firstAxisStr);

  std::vector<xml::Node*> sortedPlacemarks_arr[2];
  std::vector<std::string> typeNamesVec;
  std::vector<int> typeFlagVec;

  if (typeFlag != ALL_TYPE) {
    typeNamesVec.push_back(type_paramStr);
    typeFlagVec.push_back(typeFlag);
  }
  else {
    typeNamesVec = {"pin", "path"};
    typeFlagVec = {PIN_TYPE, PATH_TYPE};
  }

  for (int i = 0; i < typeNamesVec.size(); i++) {

    std::vector<xml::Node*> placemarks = cropPlacemarkFunc(
      kmlNode,
      axisStrVec,
      typeNamesVec.at(i),
      false,
      false
    );

    if (placemarks.size() > 0) {
      sortedPlacemarks_arr[i] = kml::Sorter().orderApart(
        placemarks,
        startPt,
        typeFlagVec.at(i),
        isFolderInsertion,
        isIncludeFolders
      );
    }
    else std::cerr
      << "KML-> Sorting error. No cropped " << typeNamesVec.at(i) << "s as input\n";
  }

  // returns folder single vector
  if (isFolderInsertion && typeFlag == ALL_TYPE) {
    return kml::Sorter().orderAll(sortedPlacemarks_arr, isIncludeFolders);
  }
  /**
   * Returns placemarks vector if 'isFolderInsertion' is true.
   * Otherwise returns one type folder single vector.
   */
  else {
    std::vector<xml::Node*> returnSortedPlacemarks;

    for (int i = 0; i < typeFlagVec.size(); i++) {
      returnSortedPlacemarks.insert(
        returnSortedPlacemarks.end(),
        sortedPlacemarks_arr[i].begin(),
        sortedPlacemarks_arr[i].end()
      );
    }

    return returnSortedPlacemarks;
  }

  return std::vector<xml::Node*>{};
}

// write file (end of process)
void call_briefer::writeFileFunc(
  xml::Node *kmlNode,
  std::string &fileDir_out
) {
  if (kmlNode) {
    // set name of 'main folder' upto 'root' element as 'fileDir_out' name
    kml::General::setKMLDocumentName(kmlNode, fileDir_out);

    xml::Writer::stringify(fileDir_out, kmlNode);
    std::cout << "\n**SUCCEEDED**\n";

    delete kmlNode;
  }
  else std::cerr << "\n**FAILED**\n";
}

// return command working folder
xml::Node *call_briefer::selectFunctionByType(
  std::string inputType,
  const std::vector<std::string> &seqTypeStrVec,
  const std::vector<std::function<xml::Node*()>> &seqFuncVec
) {
  // 'seqTypeStrVec' and 'seqFuncVec' must equal in size
  for (int i = 0; i < seqTypeStrVec.size(); i++) {
    if (mini_tool::isStringContains(inputType, seqTypeStrVec.at(i), true)) {
      return seqFuncVec.at(i)();
    }
  }

  // set default call to the first function
  std::cerr
    << "KML-TOWN-> Placemarks type input warning. Unknown type of '"
    << inputType << "'. Default set to '" << seqTypeStrVec.at(0) << "'\n";

  return seqFuncVec.at(0)();
}

void call_briefer::repairAnomalyDegreeSign(std::vector<std::string> &axisStrVec) {

  for (int i = 0; i < axisStrVec.size(); i++) {
    mini_tool::completeDegreeCoordinateSecondsSign(axisStrVec.at(i));
  }

  for (auto &str : axisStrVec) {
    int ctr = 0;
    bool isAnomaly = false;

    std::vector<int>
      normalDegreeSignComplementIndexes,
      normalDegreeSignComplementCharCodes;

    for (auto &ch : str) {

      if (ch == char(ANM_DEG_INT_CHAR_PT1)) {
        ch = char(DEG_INT_CHAR_PT1);
        isAnomaly = true;
      }
      else if (ch == char(ANM_DEG_INT_CHAR_PT2)) {
        ch = char(DEG_INT_CHAR_PT2);
        isAnomaly = true;
      }

      if (!isAnomaly) {
        if (ch == char(DEG_INT_CHAR_PT1) && ctr != str.size() - 1) {
          if (str.at(ctr + 1) != char(DEG_INT_CHAR_PT2)) {
            normalDegreeSignComplementIndexes.push_back(ctr + 1);
            normalDegreeSignComplementCharCodes.push_back(DEG_INT_CHAR_PT2);
          }
        }
        else if (ch == char(DEG_INT_CHAR_PT2) && ctr != 0) {
          if (str.at(ctr - 1) != char(DEG_INT_CHAR_PT1)) {
            normalDegreeSignComplementIndexes.push_back(ctr);
            normalDegreeSignComplementCharCodes.push_back(DEG_INT_CHAR_PT1);
          }
        }
      }

      ctr++;
    }

    if (!isAnomaly) {
      int strIndexExpandRate = 0;
      ctr = 0;

      for (auto &index : normalDegreeSignComplementIndexes) {
        str.insert(
          str.begin() + index + strIndexExpandRate,
          char(normalDegreeSignComplementCharCodes.at(ctr))
        );
        ctr++;
        strIndexExpandRate++;
      }
    }
  }
}

void call_briefer::repairAnomalyDegreeSign(std::string &axisStr) {
  std::vector<std::string> axisStrVec = {axisStr};
  repairAnomalyDegreeSign(axisStrVec);
  axisStr = axisStrVec.front();
}

#endif // __CALL_BRIEFER_CPP__