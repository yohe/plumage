
#ifndef PLUMAGE_PLUGIN_INTERFACE_H
#define PLUMAGE_PLUGIN_INTERFACE_H

#include <string>
#include <vector>
#include <exception>

#include "plumage/plumage_config.hpp"
#include "plumage/plugin_requirement.hpp"

namespace plumage {

    class PluginInterface;

    class DeleterBase {
    public:
        virtual ~DeleterBase() {}
        virtual void operator()(void* p) const = 0;
    };

    template <class T>
    class PluginDeleter : public DeleterBase {
    public:
        virtual void operator()(void* p) const {
            delete static_cast<T*>(p);
        }
    };

    class PluginHolder {
    public:
        template<class T>
        PluginHolder(T* plugin) : plugin_(plugin){
            deleter_ = new PluginDeleter<T>();
        }

        ~PluginHolder() {
            (*deleter_)(plugin_);
            delete deleter_;
        }

        PluginInterface* get() const {
            return plugin_;
        }
    private:
        PluginInterface* plugin_;
        DeleterBase* deleter_;
    };

    class PluginInterface {
    public:
        PluginInterface(std::string name) : pluginName_(name){
        }

        virtual ~PluginInterface() {
        }

        const char* getPlumageVersion() const {
            return PLUMAGE_VERSION;
        }
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

    private:

        virtual bool doStart() = 0;
        virtual bool doStop() = 0;
        virtual void* doCall(std::string methodName, void* paramter) = 0;

    protected:
        std::string pluginName_;
        PluginRequirement requirement_;
    };

}

#endif /* end of include guard */
