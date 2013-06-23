
#include "plumage/plugin_repository.hpp"

using namespace plumage;

RepositoryBase::~RepositoryBase() {
}

PluginInterface* RepositoryBase::registerPlugin(PluginInterface* plugin) {
    PluginInterface* pif = plugin;
    PluginInformation* pinfo = new PluginInformation(pif, PluginStatus::REGISTED);
    latestPluginVersion_ = std::max(latestPluginVersion_, pif->getPluginVersion());
    pluginMap_[pif->getPluginVersion()] = pinfo;
    std::cout << "registered plugin." << std::endl;
    return pif;
}
void RepositoryBase::unregistPlugin(PluginInterface* pif) {
    PluginMap::iterator ite = pluginMap_.begin();
    PluginMap::iterator end = pluginMap_.end();
    for(; ite != end; ++ite) {
        if(ite->second->getPlugin() == pif) {
            if(ite->second->getPluginStatus() == PluginStatus::REGISTED) {
                delete ite->second;
                pluginMap_.erase(ite->first);
                std::cout << "deleted plugin." << std::endl;
                break;
            } else {
            }
        }
    }
}

PluginRepository::PluginRepository(const std::string& repositoryName, int interfaceVersion) :
    RepositoryBase(repositoryName, interfaceVersion)
{
}

PluginRepository::~PluginRepository() {
}

