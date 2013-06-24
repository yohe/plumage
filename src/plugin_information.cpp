
#include <dlfcn.h>
#include "plumage/plugin_information.hpp"
#include "plumage/plugin_interface.hpp"

using namespace plumage;

PluginInformation::PluginInformation(PluginInterface* pif, PluginStatus::Status status, void* pluginHandle) :
    plugin_(pif),
    status_(status),
    pluginHandle_(pluginHandle)
{
}

PluginInformation::~PluginInformation() {
    PluginDeleter* deleter = plugin_->getDeleter();
    (*deleter)(plugin_);
    delete deleter;
    dlclose(pluginHandle_);
}

