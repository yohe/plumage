
#include <dlfcn.h>
#include <algorithm>
#include <sstream>

#include "plumage/plugin_manager.hpp"
#include "plumage/plugin_repository.hpp"
#include "plumage/plugin_entity.hpp"
#include "plumage/plumage_util.hpp"
#include "plumage/plugin_requirement.hpp"
#include "plumage/plumage_config.hpp"

using namespace plumage;

PluginManager::~PluginManager() {
    std::for_each(repositoryMap_.begin(), repositoryMap_.end(), MapElementDeleter());
    std::for_each(repositoryMapDebug_.begin(), repositoryMapDebug_.end(), MapElementDeleter());
}

PluginRepository* PluginManager::getPluginRepository(const std::string& pluginName,
                                                     int interfaceVersion,
                                                     bool debug)
{
    RepositoryKey key(pluginName, interfaceVersion);
    if(debug) {
        if(repositoryMapDebug_.count(key) == 0) {
            return nullptr;
        }
        return repositoryMapDebug_.at(key);
    } else {
        if(repositoryMap_.count(key) == 0) {
            return nullptr;
        }
        return repositoryMap_.at(key);
    }
}

void PluginManager::loadPlugin(const std::string& pluginPath, const std::string& loadMethod) throw (std::runtime_error) {

#ifdef DEBUG
    std::cout << "Loading.. : " << pluginPath << " --- " << std::flush;
#endif
    void* pluginLibrary = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if(!pluginLibrary) {
        throw std::runtime_error("library load error: " + pluginPath);
    }
#ifdef DEBUG
    std::cout << "OK" << std::endl;
#endif

    PluginAllocator allocator = (PluginAllocator) dlsym(pluginLibrary,
                                                          loadMethod.c_str());

    if(allocator == nullptr) {
        dlclose(pluginLibrary);
        throw std::runtime_error("method symbol error: " + loadMethod);
    }

    PluginHolder* holder = allocator();
#ifdef DEBUG
    std::cout << "holder: " << holder << std::endl;
#endif
    PluginEntity* entity = holder->get();
#ifdef DEBUG
    std::cout << "pif: " << entity << std::endl;
#endif

    if(entity == nullptr) {
        dlclose(pluginLibrary);
        throw std::runtime_error("allocate error: " + loadMethod);
    }

    PluginRepository* repos = getPluginRepository(entity->getPluginName(), entity->getInterfaceVersion(), entity->isDebug());
    if( repos == nullptr) {
        repos = new PluginRepository(this, entity->getPluginName(), entity->getInterfaceVersion());
    }

    RepositoryKey key(entity->getPluginName(), entity->getInterfaceVersion());
    repos->registerPlugin(holder, pluginLibrary);
    if(!entity->isDebug()) {
        repositoryMap_.insert(std::pair<RepositoryKey, PluginRepository*>(key, repos));
    } else {
        repositoryMapDebug_.insert(std::pair<RepositoryKey, PluginRepository*>(key, repos));
    }
#ifdef DEBUG
    std::cout << entity->getPluginName() << " registerd." << std::endl;
#endif
}

bool PluginManager::releasePlugin(std::string pluginName, int interfaceVersion, int pluginVersion) {
    return true;
}

void PluginManager::validateRequirement(PluginEntity* entity) throw (std::runtime_error) {
    const PluginRequirement& requirement = entity->getRequirement();
    //std::map<std::string, PluginEntity*>& pluginMap = entity->getRequiredPlugins();

#ifdef CXX03
    std::set<PluginRequirementInfo>::iterator ite = requirement.getRequirementList().begin();
    std::set<PluginRequirementInfo>::iterator end = requirement.getRequirementList().end();
    for(; ite != end; ++ite) {
        const PluginRequirementInfo& req = *ite;
#else
    for(const PluginRequirementInfo& req: requirement.getRequirementList()) {
#endif
        PluginRepository* repos = getPluginRepository(req.pluginName_, req.version_);
        if(repos == nullptr) {
            std::stringstream ss;
            ss << "Requirement error : The interface version \"" << req.version_ << " of " << req.pluginName_ << "\" is not registerd.";
            throw std::runtime_error(ss.str().c_str());
        }
        if(PluginRepository::NO_ACTIVATE == repos->getActivatedVersion()) {
            std::stringstream ss;
            ss << "Requirement error The interface version \"" << req.version_ << " of " << req.pluginName_<< "\" is not activated.";
            throw std::runtime_error(ss.str().c_str());
        }
        PluginInterface* pif = repos->getActivatedPlugin();
        PluginInformation* info = repos->pluginMap_[pif->getPluginVersion()];
        PluginEntity* refEntity = info->getHolder()->get();
        refEntity->referringPlugins_.insert(std::make_pair(pif->getPluginName(), pif));

        entity->requiredPlugins_.insert(std::make_pair(req.pluginName_, repos->getActivatedPlugin()));
    }
}

void PluginManager::validateReference(PluginEntity* entity) throw (std::runtime_error) {
    for(auto value : entity->referringPlugins_) {
        PluginRepository* repos = getPluginRepository(value.first, value.second->getInterfaceVersion());
        if(repos == nullptr) {
            continue;
        }
        if(PluginRepository::NO_ACTIVATE != repos->getActivatedVersion()) {
            std::stringstream ss;
            ss << "Reference error : This plugin is referring other plugin(" << value.first <<") now.";
            throw std::runtime_error(ss.str().c_str());
        }
    }
}

