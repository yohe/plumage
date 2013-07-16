
#ifndef PLUMAGE_PLUGIN_INFORMATION_H
#define PLUMAGE_PLUGIN_INFORMATION_H


namespace plumage {

    class PluginHolder;

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
        PluginInformation(PluginHolder* pif, PluginStatus::Status status, void* pluginHandle);

        virtual ~PluginInformation();

        PluginHolder* getHolder() const {
            return holder_;
        }

        PluginStatus::Status getStatus() const {
            return status_;
        }

        void setStatus(PluginStatus::Status status) {
            if(status_ < status) {
                status_ = status;
            }
        }

    private:
        PluginHolder* holder_;
        PluginStatus::Status status_;
        void* pluginHandle_;
    };

}
#endif

