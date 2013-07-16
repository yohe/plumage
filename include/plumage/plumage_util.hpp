#ifndef PLUMAGE_PLUMAGE_UTIL_H
#define PLUMAGE_PLUMAGE_UTIL_H

namespace plumage {

    class MapElementDeleter {
    public:
        template <class T>
        void operator()(T& element) {
            delete element.second;
        };
    };

}

#endif 
