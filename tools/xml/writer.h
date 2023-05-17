#ifndef __XML_WRITER_H__
#define __XML_WRITER_H__

class Writer {
    public:
        static void stringify(
            std::string fileDir_out,
            Node *root,
            bool isWithRoot = true
        );

    private:

        static std::string stringifyAttributes(std::vector<Attribute> *attributes);
        static std::string pullChildrenStrings(std::vector<Node*> *children);
};

#endif // __XML_WRITER_H__