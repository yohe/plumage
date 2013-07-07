
#ifndef PLUMAGE_PLUGIN_REQUIREMENT_H
#define PLUMAGE_PLUGIN_REQUIREMENT_H

#include <list>
#include <string>

namespace plumage {

    struct PluginRequirementInfo {
        PluginRequirementInfo(const std::string& pluginName, int version)
            : pluginName_(pluginName),
              version_(version)
        {
        }
        std::string pluginName_;
        int version_;

        bool operator==(const PluginRequirementInfo& rhs) {
            if(pluginName_ != rhs.pluginName_) {
                return false;
            }
            return true;
        }
    };

    class PluginRequirement {
    public:
        PluginRequirement();
        ~PluginRequirement();

        void addRequirement(const std::string& pluginName, int interfaceVersion);
        const std::list<PluginRequirementInfo>& getRequirementList() const;

    private:
        std::list<PluginRequirementInfo> requirementList_;
    };
}

#endif

