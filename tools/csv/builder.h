#ifndef __CSV_BUILDER_H__
#define __CSV_BUILDER_H__

#include "util.h"

namespace csv {

class Builder {
    public:
        /* NOTE:
        *  the columns are the first list of folders from main folder
        *  the folders inside each of them are ignored
        *  (directly to the placemarks)
        */
        bool compose(
            std::string fileDir_out,
            xml::Node *mainFolderNode,
            std::string separatorSign = "|"
        );

    private:
        std::vector<xml::Node*> pullRowNodes(xml::Node *parent);
};
}

#endif // __TXT_BUILDER_H__