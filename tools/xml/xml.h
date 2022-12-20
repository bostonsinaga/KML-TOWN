#ifndef __XML_H__
#define __XML_H__

#define XML_TOPPER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
#define XML_TOPPER_COUNT 38

namespace xml {
    #include "node.cpp"
    #include "reader.cpp"
    #include "writer.cpp"
}

#endif // __XML_H__