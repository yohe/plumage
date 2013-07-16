
#include <dlfcn.h>
#include <algorithm>

#include "plumage/plugin_manager.hpp"
#include "plumage/plugin_repository.hpp"
#include "plumage/plugin_interface.hpp"
#include "plumage/plumage_util.hpp"

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
        throw std::runtime_error("allocate error: " + loadMethod);
    }

    try {
        validateRequirement(pif->getRequirement());
    } catch (const std::runtime_error& e) {
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
}

