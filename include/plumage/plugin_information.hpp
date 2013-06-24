
#ifndef PLUMAGE_PLUGIN_INFORMATION_H
#define PLUMAGE_PLUGIN_INFORMATION_H


namespace plumage {

    class PluginInterface;

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
        PluginInformation(PluginInterface* pif, PluginStatus::Status status, void* pluginHandle);

        virtual ~PluginInformation();

        PluginInterface* getPlugin() const {
            return plugin_;
        }

        PluginStatus::Status getPluginStatus() const {
            return status_;
        }
        
    private:
        PluginInterface* plugin_;
        PluginStatus::Status status_;
        void* pluginHandle_;
    };

}
#endif

