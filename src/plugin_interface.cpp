
#include "plumage/plugin_interface.hpp"
#include "plumage/plugin_entity.hpp"

using namespace plumage;

std::string PluginInterface::getPluginName() const {
    return entity_->getPluginName();
}
int PluginInterface::getPluginVersion() const  {
    return entity_->getPluginVersion();
}
int PluginInterface::getInterfaceVersion() const  {
    return entity_->getInterfaceVersion();
}
bool PluginInterface::start() {
    return entity_->start();
}
bool PluginInterface::stop() {
    return entity_->stop();
}
bool PluginInterface::isDebug() const  {
    return entity_->isDebug();
}
bool PluginInterface::isCompatible(int pluginVersion) const  {
    return entity_->isCompatible(pluginVersion);
}
bool PluginInterface::isCallable(const std::string& methodName) const  {
    return entity_->isCallable(methodName);
}

void* PluginInterface::call(const std::string& methodName, boost::any& parameter) throw(std::exception)  {
    return entity_->call(methodName, parameter);
}

