
#include <algorithm>

#include "plumage/plugin_repository.hpp"
#include "plumage/plugin_interface.hpp"
#include "plumage/plumage_util.hpp"

using namespace plumage;

PluginRepository::~PluginRepository() {
    std::for_each(pluginMap_.begin(), pluginMap_.end(), MapElementDeleter());
}

PluginInterface* PluginRepository::registerPlugin(PluginHolder* holder, void* pluginHandle) {
    PluginInterface* pif = holder->get();;
    PluginInformation* pinfo = new PluginInformation(holder, PluginStatus::REGISTED, pluginHandle);
    pluginMap_[pif->getPluginVersion()] = pinfo;
    std::cout << "registered plugin." << std::endl;
    return pif;
}
void PluginRepository::unregistPlugin(PluginInterface* pif) {
    PluginMap::iterator ite = pluginMap_.begin();
    PluginMap::iterator end = pluginMap_.end();
    for(; ite != end; ++ite) {
        if(ite->second->getHolder()->get() == pif) {
            if(ite->second->getStatus() == PluginStatus::REGISTED) {
                delete ite->second;
                pluginMap_.erase(ite->first);
                std::cout << "deleted plugin." << std::endl;
                break;
            } else {
            }
        }
    }
}

bool PluginRepository::isActivated(int pluginVersion) const {
    if(pluginMap_.count(pluginVersion) == 0) {
        return false;
    }

    PluginInformation* pInfo = pluginMap_.at(pluginVersion);
    return (pInfo->getStatus() == PluginStatus::ACTIVATED);
}

PluginInterface* PluginRepository::getActivatedPlugin() const {
    PluginMap::const_iterator ite = pluginMap_.find(activatedPluginVersion_);
    if(ite == pluginMap_.end()) {
        return NULL;
    }
    PluginInformation* pinfo = ite->second;
    PluginInterface* pif = pinfo->getHolder()->get();
    return (pif);
}

PluginInterface* PluginRepository::getPlugin(int pluginVersion) const {
    PluginMap::const_iterator ite = pluginMap_.find(pluginVersion);
    if(ite == pluginMap_.end()) {
        return NULL;
    }
    PluginInformation* pinfo = ite->second;
    PluginInterface* pif = pinfo->getHolder()->get();
    return (pif);
}

bool PluginRepository::activate(int pluginVersion) {
    if(activatedPluginVersion_ != 0) {
        return false;
    }
    PluginMap::const_iterator ite = pluginMap_.find(pluginVersion);
    if(ite == pluginMap_.end()) {
        return false;
    }
    PluginInformation* pinfo = ite->second;
    pinfo->setStatus(PluginStatus::ACTIVATED);
    activatedPluginVersion_ = pluginVersion;
    return true;
}

bool PluginRepository::deactivate() {
    if(activatedPluginVersion_ == 0) {
        return false;
    }
    return true;
}
