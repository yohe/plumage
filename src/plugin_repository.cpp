
#include <algorithm>

#include "plumage/plugin_repository.hpp"
#include "plumage/plugin_manager.hpp"
#include "plumage/plugin_entity.hpp"
#include "plumage/plumage_util.hpp"

using namespace plumage;

PluginRepository::~PluginRepository() {
    std::for_each(pluginMap_.begin(), pluginMap_.end(), MapElementDeleter());
}

PluginEntity* PluginRepository::registerPlugin(PluginHolder* holder, void* pluginHandle) {
    PluginEntity* pif = holder->get();;
    PluginInformation* pinfo = new PluginInformation(holder, PluginStatus::REGISTED, pluginHandle);
    pluginMap_[pif->getPluginVersion()] = pinfo;
    std::cout << "registered plugin." << std::endl;
    return pif;
}
void PluginRepository::unregistPlugin(PluginEntity* entity) {
    PluginMap::iterator ite = pluginMap_.begin();
    PluginMap::iterator end = pluginMap_.end();
    for(; ite != end; ++ite) {
        if(ite->second->getHolder()->get() == entity) {
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
        return nullptr;
    }
    PluginInformation* pinfo = ite->second;
    PluginEntity* entity = pinfo->getHolder()->get();
    return entity->get();
}

PluginInterface* PluginRepository::getPlugin(int pluginVersion) const {
    PluginMap::const_iterator ite = pluginMap_.find(pluginVersion);
    if(ite == pluginMap_.end()) {
        return nullptr;
    }
    PluginInformation* pinfo = ite->second;
    PluginEntity* entity = pinfo->getHolder()->get();
    return entity->get();
}

bool PluginRepository::activate(int pluginVersion) throw (std::runtime_error) {
    if(activatedPluginVersion_ != 0) {
        return false;
    }
    PluginMap::const_iterator ite = pluginMap_.find(pluginVersion);
    if(ite == pluginMap_.end()) {
        return false;
    }
    PluginInformation* pinfo = ite->second;

    try {
        manager_->validateRequirement(pinfo->getHolder()->get());
        pinfo->getHolder()->get()->start();
        pinfo->setStatus(PluginStatus::ACTIVATED);
        activatedPluginVersion_ = pluginVersion;
    } catch (const std::runtime_error& e) {
        throw e;
    }
    return true;
}

bool PluginRepository::deactivate() throw (std::runtime_error) {
    if(activatedPluginVersion_ == 0) {
        return false;
    }
    PluginMap::const_iterator ite = pluginMap_.find(activatedPluginVersion_);
    if(ite == pluginMap_.end()) {
        return false;
    }
    PluginInformation* pinfo = ite->second;

    try {
        manager_->validateReference(pinfo->getHolder()->get());
    } catch (const std::runtime_error& e) {
        throw e;
    }

    return true;
}
