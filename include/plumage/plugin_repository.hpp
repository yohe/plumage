#ifndef PLUMAGE_PLUGIN_REPOSITORY_H
#define PLUMAGE_PLUGIN_REPOSITORY_H

#include <algorithm>
#include <iostream>
#include <string>
#include <map>

#include "plumage/plugin_information.hpp"

namespace plumage {

    class PluginInterface;

    class PluginRepository {
    protected:
    public:

        enum DefinedValue : int {
            NO_ACTIVATE = 0 
        };

        PluginRepository(const std::string& repositoryName, int interfaceVersion) : 
            repositoryName_(repositoryName),
            interfaceVersion_(interfaceVersion),
            activatedPluginVersion_(NO_ACTIVATE)
        {}

        virtual ~PluginRepository();

        PluginInterface* registerPlugin(PluginHolder* plugin, void* pluginHandle);

        void unregistPlugin(PluginInterface* pif);

        std::string getRepositoryName() const {
            return repositoryName_;
        }

        PluginInterface* getActivatedPlugin() const;
        PluginInterface* getPlugin(int pluginVersion) const;

        bool isActivated(int pluginVersion) const;
        int getActivatedVersion() const {
            return activatedPluginVersion_;
        }
        bool activate(int pluginVersion);
        bool deactivate();

        std::map<int, PluginInterface*> getPluginList() const;

    protected:
        typedef std::map<int, PluginInformation*> PluginMap;
        PluginMap pluginMap_;
        std::string repositoryName_;
        int interfaceVersion_;
        int activatedPluginVersion_;

    };

}
#endif /* end of include guard */
