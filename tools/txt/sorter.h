#ifndef __TXT_SORTER_H__
#define __TXT_SORTER_H__

/*
*   SORT LIKE A CHAINS
*   started from bottom left to top right coordinates
*   (can be use for prevent doodling paths)
*/

class Sorter {
    public:
        Sorter() {}
        
        void orderCoordinates(
            std::vector<std::string> &dateStrVector,
            std::vector<std::string> &coorStrVector
        ) {
            std::vector<kml::Point> coorPointVec;

            for (auto &coorStr : coorStrVector) {
                coorPointVec.push_back(kml::Point(coorStr));
            }

            std::vector<double> hypotenuses;

            /*  haven't noticed 'kml::Point(0, 0)' */
            for (auto &coorPoint : coorPointVec) {
                hypotenuses.push_back(
                    std::sqrt(std::pow(coorPoint.x, 2) + std::pow(coorPoint.y, 2))
                );
            }

            // changable numbers
            std::vector<int> sortedIndexes;
            for (int i = 0; i < hypotenuses.size(); i++) {
                sortedIndexes.push_back(i);
            }

            // sorting lowest to highest
            for (int i = 0; i < hypotenuses.size(); i++) {
                for (int j = 0; j < hypotenuses.size(); j++) {
                    if (hypotenuses.at(i) < hypotenuses.at(j)) {

                        double bufferHypo = hypotenuses.at(i);
                        hypotenuses.at(i) = hypotenuses.at(j);
                        hypotenuses.at(j) = bufferHypo;

                        int bufferIndex = sortedIndexes.at(i);
                        sortedIndexes.at(i) = sortedIndexes.at(j);
                        sortedIndexes.at(j) = bufferIndex;
                    }
                }
            }

            // adjust to new sorted indexes
            for (int i = 0; i < coorStrVector.size(); i++) {

                std::string coorBuffer = coorStrVector.at(i);
                coorStrVector.at(i) = coorStrVector.at(sortedIndexes.at(i));
                coorStrVector.at(sortedIndexes.at(i)) = coorBuffer;

                std::string dateBuffer = dateStrVector.at(i);
                dateStrVector.at(i) = dateStrVector.at(sortedIndexes.at(i));
                dateStrVector.at(sortedIndexes.at(i)) = dateBuffer;
            }
        }
};

#endif // __TXT_SORTER_H__