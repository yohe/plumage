
#include "plumage/plugin_interface.hpp"

using namespace plumage;

bool PluginInterface::start() {
    bool ret = doStart();
    return ret;
}

bool PluginInterface::stop() {
    bool ret = doStop();
    return ret;
}

void* PluginInterface::call(const std::string& methodName, void* paramter)  throw (std::exception) {
    if(isCallable(methodName) == false) {
        return nullptr;
        // throw exception;
    }
    return doCall(methodName, paramter);
}

