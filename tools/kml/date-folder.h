#ifndef __KML_DATE_FOLDER_H__
#define __KML_DATE_FOLDER_H__

/* called as 'folder-by-date' (will pack placemarks by their date in name or description) */

/* for now the pattern is 'dd/mm/yyyy' */

class DateFolder {
    public:
        DateFolder() {}
        void packNumeral(xml::Node *kmlNode);

    private:
        std::vector<int> testNumeralDate(std::string testStr);
        int convertMonthInDaysCount(int month);
};

#endif // __KML_DATE_FOLDER_H__