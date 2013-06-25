
#include "plugin/sample_plugin.hpp"
#include "plumage/plugin_repository.hpp"
#include "plumage/plugin_manager.hpp"

int main(int argc, char const* argv[])
{
    plumage::PluginManager manager;
#ifdef MAC_OSX
    manager.loadPlugin("./libsample_plugin.dylib", "createSamplePlugin");
#else
    manager.loadPlugin("./libsample_plugin.so", "createSamplePlugin");
#endif

    plumage::PluginRepository* repos = manager.getPluginRepository("SamplePlugin", 1, true);
    SamplePlugin* pif = repos->getPlugin<SamplePlugin>();
    pif->start();
    pif->stop();
    return 0;
}
