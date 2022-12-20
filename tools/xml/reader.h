#ifndef __XML_READER_H__
#define __XML_READER_H__

class Reader {
    public:
        Node *fileParse(std::string fileName);

        // input must only has one root in text form
        Node *parse(std::string text, std::string fileName = ".xml");
};

#endif // __XML_READER_H__