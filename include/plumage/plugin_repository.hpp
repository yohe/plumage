#ifndef PLUMAGE_PLUGIN_REPOSITORY_H
#define PLUMAGE_PLUGIN_REPOSITORY_H

#include <algorithm>
#include <iostream>
#include <string>
#include <map>

#include "plumage/plugin_information.hpp"

namespace plumage {

    class PluginInterface;

    class RepositoryBase {
    protected:
    public:

        RepositoryBase(const std::string& repositoryName, int interfaceVersion) : 
            repositoryName_(repositoryName),
            interfaceVersion_(interfaceVersion),
            latestPluginVersion_(0)
        {}

        virtual ~RepositoryBase();

        PluginInterface* registerPlugin(PluginInterface* plugin, void* pluginHandle);

        void unregistPlugin(PluginInterface* pif);

        std::string getRepositoryName() const {
            return repositoryName_;
        }

    protected:
        typedef std::map<int, PluginInformation*> PluginMap;
        PluginMap pluginMap_;
        std::string repositoryName_;
        int interfaceVersion_;
        int latestPluginVersion_;

    };

    class PluginRepository : public RepositoryBase {
    public:
        PluginRepository(const std::string& repositoryName, int interfaceVersion);
        virtual ~PluginRepository();

        template <class T>
        T* getPlugin(int pluginVersion = -1) const {
            if(pluginVersion == -1) {
                pluginVersion = latestPluginVersion_;
            }
            PluginMap::const_iterator ite = pluginMap_.find(pluginVersion);
            if(ite == pluginMap_.end()) {
                return NULL;
            }
            PluginInformation* pinfo = ite->second;
            PluginInterface* pif = pinfo->getPlugin();
            return dynamic_cast<T*>(pif);
        }

    protected:
    };

}
#endif /* end of include guard */
