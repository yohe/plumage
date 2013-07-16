
#include <dlfcn.h>
#include "plumage/plugin_information.hpp"
#include "plumage/plugin_interface.hpp"

using namespace plumage;

PluginInformation::PluginInformation(PluginHolder* holder, PluginStatus::Status status, void* pluginHandle) :
    holder_(holder),
    status_(status),
    pluginHandle_(pluginHandle)
{
}

PluginInformation::~PluginInformation() {
    delete holder_;
    dlclose(pluginHandle_);
}

