#ifndef __MENU_H__
#define __MENU_H__

#define TOTAL_COMMANDS_COUNT 9
#define MAX_INPUT_STRINGS_COUNT 5

#define MAIN_MENU_FLAG 0
#define HELP_MENU_FLAG 1
#define PIN_ICON_NAMES_MENU_FLAG 2
#define VERSION_NUMBER_FLAG 3
#define CONVERT_TXT_KML_FLAG 4
#define CONVERT_KML_CSV_FLAG 5
#define KML_PRE_STYLE_FLAG 6
#define KML_CREATE_PATH_FLAG 7
#define KML_JOIN_PATHS_FLAG 8

class Menu {
    public:
        int select(std::vector<std::string> inputStrings) {
            int passCount = 0, retFlag = -1;
            
            // loop 'TOTAL_COMMANDS_COUNT' times
            for (int i = 0; i < sizeof(listStrings) / sizeof(*listStrings); i++) {

                for (int j = 0; j < MAX_INPUT_STRINGS_COUNT; j++) {
                    if (inputStrings[j] == listStrings[i][j] ||
                        (inputStrings[j] != "" && listStrings[i][j] == "FILE_NAME")
                    ) {
                        passCount++;
                    }
                }
                
                if (passCount == MAX_INPUT_STRINGS_COUNT) {
                    std::cout << responseStrings[i];
                    retFlag = i;
                    break;
                }
                else passCount = 0;
            }

            if (passCount != MAX_INPUT_STRINGS_COUNT) {
                std::cerr << errorString;
            }

            return retFlag;
        }

    private:
        std::string listStrings[TOTAL_COMMANDS_COUNT][MAX_INPUT_STRINGS_COUNT] = {

            // BASIC //

            {"--menu", "", "", "", ""},
            {"--help", "", "", "", ""},
            {"--pin-icon-names", "", "", "", ""},
            {"--version", "", "", "", ""},

            // CONVERTING //
            {"--convert", "--txt-in", "FILE_NAME", "--kml-out", "FILE_NAME"},
            // {"--convert", "--txt-in", "FILE_NAME", "--kml-out", "FILE_NAME", "--pin-icon", "KML_PIN_ICON_NAME"},
            {"--convert", "--kml-in", "FILE_NAME", "--csv-out", "FILE_NAME"},

            // KML EDITOR //

            {"--kml", "--classify", "FILE_NAME", "--out", "FILE_NAME"},
            {"--kml", "--create-path", "FILE_NAME", "--out", "FILE_NAME"},
            {"--kml", "--join-paths", "FILE_NAME", "--out", "FILE_NAME"}
        };

        std::string mainMenuText = std::string(
            std::string("********************\n") +
            std::string("*     KML-TOWN     *\n") +
            std::string("* by boston_sinaga *\n") +
            std::string("********************\n\n") +
            std::string("MAIN MENU:\n") +

            std::string("~\nHELP COMMANDS:\n") +
            std::string(">>> --help or --menu\n") +
            std::string("****** show available commands\n") +
            std::string(">>> --pin-icon-names\n") +
            std::string("****** show available input's pin icon strings\n") +
            std::string(">>> --version\n") +
            std::string("****** show version number\n") +

            std::string("\n~CONVERTING COMMANDS:\n") +
            std::string(">>> --convert --txt-in [FILE_NAME] --kml-out [FILE_NAME]\n") +
            std::string("****** read '.txt' and write location keywords as '.kml' placemarks\n") +
            std::string(">>> --convert --kml-in [FILE_NAME] --csv-out [FILE_NAME] : \n") +
            std::string("****** read '.kml' and write placemarks information as formated '.csv' sheet\n") +

            std::string("\n~KML EDITOR COMMANDS:\n") +
            std::string(">>> --kml --classify [FILE_NAME] --out [FILE_NAME]\n") +
            std::string("****** set '.kml' placemark into specific type with pre-defined style\n") +
            std::string(">>> --kml --create-path [FILE_NAME] --out [FILE_NAME]\n") +
            std::string("****** set '.kml' pins into united path\n") +
            std::string(">>> --kml --join-paths [FILE_NAME] --out [FILE_NAME]\n") +
            std::string("****** set '.kml' paths into united path\n")
        );

        std::string pinIconNamesMenuText = (
            std::string("********************\n") +
            std::string("*     KML-TOWN     *\n") +
            std::string("* by boston_sinaga *\n") +
            std::string("********************\n\n") +
            std::string("PIN ICON NAMES MENU:\n\n") +

            std::string("~belows are available strings input for [KML_PIN_ICON_NAME]:\n\n") +

            std::string("\n>>> PUSHPIN:\n") +
            std::string("ylw-pushpin\n") +
            std::string("blue-pushpin\n") +
            std::string("grn-pushpin\n") +
            std::string("ltblu-pushpin\n") +
            std::string("pink-pushpin\n") +
            std::string("purple-pushpin\n") +
            std::string("red-pushpin\n") +
            std::string("wht-pushpin\n") +

            std::string("\n>>> PADDLES:\n") +
            std::string("A           blu-blank\n") +
            std::string("B           blu-diamond\n") +
            std::string("C           blu-circle\n") +
            std::string("D           blu-square\n") +
            std::string("E           blu-stars\n") +
            std::string("F           grn-blank\n") +
            std::string("G           grn-diamond\n") +
            std::string("H           grn-circle\n") +
            std::string("I           grn-square\n") +
            std::string("J           grn-stars\n") +
            std::string("K           ltblu-blank\n") +
            std::string("L           ltblu-diamond\n") +
            std::string("M           ltblu-circle\n") +
            std::string("N           ltblu-square\n") +
            std::string("O           ltblu-stars\n") +
            std::string("P           pink-blank\n") +
            std::string("Q           pink-diamond\n") +
            std::string("R           pink-circle\n") +
            std::string("S           pink-square\n") +
            std::string("T           pink-stars\n") +
            std::string("U           ylw-blank\n") +
            std::string("V           ylw-diamond\n") +
            std::string("W           ylw-circle\n") +
            std::string("X           ylw-square\n") +
            std::string("Y           ylw-stars\n") +
            std::string("Z           wht-blank\n") +
            std::string("1           wht-diamond\n") +
            std::string("2           wht-circle\n") +
            std::string("3           wht-square\n") +
            std::string("4           wht-stars\n") +
            std::string("5           red-blank\n") +
            std::string("6           red-diamond\n") +
            std::string("7           red-circle\n") +
            std::string("8           red-square\n") +
            std::string("9           red-stars\n") +
            std::string("10          purple-blank\n") +
            std::string("            purple-diamond\n") +
            std::string("            purple-circle\n") +
            std::string("            purple-square\n") +
            std::string("            purple-stars\n") +

            std::string("\n>>> SHAPES:\n") +
            std::string("arrow-reverse           airports                golf\n") +
            std::string("arrow                   ferry                   trail\n") +
            std::string("track                   heliport                shopping\n") +
            std::string("donut                   subway                  movies\n") +
            std::string("forbidden               tram                    convenience\n") +
            std::string("info-i                  info                    grocery\n") +
            std::string("polygon                 info_circle             arts\n") +
            std::string("open-diamond            flag                    homegardenbusiness\n") +
            std::string("square                  rainy                   electronics\n") +
            std::string("star                    water                   mechanic\n") +
            std::string("target                  snowflake_simple        gas_stations\n") +
            std::string("triangle                marina                  realestate\n") +
            std::string("cross-hairs             fishing                 salon\n") +
            std::string("placemark_square        sailing                 dollar\n") +
            std::string("placemark_circle        swimming                euro\n") +
            std::string("shaded_dot              ski                     yen\n") +
            std::string("dining                  parks                   firedept\n") +
            std::string("coffee                  campfire                hospitals\n") +
            std::string("bars                    picnic                  lodging\n") +
            std::string("snack_bar               campground              phone\n") +
            std::string("man                     ranger_station          caution\n") +
            std::string("woman                   toilets                 earthquake\n") +
            std::string("wheel_chair_accessible  poi                     falling_rocks\n") +
            std::string("parking_lot             hiker                   post_office\n") +
            std::string("cabs                    cycling                 police\n") +
            std::string("bus                     motorcycling            sunny\n") +
            std::string("truck                   horsebackriding         partly_cloudy\n") +
            std::string("rail                    play                    volcano\n") +
            std::string("                                                camera\n") +
            std::string("                                                webcam\n")
        );

        std::string responseStrings[TOTAL_COMMANDS_COUNT] = {
            mainMenuText,
            mainMenuText,
            pinIconNamesMenuText,
            "KML-TOWN v.0.2.0 2022-11-14\n",
            "KML-TOWN-> converting TXT-KML...\n",
            "KML-TOWN-> converting KML-CSV...\n",
            "KML-TOWN-> running...\n",
            "KML-TOWN-> running...\n",
            "KML-TOWN-> running...\n"
        };

        std::string errorString = "KML-TOWN-> Command error. Type '--help' for available commands\n";
};

#endif // __MENU_H__