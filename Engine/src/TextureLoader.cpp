#include "TextureLoader.h"
using namespace Hiage;
using namespace std;

Texture TextureLoader::LoadResource(string filename, string name) const
{
    return Texture();
}

///// Creates an OpenGL texture from an image.
//Texture LoadResource(string filename, string name)
//{
//    int imageId;
//    Texture tex = null;
//
//    //Initialize DevIL
//    Il.ilInit();
//
//    Il.ilGenImages(1, out imageId);
//    Il.ilBindImage(imageId);
//
//    if (Il.ilLoadImage(filename))
//    {
//        //Successfully loaded the image. Generate the OpenGL texture
//        try
//        {
//            tex = Renderer.GenerateTexture(Il.ilGetData(), Il.ilGetInteger(Il.IL_IMAGE_WIDTH), Il.ilGetInteger(Il.IL_IMAGE_HEIGHT), Il.ilGetInteger(Il.IL_IMAGE_BPP));
//        }
//        catch (Exception e)
//        {
//            Log.Write("Unable to generate GL texture " + filename + ": " + e.Message, Log.ERROR);
//            throw e;
//        }               
//        Log.Write("Loaded texture " + filename + ". Got texture id " + tex.TextureID);
//    }
//    else
//    {
//        //Failure. Clean up loaded image.
//        Il.ilDeleteImages(1, ref imageId);
//        throw new IOException("Unable to load texture \"" + filename + "\".");
//    }
//    Il.ilDeleteImages(1, ref imageId);
//
//    return tex;
//}
//}
//
