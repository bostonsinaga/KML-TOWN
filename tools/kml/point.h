#ifndef __KML_POINT_H__
#define __KML_POINT_H__

class Point {
    public:
        Point(double x_in, double y_in);

        // from string coordinate
        Point(std::string coorString);

        // the node must has coordinate string (for single coordinate)
        Point(xml::Node *coorNode);

        Point operator+(Point addPt);
        Point operator-(Point subtPt);
        Point operator*(Point multPt);
        Point operator/(Point divPt);
        std::string stringify();

        double x = 0, y = 0;
};

#endif // __KML_POINT_H__