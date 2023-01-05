#ifndef __CALL_BRIEFER_CPP__
#define __CALL_BRIEFER_CPP__

#include "call-briefer.h"

namespace call_briefer {

    std::string checkOverwrite(
        Menu &menu,
        int selectedFlag,
        int overwriteFlags,
        std::string &fileDir_in,
        std::string &fileDir_out
    ) {
        if (selectedFlag == overwriteFlags ||
            fileDir_in == fileDir_out
        ) {
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

    std::vector<xml::Node*> cropPinsFunc(
        xml::Node *kmlNode,
        std::vector<std::string*> axisStrVec,
        bool isFolderInsertion // 'false' return nodes, 'true' return empty
    ) {
        xml::Node *retContainerNode = nullptr;
        std::vector<xml::Node*> croppedPinNodes;
        bool isSucceeded = true;
        
        if (kmlNode) {
            xml::Node *mainFolderNode = kml::General().searchMainFolder(kmlNode);

            if (mainFolderNode) {
                mini_tool::completeDegreeCoordinateSecondsSign(axisStrVec.at(0));
                mini_tool::completeDegreeCoordinateSecondsSign(axisStrVec.at(1));

                // repair anomaly degree signs //

                for (auto &str : axisStrVec) {
                    int ctr = 0;
                    bool isAnomaly = false;

                    std::vector<int>
                        normalDegreeSignComplementIndexes,
                        normalDegreeSignComplementCharCodes;

                    for (auto &ch : *str) {
                        
                        if (ch == char(ANM_DEG_INT_CHAR_PT1)) {
                            ch = char(DEG_INT_CHAR_PT1);
                            isAnomaly = true;
                        }
                        else if (ch == char(ANM_DEG_INT_CHAR_PT2)) {
                            ch = char(DEG_INT_CHAR_PT2);
                            isAnomaly = true;
                        }

                        if (!isAnomaly) {
                            if (ch == char(DEG_INT_CHAR_PT1) && ctr != str->size() - 1) {
                                if (str->at(ctr + 1) != char(DEG_INT_CHAR_PT2)) {
                                    normalDegreeSignComplementIndexes.push_back(ctr + 1);
                                    normalDegreeSignComplementCharCodes.push_back(DEG_INT_CHAR_PT2);
                                }
                            }
                            else if (ch == char(DEG_INT_CHAR_PT2) && ctr != 0) {
                                if (str->at(ctr - 1) != char(DEG_INT_CHAR_PT1)) {
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
                            str->insert(
                                str->begin() + index + strIndexExpandRate,
                                char(normalDegreeSignComplementCharCodes.at(ctr))
                            );
                            ctr++;
                            strIndexExpandRate++;
                        }
                    }
                }

                // cut the pins
                kml::Cropper kmlCropper;              /* NOTE: */
                croppedPinNodes = kmlCropper.cutPins( // may return nodes if 'isFolderInsertion' false
                    mainFolderNode,                   // (actually if there were pins in selection
                    kml::Point(*axisStrVec.at(0)),    // otherwise return empty as on 'isFolderInsertion' true)
                    kml::Point(*axisStrVec.at(1)),
                    isFolderInsertion
                );

                // test the cropped folder //

                xml::Node *croppedFolderNode = (
                    mainFolderNode->getFirstChildByName("Folder", false)
                );
                
                if (croppedFolderNode && isFolderInsertion) {
                    if (croppedFolderNode->getFirstChildByName("name", false)->getInnerText()
                        == CROP_COMMAND_WORKING_FOLDER
                    ) {
                        retContainerNode = croppedFolderNode; // 'croppedPinNodes' is empty
                    }
                    else isSucceeded = false;
                }
            }
            else isSucceeded = false;
        }
        else isSucceeded = false;

        if (isSucceeded) {
            if (croppedPinNodes.size() == 0) {
                if (isFolderInsertion) {
                    return std::vector<xml::Node*>{retContainerNode};
                }
                else { /* no pins detected on 'isFolderInsertion' false (no cropped folder) */
                    std::cerr << "\n**FAILED**\n";
                    return std::vector<xml::Node*>{};
                }
            }
            else return croppedPinNodes;
        }
        else {
            std::cerr << "\n**FAILED**\n";
            return std::vector<xml::Node*>{};
        }
    }

    std::vector<xml::Node*> sortPinsFunc(
        Menu &menu,
        xml::Node *kmlNode,
        std::vector<std::string*> axisStrVec,
        bool isFolderInsertion // 'false' return nodes, 'true' return empty
    ) {
        /* 'dualismVector' may contains nodes for command that involves 'sorter' */
        std::vector<xml::Node*> dualismVector = cropPinsFunc(
            kmlNode,
            axisStrVec,
            isFolderInsertion
        );

        if (dualismVector.size() > 0) {
            // pins sorting
            kml::Sorter sorter;
            std::vector<xml::Node*> sortedPinNodes = sorter.orderPins(
                dualismVector,
                kml::Point(*axisStrVec.at(0)), // start point
                isFolderInsertion
            );
            return sortedPinNodes;
        }
        else return std::vector<xml::Node*>{};
    }

    // write file (end of process)
    void writeFileFunc(
        xml::Node *kmlNode,
        std::string &fileDir_out
    ) {
        if (kmlNode) {
            // set name of 'main folder' upto 'root' element as 'fileDir_out' name
            kml::General().setKMLDocumentName(kmlNode, fileDir_out);

            xml::Writer xmlWriter;
            xmlWriter.stringify(fileDir_out, kmlNode);
            std::cout << "\n**SUCCEEDED**\n";

            delete kmlNode;
        }
        else std::cerr << "\n**FAILED**\n";
    }

    // return command working folder
    xml::Node *selectFunctionByPlacemarkType(
        std::string placemarksType,
        const std::function<xml::Node*()> &funcPins,
        const std::function<xml::Node*()> &funcPaths
    ) {
        if (mini_tool::isStringContains(placemarksType, "path", true) &&
            mini_tool::isStringContains(placemarksType, "paths", true)
        ) {
            return funcPaths();
        }
        else {
            if (placemarksType != "pin" && placemarksType != "pins") {
                std::cerr
                    << "KML-TOWN-> Placemarks type input warning. Unknown type of '"
                    << placemarksType << "'. Default set to 'pins'\n";
            }
            return funcPins();
        }
    }
}

#endif // __CALL_BRIEFER_CPP__