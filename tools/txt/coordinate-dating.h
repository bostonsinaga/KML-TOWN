#ifndef __TXT_COORDINATE_DATING_H__
#define __TXT_COORDINATE_DATING_H__

#include "scanner.h"

namespace txt {

class CoordinateDating {
public:

    //////////////////////////////////////
    // BASED ON WHATSAPP'S EXPORTED TXT //
    //////////////////////////////////////

    /**
     * Coordinates that do not have a date
     * will use the date from the row above.
     */

    CoordinateDating() {}

    void complete(
        std::string fileDir_in,
        std::string fileDir_out = ""
    );
};
}

#endif // __TXT_COORDINATE_DATING_H__