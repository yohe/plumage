
#include <iostream>
#include "plugin/sample_plugin.hpp"

extern "C" plumage::PluginHolder* createSamplePlugin() {
    std::cout << "createSamplePlugin called" << std::endl;
    SamplePlugin* pif = new SamplePlugin("SamplePlugin");
    plumage::PluginHolder* holder = new plumage::PluginHolder(pif);
    std::cout << "holder = " << holder << std::endl;
    std::cout << "pif = " << pif << std::endl;
    return holder;
}
