
#ifndef PLUMAGE_PLUGIN_INFORMATION_H
#define PLUMAGE_PLUGIN_INFORMATION_H

#include "plumage/plugin_interface.hpp"

namespace plumage {

    class PluginStatus {
    public:
        enum Status{
            REGISTED = 1,
            ACTIVATED = 2,
            STARTED = 3,
        };
    };

    class PluginInformation {
    public:
        PluginInformation(PluginInterface* pif, PluginStatus::Status status) :
            _plugin(pif), _status(status) {}

        virtual ~PluginInformation() {
            PluginDeleter* deleter = _plugin->getDeleter();
            (*deleter)(_plugin);
            delete deleter;
        }

        PluginInterface* getPlugin() const {
            return _plugin;
        }

        PluginStatus::Status getPluginStatus() const {
            return _status;
        }

    private:
        PluginInterface* _plugin;
        PluginStatus::Status _status;

    };

}
#endif

