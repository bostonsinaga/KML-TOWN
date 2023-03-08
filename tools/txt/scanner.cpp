#ifndef __TXT_SCANNER_CPP__
#define __TXT_SCANNER_CPP__

#include "scanner.h"

/* Note:
*   'yieldParseFunc.at(dateStrVec_flag)' and
*   'yieldParseFunc.at(coorStrVec_flag)' size are certainly equal
*/

////////////////
// PINS PARSE //
////////////////

xml::Node *Scanner::parsePins(
    std::string fileDir_in,
    std::string fileDir_out
) {
    std::vector<std::vector<std::string>> yieldParseFunc = parse(
        fileDir_in, fileDir_out, "pins"
    );

    if (yieldParseFunc.size() > 0) {
        std::string styleMapId;
        xml::Node *mainFolderNode = getMainFolder(fileDir_out, &styleMapId, true);

        kml::Builder kmlBuilder;

        int dateVecCtr = 0;
        for (auto &coorStr : yieldParseFunc.at(coorStrVec_flag)) {
            mainFolderNode->addChild(
                kmlBuilder.getPin(
                    kmlBuilder.COORSTR_ZERO_ADD_ALTITUDE,
                    styleMapId,
                    coorStr,
                    "",
                    yieldParseFunc.at(dateStrVec_flag).at(dateVecCtr)
                )
            );
            dateVecCtr++;
        }
        
        xml::Node *kmlNode = mainFolderNode->getRoot();
        writeOut(fileDir_out, kmlNode);
        return kmlNode;
    }

    return nullptr;
}

/////////////////
// PATHS PARSE //
/////////////////

xml::Node *Scanner::parsePaths(
    std::string fileDir_in,
    std::string fileDir_out
) {
    std::cout
        << "TXT-> Command warning. Scan into 'paths' is not available yet.\n"
        << "      Scanning switched for 'pins'!\n";
    return parsePins(fileDir_in, fileDir_out);
}

////////////////////
// PARSE FUNCTION //
////////////////////

std::vector<std::vector<std::string>> Scanner::parse(
    std::string &fileDir_in,
    std::string &fileDir_out,
    std::string messageTypeName
) {
    std::ifstream readFile(fileDir_in);
    std::vector<std::string> textVector;
    std::string stringBuffer;

    while (std::getline(readFile, stringBuffer)) {
        textVector.push_back(stringBuffer);
    }

    if (textVector.size() == 0) {
        std::cerr
            << "TXT-> Scanning for '"<< messageTypeName <<"' error. File named '"
            << mini_tool::cutFileDirName(fileDir_in) <<"' not found or empty\n";
        
        // failed
        return std::vector<std::vector<std::string>>{};
    }

    Samples txtSamples;
    kml::Converter kmlConverter;

    /////////////////////////////////////////////////////////
    // SEARCH LOCATIONS (PLACEMARKS COORDINATES) AND DATES //
    /////////////////////////////////////////////////////////

    /* note:
    *   -no 'altitude' when converting coor
    *   -these will be the return strings (2D vector)
    */
    std::vector<std::string>
        dateStrVector,
        coorStrVector; // will has a '0' altitude extension in the back

    for (int i = 0; i < textVector.size(); i++) {
        std::string rawCoor;

        //////////
        // DATE //
        //////////

        // will be an empty string if no date detected
        std::string dateStr = txtSamples.testDateTemplate(textVector.at(i));

        if (dateStr == "" && dateStrVector.size() > 0) { // follows previous date
            dateStrVector.push_back(dateStrVector.back());
        }
        else dateStrVector.push_back(dateStr);

        /////////////////////
        // GOOGLE MAP LINK //
        /////////////////////

        rawCoor = txtSamples.testGoogleMapsTemplate(textVector.at(i));

        if (rawCoor != "") {                    
            std::vector<std::string>
                separatedCoorStrVector = kmlConverter.separateCoordinate(
                    rawCoor,
                    kmlConverter.LAT_LNG_SEPARATE_FLAG_IN,
                    kmlConverter.LNG_LAT_SEPARATE_FLAG_OUT
                );

            coorStrVector.push_back(
                separatedCoorStrVector.at(0) + "," +
                separatedCoorStrVector.at(1)
            );

            continue;
        }

        /////////////////
        // DEGREE SIGN //
        /////////////////
        
        // with letter
        rawCoor = txtSamples.testDegreeTemplateWithLetter(textVector.at(i));

        // with sign
        if (rawCoor == "") {
            rawCoor = txtSamples.testDegreeTemplateWithSign(textVector.at(i));
        }

        if (rawCoor != "") {

            // seperate coordinates
            std::vector<std::string>
                rawCoorCouple = kmlConverter.separateCoordinate(
                    rawCoor,
                    kmlConverter.LAT_LNG_SEPARATE_FLAG_IN,
                    kmlConverter.LAT_LNG_SEPARATE_FLAG_OUT
                );

            // convert into decimal in string
            rawCoorCouple = kmlConverter.convertCoor_degreeDecimal(
                rawCoorCouple,
                kmlConverter.LAT_LNG_SEPARATE_FLAG_IN,
                kmlConverter.LNG_LAT_SEPARATE_FLAG_OUT
            );

            coorStrVector.push_back(
                rawCoorCouple.at(0) + "," +
                rawCoorCouple.at(1)
            );

            continue;
        }

        ////////////////////////
        // DECIMAL COORDINATE //
        ////////////////////////
        
        rawCoor = txtSamples.testDecimalTemplate(textVector.at(i));

        if (rawCoor != "") {

            // seperate coordinates
            std::vector<std::string>
                rawCoorCouple = kmlConverter.separateCoordinate(
                    rawCoor,
                    kmlConverter.LAT_LNG_SEPARATE_FLAG_IN,
                    kmlConverter.LNG_LAT_SEPARATE_FLAG_OUT
                );

            coorStrVector.push_back(
                rawCoorCouple.at(0) + "," +
                rawCoorCouple.at(1)
            );

            continue;
        }

        //////////
        // DATE //
        //////////

        // when no coordinate detected in the line
        dateStrVector.pop_back();
    }

    if (coorStrVector.size() == 0) {
        std::cerr
            << "TXT-> Scanning for '" << messageTypeName << "' error. No coordinate found in file named '"
            << mini_tool::cutFileDirName(fileDir_in) << "'\n";
            
        // failed
        return std::vector<std::vector<std::string>>{};
    }
    else {
        std::cout
            << "TXT-> Scan for '" << messageTypeName << "' from '"
            << mini_tool::cutFileDirName(fileDir_in)
            << "' completed!\n";
    }

    // succeeded
    return std::vector<std::vector<std::string>>{
        dateStrVector,
        coorStrVector
    };
}

/////////////////////
// GET MAIN FOLDER //
/////////////////////

xml::Node *Scanner::getMainFolder(
    std::string &fileDir_out,
    std::string *styleMapId_hook,
    bool isPins
) {
    // XML CREATION //

    std::string docName = mini_tool::cutFileDirName(fileDir_out);
    kml::Builder kmlBuilder;

    // BUILDER SETUP //
    
    xml::Node *kmlNode = kmlBuilder.getSkeleton(docName);

    if (isPins) {
        // determined as 'yellow_push_pin'
        kmlBuilder.insertStyleMap(
            kmlNode,
            kmlBuilder.getPinStyleMap(styleMapId_hook)
        );
    }
    else {
        kmlBuilder.insertStyleMap(
            kmlNode,
            kmlBuilder.getPathStyleMap(styleMapId_hook)
        );
    }

    // KML CREATION //

    kmlBuilder.setTitle(kmlNode, docName);
    return kml::General().searchMainFolder(kmlNode);
}

#endif // __TXT_SCANNER_CPP__