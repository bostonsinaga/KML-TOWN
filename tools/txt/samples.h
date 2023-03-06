#ifndef __TXT_SAMPLES_H__
#define __TXT_SAMPLES_H__

class Samples {
    public:
        std::string testDateTemplate(std::string &textLine);
        std::string testGoogleMapsTemplate(std::string &textLine);

        // sample: [ 10°20'30.4"S 40°30'20.1"E ]
        std::string testDegreeTemplateWithLetter(std::string &textLine);

        // sample: [ -10°20'30.4,40°30'20.1 ]
        std::string testDegreeTemplateWithSign(std::string &textLine);

        // sample: [ -1.234,5.678 ]
        std::string testDecimalTemplate(std::string &textLine);
};

#endif // __TXT_SAMPLES_H__