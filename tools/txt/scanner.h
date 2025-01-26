#ifndef __TXT_SCANNER_H__
#define __TXT_SCANNER_H__

#include "samples.h"

namespace txt {

class Scanner {
    public:

        //////////////////////////////////////
        // BASED ON WHATSAPP'S EXPORTED TXT //
        //////////////////////////////////////

        /* note:
        *  -don't forget to take care returned node
        */

        Scanner() {}

        xml::Node *parsePins(
            std::string fileDir_in,
            std::string fileDir_out = ""
        );

        std::vector<std::string> readText(std::string &fileDir_in);

        /*
            HOW ABOUT PATHS?
            Parse paths isn't available here (too complicated)
            for the easier, it implemented at 'CONVERT TXT TO KML' section at 'main.cpp'
            (it using 'call-briefer')
        */

    private:
        enum {dateStrVec_flag, coorStrVec_flag};
        
        std::vector<std::vector<std::string>> parse(std::string &fileDir_in);

        xml::Node *getMainFolder(
            std::string &fileDir_out,
            std::string *styleMapId_hook,
            bool isPins
        );

        void writeOut(std::string fileDir_out, xml::Node *kmlNode) {
            if (fileDir_out != "" && kmlNode) {
                xml::Writer writer;
                writer.stringify(fileDir_out, kmlNode);
            }
        }
};
}

#endif // __TXT_SCANNER_H__