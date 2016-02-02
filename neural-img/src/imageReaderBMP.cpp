#include "neural2d.h"

namespace NNet {

    xySize ImageReaderBMP::getData(std::string const &filename, std::vector<float> &dataContainer, ColorChannel_t colorChannel)
    {
        FILE* f = fopen(filename.c_str(), "rb");

        if(f == NULL){
            return {0,0};
        }

        unsigned char info[54];
        if(fread(info, sizeof(unsigned char), 54, f) != 54){
            fclose(f);
            return {0, 0};
        }

        if(info[0] != 'B' || info[1] != 'M'){
            fclose(f);
            return {0, 0};
        }


        size_t dataOffset = (info[13] << 24)
                        +   (info[12] << 16)
                        +   (info[11] << 8)
                        +    info[10];

        int pixelDepth = (info[29] << 8) + info[28];
        if(pixelDepth != 24) {
            fclose(f);
            return {0,0};
        }

        uint32_t width = (info[21] << 24)
                    +    (info[20] << 16)
                    +    (info[19] << 8)
                    +     info[18];

        uint32_t height = (info[25] << 24)
                    +     (info[24] << 16)
                    +     (info[23] << 8)
                    +      info[22];
        if (fseek(f, dataOffset, SEEK_SET) != 0){
            fclose(f);
            return {0, 0};
        }

        uint32_t rowLen_padded = (width*3 + 3) & ( ~3);
        std::unique_ptr<unsigned char[]>imageData {new unsigned char[rowLen_padded]};

        dataContainer.clear();
        dataContainer.assign(width * height, 0);

        for(uint32_t y = 0; y < height; ++y){
            if(fread(imageData.get(), sizeof(unsigned char), rowLen_padded, f) != rowLen_padded)
            {
                fclose(f);
                return {0, 0};
            }

            unsigned val = 0;

            for (uint32_t x = 0; x < width; ++x){
                if(colorChannel == NNet::R){
                    val = imageData[x*3+2];  //Red
                }else if (colorChannel == NNet::G){
                    val = imageData[x*3+1];
                }else if (colorChannel == NNet::B){
                    val = imageData[x*3+0];
                }else if (colorChannel == NNet::BW){
                    val = (unsigned)(0.3 * imageData[x*3 + 2] +
                                     0.6 * imageData[x*3 + 1] +
                                     0.1 * imageData[x*3 + 0]);

                }else{
                    err << "Error: unknown pixel conversion" << endl;
                    throw exceptionImageFile();
                }

                dataContainer[flattenXY(x, (height - y) - 1, height)] = pixelToNetworkInputRange(val);
            }
        }
        fclose(f);

        return {width, height};
    }
}


