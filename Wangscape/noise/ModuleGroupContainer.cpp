#include "ModuleGroupContainer.h"

#include <fstream>
#include <iostream>

#include "EncodedModuleGroup.h"
#include "codecs/EncodedModuleGroupCodec.h"
#include "logging/Logging.h"
#include "common.h"
#include "ModuleGroup.h"

namespace noise
{
template<typename T>
ModuleGroupContainer<T>::ModuleGroupContainer(std::string module_group_description_) :
    mModuleGroupDescription(module_group_description_)
{
}

template<typename T>
void ModuleGroupContainer<T>::addSpecificModuleGroup(T key, std::string filename, unsigned int seed)
{
    auto inserted = mModuleGroups.insert(typename ModuleGroups::value_type(key, loadModuleGroup(filename, seed)));
    if (!inserted.second)
        throw std::runtime_error("Tried to load two " + mModuleGroupDescription + " module groups with the same terrain");
}

template<typename T>
void ModuleGroupContainer<T>::tryAddDefaultModuleGroup(T key, boost::optional<std::string> filename, unsigned int seed)
{
    if (mModuleGroups.find(key) == mModuleGroups.cend())
    {
        if (filename)
        {
            auto inserted = mModuleGroups.insert(typename ModuleGroups::value_type(key, loadModuleGroup(filename.get(), seed)));
        }
        else
            throw std::runtime_error("Missing " + mModuleGroupDescription + " module group, and no default module group");
    }
}

template<typename T>
ModuleGroup& ModuleGroupContainer<T>::at(const T& key)
{
    return *mModuleGroups.at(key);
}

std::shared_ptr<ModuleGroup> loadModuleGroup(std::string filename, unsigned int seed)
{
    std::ifstream ifs(filename);
    if (!ifs.good())
    {
        throw std::runtime_error("Could not open options file");
    }

    std::string str{std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()};
    EncodedModuleGroup encoded_module_group;
    try
    {
        encoded_module_group = spotify::json::decode<EncodedModuleGroup>(str.c_str());
    }
    catch (const spotify::json::decode_exception& e)
    {
        logError() << "spotify::json::decode_exception encountered at "
            << e.offset()
            << ": "
            << e.what();
        throw;
    }
    encoded_module_group.decode();
    encoded_module_group.moduleGroup->setSeeds(static_cast<int>(seed));
    return encoded_module_group.moduleGroup;
}

template class ModuleGroupContainer<TerrainID>;
template class ModuleGroupContainer<std::pair<TerrainID, TerrainID>>;

}
