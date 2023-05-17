#ifndef __KML_DATE_FOLDER_H__
#define __KML_DATE_FOLDER_H__

/*
    'packNumeral' called as 'folder-by-date'
    'spreadNumeral' called as 'date-by-folder'
    **(available only for 'dd/mm/yyyy' numeric format)
*/

class DateFolder {
    public:
        DateFolder() {}

        /*
            -pack placemarks into one folder of their date
            -the undated placemark will be inserted into 'NO DATE' folder inside working folder
        */
        bool packNumeral(xml::Node *kmlNode);

        /*
            -spread folder date string to undated placemark description
            -doesn't need to insert into new working folder
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

#endif // __KML_DATE_FOLDER_H__