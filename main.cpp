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
#include "call-briefer.h"

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

        std::string fileDir_check = call_briefer::checkOverwrite(
            menu,
            SELECTED_FLAG,
            KML_CROP_OVERWRITE_FLAG,
            inputStrings.at(2),
            inputStrings.at(8)
        );

        if (fileDir_check != "") {
            xml::Reader kmlReader;
            xml::Node *kmlNode = kmlReader.fileParse(inputStrings.at(2));

            call_briefer::cropPinsFunc(
                kmlNode,
                {&inputStrings.at(4), &inputStrings.at(6)}
            );
            call_briefer::writeFileFunc(kmlNode, fileDir_check);
        }
    }
    else if (
        SELECTED_FLAG == KML_SORT_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_SORT_OVERWRITE_FLAG
    ) {
        kml::Cropper(PRINT_NOTIFICATION, &menu);
        kml::Sorter(PRINT_NOTIFICATION, &menu);

        std::string fileDir_check = call_briefer::checkOverwrite(
            menu,
            SELECTED_FLAG,
            KML_SORT_OVERWRITE_FLAG,
            inputStrings.at(2),
            inputStrings.at(8)
        );
        
        if (fileDir_check != "") {
            xml::Reader kmlReader;
            xml::Node *kmlNode = kmlReader.fileParse(inputStrings.at(2));

            call_briefer::sortPinsFunc(
                menu,
                kmlNode,
                {&inputStrings.at(4), &inputStrings.at(6)},
                false
            );
            call_briefer::writeFileFunc(kmlNode, fileDir_check);
        }
    }
    else if (
        SELECTED_FLAG == KML_PINS_PATH_CROP_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_PINS_PATH_CROP_OVERWRITE_FLAG
    ) {
        
    }
    
    return 0;
}