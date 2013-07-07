
#ifndef PLUMAGE_PLUGIN_INTERFACE_H
#define PLUMAGE_PLUGIN_INTERFACE_H

#include <string>
#include <vector>
#include <exception>

#include "plumage/plugin_requirement.hpp"

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
        const PluginRequirement& getRequirement() {
            return requirement_;
        }

        std::string getPluginName() const { return pluginName_; }
        virtual int getPluginVersion() const = 0;
        virtual bool isDebug() const = 0;
        virtual bool isCompatible(int pluginVersion) const = 0;
        virtual bool isCallable(const std::string& methodName) const = 0;

        bool start();
        bool stop();
        void* call(const std::string& methodName, void* paramter = nullptr) throw(std::exception);

        PluginDeleter* getDeleter() const;

    private:
        friend class PluginManager;

        virtual bool doStart() = 0;
        virtual bool doStop() = 0;
        virtual void* doCall(std::string methodName, void* paramter) = 0;

    protected:
        std::string pluginName_;
        PluginDeleter* deleter_;
        PluginRequirement requirement_;
    };

}

#endif /* end of include guard */
