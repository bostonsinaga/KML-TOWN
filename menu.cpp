#ifndef __MENU_CPP__
#define __MENU_CPP__

#include "menu.h"

Menu::Menu() {
    mainMenuText_stream
        << "********************\n"
        << "*     KML-TOWN     *\n"
        << "* by boston_sinaga *\n"
        << "********************\n\n"
        << "MAIN MENU:\n"

        << "\n----------\n"
        << "BIG NOTES:\n"
        << "----------\n"
        << "-some of commands are not working because still in development\n"
        << "-'sorting' and 'cropping' for '.kml' only available for pins\n"
        << "-there may still be bugs that appear\n"

        << "\n~HELP COMMANDS:\n\n"
        << ">>> --help or --menu\n"
        << "****** show available commands\n\n"
        << ">>> --style-names\n"
        << "****** show available input's pin icon and path color names (strings)\n\n"
        << ">>> --version\n"
        << "****** show version number\n"

        << "\n~CONVERTING COMMANDS:\n\n"
        << "~~NOTE:\n"
        << "    -[PLACEMARK_TYPE] value is 'pins' or 'paths'\n\n"
        
        << ">>> --convert --txt-in [FILE_NAME] --kml-out [FILE_NAME] --type [PLACEMARK_TYPE]\n"
        << "****** read '.txt' and write location keywords as '.kml' placemarks.\n"
        << "       available types: 'pins', 'paths', 'paths-sort'\n\n"
        
        << ">>> --convert --kml-in [FILE_NAME] --csv-out [FILE_NAME] :\n"
        << "****** read '.kml' and write placemarks information as formated '.csv' sheet\n"
        << "       Notes:\n"
        << "       -the first multiple folders will be the columns\n"
        << "       -set column/folder name as 'PATH', 'PATHS' or 'JALUR' to generate path column format\n"

        << "\n~KML EDITOR COMMANDS:\n\n"
        << "~~NOTES:\n"
        << "    -DMS stands for Degrees Minutes Seconds\n"
        << "    -not using '--out' means overwrite the file\n"
        << "    -use '--start-point' and '--end-point' to select desired area in one '.kml'\n"
        << "     (if degree coordinate as the input, please remove the double quote signs (\") [usually in DMS coordinate]\n"
        << "     to prevent unexpected or overload command parameters)\n"
        << "    -selecting area is mandatory in 'crop', 'sort' and 'pins-path'\n\n"

        << "--kml --crop [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
        << "****** crop '.kml' by rectangular area generated from start to end point\n"
        << "       and insert them into new folder named '" << CROP_COMMAND_WORKING_FOLDER << "'\n\n"

        << "--kml --sort [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
        << "****** sort '.kml' start with closest placemark from start point\n"
        << "       then chaining to nearest one but only inside start to end point formed rectangular area\n"
        << "       and finally insert them into new folder named '" << SORT_COMMAND_WORKING_FOLDER << "'\n\n"

        << "--kml --pins-path [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
        << "****** draw path from pins in certain area selection\n"
        << "       and insert the path into new folder named '" << PINS_PATH_COMMAND_WORKING_FOLDER << "'\n\n"

        << "~~NOTES:\n"
        << "    -selecting area still possible here\n"
        << "    -not using area selection means process all entire '.kml' placemarks\n\n"

        << "--kml --paths-pins [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
        << "****** set points of paths as coordinate of new pins\n"
        << "       and insert the pins into new folder named '" << PATHS_PINS_COMMAND_WORKING_FOLDER << "'\n\n"

        << "--kml --join-paths [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
        << "****** join paths as one path\n"
        << "       and insert them into new folder named '" << JOIN_PATHS_COMMAND_WORKING_FOLDER << "'\n\n"

        << "--kml --split-paths [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
        << "****** split paths (get more points)\n"
        << "       and insert them into new folder named '" << SPLIT_PATHS_COMMAND_WORKING_FOLDER << "'\n\n"

        << "~~NOTE:\n"
        << "    -below commands cannot use area selection (but on styling it will prompt)\n\n"

        << "--kml --twins-check [FILE_NAME] --type [PLACEMARK_TYPE] --radius [DECIMAL_NUMBER] --out [FILE_NAME] --include-folder <optional>\n"
        << "****** checking placemark twins with given radius (0-100 meters)\n"
        << "       and insert them into new folder named '" << TWINS_CHECK_COMMAND_WORKING_FOLDER << "'\n"
        << "****** '[PLACEMARK_TYPE]' -> input can be a string of 'pin-ignore', 'path-ignore', 'all-ignore'\n"
        << "                             'pin-style', 'path-style' or 'all-style'\n"
        << "****** '--include-folder' -> include current placemarks folder/document into working folder\n\n"

        << "--kml --measure-paths [FILE_NAME] --out [FILE_NAME]\n"
        << "****** measure paths distance (whole input document)\n\n"

        << "--kml --measure-paths [FILE_NAME] --info\n"
        << "****** only get paths distance information without write file (whole input document)\n\n"

        << "--kml --remove-paths [FILE_NAME] --under-distance [MAX_DISTANCE] --out [FILE_NAME]\n"
        << "****** remove paths those have distance equal or less than given distance\n"
        << "       (input distance in meters)\n\n"

        << "--kml --remove-paths [FILE_NAME] --over-distance [MIN_DISTANCE] --out [FILE_NAME]\n"
        << "****** remove paths those have distance equal or more than given distance\n"
        << "       (input distance in meters)\n\n"

        << "--kml --classify [FILE_NAME] --out [FILE_NAME] --clean-folder <optional, choose> --include-folder <optional, choose> --include-clean-folder <optional, choose>\n"
        << "****** classify placemarks based on their style (useful for '.csv' columns maker)\n"
        << "       and insert them into new folder named '" << CLASSIFY_COMMAND_WORKING_FOLDER << "\n"
        << "****** '--clean-folder' -> clean empty folders\n"
        << "****** '--include-folder' -> include placemarks folder/document into working folder\n"
        << "****** '--include-clean-folder' -> include placemarks folder and clean empty folders\n\n"

        << "--kml --folder-by-date [FILE_NAME] --out [FILE_NAME]\n"
        << "****** pack placemarks by their date into new folder named '"<< FOLDERBYDATE_COMMAND_WORKING_FOLDER << "'\n\n"

        << "~~NOTE:\n"
        << "    -[STRING] can be any words\n\n"

        << "--kml --folder-by-text [FILE_NAME] --string [STRING] --out [FILE_NAME]\n"
        << "****** pack placemarks by search string inside their name or description\n"
        << "       and insert them into new folder named '" << FOLDERBYTEXT_COMMAND_WORKING_FOLDER << "'\n\n"

        << "~~NOTES:\n"
        << "    -no need to inserting into new folder\n"
        << "     the placemarks will stay in their directory (parent element)\n"
        << "    -please look at '--style-names' for available style names\n"
        << "    -area selection will prompt after first parameters set (enter pressed)\n\n"
        
        << "--kml --style-pins [FILE_NAME] --icon [PIN_ICON_NAME] --out [FILE_NAME]\n"
        << "****** change pins icon (default is 'ylw-pushpin' for '.txt' pins scanner)\n\n"

        << "--kml --style-pins [FILE_NAME] --scale [PIN_ICON_NAME] --out [FILE_NAME]\n"
        << "****** set pins scale (default is 1.0)\n\n"
        
        << "--kml --style-paths [FILE_NAME] --color [PATH_COLOR_NAME] --out [FILE_NAME]\n"
        << "****** change paths color (default is white)\n\n"

        << "--kml --style-paths [FILE_NAME] --thickness [PATH_COLOR_NAME] --out [FILE_NAME]\n"
        << "****** change paths thickness (default is 2.0)\n\n"

        << "--kml --style-paths [FILE_NAME] --opacity [PATH_COLOR_NAME] --out [FILE_NAME]\n"
        << "****** change paths opacity (default is 1.0 [0.0 - 1.0])\n\n"

        << "~~NOTE:\n"
        << "    -below is '.csv' editor tool\n\n"

        << "--csv --change-separator [FILE_NAME] --old-sign [STRING] --new-sign [STRING] --out [FILE_NAME]\n"
        << "****** change '.csv' columns separator string (sign)\n"
        << "       (consider to not use space in string to avoid parsing error)\n";

    styleNamesMenuText_stream
        << "********************\n"
        << "*     KML-TOWN     *\n"
        << "* by boston_sinaga *\n"
        << "********************\n\n"
        << "STYLE NAMES MENU:\n\n"

        << "~BELOW ARE AVAILABLE INPUT STRINGS FOR [PIN_ICON_NAME]:\n"

        << "\n>>> PUSHPIN:\n\n"
        << "ylw-pushpin\n"
        << "blue-pushpin\n"
        << "grn-pushpin\n"
        << "ltblu-pushpin\n"
        << "pink-pushpin\n"
        << "purple-pushpin\n"
        << "red-pushpin\n"
        << "wht-pushpin\n"

        << "\n>>> PADDLES:\n\n"
        << "A           blu-blank\n"
        << "B           blu-diamond\n"
        << "C           blu-circle\n"
        << "D           blu-square\n"
        << "E           blu-stars\n"
        << "F           grn-blank\n"
        << "G           grn-diamond\n"
        << "H           grn-circle\n"
        << "I           grn-square\n"
        << "J           grn-stars\n"
        << "K           ltblu-blank\n"
        << "L           ltblu-diamond\n"
        << "M           ltblu-circle\n"
        << "N           ltblu-square\n"
        << "O           ltblu-stars\n"
        << "P           pink-blank\n"
        << "Q           pink-diamond\n"
        << "R           pink-circle\n"
        << "S           pink-square\n"
        << "T           pink-stars\n"
        << "U           ylw-blank\n"
        << "V           ylw-diamond\n"
        << "W           ylw-circle\n"
        << "X           ylw-square\n"
        << "Y           ylw-stars\n"
        << "Z           wht-blank\n"
        << "1           wht-diamond\n"
        << "2           wht-circle\n"
        << "3           wht-square\n"
        << "4           wht-stars\n"
        << "5           red-blank\n"
        << "6           red-diamond\n"
        << "7           red-circle\n"
        << "8           red-square\n"
        << "9           red-stars\n"
        << "10          purple-blank\n"
        << "            purple-diamond\n"
        << "            purple-circle\n"
        << "            purple-square\n"
        << "            purple-stars\n"

        << "\n>>> SHAPES:\n\n"
        << "arrow-reverse           airports                golf\n"
        << "arrow                   ferry                   trail\n"
        << "track                   heliport                shopping\n"
        << "donut                   subway                  movies\n"
        << "forbidden               tram                    convenience\n"
        << "info-i                  info                    grocery\n"
        << "polygon                 info_circle             arts\n"
        << "open-diamond            flag                    homegardenbusiness\n"
        << "square                  rainy                   electronics\n"
        << "star                    water                   mechanic\n"
        << "target                  snowflake_simple        gas_stations\n"
        << "triangle                marina                  realestate\n"
        << "cross-hairs             fishing                 salon\n"
        << "placemark_square        sailing                 dollar\n"
        << "placemark_circle        swimming                euro\n"
        << "shaded_dot              ski                     yen\n"
        << "dining                  parks                   firedept\n"
        << "coffee                  campfire                hospitals\n"
        << "bars                    picnic                  lodging\n"
        << "snack_bar               campground              phone\n"
        << "man                     ranger_station          caution\n"
        << "woman                   toilets                 earthquake\n"
        << "wheel_chair_accessible  poi                     falling_rocks\n"
        << "parking_lot             hiker                   post_office\n"
        << "cabs                    cycling                 police\n"
        << "bus                     motorcycling            sunny\n"
        << "truck                   horsebackriding         partly_cloudy\n"
        << "rail                    play                    volcano\n"
        << "                                                camera\n"
        << "                                                webcam\n\n"

        << "~BELOW ARE AVAILABLE INPUT STRINGS FOR [PATH_COLOR_NAME]:\n"

        << "\n>>> COLORS:\n\n"
        << "red             green          blue\n"
        << "yellow          orange         cyan\n"
        << "magenta         purple         black\n"
        << "chocolate       white          gray\n";

    // menu respons list
    menuResponseStrings[0] = mainMenuText_stream.str();
    menuResponseStrings[1] = mainMenuText_stream.str();
    menuResponseStrings[2] = styleNamesMenuText_stream.str();
    menuResponseStrings[3] = "KML-TOWN v0.10.4 ALPHA 2023-03-22\n";
}

int Menu::select(std::vector<std::string> inputStrings) {

    int passCount = 0, retFlag = -1;
    bool isUsingSelectionArea = false;
    
    // loop 'TOTAL_COMMANDS_COUNT' times
    for (int i = 0; i < sizeof(listStrings) / sizeof(*listStrings); i++) {

        for (int j = 0; j < MAX_INPUT_STRINGS_COUNT; j++) {
            if (inputStrings.at(j) == listStrings[i][j] ||
                (inputStrings.at(j) != "" && listStrings[i][j] == "DATA_STRING")
            ) {
                passCount++;
            }

            if (!isUsingSelectionArea && inputStrings.at(j) == "--start-point") {
                isUsingSelectionArea = true;
            }
        }
        
        if (passCount == MAX_INPUT_STRINGS_COUNT) {
            if (i <= VERSION_NUMBER_FLAG) {
                std::cout << menuResponseStrings[i];
            }
            retFlag = i;
            break;
        }
        else passCount = 0;
    }

    if (passCount != MAX_INPUT_STRINGS_COUNT) {
        std::cerr << getErrorString(isUsingSelectionArea);
    }

    return retFlag;
}

void Menu::setNotification(std::string message, bool isNeedEnterKey) {
    std::cout << message;
    if (isNeedEnterKey) {
        std::cout << "\n**PRESS ENTER TO CONTINUE**\n";
        std::cin.get();
    }
    else std::cout << std::endl; // line separator
}

bool Menu::setAlert(std::string message, bool isUseFailedMessage) {
    std::cout << message << "\n**PRESS Y[yes] / N[no]**\n";

    std::string answer;
    std::cin >> answer;
    std::cout << std::endl;

    if (answer == "Y" || answer == "y") {
        return true;
    }
    else {
        if (isUseFailedMessage) {
            std::cerr << "KML-TOWN-> Command canceled!\n\n**FAILED**\n";
        }
        return false;
    }
}

// input is first CLI strings
std::string Menu::setAdditionalInput(std::string message) {
    std::cout << message;

    std::string input;
    std::cin >> input;
    std::cout << std::endl;

    return input;
}

std::string Menu::getErrorString(bool isUseDMSErrorMessage) {
    std::stringstream retStr_stream;

    if (isUseDMSErrorMessage) {
        retStr_stream
            << "KML-TOWN-> You may have to remove double quote signs (\") of degree (DMS) coordinate\n"
            << "           to prevent unexpected or overload command parameters!\n\n";
    }

    retStr_stream
        << "KML-TOWN-> Command error. Type '--help' for available commands\n"
        << "\n**FAILED**\n";

    return retStr_stream.str();
}

#endif // __MENU_CPP__