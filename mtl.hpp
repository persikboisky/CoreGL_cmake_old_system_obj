//
// Created by kisly on 16.07.2025.
//

#ifndef MTL_HPP
#define MTL_HPP

#include "../util/types.hpp"

#include <string>
#include <vector>

namespace core
{
    class mtl
    {
    private:
        std::string code = "";
        std::string info = "";

        std::vector<std::string> namesResourcesGroups = {};

        std::vector<std::string> texturesPathesKd = {};
        std::vector<std::string> texturesPathesKs = {};
        std::vector<std::string> texturesPathesNs = {};
        std::vector<std::string> texturesPathesRefl = {};
        std::vector<std::string> texturesPathesReflBump = {};
        std::vector<std::string> texturesPathesd = {};

        std::vector<color::RGB> colorsKa = {};
        std::vector<color::RGB> colorsKd = {};
        std::vector<color::RGB> colorsKs = {};
        std::vector<color::RGB> colorsKe = {};

        std::vector<float> Ns = {};
        std::vector<float> Ni = {};
        std::vector<float> d = {};

        std::vector<int> illum = {};

        void getResource();
        mtl(const char* path);

    public:
        static mtl load(const char* path);
        static mtl *ptrLoad(const char* path);

        ~mtl();

        std::string getInfo();
        std::vector<std::string> getListMtlRes();

        color::RGB getColorKa(const std::string& nameResource);
        color::RGB getColorKd(const std::string& nameResource);
        color::RGB getColorKs(const std::string& nameResource);
        color::RGB getColorKe(const std::string& nameResource);

        float getCoefficientNi(const std::string& nameResource);
        float getCoefficientNs(const std::string& nameResource);
        float getCoefficientD(const std::string& nameResource);

        int getIllum(const std::string& nameResource);

        std::string getPathToKdTexture(const std::string& nameResource);
        std::string getPathToNsTexture(const std::string& nameResource);
        std::string getPathToReflTexture(const std::string& nameResource);
    };

}

#endif //MTL_HPP
