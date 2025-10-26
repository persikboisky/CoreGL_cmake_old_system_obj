//
// Created by kisly on 20.07.2025.
//

#ifndef OBJ_HPP
#define OBJ_HPP

#include <string>
#include <vector>
#include <map>

namespace core
{
    class Image;

    struct infoObject 
    {
        float smoothingPoligons = 0;
        Image* ptrTexture = nullptr;
        std::vector<float> vertexes = {};

        ~infoObject();
    };

    class obj
    {
    private:
        std::string comments = "";
        std::map<std::string, infoObject> objects = {};
        std::vector<std::string> listNamesObjects = {};
        
        void read(const char *path, bool flagVulkanApi);

        obj(const char *path, bool flagVulkanApi);

    public:
        static obj load(const char *path, bool flagVulkanApi = false);
        static obj *ptrLoad(const char *path, bool flagVulkanApi = false);

        std::vector<std::string> getListNamesObjects();
        std::map<std::string, infoObject> getObjects();
    };
}

#endif //OBJ_HPP
