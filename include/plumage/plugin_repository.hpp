#ifndef PLUMAGE_PLUGIN_REPOSITORY_H
#define PLUMAGE_PLUGIN_REPOSITORY_H

#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

#include "plumage/plugin_information.hpp"

namespace plumage {

    class PluginManager;
    class PluginEntity;
    class PluginInterface;

    class PluginRepository {
        friend class PluginManager;
    protected:
    public:

        enum DefinedValue : int {
            NO_ACTIVATE = 0,
            LATEST_VERSION = -1 
        };

        PluginRepository(PluginManager* manager, const std::string& repositoryName, int interfaceVersion) : 
            manager_(manager),
            repositoryName_(repositoryName),
            interfaceVersion_(interfaceVersion),
            activatedPluginVersion_(NO_ACTIVATE)
        {}

        virtual ~PluginRepository();

        PluginEntity* registerPlugin(PluginHolder* plugin, void* pluginHandle);

        void unregistPlugin(PluginEntity* pif);

        std::string getRepositoryName() const {
            return repositoryName_;
        }
        int getLatestVersion() const;

        PluginInterface* getActivatedPlugin() const;
        PluginInterface* getPlugin(int pluginVersion) const;

        bool isActivated(int pluginVersion) const;
        int getActivatedVersion() const {
            return activatedPluginVersion_;
        }

        bool activate(int pluginVersion = DefinedValue::LATEST_VERSION) throw (std::runtime_error) ;
        bool deactivate() throw (std::runtime_error) ;

        std::map<int, PluginInterface*> getPluginList() const;

    protected:
        typedef std::map<int, PluginInformation*> PluginMap;
        PluginManager* manager_;
        PluginMap pluginMap_;
        std::string repositoryName_;
        int interfaceVersion_;
        int activatedPluginVersion_;

    };

}
#endif /* end of include guard */
