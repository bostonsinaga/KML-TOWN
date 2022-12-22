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

// value in kilo meters
#define EQUATOR_DISTANCE  40075.00 // longitude (-180° to 180°)
#define MERIDIAN_DISTANCE 20003.93 // latitude  (-90° to 90°)

#include "mini-tool.cpp"
#include "menu.cpp"
#include "tools/xml/xml.h"
#include "tools/kml/kml.h"
#include "tools/txt/txt.h"
#include "tools/csv/csv.h"
#include "call-briefer.cpp"

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

    ////////////////////////
    //// MENU SELECTION ////
    ////////////////////////

    Menu menu = Menu();

    // make row space under the command in CLI
    std::cout << std::endl;

    // 50 available commands (most varied with different parameters)
    int SELECTED_FLAG = menu.select(inputStrings);

    //////////////////////////
    // CONVERT TXT KML FLAG //
    //////////////////////////

    if (SELECTED_FLAG == CONVERT_TXT_KML_FLAG) {

        xml::Node *kmlNode = call_briefer::selectFunctionByPlacemarkType(
            inputStrings.at(6),
            [=]()->xml::Node* {return txt::Scanner().parsePins(
                inputStrings.at(2), inputStrings.at(4)
            );},
            [=]()->xml::Node* {return txt::Scanner().parsePaths(
                inputStrings.at(2), inputStrings.at(4)
            );}
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
            std::string separatorSign = "|";

            if (menu.setAlert(
                "KML-TOWN-> Do you want to change columns separator? (default is '|')\n",
                false
            )) {
                csv::singleCharacterInputNotify(menu, false);
                std::string additionalInput = menu.setAdditionalInput(
                    "KML-TOWN-> Set columns separator sign:  "
                );
                separatorSign = additionalInput;
            }

            xml::Node *mainFolderNode = kml::General().searchMainFolder(kmlNode);

            if (mainFolderNode) {
                csv::Builder csvBuilder;
                if (csvBuilder.compose(inputStrings.at(4), mainFolderNode, separatorSign)) {
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

    //////////////
    // KML CROP //
    //////////////
    
    else if (
        SELECTED_FLAG == KML_CROP_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_CROP_OVERWRITE_FLAG
    ) {
        kml::Cropper().printNotification(menu);

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

            if (call_briefer::cropPinsFunc(
                kmlNode,
                {&inputStrings.at(4), &inputStrings.at(6)},
                true
            ).size() > 0) {
                call_briefer::writeFileFunc(kmlNode, fileDir_check);
            }
        }
    }

    //////////////
    // KML SORT //
    //////////////

    else if (
        SELECTED_FLAG == KML_SORT_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_SORT_OVERWRITE_FLAG
    ) {
        kml::Cropper().printNotification(menu);
        kml::Sorter().printNotification(menu);

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

            if (call_briefer::sortPinsFunc(
                menu,
                kmlNode,
                {&inputStrings.at(4), &inputStrings.at(6)},
                true
            ).size() > 0) {
                call_briefer::writeFileFunc(kmlNode, fileDir_check);
            }
        }
    }

    //////////////////////
    // KML PINS TO PATH //
    //////////////////////

    else if (
        SELECTED_FLAG == KML_PINS_PATH_CROP_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_PINS_PATH_CROP_OVERWRITE_FLAG
    ) {
        kml::Cropper().printNotification(menu);
        kml::Sorter().printNotification(menu);

        std::string fileDir_check = call_briefer::checkOverwrite(
            menu,
            SELECTED_FLAG,
            KML_PINS_PATH_CROP_OVERWRITE_FLAG,
            inputStrings.at(2),
            inputStrings.at(8)
        );
        
        if (fileDir_check != "") {
            xml::Reader kmlReader;
            xml::Node *kmlNode = kmlReader.fileParse(inputStrings.at(2));

            std::vector<xml::Node*> sortedPinNodes = call_briefer::sortPinsFunc(
                menu,
                kmlNode,
                {&inputStrings.at(4), &inputStrings.at(6)},
                false
            );

            if (sortedPinNodes.size() > 0) {
                kml::Placemark().pinsPath(
                    kmlNode,
                    sortedPinNodes
                );
                call_briefer::writeFileFunc(kmlNode, fileDir_check);
            }
        }
    }

    /////////////////////
    // KML TWINS CHECK //
    /////////////////////

    else if (
        SELECTED_FLAG == KML_TWINS_CHECK_NEWFILE_FLAG ||
        SELECTED_FLAG == KML_TWINS_CHECK_OVERWRITE_FLAG
    ) {
        std::string fileDir_check = call_briefer::checkOverwrite(
            menu,
            SELECTED_FLAG,
            KML_TWINS_CHECK_OVERWRITE_FLAG,
            inputStrings.at(2),
            inputStrings.at(8)
        );
        
        if (fileDir_check != "") {
            xml::Reader kmlReader;

            xml::Node
                *kmlNode = kmlReader.fileParse(inputStrings.at(2)),
                *twinsCheckedFolder;

            twinsCheckedFolder = call_briefer::selectFunctionByPlacemarkType(
                inputStrings.at(4),
                [=]()->xml::Node* {return kml::TwinsChecker().findPins(kmlNode, inputStrings.at(6));},
                [=]()->xml::Node* {return kml::TwinsChecker().findPaths(kmlNode, inputStrings.at(6));}
            );

            if (twinsCheckedFolder) {
                call_briefer::writeFileFunc(kmlNode, fileDir_check);
            }
            else {
                delete kmlNode;
                std::cerr << "\n**FAILED**\n";
            }
        }
    }

    //////////////////////////
    // CSV CHANGE SEPARATOR //
    //////////////////////////

    else if (
        SELECTED_FLAG == CSV_CHANGE_SEPARATOR_NEWFILE_FLAG ||
        SELECTED_FLAG == CSV_CHANGE_SEPARATOR_OVERWRITE_FLAG
    ) {
        std::string fileDir_check = call_briefer::checkOverwrite(
            menu,
            SELECTED_FLAG,
            CSV_CHANGE_SEPARATOR_OVERWRITE_FLAG,
            inputStrings.at(2),
            inputStrings.at(8)
        );
        
        if (fileDir_check != "") {
            csv::singleCharacterInputNotify(menu, true);

            csv::changeCSVSeparator(
                inputStrings.at(2),
                fileDir_check,
                inputStrings.at(4),
                inputStrings.at(6)
            );
        }
    }
    
    return 0;
}