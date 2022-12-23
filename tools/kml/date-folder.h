#ifndef __KML_DATE_FOLDER_H__
#define __KML_DATE_FOLDER_H__

/* called as 'folder-by-date' (will pack placemarks by their date in name or description)*/

class DateFolder {
    public:
        DateFolder() {}
        xml::Node *pack(xml::Node *kmlNode);

    private:
        std::string testDateTemplate(std::string strIn);

        // this feature coming soon!!
        std::vector<std::string>
            englishMonthNames {
                "january", "february", "march", "april",
                "may", "june", "july", "august",
                "september", "october", "november", "december"
            },
            indonesianMonthNames {
                "januari", "februari", "maret", "april",
                "mei", "juni", "juli", "agustus",
                "september", "oktober", "november", "desember"
            };
};

#endif // __KML_DATE_FOLDER_H__