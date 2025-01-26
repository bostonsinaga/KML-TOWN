#ifndef __TXT_COORDINATE_DATING_CPP__
#define __TXT_COORDINATE_DATING_CPP__

#include "coordinate-dating.h"

namespace txt {

void CoordinateDating::complete(
    std::string fileDir_in,
    std::string fileDir_out
) {
    Samples txtSamples;
    Scanner txtScanner;
    std::string latestDate = "";
    bool isAnyComplete = false;

    std::vector<std::string> textVector = txtScanner.readText(fileDir_in);

    for (auto &line : textVector) {
        std::string testDate = txtSamples.testDateTemplate(line, "");

        if (testDate != "") {
            latestDate = testDate;
            continue;
        }

        std::string currentCoor = txtSamples.testDecimalTemplate(line);

        if (currentCoor == "") {
            currentCoor = txtSamples.testDegreeTemplateWithLetter(line);

            if (currentCoor == "") {
                currentCoor = txtSamples.testDegreeTemplateWithSign(line);
            }
        }

        if (currentCoor != "") line = latestDate + " @ " + line;
        isAnyComplete = true;
    }

    if (fileDir_out != "" && isAnyComplete) {

        std::ofstream writeFile(fileDir_out);
        int length = textVector.size();

        for (int i = 0; i < length; i++) {
            writeFile << textVector.at(i) << (i != length - 1 ? "\n" : "");
        }

        std::cout << "TXT-> Output is saved to '" + fileDir_out + "'\n";
        std::cout << "\n**SUCCEEDED**\n";
    }
    else {
        std::cout << "TXT-> No change occurred to '" + fileDir_in + "'\n";
        std::cout << "\n**COMPLETED**\n";
    }
}
}

#endif // __TXT_COORDINATE_DATING_CPP__