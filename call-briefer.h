#ifndef __CALL_BRIEFER_H__
#define __CALL_BRIEFER_H__

namespace call_briefer {

    std::string checkOverwrite(
        Menu &menu,
        std::string &fileDir_in,
        std::string &fileDir_out
    );

    std::vector<xml::Node*> cropPinsFunc(
        xml::Node *kmlNode,
        std::vector<std::string*> axisStrVec,
        bool isFolderInsertion // 'false' return nodes, 'true' return empty
    );

    std::vector<xml::Node*> sortPinsFunc(
        Menu &menu,
        xml::Node *kmlNode,
        std::vector<std::string*> axisStrVec,
        bool isFolderInsertion // 'false' return nodes, 'true' return empty
    );

    // write file (end of process)
    void writeFileFunc(
        xml::Node *kmlNode,
        std::string &fileDir_out
    );

    // return command working folder
    xml::Node *selectFunctionByType(
        std::string placemarksType,
        const std::function<xml::Node*()> &funcPins,
        const std::function<xml::Node*()> &funcPaths
    );
}

#endif // __CALL_BRIEFER_H__