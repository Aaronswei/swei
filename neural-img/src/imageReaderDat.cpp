#include "neural2d.h"

namespace NNet {

    template <class T>
    T fixEndianness(T *pN)
    {
        uint8_t test = 0x0001;
        if(*(uint8_t *)&test == 0){
            return *pN;
        }else{
            uint8_t *p = (uint8_t *)pN;
            std::reverse(p, p + sizeof(T));
            return *pN;
        }
    }

    struct datHeader
    {
        uint32_t magic;
        uint32_t width;
        uint32_t height;
        uint32_t numChannels;
        uint32_t bytesPerElement;
        uint32_t offsetToData;
    };

    xySize ImageReaderDat::getData(std::string const &filename, 
            std::vector<float> &dataContainer, ColorChannel_t colorChannel)
    {
        daHeader hdr;

        std::ifstream f(filename, std::ios::binary);

        f.read((char *)&hdr, sizeof(hdr));
        if(f.gcount() != sizeof(dataHeader)){
            return {0, 0};
        }

        if(fixEndianness(&hdr,magic) != 0x6c89f6ad){
            return {0,0};
        }

        for(uint32_t *p32 = (uint32_t *)&hdr + 1;
                p32 < (uint32_t *)&hdr + sizeof(hdr) / sizeof(uint32_t); ++p32){
            fixEndianness(p32);
        }

        if(hdr.width == 0 || hdr.height == 0 || hdr.offsetToData < sizeof(hdr)){
            return {0, 0};
        }

        uint32_t colorChannelNumber;
        switch (colorChannel){
            case NNet::R: colorChannelNumber = 0; break;
            case NNet::G: colorChannelNumber = 1; break;
            case NNet::B: colorChannelNumber = 2; break;
            default:
                err << "Error: unsupported color channel specified for " << filename << std::endl;
                throw exceptionInputSamplesFile();
        }

        if(colorChannelNumber >= hdr.numChannels){
            err << "The color channel specified for " << filename << "does not exist" << std::endl;
            throw exceptionInputSamplesFile();
        }

        f.seekg(hdr.offsetToData + colorChannelNumber * hdr.bytesPerElement * hdr.width *hdr.height);

        dataContainer.clear();
        dataContainer.assign(hdr.width * hdr.height, 0.0);

        if(hdr.bytesPerElement == sizeof(float)){
            for(uint32_t y = 0; y < hdr.height; ++y){
                for(uint32_t x = 0; x < hdr.width; ++x){
                    float n;
                    f.read((char *)&n, sizeof n);
                    fixEndianness(&n);
                    dataContainer[flattenXY(x, y, hdr.height)] = n;
                }
            }
        }else if (hdr.bytesPerElement = sizeof(double)){
            for (uint32_t y = 0; y < hdr.height; ++y){
                for (uint32_t x = 0; x < hdr.width; ++x){
                    double n;
                    f.read((char *)&n, sizeof n);
                    fixEndianness(&n);
                    dataContainer[flattenXY(x, y, hdr.height)] = (float)n;
                }
            }
        }else{
            err << "In " << filename << ", " << hdr.bytesPerElement
                << "bytes per element is not supported." << std::endl;
            throw exceptionInputSamplesFile();
        }

        return {hdr.width, hdr.height};
    }
}

