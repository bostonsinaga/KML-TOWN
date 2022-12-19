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
        fileDir_in, fileDir_out, "Pins"
    );

    if (yieldParseFunc.size() > 0) {
        std::string styleMapId;
        xml::Node *mainFolderNode = getMainFolder(fileDir_out, &styleMapId, true);

        kml::Builder builderKML;

        int dateVecCtr = 0;
        for (auto &coorStr : yieldParseFunc.at(coorStrVec_flag)) {
            mainFolderNode->addChild(
                builderKML.getPin(
                    builderKML.COORSTR_ZERO_ADD_ALTITUDE,
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
    std::vector<std::vector<std::string>> yieldParseFunc = parse(
        fileDir_in, fileDir_out, "Paths"
    );

    // firstly sort to prevent doodling paths
    Sorter().orderCoordinates(
        yieldParseFunc.at(dateStrVec_flag),
        yieldParseFunc.at(coorStrVec_flag)
    );

    if (yieldParseFunc.size() > 0) {
        std::string styleMapId;
        xml::Node *mainFolderNode = getMainFolder(fileDir_out, &styleMapId, false);

        kml::Builder builderKML;

        for (int i = 0; i < yieldParseFunc.at(coorStrVec_flag).size() - 1; i++) {
            
            std::vector<std::string> coorStrPair {
                yieldParseFunc.at(coorStrVec_flag).at(i),
                yieldParseFunc.at(coorStrVec_flag).at(i + 1)
            };

            mainFolderNode->addChild(
                builderKML.getPath(
                    builderKML.COORSTR_ZERO_ADD_ALTITUDE,
                    styleMapId,
                    coorStrPair,
                    "Path " + std::to_string(i + 1),
                    yieldParseFunc.at(dateStrVec_flag).at(i)
                )
            );
        }
        
        xml::Node *kmlNode = mainFolderNode->getRoot();
        writeOut(fileDir_out, kmlNode);
        return kmlNode;
    }

    return nullptr;
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
            << "TXT-> '"
            << mini_tool::cutFileDirName(fileDir_in)
            << "' error! file not found or empty\n";
        
        // failed
        return std::vector<std::vector<std::string>>{};
    }

    Sample sample;
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

        // DATE //
        // will be an empty string if no date detected
        dateStrVector.push_back(sample.testDateTemplate(textVector.at(i)));

        /////////////////////
        // GOOGLE MAP LINK //
        /////////////////////

        rawCoor = sample.testGoogleMapsTemplate(textVector.at(i));

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
        rawCoor = sample.testDegreeTemplateWithLetter(textVector.at(i));

        // with sign
        if (rawCoor == "") {
            rawCoor = sample.testDegreeTemplateWithSign(textVector.at(i));
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

        // when no coordinate detected in the line
        dateStrVector.pop_back();
    }

    if (coorStrVector.size() == 0) {
        std::cerr
            << "TXT-> " << messageTypeName << " scanning error. No coordinate found in '"
            << mini_tool::cutFileDirName(fileDir_in)
            << "' \n";
            
        // failed
        return std::vector<std::vector<std::string>>{};
    }
    else {
        std::cout
            << "TXT-> " << messageTypeName << " scan for '"
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
    kml::Builder builderKML;

    // BUILDER SETUP //
    
    xml::Node *kmlNode = builderKML.getSkeleton(docName);

    if (isPins) {
        // determined as 'yellow_push_pin'
        builderKML.insertStyleMap(
            kmlNode,
            builderKML.getPinStyleMap(styleMapId_hook)
        );
    }
    else {
        builderKML.insertStyleMap(
            kmlNode,
            builderKML.getPathStyleMap(styleMapId_hook)
        );
    }

    // KML CREATION //

    builderKML.setTitle(kmlNode, docName);
    return kml::searchMainFolder(kmlNode);
}