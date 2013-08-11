
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "plumage/plugin_requirement.hpp"
#include "plumage/plugin_interface.hpp"

using namespace plumage;

PluginRequirement::PluginRequirement() {}
PluginRequirement::~PluginRequirement() {}


void PluginRequirement::addRequirement(const std::string& pluginName, int interfaceVersion) throw (std::logic_error) {
    // The requirement do not accept the diffrent versions of the same plugin name.
    PluginRequirementInfo info(pluginName, interfaceVersion);
    if(requirementList_.find(info) != requirementList_.end()) {
        std::stringstream ss;
        ss << "requirement error: " << pluginName << " already added.";
        throw std::logic_error(ss.str());
    }

    requirementList_.insert(info);
}

const std::set<PluginRequirementInfo>& PluginRequirement::getRequirementList() const {
    return requirementList_;
}

