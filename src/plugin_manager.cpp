
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

bool PluginManager::loadPlugin(const std::string& pluginPath, const std::string& loadMethod) {

    std::cout << "Loading.. : " << pluginPath << " --- " << std::flush;
    void* pluginLibrary = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if(!pluginLibrary) {
        // throw Exception;
        std::cout << "NG" << std::endl;
        return false;
    }
    std::cout << "OK" << std::endl;

    PluginAllocator allocator = (PluginAllocator) dlsym(pluginLibrary,
                                                          loadMethod.c_str());

    if(allocator == nullptr) {
        std::cout << "NG" << std::endl;
        return false;
    }

    PluginInterface* pif = allocator();

    if(pif == nullptr) {
        // throw Exception;
        return false;
    }

    if(validateRequirement(pif->getRequirement()) == false) {
        //throw exception;
        return false;
    }

    PluginRepository* repos = getPluginRepository(pif->getPluginName(), pif->getInterfaceVersion(), pif->isDebug());
    if( repos == nullptr) {
        repos = new PluginRepository(pif->getPluginName(), pif->getInterfaceVersion());
    }

    RepositoryKey key(pif->getPluginName(), pif->getInterfaceVersion());
    repos->registerPlugin(pif, pluginLibrary);
    if(!pif->isDebug()) {
        std::cout << "ReleaseMode" << std::endl;
        repositoryMap_.insert(std::pair<RepositoryKey, PluginRepository*>(key, repos));
    } else {
        std::cout << "DebugMode" << std::endl;
        repositoryMapDebug_.insert(std::pair<RepositoryKey, PluginRepository*>(key, repos));
    }

    return true;
}

bool PluginManager::validateRequirement(const PluginRequirement& requirement) {
    return true;
}
