
#include "plumage/plugin_entity.hpp"
#include "plumage/plugin_interface.hpp"

using namespace plumage;

PluginEntity::PluginEntity(std::string name) : pluginName_(name), interface_(this) {
}

PluginEntity::~PluginEntity() {
}

bool PluginEntity::start() {
    bool ret = doStart();
    return ret;
}

bool PluginEntity::stop() {
    bool ret = doStop();
    return ret;
}

void* PluginEntity::call(const std::string& methodName, void* paramter)  throw (std::exception) {
    if(isCallable(methodName) == false) {
        return nullptr;
        // throw exception;
    }
    return doCall(methodName, paramter);
}

