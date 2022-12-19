#ifndef __MENU_H__
#define __MENU_H__

#define TOTAL_COMMANDS_COUNT 49
#define MAX_INPUT_STRINGS_COUNT 9

#define MAIN_MENU_FLAG 0
#define HELP_MENU_FLAG 1
#define STYLE_NAMES_MENU_FLAG 2
#define VERSION_NUMBER_FLAG 3
#define CONVERT_TXT_KML_AS_PINS_FLAG 4
#define CONVERT_TXT_KML_AS_PATHS_FLAG 5
#define CONVERT_KML_CSV_FLAG 6
#define KML_CROP_NEWFILE_FLAG 7
#define KML_CROP_OVERWRITE_FLAG 8
#define KML_SORT_NEWFILE_FLAG 9
#define KML_SORT_OVERWRITE_FLAG 10
#define KML_PINS_PATH_CROP_NEWFILE_FLAG 11
#define KML_PINS_PATH_CROP_OVERWRITE_FLAG 12
#define KML_PATHS_PINS_CROP_NEWFILE_FLAG 13
#define KML_PATHS_PINS_CROP_OVERWRITE_FLAG 14
#define KML_PATHS_PINS_NEWFILE_FLAG 15
#define KML_PATHS_PINS_OVERWRITE_FLAG 16
#define KML_JOIN_PATHS_CROP_NEWFILE_FLAG 17
#define KML_JOIN_PATHS_CROP_OVERWRITE_FLAG 18
#define KML_JOIN_PATHS_NEWFILE_FLAG 19
#define KML_JOIN_PATHS_OVERWRITE_FLAG 20
#define KML_SPLIT_PATHS_CROP_NEWFILE_FLAG 21
#define KML_SPLIT_PATHS_CROP_OVERWRITE_FLAG 22
#define KML_SPLIT_PATHS_NEWFILE_FLAG 23
#define KML_SPLIT_PATHS_OVERWRITE_FLAG 24
#define KML_FILTER_AUTO_NEWFILE_FLAG 25
#define KML_FILTER_AUTO_FLAG 26
#define KML_FILTER_PINICON_NEWFILE_FLAG 27
#define KML_FILTER_PINICON_OVERWRITE_FLAG 28
#define KML_FILTER_PATHCOLOR_NEWFILE_FLAG 29
#define KML_FILTER_PATHCOLOR_OVERWRITE_FLAG 30
#define KML_FILTER_NAME_NEWFILE_FLAG 31
#define KML_FILTER_NAME_OVERWRITE_FLAG 32
#define KML_FILTER_DESCRIPTION_NEWFILE_FLAG 33
#define KML_FILTER_DESCRIPTION_OVERWRITE_FLAG 34
#define KML_FILTER_TEXT_NEWFILE_FLAG 35
#define KML_FILTER_TEXT_OVERWRITE_FLAG 36
#define KML_STYLEPINS_ICON_NEWFILE_FLAG 37
#define KML_STYLEPINS_ICON_OVERWRITE_FLAG 38
#define KML_STYLEPINS_SCALE_NEWFILE_FLAG 39
#define KML_STYLEPINS_SCALE_OVERWRITE_FLAG 40
#define KML_STYLEPATH_COLOR_NEWFILE_FLAG 41
#define KML_STYLEPATH_COLOR_OVERWRITE_FLAG 42
#define KML_STYLEPATH_THICKNESS_NEWFILE_FLAG 43
#define KML_STYLEPATH_THICKNESS_OVERWRITE_FLAG 44
#define KML_STYLEPATH_OPACITY_NEWFILE_FLAG 45
#define KML_STYLEPATH_OPACITY_OVERWRITE_FLAG 46
#define CSV_CHANGE_SEPARATOR_NEWFILE_FLAG 47
#define CSV_CHANGE_SEPARATOR_OVERWRITE_FLAG 48

#define COMMAND_WORKING_FOLDER "--KML-TOWN --"
#define CROP_COMMAND_WORKING_FOLDER "--KML-TOWN --CROP"
#define SORT_COMMAND_WORKING_FOLDER "--KML-TOWN --SORT"
#define PINS_PATH_COMMAND_WORKING_FOLDER "--KML-TOWN --PINS-PATH"
#define PATHS_PINS_COMMAND_WORKING_FOLDER "--KML-TOWN --PATHS-PINS"
#define JOIN_PATHS_COMMAND_WORKING_FOLDER "--KML-TOWN --JOIN-PATHS"
#define SPLIT_PATHS_COMMAND_WORKING_FOLDER "--KML-TOWN --SPLIT-PATHS"
#define CLASSIFY_COMMAND_WORKING_FOLDER "--KML-TOWN --CLASSIFY"
#define FILTER_PINICON_COMMAND_WORKING_FOLDER "--KML-TOWN --FILTER-PINICON"
#define FILTER_PATHCOLOR_COMMAND_WORKING_FOLDER "--KML-TOWN --FILTER-PATHCOLOR"
#define FILTER_NAME_COMMAND_WORKING_FOLDER "--KML-TOWN --FILTER-NAME"
#define FILTER_DESCRIPTION_COMMAND_WORKING_FOLDER "--KML-TOWN --FILTER-DESCRIPTION"
#define FILTER_TEXT_COMMAND_WORKING_FOLDER "--KML-TOWN --FILTER-TEXT"

#define PRINT_NOTIFICATION 1

class Menu {
    public:
        Menu() {
            mainMenuText_stream
                << "********************\n"
                << "*     KML-TOWN     *\n"
                << "* by boston_sinaga *\n"
                << "********************\n\n"
                << "MAIN MENU:\n"

                << "\n~HELP COMMANDS:\n\n"
                << ">>> --help or --menu\n"
                << "****** show available commands\n\n"
                << ">>> --style-names\n"
                << "****** show available input's pin icon and path color names (strings)\n\n"
                << ">>> --version\n"
                << "****** show version number\n"

                << "\n~CONVERTING COMMANDS:\n\n"
                << ">>> --convert --txt-in [FILE_NAME] --kml-out [FILE_NAME] --[pins or paths]\n"
                << "****** read '.txt' and write location keywords as '.kml' placemarks\n\n"
                << ">>> --convert --kml-in [FILE_NAME] --csv-out [FILE_NAME] : \n"
                << "****** read '.kml' and write placemarks information as formated '.csv' sheet\n"

                << "\n~KML EDITOR COMMANDS:\n\n"
                << "~~NOTE:\n"
                << "    -DMS stands for Degrees Minutes Seconds\n"
                << "    -not using '--out' means overwrite the file\n"
                << "    -use '--start-point' and '--end-point' to select desired area in one '.kml'\n"
                << "     (if degree coordinate as the input, please remove the double quote signs (\") [usually in DMS coordinate]\n"
                << "      to prevent unexpected or overload command parameters)\n"
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
                <<"        and insert the path into new folder named '" << PINS_PATH_COMMAND_WORKING_FOLDER << "'\n\n"

                << "~~NOTE:\n"
                << "    -selecting area still possible here\n"
                << "    -not using area selection means process all entire '.kml' placemarks\n\n"

                << "--kml --paths-pins [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
                << "****** set points of paths as coordinate of new pins\n"
                <<"        and insert the pins into new folder named '" << PATHS_PINS_COMMAND_WORKING_FOLDER << "'\n\n"

                << "--kml --join-paths [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
                << "****** join paths as one path \n"
                << "       and insert them into new folder named '" << JOIN_PATHS_COMMAND_WORKING_FOLDER << "'\n\n"

                << "--kml --split-paths [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
                << "****** split paths (get more points)\n"
                << "       and insert them into new folder named '" << SPLIT_PATHS_COMMAND_WORKING_FOLDER << "'\n\n"

                << "~~NOTE:\n"
                << "    -below commands cannot use area selection (but on styling it will prompt)\n"
                << "    -[STRING] can be any words\n\n"

                << "--kml --classify [FILE_NAME] --out [FILE_NAME]\n"
                << "****** classify placemarks based on their type or style\n"
                << "       and insert them into new folder named '" << CLASSIFY_COMMAND_WORKING_FOLDER << "'\n"
                << "       (their type or style being sub folder name)\n\n"

                << "~~NOTE:\n"
                << "    -style and text editor will prompt after\n"
                << "     first parameters set (enter pressed)\n\n"

                << "--kml --filter [FILE_NAME] --pin-icon [PIN_ICON_NAME] --out [FILE_NAME]\n"
                << "****** filter placemarks based on their pin icon style\n"
                << "       and insert them into new folder named '" << FILTER_PINICON_COMMAND_WORKING_FOLDER << "'\n\n"

                << "--kml --filter [FILE_NAME] --path-color [PATH_COLOR_NAME] --out [FILE_NAME]\n"
                << "****** filter placemarks based on their path color style\n"
                << "       and insert them into new folder named '" << FILTER_PATHCOLOR_COMMAND_WORKING_FOLDER << "'\n\n"

                << "--kml --filter [FILE_NAME] --name [STRING] --out [FILE_NAME]\n"
                << "****** filter placemarks based on their name string\n"
                << "       and insert them into new folder named '" << FILTER_NAME_COMMAND_WORKING_FOLDER << "'\n\n"
                
                << "--kml --filter [FILE_NAME] --description [STRING] --out [FILE_NAME]\n"
                << "****** filter placemarks based on their description string\n"
                << "       and insert them into new folder named '" << FILTER_DESCRIPTION_COMMAND_WORKING_FOLDER << "'\n\n"

                << "--kml --filter [FILE_NAME] --text [STRING] --out [FILE_NAME]\n"
                << "****** filter placemarks based on their name or description string\n"
                << "       and insert them into new folder named '" << FILTER_TEXT_COMMAND_WORKING_FOLDER << "'\n\n"

                << "~~NOTE:\n"
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
            menuResponseStrings[3] = "KML-TOWN v.0.3.0 2022-12-09\n";
        }

        int select(std::vector<std::string> inputStrings) {

            int passCount = 0, retFlag = -1;
            bool isUsingKMLTool = false;
            
            // loop 'TOTAL_COMMANDS_COUNT' times
            for (int i = 0; i < sizeof(listStrings) / sizeof(*listStrings); i++) {

                for (int j = 0; j < MAX_INPUT_STRINGS_COUNT; j++) {
                    if (inputStrings.at(j) == listStrings[i][j] ||
                        (inputStrings.at(j) != "" && listStrings[i][j] == "DATA_STRING")
                    ) {
                        passCount++;
                    }

                    if (!isUsingKMLTool && inputStrings.at(j) == "--start-point") {
                        isUsingKMLTool = true;
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
                std::cerr << getErrorString(isUsingKMLTool);
            }

            return retFlag;
        }

        void setNotification(std::string message, bool isNeedEnterKey = true) {
            std::cout << message;
            if (isNeedEnterKey) {
                std::cout << "\n**PRESS ENTER TO CONTINUE**\n";
                std::cin.get();
            }
            else std::cout << std::endl; // line separator
        }

        bool setAlert(std::string message, bool isUseFailedMessage = true) {
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
        std::string setAdditionalInput(std::string message) {
            std::cout << message;

            std::string input;
            std::cin >> input;
            std::cout << std::endl;

            return input;
        }

    private:
        std::string listStrings[TOTAL_COMMANDS_COUNT][MAX_INPUT_STRINGS_COUNT] = {

            // BASIC //
            {"--menu", "", "", "", "", "", "", "", ""},
            {"--help", "", "", "", "", "", "", "", ""},
            {"--style-names", "", "", "", "", "", "", "", ""},
            {"--version", "", "", "", "", "", "", "", ""},

            // CONVERTING //
            {"--convert", "--txt-in", "DATA_STRING", "--kml-out", "DATA_STRING", "--pins", "", "", ""},
            {"--convert", "--txt-in", "DATA_STRING", "--kml-out", "DATA_STRING", "--paths", "", "", ""},
            {"--convert", "--kml-in", "DATA_STRING", "--csv-out", "DATA_STRING", "", "", "", ""},

            // KML EDITOR (no '--out' means output will overwrite file input) //

            // >>>> CROP
            {"--kml", "--crop", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--crop", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", ""},

            // >>>> SORT
            {"--kml", "--sort", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--sort", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", ""},

            // >>>> PINS AND PATH
            {"--kml", "--pins-path", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--pins-path", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", ""},

            {"--kml", "--paths-pins", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--paths-pins", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", ""},
            {"--kml", "--paths-pins", "DATA_STRING", "--out", "DATA_STRING", "", "", "", ""},
            {"--kml", "--paths-pins", "DATA_STRING", "", "", "", "", "", ""},

            {"--kml", "--join-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--join-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", ""},
            {"--kml", "--join-paths", "DATA_STRING", "--out", "DATA_STRING", "", "", "", ""},
            {"--kml", "--join-paths", "DATA_STRING", "", "", "", "", "", ""},

            {"--kml", "--split-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--split-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", ""},
            {"--kml", "--split-paths", "DATA_STRING", "--out", "DATA_STRING", "", "", "", ""},
            {"--kml", "--split-paths", "DATA_STRING", "", "", "", "", "", ""},

            // >>>> FILTER
            {"--kml", "--classify", "DATA_STRING", "--out", "DATA_STRING", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "", "", "", "", "", ""},

            {"--kml", "--filter", "DATA_STRING", "--pin-icon", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--filter", "DATA_STRING", "--pin-icon", "DATA_STRING", "", "", "", ""},

            {"--kml", "--filter", "DATA_STRING", "--path-color", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--filter", "DATA_STRING", "--path-color", "DATA_STRING", "", "", "", ""},

            {"--kml", "--filter", "DATA_STRING", "--name", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--filter", "DATA_STRING", "--name", "DATA_STRING", "", "", "", ""},

            {"--kml", "--filter", "DATA_STRING", "--description", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--filter", "DATA_STRING", "--description", "DATA_STRING", "", "", "", ""},

            {"--kml", "--filter", "DATA_STRING", "--text", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--filter", "DATA_STRING", "--text", "DATA_STRING", "", "", "", ""},

            // >>>> STYLE
            {"--kml", "--style-pins", "DATA_STRING", "--icon", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--style-pins", "DATA_STRING", "--icon", "DATA_STRING", "", "", "", ""},
            {"--kml", "--style-pins", "DATA_STRING", "--scale", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--style-pins", "DATA_STRING", "--scale", "DATA_STRING", "", "", "", ""},

            {"--kml", "--style-paths", "DATA_STRING", "--color", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--color", "DATA_STRING", "", "", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--thickness", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--thickness", "DATA_STRING", "", "", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--opacity", "DATA_STRING", "--out", "DATA_STRING", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--opacity", "DATA_STRING", "", "", "", ""},

            // CSV //
            {"--csv", "--change-separator", "DATA_STRING", "--old-sign", "DATA_STRING", "--new-sign", "DATA_STRING", "--out", "DATA_STRING"},
            {"--csv", "--change-separator", "DATA_STRING", "--old-sign", "DATA_STRING", "--new-sign", "DATA_STRING", "", ""}
        };

        std::stringstream
            mainMenuText_stream,
            styleNamesMenuText_stream;
        
        std::string menuResponseStrings[4];

        std::string getErrorString(bool isUseDMSErrorMessage) {
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
};

#endif // __MENU_H__