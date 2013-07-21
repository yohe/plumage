
#include <dlfcn.h>
#include <algorithm>
#include <sstream>

#include "plumage/plugin_manager.hpp"
#include "plumage/plugin_repository.hpp"
#include "plumage/plugin_interface.hpp"
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
        throw std::runtime_error("method symbol error: " + loadMethod);
    }

    PluginHolder* holder = allocator();
    std::cout << "holder: " << holder << std::endl;
    PluginInterface* pif = holder->get();
    std::cout << "pif: " << pif << std::endl;

    if(pif == nullptr) {
        dlclose(pluginLibrary);
        throw std::runtime_error("allocate error: " + loadMethod);
    }

    try {
        validateRequirement(pif->getRequirement());
    } catch (const std::runtime_error& e) {
        dlclose(pluginLibrary);
        throw e;
    }

    PluginRepository* repos = getPluginRepository(pif->getPluginName(), pif->getInterfaceVersion(), pif->isDebug());
    if( repos == nullptr) {
        repos = new PluginRepository(pif->getPluginName(), pif->getInterfaceVersion());
    }

    RepositoryKey key(pif->getPluginName(), pif->getInterfaceVersion());
    repos->registerPlugin(holder, pluginLibrary);
    if(!pif->isDebug()) {
        repositoryMap_.insert(std::pair<RepositoryKey, PluginRepository*>(key, repos));
    } else {
        repositoryMapDebug_.insert(std::pair<RepositoryKey, PluginRepository*>(key, repos));
    }
}

void PluginManager::validateRequirement(const PluginRequirement& requirement) throw (std::runtime_error) {
    for(const PluginRequirementInfo& req: requirement.getRequirementList()) {
        PluginRepository* repos = getPluginRepository(req.pluginName_, req.version_);
        if(repos == nullptr) {
            std::stringstream ss;
            ss << "Requirement error : " << req.pluginName_ << " of the interface version \"" << req.version_ << "\" is not registerd.";
            throw std::runtime_error(ss.str().c_str());
        }
        if(PluginRepository::NO_ACTIVATE == repos->getActivatedVersion()) {
            std::stringstream ss;
            ss << "Requirement error : " << req.pluginName_ << " of the interface version \"" << req.version_ << "\" is not activated.";
            throw std::runtime_error(ss.str().c_str());
        }
    }
}

