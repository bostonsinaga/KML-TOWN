#ifndef __CALL_BRIEFER_H__
#define __CALL_BRIEFER_H__

namespace call_briefer {

    std::string checkOverwrite(
        Menu &menu,
        int selectedFlag,
        int overwriteFlag,
        std::string &fileDir_in,
        std::string &fileDir_out
    ) {
        if (selectedFlag == overwriteFlag) {
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

    xml::Node *cropPinsFunc(
        xml::Node *kmlNode,
        std::vector<std::string*> axisStrVec
    ) {
        xml::Node *retContainerNode = nullptr;
        
        if (kmlNode) {
            xml::Node *mainFolderNode = kml::searchMainFolder(kmlNode);

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
                kml::Cropper kmlCropper;
                kmlCropper.cutPins(
                    mainFolderNode,
                    kml::Point(*axisStrVec.at(0)),
                    kml::Point(*axisStrVec.at(1))
                );

                // test the cropped folder
                xml::Node *croppedFolderNode = mainFolderNode->getFirstChildByName("Folder", false);
                
                if (croppedFolderNode) {
                    if (croppedFolderNode->getFirstChildByName("name", false)->getInnerText()
                        == CROP_COMMAND_WORKING_FOLDER
                    ) {
                        retContainerNode = croppedFolderNode;
                    }
                    else std::cerr << "\n**FAILED**\n";
                }
                else std::cerr << "\n**FAILED**\n";
            }
            else std::cerr << "\n**FAILED**\n";
        }
        else std::cerr << "\n**FAILED**\n";

        return retContainerNode;
    }

    std::vector<std::string> sortPinsFunc(
        Menu &menu,
        xml::Node *kmlNode,
        std::vector<std::string*> axisStrVec,
        bool isReturnCoordinates
    ) {
        xml::Node *croppedFolderNode = cropPinsFunc(
            kmlNode,
            axisStrVec
        );

        // pins sorting //
        kml::Sorter sorter;
        std::vector<std::string> coorStrVec = sorter.orderPins(
            croppedFolderNode,
            kml::Point(*axisStrVec.at(0)), // start point
            isReturnCoordinates
        );

        return coorStrVec;
    }

    // write file (end of process)
    void writeFileFunc(
        xml::Node *kmlNode,
        std::string &fileDir_out
    ) {
        if (kmlNode) {
            // set name of 'main folder' upto 'root' element as 'fileDir_out' name
            kml::setKMLDocumentName(kmlNode, fileDir_out);

            xml::Writer xmlWriter;
            xmlWriter.stringify(fileDir_out, kmlNode);
            std::cout << "\n**SUCCEEDED**\n";

            delete kmlNode;
        }
    }
}

#endif // __CALL_BRIEFER_H__