#ifndef GENERALHELPERFUNCTIONS_H_INCLUDED
#define GENERALHELPERFUNCTIONS_H_INCLUDED

#include <vector>
#include <string>

namespace General
{
    template <typename Type>
    bool findValueInVector(const std::vector<Type> &source, const Type &num)
    {
        for(const auto &val : source)
        {
            if(val == num)
            {
                return true;
            }
        }
        return false;
    }

    void writeBesides(const std::string &file,
                      const std::vector<std::size_t> &numsOfString,
                      const std::vector<std::string> &content);

    std::string toWindows1251(const std::string &str);

}

#endif
