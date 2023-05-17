#ifndef __XML_READER_H__
#define __XML_READER_H__

class Reader {
    public:
        static Node *fileParse(std::string fileName, bool isNotify = true);

        // input must only has one root in text form
        static Node *parse(
            std::string text,
            std::string fileName = ".xml",
            bool isNotify = false
        );
};

#endif // __XML_READER_H__