#ifndef __CSV_H__
#define __CSV_H__

namespace csv {
    bool changeCSVSeparator(
        std::string fileDir_in,
        std::string preSep,
        std::string newSep
    ) {
        std::ifstream readFile(fileDir_in);
        std::string text = "", strBuff;

        while (std::getline(readFile, strBuff)) {
            text += strBuff;
        }

        if (text != "") {
            for (int i = 0; i < text.size(); i++) {
                size_t found = text.find(preSep);
                if (found != std::string::npos) {
                    text.erase(
                        text.begin() + found,
                        text.begin() + found + preSep.size()
                    );
                    text.insert(found, newSep);
                }
                else break;
            }
            return true;
        }
        return false;
    }

    #include "builder.h"
}

#endif // __CSV_H__