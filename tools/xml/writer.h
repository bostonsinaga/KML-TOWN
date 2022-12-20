#ifndef __XML_WRITER_H__
#define __XML_WRITER_H__

class Writer {
    public:
        void stringify(
            std::string fileDir_out,
            Node *root,
            bool isWithRoot = true
        );

    private:

        std::string stringifyAttributes(std::vector<Attribute> *attributes);
        std::string pullChildrenStrings(std::vector<Node*> *children);
};

#endif // __XML_WRITER_H__