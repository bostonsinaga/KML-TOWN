#ifndef __KML_PLACEMARK_H__
#define __KML_PLACEMARK_H__

class Placemark {
    public:
        void pinsPath(
            xml::Node *kmlNode,
            std::vector<xml::Node*> &sortedPinNodes
        ) {
            Builder builderKML;

            std::string styleMapId;
            builderKML.insertStyleMap(
                kmlNode,
                builderKML.getPathStyleMap(&styleMapId)
            );

            std::vector<std::string> coorStrVec;
            for (auto &pinNode : sortedPinNodes) {
                coorStrVec.push_back(
                    pinNode->getFirstDescendantByName("coordinates", false)->getInnerText()
                );
            }

            std::vector<xml::Node*> pathNodes {builderKML.getPath(
                builderKML.COORSTR_NO_ADD_ALTITUDE,
                styleMapId,
                coorStrVec,
                "Path of Selected Pins",
                "Created with KML-TOWN"
            )};

            insertEditedPlacemarksIntoFolder(
                PINS_PATH_COMMAND_WORKING_FOLDER,
                searchMainFolder(kmlNode),
                pathNodes,
                {"Pins-path unifying", "Pins-path unify"},
                ""
            );
        }
};

#endif // __KML_PLACEMARK_H__
