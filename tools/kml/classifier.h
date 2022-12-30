#ifndef __CLASSIFIER_H__
#define __CLASSIFIER_H__

class Classifier {
    public:
        Classifier() {}
        void rearrange(xml::Node *kmlNode, bool isClean);
};

#endif // __CLASSIFIER_H__