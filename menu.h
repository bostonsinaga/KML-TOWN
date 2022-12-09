#ifndef __MENU_H__
#define __MENU_H__

#define TOTAL_COMMANDS_COUNT 11
#define MAX_INPUT_STRINGS_COUNT 9

#define MAIN_MENU_FLAG 0
#define HELP_MENU_FLAG 1
#define PIN_ICON_NAMES_MENU_FLAG 2
#define VERSION_NUMBER_FLAG 3
#define CONVERT_TXT_KML_FLAG 4
#define CONVERT_KML_CSV_FLAG 5
#define KML_CROP_FLAG 6
#define KML_SORT_FLAG 7
#define KML_PRE_STYLE_FLAG 8
#define KML_CREATE_PATH_FLAG 9
#define KML_JOIN_PATHS_FLAG 10

#define CROP_COMMAND_WORKING_FOLDER "KML-TOWN --CROP"
#define SORT_COMMAND_WORKING_FOLDER "KML-TOWN --SORT"

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
                << ">>> --pin-icon-names\n"
                << "****** show available input's pin icon strings\n\n"
                << ">>> --version\n"
                << "****** show version number\n"

                << "\n~CONVERTING COMMANDS:\n\n"
                << ">>> --convert --txt-in [FILE_NAME] --kml-out [FILE_NAME]\n"
                << "****** read '.txt' and write location keywords as '.kml' placemarks\n\n"
                << ">>> --convert --kml-in [FILE_NAME] --csv-out [FILE_NAME] : \n"
                << "****** read '.kml' and write placemarks information as formated '.csv' sheet\n"

                << "\n~KML EDITOR COMMANDS:\n\n"
                << "--kml --crop [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
                << "****** crop '.kml' by rectangular area generated from start to end point\n"
                << "       and insert them into new folder named '" << CROP_COMMAND_WORKING_FOLDER << "'\n\n"

                << "--kml --sort [FILE_NAME] --start-point [COORDINATE] --end-point [COORDINATE] --out [FILE_NAME]\n"
                << "****** sort '.kml' start with closest placemark from start point\n"
                << "       then chaining to nearest one but only inside start to end point formed rectangular area\n"
                << "       and finally insert them into new folder named '" << SORT_COMMAND_WORKING_FOLDER << "'\n\n"

                << ">>> --kml --classify [FILE_NAME] --out [FILE_NAME]\n"
                << "****** group '.kml' placemarks with their similar style into some folders\n\n"

                << ">>> --kml --create-path [FILE_NAME] --out [FILE_NAME]\n"
                << "****** set '.kml' pins into united path\n\n"

                << ">>> --kml --join-paths [FILE_NAME] --out [FILE_NAME]\n"
                << "****** set '.kml' paths into united path\n";

            pinIconNamesMenuText_stream
                << "********************\n"
                << "*     KML-TOWN     *\n"
                << "* by boston_sinaga *\n"
                << "********************\n\n"
                << "PIN ICON NAMES MENU:\n\n"

                << "~belows are available strings input for [KML_PIN_ICON_NAME]:\n"

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
                << "                                                webcam\n";

            // menu respons list
            menuResponseStrings[0] = mainMenuText_stream.str();
            menuResponseStrings[1] = mainMenuText_stream.str();
            menuResponseStrings[2] = pinIconNamesMenuText_stream.str();
            menuResponseStrings[3] = "KML-TOWN v.0.2.0 2022-11-25\n";
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

        void setNotification(std::string message) {
            std::cout << message << "\n**PRESS ENTER TO CONTINUE**\n";
            std::cin.get();
        }

        bool setAlert(std::string message) {
            std::cout << message << "\n**PRESS Y[yes] / N[no]**\n";

            std::string answer;
            std::cin >> answer;
            std::cout << std::endl;

            if (answer == "Y" || answer == "y") {
                return true;
            }
            else {
                std::cerr << "KML-TOWN-> Command canceled!\n\n**FAILED**\n";
                return false;
            }
        }

    private:
        std::string listStrings[TOTAL_COMMANDS_COUNT][MAX_INPUT_STRINGS_COUNT] = {

            // BASIC //
            {"--menu", "", "", "", ""},
            {"--help", "", "", "", ""},
            {"--pin-icon-names", "", "", "", ""},
            {"--version", "", "", "", ""},

            // CONVERTING //
            {"--convert", "--txt-in", "DATA_STRING", "--kml-out", "DATA_STRING"},
            {"--convert", "--kml-in", "DATA_STRING", "--csv-out", "DATA_STRING"},

            // KML EDITOR //

            {"--kml", "--crop", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--sort", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING"},

            {"--kml", "--classify", "DATA_STRING", "--out", "DATA_STRING"},
            
            {"--kml", "--create-path", "DATA_STRING", "--out", "DATA_STRING"},
            {"--kml", "--join-paths", "DATA_STRING", "--out", "DATA_STRING"}
        };

        std::stringstream
            mainMenuText_stream,
            pinIconNamesMenuText_stream;
        
        std::string menuResponseStrings[4];

        std::string getErrorString(bool isUseDMSErrorMessage) {
            std::stringstream retStr_stream;

            if (isUseDMSErrorMessage) {
                retStr_stream
                    << "KML-TOWN-> You may have to remove double quote sign (\") of degree (DMS) coordinate,\n"
                    << "           to prevent unexpected or overload command parameters!\n\n";
            }

            retStr_stream
                << "KML-TOWN-> Command error. Type '--help' for available commands\n"
                << "\n**FAILED**\n";

            return retStr_stream.str();
        }
};

#endif // __MENU_H__