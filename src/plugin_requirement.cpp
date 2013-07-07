
#include <algorithm>
#include "plumage/plugin_requirement.hpp"

using namespace plumage;

PluginRequirement::PluginRequirement() {}
PluginRequirement::~PluginRequirement() {}


void PluginRequirement::addRequirement(const std::string& pluginName, int interfaceVersion) {
    // The requirement do not accept the diffrent versions of the same plugin name.
    PluginRequirementInfo info(pluginName, interfaceVersion);
    if(std::find(requirementList_.begin(), requirementList_.end(), info) != requirementList_.end()) {
        return;
        // throw exception
    }

    requirementList_.push_back(info);
}

const std::list<PluginRequirementInfo>& PluginRequirement::getRequirementList() const {
    return requirementList_;
}

