
#include <utility>

#include "plumage/plugin_entity.hpp"
#include "plumage/plugin_interface.hpp"

using namespace plumage;

PluginEntity::PluginEntity(std::string name) : started_(false), pluginName_(name), interface_(this) {
}

PluginEntity::~PluginEntity() {
}

bool PluginEntity::isStarted() const {
    return started_;
}
bool PluginEntity::start() {
    if(!isStarted()) {
        bool ret = doStart();
        started_ = ret;
        return ret;
    }
    return true;
}

bool PluginEntity::stop() {
    if(isStarted()) {
        bool ret = doStop();
        started_ = !ret;
        return ret;
    }
    return false;
}

boost::any PluginEntity::call(const std::string& methodName, boost::any& paramter)  throw (std::exception) {
    if(isCallable(methodName) == false) {
        std::string mes = methodName;
        mes += " is not support.";
        throw std::logic_error(mes.c_str());
    }
    return std::move(doCall(methodName, paramter));
}

PluginInterface* PluginEntity::getRequiredPlugin(const std::string& pluginName) const {
    if(requiredPlugins_.count(pluginName) == 0) {
        return nullptr;
    }
    return requiredPlugins_.at(pluginName);
}
