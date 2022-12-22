#ifndef __KML_H__
#define __KML_H__

/* NOTE:
*   there's only be one element as child of 'kml' element
*   otherwise, the Google Earth will reject it 
*/

namespace kml {
    #include "general.cpp"
    #include "style-strings.cpp"
    #include "converter.cpp"
    #include "point.cpp"
    #include "builder.cpp"
    #include "cropper.cpp"
    #include "sorter.cpp"
    #include "placemark.cpp"
    #include "twins-checker.cpp"
}

#endif // __KML_H__