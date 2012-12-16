#include <string>
#include <map>

#include <rapidxml/rapidxml.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <unordered_map>

#include "TextureLoader.h"
#include "SpriteLoader.h"
#include "TilesetLoader.h"
#include "MapLoader.h"
#include "ObjectLoader.h"
#include "AudioClipLoader.h"
#include "MusicLoader.h"

#include "Exceptions.h"
#include "ResourceManager.h"
#include "Log.h"
#include "Util.h"

using namespace rapidxml;
namespace bf = boost::filesystem;
using namespace std;
using namespace Hiage;


string GetAttributeValue(xml_node<>* node, string name, Log& log);
vector<bf::path> GetXmlFiles(const bf::path& p, Log& log);

template<class T>
Resource<T> CreateResource(string filename, string name)
{
    return Resource<T>(filename, name);
}

template<class V> 
bool InsertResource(ResourceDictionary<V>& dict, string filename, string name, Log& log)
{
    log << Debug << "ResourceManager" << "Inserting resource with ID " << name << ", filename " << filename << endl;
    if (dict.find(name) != dict.end())
    {
        log << Warning << "ResourceManager" <<  "Resource with name \"" << name << "\" already added. Ignored." << endl;
        return false;
    }
    
    dict.insert(make_pair(name, CreateResource<V>(filename, name))); 

    return true;
}

template<class T>
const T& GetResource(ResourceDictionary<T> dict, string name, Log& log, ResourceLoader<T>& resourceLoader)
{
    typename ResourceDictionary<T>::iterator iter = dict.find(name);
    if (iter == dict.end())
        throw range_error("Resource with name " + name + " was not found.");

    Resource<T>& resource = iter->second;
    
    if (!resource.IsLoaded())
    {
        log << Debug << "ResourceManager" << "Resource " << name << " was not loaded. Loading it..." << endl;
        resource.Load(resourceLoader);
    }

    return resource.getResource();
}

struct Hiage::ResourceManager_impl
{

    ResourceManager_impl(Log& log) : log(log)
    {
    }

    void LoadResourceFiles(std::vector<boost::filesystem::path> files)
    {
        log << "ResourceManager" << "Loading " << files.size() << " resource files..." << endl;

        for (auto& f : files)
            LoadResourceFile(f);
    }

    void LoadResourceFile(bf::path file)
    {
        log << "ResourceManager" <<  "Loading resource XML " << file.string() << endl;

        xml_document<> doc;
        string sourceStr = ReadFileToEnd(file.string());
        char* source = doc.allocate_string(sourceStr.c_str(), sourceStr.size()+1);
        doc.parse<0>(source);
        xml_node<>* rootNode = doc.first_node();

        if (rootNode == NULL)
            throw runtime_error("Invalid XML: " + file.string() + " has no top-level element. Is it empty?");

        if (rootNode->name() == string("resources"))
            LoadResourcesXml(doc, file);
        else
            LoadObjectXml(doc, file);
    }

    void LoadResourcesXml(xml_document<>& doc, const bf::path& filePath)
    {
        bf::path directory = filePath.branch_path();

        log.Write("ResourceManager", "Loading resources from \"" + filePath.string() + "\" in directory " + directory.string());

        xml_node<>* resourcesNode = doc.first_node("resources");

        //Read resource entries
        for (xml_node<>* node = resourcesNode->first_node("resource"); node; node = node->next_sibling("resource"))
            InsertResourceFromNode(node, directory);
    }

    void InsertResourceFromNode(xml_node<>* node, bf::path& directory)
    {
        string type = GetAttributeValue(node, "type", log);
        string name = GetAttributeValue(node, "name", log);
        string file = (directory / GetAttributeValue(node, "path", log)).string();
    
        if (type == "sound")
            InsertResource(sounds, file, name, log);
        else if (type == "music")
            InsertResource(music, file, name, log);
        else if (type == "texture")
            InsertResource(textures, file, name, log);
        else
            log.Write(Warning, "ResourceManager", "Invalid resource type for resource library file: \"" + type + "\". Resource ignored.");

        log.Write("ResourceManager", "Added " + type + " resource \"" + name + "\" with path \"" + file + "\"");
    }

    void LoadObjectXml(xml_document<>& doc, const bf::path& file)
    {
        xml_node<>* root = doc.first_node();
        string id = GetAttributeValue(root, "id", log);
        string type = root->name();

        if (type == "sprite")
            InsertResource(sprites, file.string(), id, log);
        else if (type == "tileset")
            InsertResource(tilesets, file.string(), id, log);
        else if (type == "map")
            InsertResource(tilemaps, file.string(), id, log);
        else if (type == "object")
            InsertResource(objects, file.string(), id, log);
        else
            log << Warning << "ResourceManager" << "Unknown resource type: \"" << type << "\". Resource " << id << " ignored." << endl;
    }

    Log& log;

    ResourceDictionary<Texture>	         textures;
    ResourceDictionary<SpriteDescriptor> sprites;
    //std::map<std::string, Resource<ISE.FTFont> >  		fonts;
    ResourceDictionary<Tileset>          tilesets;
    ResourceDictionary<Sound>            sounds;
    ResourceDictionary<Music>            music;
    ResourceDictionary<MapDescriptor>    tilemaps;
    ResourceDictionary<ObjectDescriptor> objects;
};

ResourceManager::ResourceManager(Log& log) : log(log)
{
    impl = unique_ptr<ResourceManager_impl>(new ResourceManager_impl(log));
}


ResourceManager::~ResourceManager()
{
}

/// <summary>
/// Recursively load all resources under the given path
/// </summary>
void ResourceManager::LoadResources(string path)
{
    bf::path p(path);

    if (!bf::exists(p))
        throw FileNotFoundException(string("Directory does not exist: ") + path);

    vector<bf::path> xmlFiles = GetXmlFiles(p, log);
    impl->LoadResourceFiles(xmlFiles);
}

/// <summary>
/// Retrieve a texture. Load it if neccesary.
/// </summary>
const Texture& ResourceManager::GetTexture(string name)
{
    TextureLoader loader;
    return GetResource<Texture>(impl->textures, name, log, loader);
}

const SpriteDescriptor& ResourceManager::GetSpriteDescriptor(string name)
{
    SpriteLoader loader;
    return GetResource<SpriteDescriptor>(impl->sprites, name, log, loader);
}

const Tileset& ResourceManager::GetTileset(string name)
{
    TilesetLoader loader;
    return GetResource<Tileset>(impl->tilesets, name, log, loader);
}

const MapDescriptor& ResourceManager::GetMapDescriptor(string name)
{
    MapLoader loader;
    return GetResource<MapDescriptor>(impl->tilemaps, name, log, loader);
}

const ObjectDescriptor& ResourceManager::GetObjectDescriptor(string name)
{
    ObjectLoader loader;
    return GetResource<ObjectDescriptor>(impl->objects, name, log, loader);
}

const Sound& ResourceManager::GetAudioClip(string name)
{
    AudioClipLoader loader;
    return GetResource<Sound>(impl->sounds, name, log, loader);
}

const Music& ResourceManager::GetMusic(string name)
{
    MusicLoader loader;
    return GetResource<Music>(impl->music, name, log, loader);
}

//Private helper functions
vector<bf::path> GetXmlFiles(const bf::path& p, Log& log)
{
    log << "ResourceManager" << "Searching for resource files under " << p.string() << endl;
    vector<bf::path> files;
    bf::directory_iterator end;
    for (bf::directory_iterator iter(p); iter != end; ++iter) 
    {
        if (bf::is_regular_file(iter->path()) && bf::extension(iter->path()) == ".xml")
        {
            files.push_back(iter->path());
            log << "ResourceManager" << "Found resource XML " << iter->path().string() << endl;
        }
        else if(bf::is_directory(iter->path()))
        {
            vector<bf::path> moreFiles = GetXmlFiles(iter->path(), log);
            files.insert(files.end(), moreFiles.begin(), moreFiles.end());
        }
    }

    return files;
}

string GetAttributeValue(xml_node<>* node, string name, Log& log)
{
    log << Trace << "ResourceManager" << "GetAttributeValue " << name << " on node " << node->name() << endl;
    if (node == NULL)
        throw runtime_error("GetAttributeValue: node cannot be NULL");

    xml_attribute<>* attr = node->first_attribute(name.c_str()); 
    if (attr == 0)
        return "";

    return attr->value();
}

