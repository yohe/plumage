
#ifndef PLUMAGE_PLUGIN_REQUIREMENT_H
#define PLUMAGE_PLUGIN_REQUIREMENT_H

#include <set>
#include <string>
#include <stdexcept>

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
        bool operator<(const PluginRequirementInfo& rhs) const {
            if(pluginName_ < rhs.pluginName_) {
                return true;
            }
            return false;
        }

        bool operator>(const PluginRequirementInfo& rhs) const {
            return !(*this < rhs);
        }
    };

    class PluginRequirement {
    public:
        PluginRequirement();
        ~PluginRequirement();

        void addRequirement(const std::string& pluginName, int interfaceVersion) throw (std::logic_error);
        const std::set<PluginRequirementInfo>& getRequirementList() const;

    private:
        std::set<PluginRequirementInfo> requirementList_;
    };
}

#endif

