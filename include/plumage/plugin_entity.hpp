
#ifndef PLUMAGE_PLUGIN_ENTITY_H
#define PLUMAGE_PLUGIN_ENTITY_H

#include <string>
#include <vector>
#include <exception>
#include <map>

#include <boost/any.hpp>

#include "plumage/plumage_config.hpp"
#include "plumage/plugin_requirement.hpp"
#include "plumage/plugin_interface.hpp"

namespace plumage {

    class PluginEntity;

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

        PluginEntity* get() const {
            return plugin_;
        }
    private:
        PluginEntity* plugin_;
        DeleterBase* deleter_;
    };

    class PluginManager;
    class PluginInterface;
    class PluginEntity {
        friend class PluginHolder;
        friend class PluginManager;

    public:
        PluginEntity(std::string name);

        virtual ~PluginEntity();

        const char* getPlumageVersion() const {
            return PLUMAGE_VERSION;
        }

        std::string getPluginName() const { return pluginName_; }
        virtual int getPluginVersion() const = 0;
        virtual int getInterfaceVersion() const = 0;
        PluginInterface* get() {
            return &interface_;
        }

        const PluginRequirement& getRequirement() {
            return requirement_;
        }
        PluginInterface* getRequiredPlugin(const std::string& pluginName) const;

        virtual bool isDebug() const = 0;
        virtual bool isCompatible(int pluginVersion) const = 0;
        virtual bool isCallable(const std::string& methodName) const = 0;

        bool start();
        bool stop();
        boost::any call(const std::string& methodName, boost::any& paramter) throw(std::exception);

    private:
        virtual bool doStart() = 0;
        virtual bool doStop() = 0;
        virtual boost::any doCall(std::string methodName, boost::any& paramter) throw (std::exception) = 0;

    protected:
        std::string pluginName_;
        PluginRequirement requirement_;
        
    private:
        // Reference to
        std::map<std::string, PluginInterface*> requiredPlugins_;
        // Reference from
        std::map<std::string, PluginInterface*> referringPlugins_;
        PluginInterface interface_;
    };

}

#endif /* end of include guard */
