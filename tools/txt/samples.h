#ifndef __TXT_SAMPLES_H__
#define __TXT_SAMPLES_H__

class Samples {
    public:
        std::string testDateTemplate(std::string &textLine) {

            int slashCtr = 0;
            std::string date = "", month = "", year = "", *timePtr = &date;

            /* the template is: (following WhatsApp '.txt' chat export format)
            *  dd / mm / 20yy
            */

            std::vector<char> fingerNums {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
            };

            // only number and slash sign
            for (auto &ch : textLine) {

                // still searching
                if (ch == '/') {
                    slashCtr++;
                    if (slashCtr == 1) timePtr = &month;
                    else if (slashCtr == 2) timePtr = &year;
                }
                else {
                    bool isLoopBreak = false;
                    for (auto &chNum : fingerNums) {
                        if (ch == chNum) {
                            timePtr->push_back(ch);
                            isLoopBreak = true;
                            break;
                        }
                    }

                    if (isLoopBreak) break;
                    else {
                        slashCtr = 0;
                        date = "";
                        month = "";
                        year = "";
                        timePtr = &date;
                    }
                }

                // stop searching
                if (year.size() == 2 && slashCtr == 2) {
                    return date + "/" + month + "/20" + year;
                }
            }

            return "";
        }

        std::string
            google_map_tag = "https://maps.google.com/?q=",
            degree_tag = "\xB0";

        std::vector<std::string> warningTags {
            "http",

            //////////////////
            // HSP SPECIALS //
            //////////////////

            "6m",
            "7m",
            "9m",
            "dc",
            "dropcore",
            "core",
            "tube",
            "6c",
            "12c",
            "24c",
            "48c",
            "96c"
        };
};

#endif // __TXT_SAMPLES_H__