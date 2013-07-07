
#ifndef PLUMAGE_PLUGIN_MANAGER_H
#define PLUMAGE_PLUGIN_MANAGER_H

#include <string>
#include <map>

namespace plumage {

    class PluginRepository;
    class PluginInterface;
    class PluginRequirement;

    class PluginManager {

        typedef PluginInterface* (*PluginAllocator)();
        class RepositoryKey {
        public:
            RepositoryKey(std::string pluginName, int interfaceVersion) {
                pluginName_ = pluginName;
                version_ = interfaceVersion;
            }

            bool operator==(const RepositoryKey& key) const {
                if(version_ != key.version_) {
                    return false;
                }
                if(pluginName_ != key.pluginName_) {
                    return false;
                }
                return true;
            }

            bool operator<(const RepositoryKey& key) const {
                if(version_ == key.version_) {
                    return (pluginName_ < key.pluginName_);
                }
                
                return (version_ < key.version_);
            }
            bool operator>(const RepositoryKey& key) const {
                return !(this->operator<(key));
            }
        protected:
            std::string pluginName_;
            int version_;
        };
    public:
        PluginManager(std::string version = "0.1") : version_(version) {}

        ~PluginManager();

        bool loadPlugin(const std::string& pluginPath, const std::string& loadMethod);
        bool releasePlugin(std::string pluginName, int interfaceVersion, int pluginVersion);

        PluginRepository* getPluginRepository(const std::string& pluginName,
                                              int interfaceVersion,
                                              bool debug = false); 

        virtual bool setup(const std::string& configFile); 
        //virtual bool checkCompatibility(PluginInterface* pif);

        bool validateRequirement(const PluginRequirement& requirement);

    private:
        typedef std::map<RepositoryKey, PluginRepository*> PluginRepositoryMap;
        PluginRepositoryMap repositoryMap_;
        PluginRepositoryMap repositoryMapDebug_;

        std::string version_;
    };

}

#endif /* end of include guard */
