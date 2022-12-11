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

std::string checkOverwrite(
    Menu &menu,
    int selectedFlag,
    int overwriteFlag,
    std::string &fileDir_in,
    std::string &fileDir_out
);

xml::Node *cropPinsModule(
    xml::Node *kmlNode,
    std::vector<std::string*> coorStrVec
);

std::vector<std::string> sortPinsModule(
    Menu &menu,
    xml::Node *kmlNode,
    std::vector<std::string*> coorStrVec_in,
    bool isReturnCoordinates
);

void writeFileModule(
    xml::Node *kmlNode,
    std::string &fileDir_out
);

//////////
// MAIN //
//////////

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

    // 50 available commands (most varied with different parameters)
    int SELECTED_FLAG = menu.select(inputStrings);

    //////////////////////////
    // CONVERT TXT KML FLAG //
    //////////////////////////

    if (SELECTED_FLAG == CONVERT_TXT_KML_FLAG) {
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
    }

    //////////////////////////
    // CONVERT KML CSV FLAG //
    //////////////////////////

    else if (SELECTED_FLAG == CONVERT_KML_CSV_FLAG) {
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
    }
    else if (
        SELECTED_FLAG == KML_CROP_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_CROP_OVERWRITE_FLAG
    ) {
        kml::Cropper(PRINT_NOTIFICATION, &menu);

        std::string fileDir_check = checkOverwrite(
            menu,
            SELECTED_FLAG,
            KML_CROP_OVERWRITE_FLAG,
            inputStrings.at(2),
            inputStrings.at(8)
        );

        if (fileDir_check != "") {
            xml::Reader kmlReader;
            xml::Node *kmlNode = kmlReader.fileParse(inputStrings.at(2));

            cropPinsModule(
                kmlNode,
                {&inputStrings.at(4), &inputStrings.at(6)}
            );
            writeFileModule(kmlNode, fileDir_check);
        }
    }
    else if (
        SELECTED_FLAG == KML_SORT_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_SORT_OVERWRITE_FLAG
    ) {
        kml::Cropper(PRINT_NOTIFICATION, &menu);
        kml::Sorter(PRINT_NOTIFICATION, &menu);
        
        std::string fileDir_check = checkOverwrite(
            menu,
            SELECTED_FLAG,
            KML_SORT_OVERWRITE_FLAG,
            inputStrings.at(2),
            inputStrings.at(8)
        );
        
        if (fileDir_check != "") {
            xml::Reader kmlReader;
            xml::Node *kmlNode = kmlReader.fileParse(inputStrings.at(2));

            sortPinsModule(
                menu,
                kmlNode,
                {&inputStrings.at(4), &inputStrings.at(6)},
                false
            );
            writeFileModule(kmlNode, fileDir_check);
        }
    }
    else if (
        SELECTED_FLAG == KML_PINS_PATH_CROP_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_PINS_PATH_CROP_OVERWRITE_FLAG
    ) {
        
    }
    
    return 0;
}

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

xml::Node *cropPinsModule(
    xml::Node *kmlNode,
    std::vector<std::string*> coorStrVec
) {
    xml::Node *retContainerNode = nullptr;
    
    if (kmlNode) {
        xml::Node *mainFolderNode = kml::searchMainFolder(kmlNode);

        if (mainFolderNode) {
            mini_tool::completeDegreeCoordinateSecondsSign(coorStrVec.at(0));
            mini_tool::completeDegreeCoordinateSecondsSign(coorStrVec.at(1));

            // repair anomaly degree signs //

            for (auto &str : coorStrVec) {
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
                kml::Point(*coorStrVec.at(0)),
                kml::Point(*coorStrVec.at(1))
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

std::vector<std::string> sortPinsModule(
    Menu &menu,
    xml::Node *kmlNode,
    std::vector<std::string*> coorStrVec_in,
    bool isReturnCoordinates
) {
    xml::Node *croppedFolderNode = cropPinsModule(
        kmlNode,
        coorStrVec_in
    );

    // pins sorting //
    kml::Sorter sorter;
    std::vector<std::string> coorStrVec_sorted = sorter.orderPins(
        croppedFolderNode,
        kml::Point(*coorStrVec_in.at(0)), // start point
        isReturnCoordinates
    );

    return coorStrVec_sorted;
}

// write file (end of process)
void writeFileModule(
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