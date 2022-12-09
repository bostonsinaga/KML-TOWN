#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <functional>
#include <algorithm>

#define X 0
#define Y 1

/* degree sign input in literal (°) reformatted with 2 characters */
#define DEG_INT_CHAR_PT1 -62 // normal degree sign
#define DEG_INT_CHAR_PT2 -80

/*
*   degree sign anomaly may appear when
*   receiving degree coordinate parameter from '.bat' or '.sh' file
*/
#define ANM_DEG_INT_CHAR_PT1 45 // anomaly degree sign
#define ANM_DEG_INT_CHAR_PT2 166

#include "mini-tool.cpp"
#include "menu.h"
#include "tools/xml/xml.h"
#include "tools/kml/kml.cpp"
#include "tools/txt/txt.h"
#include "tools/csv/csv.h"

void cropPinsCallback(
    xml::Node *kmlNode,
    std::vector<std::string*> coorStr,
    std::string fileDir_out,
    const std::function<void(xml::Node*, std::vector<std::string*>&)> &callback
);

int main(int argc, char *argv[]) {

    /*
    *   set input fit 'MAX_INPUT_STRINGS_COUNT' of
    *   'listStrings''s 1st dimension array count (in 'menu.h')
    */

    std::vector<std::string> inputStrings;
    for (int i = 1; i < argc; i++) {
        inputStrings.push_back(argv[i]);
    }
    argc--;

    if (argc < MAX_INPUT_STRINGS_COUNT) {
        for (int i = 0; i < MAX_INPUT_STRINGS_COUNT - argc; i++) {
            inputStrings.push_back("");
        }
    }
    else if (argc > MAX_INPUT_STRINGS_COUNT) {
        for (int i = 0; i < argc - MAX_INPUT_STRINGS_COUNT; i++) {
            inputStrings.pop_back();
        }
    }

    ////////////////////
    // MENU SELECTION //
    ////////////////////

    Menu menu = Menu();

    // make row space under the command in CLI
    std::cout << std::endl;

    switch (menu.select(inputStrings)) {
        case CONVERT_TXT_KML_FLAG: {
            txt::Scanner txtScanner;

            xml::Node *kmlNode = txtScanner.parse(
                inputStrings.at(2),
                inputStrings.at(4)
            );

            if (kmlNode) {
                std::cout << "\n**SUCCEEDED**\n";
                delete kmlNode;
            }
            else std::cerr << "\n**FAILED**\n";
        break;}
        case CONVERT_KML_CSV_FLAG: {
            xml::Reader kmlReader;
            xml::Node *kmlNode = kmlReader.fileParse(inputStrings.at(2));

            if (kmlNode) {
                xml::Node *mainFolderNode = kml::searchMainFolder(kmlNode);

                if (mainFolderNode) {
                    csv::Builder csvBuilder;
                    if (csvBuilder.compose(inputStrings.at(4), mainFolderNode)) {
                        std::cout << "\n**SUCCEEDED**\n";
                    }
                }
                else std::cerr << "\n**FAILED**\n";

                delete kmlNode;
            }
            else std::cerr
                << "KML-TOWN-> Error. '"
                << inputStrings.at(2)
                << "' not found or empty\n"
                << "\n**FAILED**\n";
        break;}
        case KML_CROP_FLAG: {
            kml::Cropper(PRINT_NOTIFICATION, &menu);
            xml::Reader kmlReader;

            cropPinsCallback(
                kmlReader.fileParse(inputStrings.at(2)),
                {&inputStrings.at(4), &inputStrings.at(6)},
                inputStrings.at(8),
                [=](xml::Node *node, std::vector<std::string*> &pt){}
            );
        break;}
        case KML_SORT_FLAG: {
            kml::Cropper(PRINT_NOTIFICATION, &menu);
            kml::Sorter(PRINT_NOTIFICATION, &menu);
            xml::Reader kmlReader;

            cropPinsCallback(
                kmlReader.fileParse(inputStrings.at(2)),
                {&inputStrings.at(4), &inputStrings.at(6)},
                inputStrings.at(8),
                
                // placemarks sorting callback
                [=](xml::Node *containerNode, std::vector<std::string*> &cropPt) {
                    kml::Sorter dirSort;
                    dirSort.orderPins(
                        containerNode,
                        kml::Point(*cropPt.at(0))
                    );
                }
            );
        break;}
        case KML_CREATE_PATH_FLAG: {

        break;}
        case KML_JOIN_PATHS_FLAG: {

        break;}
    }
    
    return 0;
}

void cropPinsCallback(
    xml::Node *kmlNode,
    std::vector<std::string*> coorStr,
    std::string fileDir_out,
    const std::function<void(xml::Node*, std::vector<std::string*>&)> &callback
) { 
    if (kmlNode) {
        xml::Node *mainFolderNode = kml::searchMainFolder(kmlNode);

        if (mainFolderNode) {
            mini_tool::completeDegreeCoordinateSecondsSign(coorStr.at(0));
            mini_tool::completeDegreeCoordinateSecondsSign(coorStr.at(1));

            // repair anomaly degree signs //

            for (auto &str : coorStr) {
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
                kml::Point(*coorStr.at(0)),
                kml::Point(*coorStr.at(1))
            );

            // test the cropped folder
            xml::Node *croppedFolder = mainFolderNode->getFirstChildByName("Folder", false);
            
            if (croppedFolder) {
                bool isFileWrite = true;

                // the pins must have been crop
                if (croppedFolder->getFirstChildByName("name")->getInnerText()
                    == CROP_COMMAND_WORKING_FOLDER
                ) {
                    // call the callback
                    callback(croppedFolder, coorStr);
                }
                else isFileWrite = false;

                // write file (end of process)
                if (isFileWrite) {
                    // set name of 'main folder' upto 'root' element as 'fileDir_out' name
                    kml::setKMLDocumentName(kmlNode, fileDir_out);

                    xml::Writer xmlWriter;
                    xmlWriter.stringify(fileDir_out, kmlNode);
                    std::cout << "\n**SUCCEEDED**\n";
                }
                // no need to write file if no cropped pins
                else std::cerr << "\n**FAILED**\n";
            }
            else std::cerr << "\n**FAILED**\n";
        }
        else std::cerr << "\n**FAILED**\n";

        delete kmlNode;
    }
    else std::cerr << "\n**FAILED**\n";
}