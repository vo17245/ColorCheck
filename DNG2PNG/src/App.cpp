#include "Image.h"
#include <iostream>
#include <string>
void Usage()
{
    std::cout<<"dng2png <dng_path> <png_path>"<<std::endl;;
}

int main(int argc,char** argv)
{
    if(argc!=3)
    {
        Usage();
        exit(-1);
    }
    std::string dng_path=argv[1];
    std::string png_path=argv[2];
    DNGImage dng(dng_path);
    unsigned short max=0;
    unsigned short min=UINT16_MAX;
    for(size_t row=0;row<dng.GetHeight();row++)
    {
        for(size_t col=0;col<dng.GetWidth();col++)
        {
            unsigned short light=*(unsigned short*)dng.At(row,col);
            if(max<light)
            {
                max=light;
            }
            else if (min>light)
            {
                min=light;
            }
            
        }
    }
    Image img(dng.GetWidth(),dng.GetHeight(),3);
    for(size_t row=0;row<img.GetHeight();row++)
    {
        for(size_t col=0;col<img.GetWidth();col++)
        {
            unsigned short light=*(unsigned short*)dng.At(row,col);
            float a=(float)light;
            a-=min;
            a=a/(max-min);
            a=a*255;
            img.At(row,col)[dng.ColorAt(row,col)]=(unsigned short)a;
        }
    }
    img.SaveAsPNG(png_path);
}