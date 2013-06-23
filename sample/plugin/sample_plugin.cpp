
#include <iostream>
#include "plugin/sample_plugin.hpp"

extern "C" plumage::PluginInterface* createSamplePlugin() {
    std::cout << "createSamplePlugin called" << std::endl;
    plumage::PluginInterface* pif = new SamplePlugin("SamplePlugin");
    std::cout << "pif = " << pif << std::endl;
    return pif;
}
