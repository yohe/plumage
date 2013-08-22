
#include <utility>

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

boost::any PluginEntity::call(const std::string& methodName, boost::any& paramter)  throw (std::exception) {
    if(isCallable(methodName) == false) {
        std::string mes = methodName;
        mes += " is not support.";
        throw std::logic_error(mes.c_str());
    }
    return std::move(doCall(methodName, paramter));
}

