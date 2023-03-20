#ifndef __MENU_H__
#define __MENU_H__

#define TOTAL_COMMANDS_COUNT 59
#define MAX_INPUT_STRINGS_COUNT 10

#define MAIN_MENU_FLAG 0
#define HELP_MENU_FLAG 1
#define STYLE_NAMES_MENU_FLAG 2
#define VERSION_NUMBER_FLAG 3
#define CONVERT_TXT_KML_FLAG 4
#define CONVERT_KML_CSV_FLAG 5
#define KML_CROP_NEWFILE_FLAG 6
#define KML_CROP_OVERWRITE_FLAG 7
#define KML_SORT_NEWFILE_FLAG 8
#define KML_SORT_OVERWRITE_FLAG 9
#define KML_PINS_PATH_CROP_NEWFILE_FLAG 10
#define KML_PINS_PATH_CROP_OVERWRITE_FLAG 11
#define KML_PATHS_PINS_CROP_NEWFILE_FLAG 12
#define KML_PATHS_PINS_CROP_OVERWRITE_FLAG 13
#define KML_PATHS_PINS_NEWFILE_FLAG 14
#define KML_PATHS_PINS_OVERWRITE_FLAG 15
#define KML_JOIN_PATHS_CROP_NEWFILE_FLAG 16
#define KML_JOIN_PATHS_CROP_OVERWRITE_FLAG 17
#define KML_JOIN_PATHS_NEWFILE_FLAG 18
#define KML_JOIN_PATHS_OVERWRITE_FLAG 19
#define KML_SPLIT_PATHS_CROP_NEWFILE_FLAG 20
#define KML_SPLIT_PATHS_CROP_OVERWRITE_FLAG 21
#define KML_SPLIT_PATHS_NEWFILE_FLAG 22
#define KML_SPLIT_PATHS_OVERWRITE_FLAG 23
#define KML_TWINS_CHECK_NEWFILE_FLAG 24
#define KML_TWINS_CHECK_OVERWRITE_FLAG 25
#define KML_TWINS_CHECK_INCLUDE_FOLDER_NEWFILE_FLAG 26
#define KML_TWINS_CHECK_INCLUDE_FOLDER_OVERWRITE_FLAG 27
#define KML_MEASURE_PATHS_NEWFILE_FLAG 28
#define KML_MEASURE_PATHS_OVERWRITE_FLAG 29
#define KML_MEASURE_PATHS_INFO_FLAG 30
#define KML_REMOVE_PATH_UNDER_DISTANCE_NEWFILE_FLAG 31
#define KML_REMOVE_PATH_UNDER_DISTANCE_OVERWRITE_FLAG 32
#define KML_REMOVE_PATH_OVER_DISTANCE_NEWFILE_FLAG 33
#define KML_REMOVE_PATH_OVER_DISTANCE_OVERWRITE_FLAG 34
#define KML_CLASSIFY_NEWFILE_FLAG 35
#define KML_CLASSIFY_OVERWRITE_FLAG 36
#define KML_CLASSIFY_CLEAN_FOLDER_NEWFILE_FLAG 37
#define KML_CLASSIFY_CLEAN_FOLDER_OVERWRITE_FLAG 38
#define KML_CLASSIFY_INCLUDE_FOLDER_NEWFILE_FLAG 39
#define KML_CLASSIFY_INCLUDE_FOLDER_OVERWRITE_FLAG 40
#define KML_CLASSIFY_INCLUDE_CLEAN_FOLDER_NEWFILE_FLAG 41
#define KML_CLASSIFY_INCLUDE_CLEAN_FOLDER_OVERWRITE_FLAG 42
#define KML_FOLDERBYDATE_NEWFILE_FLAG 43
#define KML_FOLDERBYDATE_OVERWRITE_FLAG 44
#define KML_FILTER_TEXT_NEWFILE_FLAG 45
#define KML_FILTER_TEXT_OVERWRITE_FLAG 46
#define KML_STYLEPINS_ICON_NEWFILE_FLAG 47
#define KML_STYLEPINS_ICON_OVERWRITE_FLAG 48
#define KML_STYLEPINS_SCALE_NEWFILE_FLAG 49
#define KML_STYLEPINS_SCALE_OVERWRITE_FLAG 50
#define KML_STYLEPATH_COLOR_NEWFILE_FLAG 51
#define KML_STYLEPATH_COLOR_OVERWRITE_FLAG 52
#define KML_STYLEPATH_THICKNESS_NEWFILE_FLAG 53
#define KML_STYLEPATH_THICKNESS_OVERWRITE_FLAG 54
#define KML_STYLEPATH_OPACITY_NEWFILE_FLAG 55
#define KML_STYLEPATH_OPACITY_OVERWRITE_FLAG 56
#define CSV_CHANGE_SEPARATOR_NEWFILE_FLAG 57
#define CSV_CHANGE_SEPARATOR_OVERWRITE_FLAG 58

#define COMMAND_WORKING_FOLDER "KML-TOWN  --"
#define CROP_COMMAND_WORKING_FOLDER "KML-TOWN  --CROP"
#define SORT_COMMAND_WORKING_FOLDER "KML-TOWN  --SORT"
#define PINS_PATH_COMMAND_WORKING_FOLDER "KML-TOWN  --PINS-PATH"
#define PINS_PATH_SEGMENTS_COMMAND_WORKING_FOLDER "KML-TOWN  --PINS-PATH-SEGMENTS"
#define PATHS_PINS_COMMAND_WORKING_FOLDER "KML-TOWN  --PATHS-PINS"
#define JOIN_PATHS_COMMAND_WORKING_FOLDER "KML-TOWN  --JOIN-PATHS"
#define SPLIT_PATHS_COMMAND_WORKING_FOLDER "KML-TOWN  --SPLIT-PATHS"
#define TWINS_CHECK_COMMAND_WORKING_FOLDER "KML-TOWN  --TWINS-CHECK"
#define CLASSIFY_COMMAND_WORKING_FOLDER "KML-TOWN  --CLASSIFY"
#define FOLDERBYDATE_COMMAND_WORKING_FOLDER "KML-TOWN --FOLDER-BY-DATE"
#define FOLDERBYTEXT_COMMAND_WORKING_FOLDER "KML-TOWN  --FOLDER-BY-TEXT"

class Menu {
    public:
        Menu();
        int select(std::vector<std::string> inputStrings);
        void setNotification(std::string message, bool isNeedEnterKey = true);
        bool setAlert(std::string message, bool isUseFailedMessage = true);
        std::string setAdditionalInput(std::string message); // input is first CLI strings

    private:
        std::string getErrorString(bool isUseDMSErrorMessage);

        // VARIABLES //

        std::string listStrings[TOTAL_COMMANDS_COUNT][MAX_INPUT_STRINGS_COUNT] = {

            // BASIC //
            {"--menu", "", "", "", "", "", "", "", "", ""},
            {"--help", "", "", "", "", "", "", "", "", ""},
            {"--style-names", "", "", "", "", "", "", "", "", ""},
            {"--version", "", "", "", "", "", "", "", "", ""},

            // CONVERTING //
            {"--convert", "--txt-in", "DATA_STRING", "--kml-out", "DATA_STRING", "--type", "DATA_STRING", "", "", ""},
            {"--convert", "--kml-in", "DATA_STRING", "--csv-out", "DATA_STRING", "", "", "", "", ""},

            // KML EDITOR (no '--out' means output will overwrite file input) //

            // >>>> CROP
            {"--kml", "--crop", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--kml", "--crop", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", "", ""},

            // >>>> SORT
            {"--kml", "--sort", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--kml", "--sort", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", "", ""},

            // >>>> PINS AND PATH
            {"--kml", "--pins-path", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--kml", "--pins-path", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", "", ""},

            {"--kml", "--paths-pins", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--kml", "--paths-pins", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", "", ""},
            {"--kml", "--paths-pins", "DATA_STRING", "--out", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--paths-pins", "DATA_STRING", "", "", "", "", "", "", ""},

            {"--kml", "--join-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--kml", "--join-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", "", ""},
            {"--kml", "--join-paths", "DATA_STRING", "--out", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--join-paths", "DATA_STRING", "", "", "", "", "", "", ""},

            {"--kml", "--split-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--kml", "--split-paths", "DATA_STRING", "--start-point", "DATA_STRING", "--end-point", "DATA_STRING", "", "", ""},
            {"--kml", "--split-paths", "DATA_STRING", "--out", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--split-paths", "DATA_STRING", "", "", "", "", "", "", ""},

            {"--kml", "--twins-check", "DATA_STRING", "--type", "DATA_STRING", "--radius", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--kml", "--twins-check", "DATA_STRING", "--type", "DATA_STRING", "--radius", "DATA_STRING", "", "", ""},
            {"--kml", "--twins-check", "DATA_STRING", "--type", "DATA_STRING", "--radius", "DATA_STRING", "--out", "DATA_STRING", "--include-folder"},
            {"--kml", "--twins-check", "DATA_STRING", "--type", "DATA_STRING", "--radius", "DATA_STRING", "--include-folder", "", ""},

            {"--kml", "--measure-paths", "DATA_STRING", "--out", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--measure-paths", "DATA_STRING", "", "", "", "", "", "", ""},
            {"--kml", "--measure-paths", "DATA_STRING", "--info", "", "", "", "", "", ""},

            {"--kml", "--remove-paths", "DATA_STRING", "--under-distance", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--remove-paths", "DATA_STRING", "--under-distance", "DATA_STRING", "", "", "", "", ""},

            {"--kml", "--remove-paths", "DATA_STRING", "--over-distance", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--remove-paths", "DATA_STRING", "--over-distance", "DATA_STRING", "", "", "", "", ""},

            // >>>> FILTER
            {"--kml", "--classify", "DATA_STRING", "--out", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "", "", "", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "--out", "DATA_STRING", "--clean-folder", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "--clean-folder", "", "", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "--out", "DATA_STRING", "--include-folder", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "--include-folder", "", "", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "--out", "DATA_STRING", "--include-clean-folder", "", "", "", ""},
            {"--kml", "--classify", "DATA_STRING", "--include-clean-folder", "", "", "", "", "", ""},

            {"--kml", "--folder-by-date", "DATA_STRING", "--out", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--folder-by-date", "DATA_STRING", "", "", "", "", "", "", ""},

            {"--kml", "--folder-by-text", "DATA_STRING", "--string", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--folder-by-text", "DATA_STRING", "--string", "DATA_STRING", "", "", "", "", ""},

            // >>>> STYLE
            {"--kml", "--style-pins", "DATA_STRING", "--icon", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--style-pins", "DATA_STRING", "--icon", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--style-pins", "DATA_STRING", "--scale", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--style-pins", "DATA_STRING", "--scale", "DATA_STRING", "", "", "", "", ""},

            {"--kml", "--style-paths", "DATA_STRING", "--color", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--color", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--thickness", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--thickness", "DATA_STRING", "", "", "", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--opacity", "DATA_STRING", "--out", "DATA_STRING", "", "", ""},
            {"--kml", "--style-paths", "DATA_STRING", "--opacity", "DATA_STRING", "", "", "", "", ""},

            // CSV //
            {"--csv", "--change-separator", "DATA_STRING", "--old-sign", "DATA_STRING", "--new-sign", "DATA_STRING", "--out", "DATA_STRING", ""},
            {"--csv", "--change-separator", "DATA_STRING", "--old-sign", "DATA_STRING", "--new-sign", "DATA_STRING", "", "", ""}
        };

        std::stringstream
            mainMenuText_stream,
            styleNamesMenuText_stream;
        
        std::string menuResponseStrings[4];
};

#endif // __MENU_H__