#include <string>
#include <map>

#include <boost/filesystem.hpp>
#include <libxml/parser.h>
#include <libxml/xpath.h>

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

xmlDocPtr ParseXmlFile(const bf::path& p);
vector<bf::path> GetXmlFiles(const bf::path& p);
xmlNodePtr SelectSingleNode(xmlXPathContextPtr context, string xpath);
xmlNodeSetPtr SelectNodes(xmlXPathContextPtr context, string xpath);
string GetAttributeValue(xmlNodePtr node, string name);

template<class T>
Resource<T> CreateResource(string filename, string name)
{
    return Resource<T>(filename, name);
}

template<class V> 
bool InsertResource(ResourceDictionary<V> dict, string name, string filename)
{
    if (dict.find(name) != dict.end())
        return false;

        //log.Write(Warning, "ResourceManager", "Resource with name \"" + name + "\" already added. Ignored.");
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
        resource.Load(resourceLoader);

    return resource.getResource();
}

ResourceManager::ResourceManager(Log& log) : log(log)
{
    //ctor
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

    vector<bf::path> xmlFiles = GetXmlFiles(p);
    LoadResourceFiles(xmlFiles);
}

void ResourceManager::LoadResourceFiles(vector<bf::path> files)
{
    xmlInitParser();

    for (auto& f : files)
        LoadResourceFile(f);

    xmlCleanupParser();
}

void ResourceManager::LoadResourceFile(bf::path file)
{
    xmlDocPtr document = ParseXmlFile(file);
    xmlXPathContextPtr context = xmlXPathNewContext(document);
    
    xmlNodePtr rootNode = SelectSingleNode(context, "/*");
    if (rootNode == NULL)
        throw runtime_error("Invalid XML: " + file.string() + " has no top-level element. Is it empty?");

    if (string("resources") == (char*)rootNode->name)
        LoadResourcesXml(document, file);
    else
        LoadObjectXml(document, file);
}

void ResourceManager::LoadObjectXml(xmlDocPtr doc, const bf::path& file)
{
    xmlNodePtr root = SelectSingleNode(xmlXPathNewContext(doc), "/*");
    string id = GetAttributeValue(root, "id");
    string name = GetAttributeValue(root, "name");

    if (id == "sprite")
        InsertResource(sprites, file.string(), name);
    else if (id == "tileset")
        InsertResource(tilesets, file.string(), name);
    else if (id == "map")
        InsertResource(tilemaps, file.string(), name);
    else if (id == "object")
        InsertResource(objects, file.string(), name);
    else
        log.Write(Warning, "ResourceManager", "Unknown resource type: \"" + name + "\". Resource ignored.");
}

/// <summary>
/// Load resources from a resource XML file.
/// </summary>
void ResourceManager::LoadResourcesXml(xmlDocPtr doc, const bf::path& filePath)
{
    bf::path directory = filePath.branch_path();

    log.Write("ResourceManager", "Loading resources from \"" + filePath.string() + "\" in directory " + directory.string());

    xmlNodeSetPtr nodes = SelectNodes(xmlXPathNewContext(doc), "/resources/resource");

    //Read resource entries
    for (int i = 0; i < nodes->nodeNr; i++)
    {
        xmlNodePtr node = nodes->nodeTab[i];

        string type = GetAttributeValue(node, "type");
        string name = GetAttributeValue(node, "name");
        string file = (directory / GetAttributeValue(node, "path")).string();

        if (type == "sound")
            InsertResource(sounds, file, name);
        else if (type == "music")
            InsertResource(music, file, name);
        else if (type == "texture")
            InsertResource(textures, file, name);

//        else if (type == "font")
//        {
//            if (fonts.find(name) != fonts.end())
//                log.Write(Warning, "ResourceManager", "Font with name \"" + name + "\" already added. Resource ignored.");
//            else
//                fonts[name] = CreateResource<ISE.FTFont>(file, name);
//        }
        else
            log.Write(Warning, "ResourceManager", "Invalid resource type for resource library file: \"" + type + "\". Resource ignored.");

        log.Write("ResourceManager", "Added " + type + " resource with path \"" + file + "\"");
    }
}

/// <summary>
/// Retrieve a texture. Load it if neccesary.
/// </summary>
const Texture& ResourceManager::GetTexture(string name)
{
    TextureLoader loader;
    return GetResource<Texture>(textures, name, log, loader);
}

const SpriteDescriptor& ResourceManager::GetSpriteDescriptor(string name)
{
    SpriteLoader loader;
    return GetResource<SpriteDescriptor>(sprites, name, log, loader);
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
    return GetResource<Tileset>(tilesets, name, log, loader);
}

const MapDescriptor& ResourceManager::GetMapDescriptor(string name)
{
    MapLoader loader;
    return GetResource<MapDescriptor>(tilemaps, name, log, loader);
}

const ObjectDescriptor& ResourceManager::GetObjectDescriptor(string name)
{
    ObjectLoader loader;
    return GetResource<ObjectDescriptor>(objects, name, log, loader);
}

const Sound& ResourceManager::GetAudioClip(string name)
{
    AudioClipLoader loader;
    return GetResource<Sound>(sounds, name, log, loader);
}

const Music& ResourceManager::GetMusic(string name)
{
    MusicLoader loader;
    return GetResource<Music>(music, name, log, loader);
}

////The list of all tileset names
//IEnumerable<string> Tilesets
//{
//    get
//    {
//        return tilesets.Keys;
//    }
//}
//
////List of all object names
//IEnumerable<string> Objects
//{
//    get
//    {
//        return objects.Keys;
//    }
//}
//
//IEnumerable<string> Textures
//{
//    get
//    {
//        return textures.Keys;
//    }
//}

//Private helper functions
vector<bf::path> GetXmlFiles(const bf::path& p)
{
    vector<bf::path> files;
    bf::directory_iterator end;
    for (bf::directory_iterator iter(p); iter != end; ++iter) 
    {
        if (bf::is_regular_file(iter->path()) && bf::extension(iter->path()) == "xml")
            files.push_back(iter->path());
        else if(bf::is_directory(iter->path()))
        {
            vector<bf::path> moreFiles = GetXmlFiles(iter->path());
            files.insert(files.end(), moreFiles.begin(), moreFiles.end());
        }
    }

    return files;
}

xmlDocPtr ParseXmlFile(const bf::path& p)
{
    xmlDocPtr doc = xmlParseFile(p.string().c_str());

    if (doc == NULL)
        throw runtime_error(string("Failed to parse XML document ") + p.string());

    return doc;
}

xmlNodePtr SelectSingleNode(xmlXPathContextPtr context, string xpath)
{
    if (context == NULL)
        return NULL;

    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), context);

    if (result == NULL)
        throw runtime_error("Error in xpath expression " + xpath);

    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
        return NULL;

    return result->nodesetval->nodeTab[0];
}

xmlNodeSetPtr SelectNodes(xmlXPathContextPtr context, string xpath)
{
    if (context == NULL)
        return NULL;

    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), context);

    if (result == NULL)
        throw runtime_error("Error in xpath expression " + xpath);

    return result->nodesetval;

}

string GetInnerText(xmlDocPtr doc, xmlNodePtr node)
{
    xmlChar* str = xmlNodeListGetString(doc, node, 1);
    string result((char*)str);
    xmlFree(str);

    return result;
}

string GetAttributeValue(xmlNodePtr node, string name)
{
    if (node == NULL)
        throw runtime_error("GetAttributeValue: node cannot be NULL");

    xmlChar* str = xmlGetProp(node, (const xmlChar*)name.c_str());
    string result((char*)str);
    xmlFree(str);

    return result;
}


