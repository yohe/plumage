
#ifndef PLUMAGE_COMPATIBILITY_CHECKER_H
#define PLUMAGE_COMPATIBILITY_CHECKER_H

#include <string>

namespace plumage {

class CompatibilityChecker {
public:
    CompatibilityChecker(const int version);

    bool isCompatible(const int lowVersion, const int highVersion);

private:
    bool isLowerEqualThanArgv(const int version);
    bool isHigherEqualThanArgv(const int version);

private:
    int _version;
};

CompatibilityChecker::CompatibilityChecker(const int version) : _version(version) {
}

bool CompatibilityChecker::isHigherEqualThanArgv(const int version) {
    if(_version >= version) {
        return true;
    }
    return false;
}

bool CompatibilityChecker::isLowerEqualThanArgv(const int version) {
    if(_version <= version) {
        return true;
    }
    return false;
}

bool CompatibilityChecker::isCompatible(const int lowVersion, const int highVersion) {
    if(!isHigherEqualThanArgv(lowVersion)) {
        return false;
    }

    if(!isLowerEqualThanArgv(highVersion)) {
        return false;
    }

    return true;
}

}

#endif /* end of include guard */
