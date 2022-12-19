#ifndef __TXT_SCANNER_H__
#define __TXT_SCANNER_H__

class Scanner {
    public:

        //////////////////////////////////////
        // BASED ON WHATSAPP'S EXPORTED TXT //
        //////////////////////////////////////

        /* note:
        *  -don't forget to take care returned node
        */

        xml::Node *parsePins(
            std::string fileDir_in,
            std::string fileDir_out = ""
        );

        xml::Node *parsePaths(
            std::string fileDir_in,
            std::string fileDir_out = ""
        );

    private:
        enum {dateStrVec_flag, coorStrVec_flag};

        std::vector<std::vector<std::string>> parse(
            std::string &fileDir_in,
            std::string &fileDir_out,
            std::string messageTypeName
        );

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

#endif // __TXT_SCANNER_H__