#ifndef __KML_DATE_FOLDER_H__
#define __KML_DATE_FOLDER_H__

#include "placemark.h"

/**
 * The 'packNumeral' called as 'folder-by-date'.
 * The 'spreadNumeral' called as 'date-by-folder'.
 * These available only for 'dd/mm/yyyy' numeric format.
 */

namespace kml {

class DateFolder {
    public:
        DateFolder() {}

        /**
         * Pack placemarks into one folder of their date
         * The undated placemark will be inserted into 'NO DATE' folder inside working folder
         */
        bool packNumeral(xml::Node *kmlNode);

        /**
         * Spread folder date string to undated placemark description.
         * Doesn't need to insert into new working folder.
         */
        bool spreadNumeral(xml::Node *kmlNode, bool isOverrideDated = false);

    private:
        // returns day, month, year in integers vector
        std::vector<int> testNumeralDate(std::string testStr);

        // return day counts of given decimal month
        int convertMonthInDaysCount(int month);

        // convert DMY vector to string
        std::string convertDMYVectorToString(std::vector<int> &dmyIntVec_in);
};
}

#endif // __KML_DATE_FOLDER_H__