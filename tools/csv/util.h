#ifndef __CSV_UTIL_H__
#define __CSV_UTIL_H__

#include "../kml/kml.h"

namespace csv {

  class Util {
  public:
    static void changeCSVSeparator(
      std::string fileDir_in,
      std::string fileDir_out,
      std::string oldSign,
      std::string newSign
    );

    static void singleCharacterInputNotify(
      Menu &menu,
      bool isNeedEnterKey
    );
  };
}

#endif // __CSV_UTIL_H__