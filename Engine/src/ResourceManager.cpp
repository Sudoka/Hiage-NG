#include <string>
#include <map>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
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

namespace bf = boost::filesystem;
using namespace std;
using namespace Hiage;
using boost::format;

xmlDocPtr ParseXmlFile(const bf::path& p, Log& log);
vector<bf::path> GetXmlFiles(const bf::path& p, Log& log);
xmlNodePtr SelectSingleNode(xmlXPathContextPtr context, string xpath, Log& log);
xmlNodeSetPtr SelectNodes(xmlXPathContextPtr context, string xpath, Log& log);
string GetAttributeValue(xmlNodePtr node, string name, Log& log);

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
        xmlInitParser();

        for (auto& f : files)
            LoadResourceFile(f);

        xmlCleanupParser();
    }

    void LoadResourceFile(bf::path file)
    {
        xmlDocPtr document = ParseXmlFile(file, log);
        xmlXPathContextPtr context = xmlXPathNewContext(document);
        xmlNodePtr rootNode = SelectSingleNode(context, "/*", log);
        if (rootNode == NULL)
            throw runtime_error("Invalid XML: " + file.string() + " has no top-level element. Is it empty?");

        if (string("resources") == (char*)rootNode->name)
            LoadResourcesXml(document, file);
        else
            LoadObjectXml(document, file);
    }

    void LoadResourcesXml(xmlDocPtr doc, const bf::path& filePath)
    {
        bf::path directory = filePath.branch_path();

        log.Write("ResourceManager", "Loading resources from \"" + filePath.string() + "\" in directory " + directory.string());

        xmlNodeSetPtr nodes = SelectNodes(xmlXPathNewContext(doc), "/resources/resource", log);

        //Read resource entries
        for (int i = 0; i < nodes->nodeNr; i++)
        {
            xmlNodePtr node = nodes->nodeTab[i];

            string type = GetAttributeValue(node, "type", log);
            string name = GetAttributeValue(node, "name", log);
            string file = (directory / GetAttributeValue(node, "path", log)).string();

            if (type == "sound")
                InsertResource(sounds, file, name, log);
            else if (type == "music")
                InsertResource(music, file, name, log);
            else if (type == "texture")
                InsertResource(textures, file, name, log);

//            else if (type == "font")
//                InsertResource(fonts, file, name, log);
            else
                log.Write(Warning, "ResourceManager", "Invalid resource type for resource library file: \"" + type + "\". Resource ignored.");

            log.Write("ResourceManager", "Added " + type + " resource \"" + name + "\" with path \"" + file + "\"");
        }
    }

    void LoadObjectXml(xmlDocPtr doc, const bf::path& file)
    {
        xmlNodePtr root = SelectSingleNode(xmlXPathNewContext(doc), "/*", log);
        string id = GetAttributeValue(root, "id", log);

        if (id == "sprite")
            InsertResource(sprites, file.string(), id, log);
        else if (id == "tileset")
            InsertResource(tilesets, file.string(), id, log);
        else if (id == "map")
            InsertResource(tilemaps, file.string(), id, log);
        else if (id == "object")
            InsertResource(objects, file.string(), id, log);
        else
            log << Warning << "ResourceManager" << "Unknown resource type: \"" << (const char*)root->name << "\". Resource ignored.";
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

//const ISE.FTFont& GetFont(string name)
//{
//    if (fonts.ContainsKey(name))
//    {
//        if (!fonts[name].IsLoaded)
//        {
//            fonts[name].Load(new FontLoader());
//        }
//        return fonts[name].Content;
//    }
//    throw new KeyNotFoundException("Font with name " + name + " does not exist.");
//}

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

xmlDocPtr ParseXmlFile(const bf::path& p, Log& log)
{
    string s = p.string();
    log << Debug << "ResourceManager" << "Parsing XML " << s << endl;
    xmlDocPtr doc = xmlParseFile(s.c_str());
    if (doc == NULL)
        throw runtime_error(string("Failed to parse XML document ") + p.string());

    log << Debug << "ResourceManager" << "Parsing completed with non-null result" << endl;

    return doc;
}

xmlNodePtr SelectSingleNode(xmlXPathContextPtr context, string xpath, Log& log)
{
    log << Trace << "ResourceManager" << "SelectSingleNode on " << xpath << endl;
    if (context == NULL)
        return NULL;

    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), context);

    if (result == NULL)
        throw runtime_error("Error in xpath expression " + xpath);

    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
        return NULL;

    return result->nodesetval->nodeTab[0];
}

xmlNodeSetPtr SelectNodes(xmlXPathContextPtr context, string xpath, Log& log)
{
    log << Trace << "ResourceManager" << "SelectNodes on " << xpath << endl;
    if (context == NULL)
        return NULL;

    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), context);

    if (result == NULL)
        throw runtime_error("Error in xpath expression " + xpath);

    return result->nodesetval;

}

string GetInnerText(xmlDocPtr doc, xmlNodePtr node, Log& log)
{
    log << Trace << "ResourceManager" << "GetInnerText on node " << (const char*)node->name << endl;
    xmlChar* str = xmlNodeListGetString(doc, node, 1);
    string result((char*)str);
    xmlFree(str);

    return result;
}

string GetAttributeValue(xmlNodePtr node, string name, Log& log)
{
    log << Trace << "ResourceManager" << "GetAttributeValue " << name << " on node " << (const char*)node->name << endl;
    if (node == NULL)
        throw runtime_error("GetAttributeValue: node cannot be NULL");

    xmlChar* str = xmlGetProp(node, (const xmlChar*)name.c_str());
    string result((char*)str);
    xmlFree(str);

    return result;
}


