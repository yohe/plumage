
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "plumage/plugin_requirement.hpp"

using namespace plumage;

PluginRequirement::PluginRequirement() {}
PluginRequirement::~PluginRequirement() {}


void PluginRequirement::addRequirement(const std::string& pluginName, int interfaceVersion) throw (std::logic_error) {
    // The requirement do not accept the diffrent versions of the same plugin name.
    PluginRequirementInfo info(pluginName, interfaceVersion);
    if(std::find(requirementList_.begin(), requirementList_.end(), info) != requirementList_.end()) {
        std::stringstream ss;
        ss << "requirement error: " << pluginName << " already added.";
        throw std::logic_error(ss.str());
    }

    requirementList_.push_back(info);
}

const std::list<PluginRequirementInfo>& PluginRequirement::getRequirementList() const {
    return requirementList_;
}

