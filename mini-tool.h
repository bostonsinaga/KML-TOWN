#ifndef __BASE_TOOL_H__
#define __BASE_TOOL_H__

namespace mini_tool {

    std::string cutFileDirName(std::string &fileDir) {
        std::string cutName = fileDir;

        for (int i = 0; i < fileDir.size(); i++) {
            size_t foundDex = cutName.find("/");
            
            if (foundDex != std::string::npos) {
                cutName = cutName.substr(foundDex + 1);
            }
            else break;
        }

        return cutName;
    }
}

#endif // __BASE_TOOL_H__