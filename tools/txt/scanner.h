#ifndef __TXT_SCANNER_H__
#define __TXT_SCANNER_H__

class Scanner {
    public:

        //////////////////////////////////////
        // BASED ON WHATSAPP'S EXPORTED TXT //
        //////////////////////////////////////

        /* note:
        *  don't forget to take care returned node
        */
        xml::Node *parse(
            std::string fileDir_in,
            std::string fileDir_out = ""   // empty for no writing
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
                return nullptr;
            }

            Sample sample;
            kml::Converter kmlConverter;

            /////////////////////////////////////////////////////////
            // SEARCH LOCATIONS (PLACEMARKS COORDINATES) AND DATES //
            /////////////////////////////////////////////////////////

            /* note: no 'altitude' when converting coor */
            std::vector<std::string> dateStrVector, coorStrVector;

            for (int i = 0; i < textVector.size(); i++) {
                std::string rawCoor;

                // DATE //
                // can be an empty string if no date detected
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
                        separatedCoorStrVector.at(1) + ",0"
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
                        rawCoorCouple.at(1) + ",0"
                    );

                    continue;
                }

                // when no coordinate detected in the line
                dateStrVector.pop_back();
            }

            if (coorStrVector.size() == 0) {
                std::cerr
                    << "TXT-> Scan error. No coordinate found in '"
                    << mini_tool::cutFileDirName(fileDir_in)
                    << "' \n";
                    
                return nullptr;
            }
            else {
                std::cout
                    << "TXT-> Scan for '"
                    << mini_tool::cutFileDirName(fileDir_in)
                    << "' completed!\n";
            }

            // xml creation //

            kml::Builder builderKML;
            builderKML.setup();
            std::string docName = mini_tool::cutFileDirName(fileDir_out);

            xml::Node *kmlNode = builderKML.createAsPlacemarks(
                dateStrVector, coorStrVector, docName
            );

            // writable
            if (fileDir_out != "" && kmlNode) {
                xml::Writer writer;
                writer.stringify(fileDir_out, kmlNode);
            }
            
            return kmlNode;
        }
};

#endif // __TXT_SCANNER_H__