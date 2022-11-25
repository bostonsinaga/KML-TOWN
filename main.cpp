#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>

#include "mini-tool.h"
#include "menu.h"
#include "tools/xml/xml.h"
#include "tools/kml/kml.h"
#include "tools/txt/txt.h"

int main(int argc, char *argv[]) {

    // set input fit 'MAX_INPUT_STRINGS_COUNT' array count //

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

    // test
    xml::Reader reader;
    xml::Node *docNode = reader.fileParse(inputStrings.at(0));

    if (docNode) {
        xml::Writer writer;
        writer.stringify(inputStrings.at(1), docNode);
        delete docNode;
    }

    Menu menu;

    switch (menu.select(inputStrings)) {
        case CONVERT_TXT_KML_FLAG: {
            // txt::Scanner txtScanner;

            // xml::Node *kmlNode = txtScanner.parse(
            //     inputStrings.at(2),
            //     inputStrings.at(4)
            // );

            // if (kmlNode) delete kmlNode;
        break;}
        case CONVERT_KML_CSV_FLAG: {

        break;}
        case KML_PRE_STYLE_FLAG: {

        break;}
        case KML_CREATE_PATH_FLAG: {

        break;}
        case KML_JOIN_PATHS_FLAG: {

        break;}
    }
    
    return 0;
}