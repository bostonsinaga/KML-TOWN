#ifndef __CSV_H__
#define __CSV_H__

namespace csv {
    void changeCSVSeparator(
        std::string fileDir_in,
        std::string fileDir_out,
        std::string preSign,
        std::string newSign
    ) {
        std::ifstream readFile(fileDir_in);
        std::string text = "", strBuff;

        while (std::getline(readFile, strBuff)) {
            text += strBuff + "\n";
        }

        if (text != "") {
            for (int i = 0; i < text.size(); i++) {
                size_t found = text.find(preSign);
                if (found != std::string::npos) {
                    text.erase(
                        text.begin() + found,
                        text.begin() + found + preSign.size()
                    );
                    text.insert(found, newSign);
                }
                else break;
            }

            std::ofstream writeFile(fileDir_out);
            writeFile << text;
            writeFile.close();

            std::cerr << "CSV-> Change columns separator sign completed!\n";
        }
        else {
            std::cerr << "CSV-> Error. '" << fileDir_in << "' is empty\n";
        }
    }

    void singleCharacterInputNotify(Menu &menu, bool isNeedEnterKey) {
        menu.setNotification(
            std::string("KML-TOWN-> Please input single character like '|', '&', etc. to be inside double quotes (\"x\")\n") +
            std::string("           Warnings:\n") +
            std::string("           -don't input '*' (asterisk) wildcard character\n") +
            std::string("            because this will attach all current directory file names\n") +
            std::string("           -avoid input '\"' (double quotes) sign, it may cause unintended multi lines row\n") +
            std::string("            that supposed to be new row (if contains new line sign)\n"),
            isNeedEnterKey
        );
    }

    #include "builder.cpp"
}

#endif // __CSV_H__