#ifndef __KML_POINT_H__
#define __KML_POINT_H__

#include "converter.h"

namespace kml {

/**
 * Point in degree (°).
 * But not limited for distance as meter.
 */
class Point {
    public:
        /* '0,0' point is in near Central Africa */
        Point() {};
        Point(LD x_in, LD y_in);

        // from string coordinate
        Point(std::string coorString);

        // the node must has coordinate string (for single coordinate)
        Point(xml::Node *coorNode);

        Point operator+(Point addPt);
        Point operator-(Point subtPt);
        Point operator*(Point multPt);
        Point operator/(Point divPt);

        Point operator+(LD val);
        Point operator-(LD val);
        Point operator*(LD val);
        Point operator/(LD val);

        bool isEqualTo(Point ptIn);

        static std::vector<Point> getPathPointsFromString(std::string coorString);

        static std::vector<std::string> stringifyVector(
            std::vector<Point> points,
            bool isAddZeroAltitude = true
        );

        std::string stringify(bool isAddZeroAltitude = true);

        static bool isBetween(
            Point &testPt,
            Point &startPt,
            Point &endPt
        );

        LD x = 0, y = 0;

    private:
        // keep value precision
        std::string to_string_with_precision(LD &axis);
};
}

#endif // __KML_POINT_H__