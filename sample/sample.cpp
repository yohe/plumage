
#include "plugin/sample_plugin.hpp"
#include "plumage/plugin_repository.hpp"
#include "plumage/plugin_manager.hpp"

int main(int argc, char const* argv[])
{
    plumage::PluginManager manager;
    manager.loadPlugin("./libsample_plugin.dylib", "createSamplePlugin");

    plumage::PluginRepository* repos = manager.getPluginRepository("SamplePlugin", 1, true);
    SamplePlugin* pif = repos->getPlugin<SamplePlugin>();
    pif->start();
    pif->stop();
    return 0;
}
