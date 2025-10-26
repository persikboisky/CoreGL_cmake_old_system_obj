//
// Created by kisly on 20.07.2025.
//

#include "obj.hpp"
#include "mtl.hpp"
#include "code.hpp"
#include "image.hpp"
#include "../config.hpp"
#include "../util/console.hpp"
#include "../util/coders.hpp"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <functional>

#define CALCULATE_N_POLIGONS(N_VERTEXES) N_VERTEXES / 9

namespace core
{
    infoObject::~infoObject()
    {
        delete[] this->ptrTexture;  
    }

    void obj::read(const char *path, bool flagVulkanApi)
    {
        std::string code = code::loadStr(path);

        unsigned int sizePathToDirect = 0;
        for (unsigned int index = std::string(path).size() - 1;; index--)
        {
            if (path[index] == '/')
            {
                sizePathToDirect = index + 1;
                break;
            }
        }

        std::string pathToMtl = "";
        for (;;)
        {
            if (pathToMtl.size() == sizePathToDirect)
                break;
            pathToMtl += path[pathToMtl.size()];
        }

        std::string PathToDirect = pathToMtl;

        std::string nameMtlFile = "";
        for (unsigned int gIndex = 0; gIndex < code.size(); gIndex++)
        {
            if (code[gIndex] == '#')
            {
                for (;; gIndex++)
                {
                    if (code[gIndex] == '\n')
                    {
                        break;
                    }
                }
            }

            else if (
                code[gIndex] == 'm' &&
                code[gIndex + 1] == 't' &&
                code[gIndex + 2] == 'l' &&
                code[gIndex + 3] == 'l' &&
                code[gIndex + 4] == 'i' &&
                code[gIndex + 5] == 'b')
            {
                gIndex += 6;
                for (;; gIndex++)
                {
                    if (code[gIndex] == ' ')
                    {
                        for (;; gIndex++)
                        {
                            if (code[gIndex] != ' ')
                            {
                                break;
                            }
                        }
                    }

                    else if (code[gIndex] == '\n')
                    {
                        gIndex = code.size();
                        break;
                    }

                    nameMtlFile += code[gIndex];
                }
            }
        }

        mtl *mtlObj = nullptr;
        bool flagMtl = false;

        try
        {
            pathToMtl += nameMtlFile;
            mtlObj = mtl::ptrLoad(pathToMtl.c_str());
            flagMtl = true;
        }
        catch (...)
        {
            console::printTime();
            std::cout << "Warning: I didn't read or analysis Mtl to path: " << pathToMtl << std::endl;
            flagMtl = false;
        }

        std::map<std::string, Image*> ptrArrayTextureCode = {};
        if (flagMtl)
        {
            for (auto nameResource : mtlObj->getListMtlRes())
            {
                std::string pathToTexture = PathToDirect;
                pathToTexture += mtlObj->getPathToKdTexture(nameResource);

                if (mtlObj->getPathToKdTexture(nameResource) == "")
                    ptrArrayTextureCode[nameResource] = nullptr;
                else
                    ptrArrayTextureCode[nameResource] = core::Image::ptrLoad(pathToTexture.c_str());
            }
        }

        std::string comments = "";
        std::string selectNameObejct = "";
        std::string selectNameResource = "";

        std::vector<float> vertexes = {};
        std::vector<float> normals = {};
        std::vector<float> texutresCoord = {};

        bool flagCheckFormatF = true;
        int FormatF = 0;

        for (unsigned int index = 0; index < code.size(); index++)
        {
            if (code[index] == '#')
            {
                index += 1;

                for (;; index++)
                {
                    comments += code[index];
                    if (code[index] == '\n')
                        break;
                }
            }

            else if (
                code[index] == 'v' &&
                code[index + 1] == ' ')
            {
                index += 2;
                std::string value = "";
                for (;; index++)
                {
                    if (code[index] == ' ')
                    {
                        for (;; index++)
                        {
                            if (code[index] != ' ')
                            {
                                index -= 1;
                                if (value != "")
                                {
                                    vertexes.push_back(std::stof(value));
                                }
                                value = "";
                                break;
                            }
                        }
                    }

                    else if (code[index] == '\n')
                    {
                        vertexes.push_back(std::stof(value));
                        break;
                    }

                    value += code[index];
                }
            }

            else if (
                code[index] == 'v' &&
                code[index + 1] == 'n')
            {
                index += 2;
                std::string value = "";
                for (;; index++)
                {
                    if (code[index] == ' ')
                    {
                        for (;; index++)
                        {
                            if (code[index] != ' ')
                            {
                                index -= 1;
                                if (value != "")
                                {
                                    normals.push_back(std::stof(value));
                                }
                                value = "";
                                break;
                            }
                        }
                    }

                    else if (code[index] == '\n')
                    {
                        normals.push_back(std::stof(value));
                        break;
                    }

                    value += code[index];
                }
            }

            else if (
                code[index] == 'v' &&
                code[index + 1] == 't')
            {
                index += 2;
                std::string value = "";
                for (;; index++)
                {
                    if (code[index] == ' ')
                    {
                        for (;; index++)
                        {
                            if (code[index] != ' ')
                            {
                                index -= 1;
                                if (value != "")
                                {
                                    texutresCoord.push_back(std::stof(value));
                                }
                                value = "";
                                break;
                            }
                        }
                    }

                    else if (code[index] == '\n')
                    {
                        texutresCoord.push_back(std::stof(value));
                        break;
                    }

                    value += code[index];
                }
            }

            else if (code[index] == 'o')
            {
                index += 1;
                std::string objectName = "";
                for (;; index++)
                {
                    if (code[index] == ' ')
                        continue;
                    else if (code[index] == '\n')
                    {
                        selectNameObejct = objectName;
                        this->listNamesObjects.push_back(objectName);
                        this->objects[objectName] = {};
                        break;
                    }
                    objectName += code[index];
                }
            }

            else if (
                code[index] == 'u' &&
                code[index + 1] == 's' &&
                code[index + 2] == 'e' &&
                code[index + 3] == 'm' &&
                code[index + 4] == 't' &&
                code[index + 5] == 'l')
            {
                index += 6;
                std::string resourceName = "";
                for (;; index++)
                {
                    if (code[index] == ' ')
                        continue;
                    else if (code[index] == '\n')
                    {
                        selectNameResource = resourceName;
                        this->objects[selectNameObejct].ptrTexture = ptrArrayTextureCode[selectNameResource];
                        break;
                    }
                    resourceName += code[index];
                }
            }

            else if (
                code[index] == 'f' &&
                code[index + 1] == ' ')
            {
                if (flagCheckFormatF)
                {
                    int saveIndex = index;
                    int n = 0;
                    index += 2;
                    for (;; index++)
                    {
                        if (code[index] == '\n')
                            break;
                        else if (code[index] == '/')
                            n++;
                    }

                    if (n / 3 == 2)
                        FormatF = 3;
                    else if (n == 0)
                        FormatF = 1;
                    else
                        FormatF = 2;

                    flagCheckFormatF = false;
                    index = saveIndex;
                }

                index += 2;
                switch (FormatF)
                {
                case 1:
                {
                    std::string value = "";
                    for (;; index++)
                    {
                        if (code[index] == ' ' || code[index] == '\n')
                        {
                            if (code[index] == ' ')
                            {
                                for (;; index++)
                                {
                                    if (code[index] != ' ')
                                    {
                                        index -= 1;
                                        break;
                                    }
                                }

                                value = "";
                            }

                            int indexElementToVertex = (value != "") ? std::stoi(value) : 0;
                            if (indexElementToVertex > 0)
							{
								this->objects[selectNameObejct].vertexes.push_back(
										vertexes[(indexElementToVertex - 1) * 3]);
								if (!flagVulkanApi)
									this->objects[selectNameObejct].vertexes.push_back(
											vertexes[(indexElementToVertex - 1) * 3 + 1]);
								else
									this->objects[selectNameObejct].vertexes.push_back(
											-vertexes[(indexElementToVertex - 1) * 3 + 1]);
								this->objects[selectNameObejct].vertexes.push_back(
										vertexes[(indexElementToVertex - 1) * 3 + 2]);
							}
                            else
                            {
                                this->objects[selectNameObejct].vertexes.push_back(0);
                                this->objects[selectNameObejct].vertexes.push_back(0);
                                this->objects[selectNameObejct].vertexes.push_back(0);
                            }
                        }

                        if (code[index] == '\n')
                        {
                            value = "";
                            break;
                        }

                        value = "";
                        continue;
                    }
                    value += code[index];
                }
                break;
                case 2:
                {
                    int indexElementToF = 0;
                    std::string value = "";

                    for (;; index++)
                    {
                        if (code[index] == '/' || code[index] == ' ' || code[index] == '\n')
                        {
                            if (code[index] == ' ')
                            {
                                for (;; index++)
                                {
                                    if (code[index] != ' ')
                                    {
                                        index -= 1;
                                        break;
                                    }
                                }

                                value = "";
                            }

                            if (indexElementToF == 0 || indexElementToF == 2 || indexElementToF == 4)
                            {
                                int indexElementToVertex = (value != "") ? std::stoi(value) : 0;
                                if (indexElementToVertex > 0)
								{
									this->objects[selectNameObejct].vertexes.push_back(
											vertexes[(indexElementToVertex - 1) * 3]);
									if (!flagVulkanApi)
										this->objects[selectNameObejct].vertexes.push_back(
												vertexes[(indexElementToVertex - 1) * 3 + 1]);
									else
										this->objects[selectNameObejct].vertexes.push_back(
												-vertexes[(indexElementToVertex - 1) * 3 + 1]);
									this->objects[selectNameObejct].vertexes.push_back(
											vertexes[(indexElementToVertex - 1) * 3 + 2]);
								}
                                else
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                }
                            }
                            else if (indexElementToF == 1 || indexElementToF == 3 || indexElementToF == 5)
                            {
                                int indexElementToTextureCoord = (value != "") ? std::stoi(value) : 0;
                                if (indexElementToTextureCoord > 0)
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(texutresCoord[(indexElementToTextureCoord - 1) * 2]);
                                    this->objects[selectNameObejct].vertexes.push_back(texutresCoord[(indexElementToTextureCoord - 1) * 2 + 1]);
                                }
                                else
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                }
                            }

                            if (code[index] == '\n')
                            {
                                value = "";
                                indexElementToF++;
                                break;
                            }

                            indexElementToF++;
                            value = "";
                            continue;
                        }

                        value += code[index];
                    }
                }
                break;
                case 3:
                    int indexElementToF = 0;
                    std::string value = "";

                    for (;; index++)
                    {
                        if (code[index] == '/' || code[index] == ' ' || code[index] == '\n')
                        {
                            if (code[index] == ' ')
                            {
                                for (;; index++)
                                {
                                    if (code[index] != ' ')
                                    {
                                        index -= 1;
                                        break;
                                    }
                                }

                                value = "";
                            }

                            if (indexElementToF == 0 || indexElementToF == 3 || indexElementToF == 6)
                            {
                                int indexElementToVertex = (value != "") ? std::stoi(value) : 0;
                                if (indexElementToVertex > 0)
								{
									this->objects[selectNameObejct].vertexes.push_back(
											vertexes[(indexElementToVertex - 1) * 3]);
									if (!flagVulkanApi)
										this->objects[selectNameObejct].vertexes.push_back(
												vertexes[(indexElementToVertex - 1) * 3 + 1]);
									else
										this->objects[selectNameObejct].vertexes.push_back(
												-vertexes[(indexElementToVertex - 1) * 3 + 1]);
									this->objects[selectNameObejct].vertexes.push_back(
											vertexes[(indexElementToVertex - 1) * 3 + 2]);
								}
                                else
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                }
                            }
                            else if (indexElementToF == 1 || indexElementToF == 4 || indexElementToF == 7)
                            {
                                int indexElementToTextureCoord = (value != "") ? std::stoi(value) : 0;
                                if (indexElementToTextureCoord > 0)
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(texutresCoord[(indexElementToTextureCoord - 1) * 2]);
                                    this->objects[selectNameObejct].vertexes.push_back(texutresCoord[(indexElementToTextureCoord - 1) * 2 + 1]);
                                }
                                else
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                }
                            }
                            else if (indexElementToF == 2 || indexElementToF == 5 || indexElementToF == 8)
                            {
                                int indexElementToNormals = (value != "") ? std::stoi(value) : 0;
                                if (indexElementToNormals > 0)
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(normals[(indexElementToNormals - 1) * 3]);
                                    this->objects[selectNameObejct].vertexes.push_back(normals[(indexElementToNormals - 1) * 3 + 1]);
                                    this->objects[selectNameObejct].vertexes.push_back(normals[(indexElementToNormals - 1) * 3 + 2]);
                                }
                                else
                                {
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                    this->objects[selectNameObejct].vertexes.push_back(0);
                                }
                            }

                            if (code[index] == '\n')
                            {
                                value = "";
                                indexElementToF++;
                                break;
                            }

                            indexElementToF++;
                            value = "";
                            continue;
                        }

                        value += code[index];
                    }
                    break;
                }
            }
        }

        if (CORE_INFO)
        {
            console::printTime();
            std::cout << "Ok: load object to path: " << path << std::endl;
            console::printTime();
            std::cout << "Objects numbers: " << this->objects.size() << std::endl;
            console::printTime();
            std::cout << "Poligons: " << CALCULATE_N_POLIGONS(vertexes.size()) << std::endl;
        }

        if (mtlObj != nullptr)
        {
            delete mtlObj;
        }

        vertexes.clear();
        normals.clear();
        texutresCoord.clear();
        ptrArrayTextureCode.clear();
    }

    obj::obj(const char *path, bool flagVulkanApi)
    {
        this->read(path, flagVulkanApi);
    }

    obj obj::load(const char *path, bool flagVulkanApi)
    {
        return obj(path, flagVulkanApi);
    }

    obj *obj::ptrLoad(const char *path, bool flagVulkanApi)
    {
        return new obj(path, flagVulkanApi);
    }

    std::vector<std::string> obj::getListNamesObjects()
    {
        return this->listNamesObjects;
    }

    std::map<std::string, infoObject> obj::getObjects()
    {
        return this->objects;
    }

} // core