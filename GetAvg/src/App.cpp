#include "Image.h"
#include <iostream>
#include <string>
#define ASSERT(x) if(!(x)){\
std::cout<<"assert failed: "<<#x<<" in "<<__FILE__<<":"<<__LINE__<<std::endl;\
exit(-1);\
}
void Usage()
{
    std::cout<<"dng2png <dng_path> <r1> <c1> <r2> <c2>"<<std::endl;;
}

int main(int argc,char** argv)
{
    if(argc!=6)
    {
        Usage();
        exit(-1);
    }
    std::string dng_path=argv[1];
    DNGImage dng(dng_path);
    ASSERT(dng.GetBitsPerPixel()==16);
    int r1=atoi(argv[2]);
    int c1=atoi(argv[3]);
    int r2=atoi(argv[4]);
    int c2=atoi(argv[5]);
    unsigned int color[3]={0};
    int cnt[3]={0};
    int max=INT32_MIN;
    for(int row=r1;row<=r2;row++)
    {
        for(int col=c1;col<=c2;col++)
        {
            unsigned short light=*(unsigned short*)dng.At(row,col);
            if(max<light)max=light;
            color[dng.ColorAt(row,col)]+=light;
            cnt[dng.ColorAt(row,col)]++;
        }
    }
    std::cout<<"r: "<<color[0]/cnt[0]<<" "<<"g: "<<color[1]/cnt[1]<<" "<<"b: "<<color[2]/cnt[2]<<std::endl;
    std::cout<<max<<std::endl;
}