
#include "plumage/plugin_interface.hpp"

using namespace plumage;

PluginInterface::~PluginInterface() {
}

bool PluginInterface::start() {
    bool ret = doStart();
    return ret;
}

bool PluginInterface::stop() {
    bool ret = doStop();
    return ret;
}

PluginDeleter* PluginInterface::getDeleter() const {
    return deleter_;
}

