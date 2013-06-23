
#ifndef PLUMAGE_PLUGIN_INTERFACE_H
#define PLUMAGE_PLUGIN_INTERFACE_H

#include <string>

namespace plumage {

    class PluginInterface;

    class PluginDeleter {
    public:
        virtual ~PluginDeleter() {}
        virtual void operator()(PluginInterface* p) const = 0;
    };

    class PluginManager;

    class PluginInterface {
    public:
        template <class Deleter>
        PluginInterface(std::string name, Deleter* deleter) : pluginName_(name){
            deleter_ = deleter;
        }
        virtual ~PluginInterface();

        virtual int getInterfaceVersion() const = 0;

        std::string getPluginName() const { return pluginName_; }
        virtual int getPluginVersion() const = 0;
        virtual bool isDebug() const = 0;

        bool start();
        bool stop();

        PluginDeleter* getDeleter() const;

    private:
        friend class PluginManager;

        virtual bool doStart() = 0;
        virtual bool doStop() = 0;
        

    protected:
        std::string pluginName_;
        PluginDeleter* deleter_;
    };

}

#endif /* end of include guard */
