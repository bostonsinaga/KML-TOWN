#ifndef __KML_BUILDER_H__
#define __KML_BUILDER_H__

class Builder {
    public:
        void setup() {
            kmlNode = getSkeleton();
            docNode = kmlNode->getChildrenByName("Document").front();
            mainFolderNode = docNode->getChildrenByName("Folder").front();

            // determined as 'yellow_push_pin'
            pinIconUrl = pinIconStrings.pinIconUrlArray[0];
            pinIconTypeFlag = pinIconStrings.getPinTyleFlag(pinIconUrl);
            styleSetNode = getStyleMap();
            insertStyleMap();
        }

        xml::Node *createAsPinsFromScanner(
            std::vector<std::string> dateStrVec,
            std::vector<std::string> axisStrVec,
            std::string docName_in = ""
        ) {
            // dates and coordinates size must be in same size
            if (dateStrVec.size() != axisStrVec.size()) {
                std::cerr << dateCoorVecErrStr;
                return nullptr;
            }

            setTitle(docName_in);

            int dateVecCtr = 0;
            for (auto &coor : axisStrVec) {
                
                mainFolderNode->addChild(
                    getPlacemark(dateStrVec.at(dateVecCtr), coor)
                );
                dateVecCtr++;
            }
            
            return kmlNode;
        }

        xml::Node *createAsPathFromSorter(
            std::vector<std::string> coorStrVec,
            std::string docName_in = ""
        ) {
            return nullptr;
        }

        xml::Node *getFolder(
            std::string name = "",
            bool isOpen = true
        ) {
            if (name == "") name = "Folder";

            return xmlReader.parse(
                std::string("<Folder>") +
                std::string("<name>") + name + std::string("</name>") +
                std::string("<open>") + std::to_string(isOpen) + std::string("</open>") +
                std::string("</Folder>"),
                "Folder [in runtime element]"
            );
        }

        // return one style map set
        xml::Node *getStyleMap() {

            std::string
                normalId, highlightId,
                normalScale, highlightScale,
                hotspotPos[2];

            pinIconNamed = mini_tool::cutFileDirName(pinIconUrl);

            normalId = "sn_" + pinIconNamed;
            highlightId = "sh_" + pinIconNamed;
            styleMapId = "msn_" + pinIconNamed;

            switch (pinIconTypeFlag) {
                case pinIconStrings.PUSHPIN_PIN_TYPE_FLAG: {
                    normalScale = pushpinScaleNormal;
                    highlightScale = pushpinScaleHighlight;
                    hotspotPos[0] = pushpinHotspotPos[0];
                    hotspotPos[1] = pushpinHotspotPos[1];
                break;}
                case pinIconStrings.PADDLE_PIN_TYPE_FLAG: {
                    normalScale = paddleScaleNormal;
                    highlightScale = paddleScaleHighlight;
                    hotspotPos[0] = paddleHotspotPos[0];
                    hotspotPos[1] = paddleHotspotPos[1];
                break;}
                case pinIconStrings.SHAPES_PIN_TYPE_FLAG: {
                    normalScale = shapesScaleNormal;
                    highlightScale = shapesScaleHighlight;
                    hotspotPos[0] = shapesHotspotPos[0];
                    hotspotPos[1] = shapesHotspotPos[1];
                break;}
            }
            
            return xmlReader.parse(
                std::string("<StyleSet>") +
                // normal style
                std::string("<Style id=\"") + normalId + std::string("\">") +
                std::string("<IconStyle>") +
                std::string("<scale>") + normalScale + std::string("</scale>") +
                std::string("<Icon>") +
                std::string("<href>") + pinIconUrl + std::string("</href>") +
                std::string("</Icon>") +
                std::string("<hotSpot x=\"")
                    + hotspotPos[0] + std::string("\" y=\"")
                    + hotspotPos[1] + std::string("\" xunits=\"pixels\" yunits=\"pixels\"/>") +
                std::string("</IconStyle>") +
                std::string("</Style>") +
                // highlight style
                std::string("<Style id=\"") + highlightId + std::string("\">") +
                std::string("<IconStyle>") +
                std::string("<scale>") + highlightScale + std::string("</scale>") +
                std::string("<Icon>") +
                std::string("<href>") + pinIconUrl + std::string("</href>") +
                std::string("</Icon>") +
                std::string("<hotSpot x=\"")
                    + hotspotPos[0] + std::string("\" y=\"")
                    + hotspotPos[1] + std::string("\" xunits=\"pixels\" yunits=\"pixels\"/>") +
                std::string("</IconStyle>") +
                std::string("</Style>") +
                // style map
                std::string("<StyleMap id=\"") + styleMapId + std::string("\">") +
                std::string("<Pair>") +
                std::string("<key>normal</key>") +
                std::string("<styleUrl>#") + normalId + std::string("</styleUrl>") +
                std::string("</Pair>") +
                std::string("<Pair>") +
                std::string("<key>highlight</key>") +
                std::string("<styleUrl>#") + highlightId + std::string("</styleUrl>") +
                std::string("</Pair>") +
                std::string("</StyleMap>") +
                std::string("</StyleSet>"),
                "StyleSet [in runtime element]"
            );
        }

        xml::Node *getPlacemark(
            std::string description,
            std::string coorStr_float,
            std::string name = ""
        ) {
            return xmlReader.parse(
                std::string("<Placemark>") +
                std::string("<name>") + name + std::string("</name>") +
                std::string("<description>") + description + std::string("</description>") +
                std::string("<styleUrl>#") + styleMapId + std::string("</styleUrl>") +
                std::string("<Point>") +
                std::string("<coordinates>") + coorStr_float + std::string("</coordinates>") +
                std::string("</Point>") +
                std::string("</Placemark>"),
                "Placemark [in runtime element]"
            );
        }

        xml::Node *getSkeleton() {
            if (docName == "") docName = "KML-TOWN";
            
            return xmlReader.parse(
                // kml doc
                std::string("<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">") +
                std::string("<Document>") +
                std::string("<name>") + docName + std::string("</name>") +

                    // main folder
                    std::string("<Folder>") +
                    std::string("<name>") + docName + std::string("</name>") +
                    std::string("<open>1</open>") +
                    // all stuff branches from here
                    std::string("</Folder>") +

                std::string("</Document>") +
                std::string("</kml>"),
                "kml [in runtime element]"
            );
        }

        void setTitle(std::string &docName_in) {
            docName = docName_in;

            if (docNode) {
                docNode->getFirstChildByName("name")->setInnerText(docName);
            }

            if (mainFolderNode) {
                mainFolderNode->getFirstChildByName("name")->setInnerText(docName);
            }
        }

        void insertStyleMap() {
            int childCtr = 0;

            for (auto &child : *docNode->getChildren()) {
                if (child->getName() == "Folder") break;
                childCtr++;
            }

            std::vector<xml::Node*> styleSetNodeVec = styleSetNode->releaseChildren();
            if (styleSetNode) delete styleSetNode;

            for (auto &node : styleSetNodeVec) {
                docNode->addChild(node, childCtr);
                childCtr++; // still pointing to named 'Folder'
            }
        }

    private:
        xml::Reader xmlReader;
        PinIconStrings pinIconStrings;

        xml::Node
            *kmlNode,
            *docNode,
            *mainFolderNode,
            *styleSetNode;

        std::string
            docName,
            pinIconUrl,
            pinIconNamed,
            styleMapId;

        int pinIconTypeFlag;

        std::string
            pushpinHotspotPos[2] = {"20", "2"},
            paddleHotspotPos[2] = {"32", "1"},
            shapesHotspotPos[2] = {"0.5", "0"};

        std::string
            pushpinScaleNormal = "1.1", pushpinScaleHighlight = "1.3",
            paddleScaleNormal = "1.1", paddleScaleHighlight = "1.3",
            shapesScaleNormal = "1.2", shapesScaleHighlight = "1.4";

        std::string dateCoorVecErrStr = (
            "KML-> Error. Vector strings of date and coordinate are not in same size\n"
        );
};

#endif // __KML_BUILDER_H__