#include "Image.h"

#define TINY_DNG_LOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINY_DNG_WRITER_IMPLEMENTATION
#include "stb/tiny_dng_loader.h"
#include "stb/tiny_dng_writer.h"
#include "stb/stb_image_write.h"


Image::Image(const std::string& path)
{
    m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (m_Data == nullptr)
    {
        throw stbi_failure_reason();
    }
    m_DataType = 0;
}

Image::Image(int width, int height, int channels)
    :m_Width(width),m_Height(height),m_Channels(channels),m_DataType(1)
{
    m_Data = new unsigned char[m_Width * m_Height * m_Channels];
}

Image::~Image()
{
    if (m_DataType == 0)
    {
        STBI_FREE(m_Data);
    }
    else
    {
        delete[] m_Data;
    }
}

int Image::SaveAsPNG(const std::string& path)
{
    int ret = stbi_write_png(path.c_str(), m_Width, m_Height, m_Channels, m_Data, m_Width * m_Channels);
    if (ret == 1)
    {
        //success
        return 1;
    }
    else
    {
        //failed
        return 0;
    }
}


DNGImage::DNGImage(const std::string& path)
{
    std::string warn, err;
    std::vector<tinydng::DNGImage> images;
    std::vector<tinydng::FieldInfo> custom_field_lists;
    bool ret = tinydng::LoadDNG(path.c_str(), custom_field_lists, &images, &warn, &err);
    if (!warn.empty()) 
    {
        std::cout << "Warn: " << warn << std::endl;
    }
    if (!err.empty()) 
    {
        std::cerr << "Err: " << err << std::endl;
    }
    if (ret) 
    {
        tinydng::DNGImage& image = images[0];
        m_Width = image.width;
        m_Height = image.height;
        m_BitsPerPixel = image.bits_per_sample;
        m_Data = std::move(image.data);
        memcpy(m_Pattern,image.cfa_pattern,4*sizeof(int));
    }
    else {
        throw "can not load dng image";
    }
}
DNGImage::~DNGImage()
{

}
int DNGImage::ColorAt(int i,int j)
{
    return m_Pattern[i%2][j%2];
}
