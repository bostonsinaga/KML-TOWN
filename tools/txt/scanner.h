#ifndef __TXT_SCANNER_H__
#define __TXT_SCANNER_H__

class Scanner {
    public:

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
                    << "TXT-TOOL-> '"
                    << mini_tool::cutFileDirName(fileDir_in)
                    << "' error! file not found or empty\n";
                return nullptr;
            }

            Samples samples;
            kml::Converter degConv;

            /////////////////////////////////////////////////////////
            // SEARCH LOCATIONS (PLACEMARKS COORDINATES) AND DATES //
            /////////////////////////////////////////////////////////

            /* note: no 'altitude' when converting coor */
            std::vector<std::string> dateStrVector, coorStrVector;
            int datesCount = 0, coorsCount = 0;

            for (int i = 0; i < textVector.size(); i++) {
                size_t found;

                //////////
                // DATE //
                //////////

                dateStrVector.push_back(samples.testDateTemplate(textVector.at(i)));
                if (dateStrVector.back() != "") datesCount++;

                /////////////////////
                // GOOGLE MAP LINK //
                /////////////////////

                found = textVector.at(i).find(samples.google_map_tag);

                if (found != std::string::npos) {
                    std::string rawCoor = "";

                    // yield a [lat,lng]
                    for (int ct = found + samples.google_map_tag.size();
                        ct < textVector.at(i).size();
                        ct++
                    ) {
                        if (textVector.at(i).at(ct) != ' ' ||
                            textVector.at(i).at(ct) != '\n'
                        ) {
                            rawCoor += textVector.at(i).at(ct);
                        }
                    }

                    std::vector<std::string>
                        separatedCoorStrVector = degConv.separateCoor(
                            rawCoor,
                            degConv.LAT_LNG_SEPARATE_FLAG_IN,
                            degConv.LNG_LAT_SEPARATE_FLAG_OUT
                        );

                    coorStrVector.push_back(
                        separatedCoorStrVector.at(0) + "," +
                        separatedCoorStrVector.at(1) + ",0"
                    );

                    coorsCount++;
                    continue;
                }

                /////////////////
                // DEGREE SIGN //
                /////////////////
                
                found = textVector.at(i).find(samples.degree_tag);

                if (found != std::string::npos) {

                    std::string rawCoor = "";
                    int startCt;

                    for (int ct = found - 2; ct >= 0; ct--) {
                        if (textVector.at(i).at(ct) == ' ') {
                            startCt = ct + 1;
                        }
                    }

                    for (int ct = startCt;
                        ct < textVector.at(i).size();
                        ct++
                    ) {
                        if ((textVector.at(i).at(ct) != ' ' &&
                            (textVector.at(i).at(ct - 1) != 'E' ||
                            textVector.at(i).at(ct - 1) != 'e') && // east
                            (textVector.at(i).at(ct - 1) != 'T' ||
                            textVector.at(i).at(ct - 1) != 't') && // timur
                            (textVector.at(i).at(ct - 1) != 'W' ||
                            textVector.at(i).at(ct - 1) != 'w') && // west
                            (textVector.at(i).at(ct - 1) != 'B' ||
                            textVector.at(i).at(ct - 1) != 'b'))   // barat
                            ||
                            (textVector.at(i).at(ct) == ' ' && (
                                textVector.at(i).at(ct - 1) == 'S' ||
                                textVector.at(i).at(ct - 1) == 's'
                            ))
                        ) {
                            rawCoor += textVector.at(i).at(ct);
                        }
                    }

                    std::vector<std::string>
                        rawCoorVec = degConv.separateCoor(
                            rawCoor,
                            degConv.LAT_LNG_SEPARATE_FLAG_IN,
                            degConv.LAT_LNG_SEPARATE_FLAG_OUT
                        );

                    rawCoorVec = degConv.convertCoor_degreeDecimal(
                        rawCoorVec,
                        degConv.LAT_LNG_SEPARATE_FLAG_IN,
                        degConv.LNG_LAT_SEPARATE_FLAG_OUT
                    );

                    coorStrVector.push_back(
                        rawCoorVec.at(0) + "," +
                        rawCoorVec.at(1) + ",0"
                    );

                    coorsCount++;
                    continue;
                }

                ////////////////////////////
                // DEFINED CUSTOM WARNING //
                ////////////////////////////

                for (auto &warn : samples.warningTags) {
                    found = textVector.at(i).find(warn);
                    if (found != std::string::npos) {
                        std::cerr
                            << "TXT-TOOL-> '"
                            << mini_tool::cutFileDirName(fileDir_in)
                            << "' warning! found '"
                            << warn << "' at line " << i+1 << "\n";
                        break;
                    }
                }

                // date and coor must be equal
                if (datesCount != coorsCount) {
                    std::cerr << "TXT-TOOL-> '"
                    << mini_tool::cutFileDirName(fileDir_in)
                    << "' error. Date and coordinate are not in pairs at line "
                    << i+1 << "\n";
                    return nullptr;
                }
            }

            if (coorStrVector.size() == 0) {
                std::cerr
                    << "TXT-TOOL-> '"
                    << mini_tool::cutFileDirName(fileDir_in)
                    << "' error! no coordinate found\n";
                return nullptr;
            }

            // xml creation //

            kml::Create createKML;
            createKML.setup();
            std::string docName = mini_tool::cutFileDirName(fileDir_out);

            xml::Node *kmlNode = createKML.asPlacemarks(
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