
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

bool PluginManager::setup(const std::string& configFile) {
    return true;
}

void PluginManager::loadPlugin(const std::string& pluginPath, const std::string& loadMethod) throw (std::runtime_error) {

    std::cout << "Loading.. : " << pluginPath << " --- " << std::flush;
    void* pluginLibrary = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if(!pluginLibrary) {
        throw std::runtime_error("library load error: " + pluginPath);
    }
    std::cout << "OK" << std::endl;

    PluginAllocator allocator = (PluginAllocator) dlsym(pluginLibrary,
                                                          loadMethod.c_str());

    if(allocator == nullptr) {
        dlclose(pluginLibrary);
        throw std::runtime_error("method symbol error: " + loadMethod);
    }

    PluginHolder* holder = allocator();
    std::cout << "holder: " << holder << std::endl;
    PluginEntity* entity = holder->get();
    std::cout << "pif: " << entity << std::endl;

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
}

void PluginManager::validateRequirement(PluginEntity* entity) throw (std::runtime_error) {
    const PluginRequirement& requirement = entity->getRequirement();
    //std::map<std::string, PluginEntity*>& pluginMap = entity->getRequiredPlugins();

    for(const PluginRequirementInfo& req: requirement.getRequirementList()) {
        PluginRepository* repos = getPluginRepository(req.pluginName_, req.version_);
        if(repos == nullptr) {
            std::stringstream ss;
            ss << "Requirement error : " << "The interface version \"" << req.version_ << " of " << req.pluginName_ << "\" is not registerd.";
            throw std::runtime_error(ss.str().c_str());
        }
        if(PluginRepository::NO_ACTIVATE == repos->getActivatedVersion()) {
            std::stringstream ss;
            ss << "Requirement error : " << "The interface version \"" << req.version_ << " of " << req.pluginName_<< "\" is not activated.";
            throw std::runtime_error(ss.str().c_str());
        }
        entity->requiredPlugins_.insert(std::make_pair(req.pluginName_, repos->getActivatedPlugin()));
    }
}

void PluginManager::validateReference(PluginEntity* entity) throw (std::runtime_error) {
}

