//
// Created by kisly on 16.07.2025.
//

#include "mtl.hpp"
#include "../util/types.hpp"
#include "../util/vector.hpp"
#include "code.hpp"
#include <string>
#include <vector>
#include <iostream>

static void loadColor(
    unsigned int &gElementID,
    std::string &code,
    std::vector<core::color::RGB> &colorVec,
    int &indexResource)
{
    for (;; gElementID++)
    {
        if (code[gElementID] == '\n')
        {
            break;
        }

        if (code[gElementID] != ' ')
        {
            std::string colorSTR = "";
            for (;; gElementID++)
            {
                if (code[gElementID] == '\n')
                {
                    gElementID -= 1;
                    colorVec[indexResource] = core::color::stringToRGB(colorSTR);
                    break;
                }
                colorSTR += code[gElementID];
            }
        }
    }
}

static void loadCoefficient(
    unsigned int &gElementID,
    std::string &code,
    std::vector<float> &Coefficient,
    int &indexResource)
{
    for (;; gElementID++)
    {
        if (code[gElementID] == '\n')
        {
            break;
        }

        if (code[gElementID] != ' ')
        {
            std::string colorSTR = "";
            for (;; gElementID++)
            {
                if (code[gElementID] == '\n')
                {
                    gElementID -= 1;
                    Coefficient[indexResource] = float(std::stod(colorSTR));
                    break;
                }
                colorSTR += code[gElementID];
            }
        }
    }
}

static void loadNumber(
    unsigned int &gElementID,
    std::string &code,
    std::vector<int> &numberVec,
    int &indexResource)
{
    for (;; gElementID++)
    {
        if (code[gElementID] == '\n')
        {
            break;
        }

        if (code[gElementID] != ' ')
        {
            std::string colorSTR = "";
            for (;; gElementID++)
            {
                if (code[gElementID] == '\n')
                {
                    gElementID -= 1;
                    numberVec[indexResource] = std::stoi(colorSTR);
                    break;
                }
                colorSTR += code[gElementID];
            }
        }
    }
}

static void loadPath(
    unsigned int &gElementID,
    std::string &code,
    std::vector<std::string> &numberVec,
    int &indexResource)
{
    for (;; gElementID++)
    {
        if (code[gElementID] == '\n')
        {
            break;
        }

        if (code[gElementID] != ' ')
        {
            std::string colorSTR = "";
            for (;; gElementID++)
            {
                if (code[gElementID] == '\n')
                {
                    gElementID -= 1;
                    numberVec[indexResource] = colorSTR;
                    break;
                }
                colorSTR += code[gElementID];
            }
        }
    }
}

void core::mtl::getResource()
{
    int indexResource = 0;
    for (unsigned int gElementID = 0; gElementID < this->code.size(); gElementID++)
    {
        // комментарий
        if (this->code[gElementID] == '#')
        {
            std::string comment = "";
            for (unsigned int elementID = gElementID + 1;; elementID++)
            {
                if (this->code[elementID] == '\n')
                {
                    comment += '\n';
                    gElementID = elementID;
                    break;
                }
                comment += this->code[elementID];
            }
            info += comment;
        }

        // считывание групп ресурсов
        else if (
            this->code[gElementID] == 'n' &&
            this->code[gElementID + 1] == 'e' &&
            this->code[gElementID + 2] == 'w' &&
            this->code[gElementID + 3] == 'm' &&
            this->code[gElementID + 4] == 't' &&
            this->code[gElementID + 5] == 'l')
        {
            std::string nameMtlRes = "";
            for (unsigned int elementID = gElementID + 6;; elementID++)
            {
                if (this->code[elementID] == ' ')
                    continue;

                if (this->code[elementID] == '\n')
                {
                    this->namesResourcesGroups.push_back(nameMtlRes);
                    this->texturesPathesKd.push_back("");
                    this->texturesPathesKs.push_back("");
                    this->texturesPathesNs.push_back("");
                    this->texturesPathesRefl.push_back("");
                    this->texturesPathesReflBump.push_back("");
                    this->texturesPathesd.push_back("");
                    this->colorsKa.push_back(color::RGB(0, 0, 0));
                    this->colorsKd.push_back(color::RGB(0, 0, 0));
                    this->colorsKs.push_back(color::RGB(0, 0, 0));
                    this->colorsKe.push_back(color::RGB(0, 0, 0));
                    this->Ns.push_back(0);
                    this->Ni.push_back(0);
                    this->d.push_back(0);
                    this->illum.push_back(0);
                    indexResource = this->namesResourcesGroups.size() - 1;
                    gElementID = elementID - 1;
                    break;
                }

                nameMtlRes += this->code[elementID];
            }
        }

        else if (
            this->code[gElementID] == 'i' &&
            this->code[gElementID + 1] == 'l' &&
            this->code[gElementID + 2] == 'l' &&
            this->code[gElementID + 3] == 'u' &&
            this->code[gElementID + 4] == 'm')
        {
            gElementID += 5;
            loadNumber(gElementID, this->code, this->illum, indexResource);
        }

        else if (
            this->code[gElementID] == 'm' &&
            this->code[gElementID + 1] == 'a' &&
            this->code[gElementID + 2] == 'p' &&
            this->code[gElementID + 3] == '_' &&
            this->code[gElementID + 4] == 'K' &&
            this->code[gElementID + 5] == 'd')
        {
            gElementID += 6;
            loadPath(gElementID, this->code, this->texturesPathesKd, indexResource);
        }

        else if (
            this->code[gElementID] == 'm' &&
            this->code[gElementID + 1] == 'a' &&
            this->code[gElementID + 2] == 'p' &&
            this->code[gElementID + 3] == '_' &&
            this->code[gElementID + 4] == 'K' &&
            this->code[gElementID + 5] == 's')
        {
            gElementID += 6;
            loadPath(gElementID, this->code, this->texturesPathesKs, indexResource);
        }

        else if (
            this->code[gElementID] == 'm' &&
            this->code[gElementID + 1] == 'a' &&
            this->code[gElementID + 2] == 'p' &&
            this->code[gElementID + 3] == '_' &&
            this->code[gElementID + 4] == 'N' &&
            this->code[gElementID + 5] == 's')
        {
            gElementID += 6;
            loadPath(gElementID, this->code, this->texturesPathesNs, indexResource);
        }

        else if (
            this->code[gElementID] == 'm' &&
            this->code[gElementID + 1] == 'a' &&
            this->code[gElementID + 2] == 'p' &&
            this->code[gElementID + 3] == '_' &&
            this->code[gElementID + 4] == 'r' &&
            this->code[gElementID + 5] == 'e' &&
            this->code[gElementID + 6] == 'f' &&
            this->code[gElementID + 7] == 'l')
        {
            gElementID += 8;
            loadPath(gElementID, this->code, this->texturesPathesRefl, indexResource);
        }

        else if (
            this->code[gElementID] == 'm' &&
            this->code[gElementID + 1] == 'a' &&
            this->code[gElementID + 2] == 'p' &&
            this->code[gElementID + 3] == '_' &&
            this->code[gElementID + 4] == 'd')
        {
            gElementID += 5;
            loadPath(gElementID, this->code, this->texturesPathesd, indexResource);
        }

        // считываем цвет окружения (Ka)
        else if (
            this->code[gElementID] == 'K' &&
            this->code[gElementID + 1] == 'a')
        {
            gElementID += 2;
            loadColor(gElementID, this->code, this->colorsKa, indexResource);
        }

        // считываем диффузный цвет (Kd)
        else if (
            this->code[gElementID] == 'K' &&
            this->code[gElementID + 1] == 'd')
        {
            gElementID += 2;
            loadColor(gElementID, this->code, this->colorsKd, indexResource);
        }

        // считываем цвет блистания (Ks)
        else if (
            this->code[gElementID] == 'K' &&
            this->code[gElementID + 1] == 's')
        {
            gElementID += 2;
            loadColor(gElementID, this->code, this->colorsKs, indexResource);
        }

        // считываем эмиссионный цвет (Ke)
        else if (
            this->code[gElementID] == 'K' &&
            this->code[gElementID + 1] == 'e')
        {
            gElementID += 2;
            loadColor(gElementID, this->code, this->colorsKe, indexResource);
        }

        // считываем считываем какой-то Ni
        else if (
            this->code[gElementID] == 'N' &&
            this->code[gElementID + 1] == 'i')
        {
            gElementID += 2;
            loadCoefficient(gElementID, this->code, this->Ni, indexResource);
        }

        // считываем фокус бликов материала Ns
        else if (
            this->code[gElementID] == 'N' &&
            this->code[gElementID + 1] == 's')
        {
            gElementID += 2;
            loadCoefficient(gElementID, this->code, this->Ns, indexResource);
        }

        // считываем какой-то d
        else if (
            this->code[gElementID] == 'd' &&
            this->code[gElementID + 1] == ' ')
        {
            gElementID += 2;
            loadCoefficient(gElementID, this->code, this->d, indexResource);
        }
    }
}

core::mtl::mtl(const char *path)
{
    this->code = code::loadStr(path);
    this->getResource();
}

core::mtl core::mtl::load(const char *path)
{
    return core::mtl(path);
}

core::mtl *core::mtl::ptrLoad(const char *path)
{
    return new core::mtl(path);
}

core::mtl::~mtl()
{
    code.clear();
    namesResourcesGroups.clear();
}

std::string core::mtl::getInfo()
{
    return this->info;
}

std::vector<std::string> core::mtl::getListMtlRes()
{
    return this->namesResourcesGroups;
}

core::color::RGB core::mtl::getColorKa(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->colorsKa[index];
}

core::color::RGB core::mtl::getColorKd(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->colorsKd[index];
}

core::color::RGB core::mtl::getColorKs(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->colorsKs[index];
}

core::color::RGB core::mtl::getColorKe(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->colorsKe[index];
}

float core::mtl::getCoefficientNi(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->Ni[index];
}

float core::mtl::getCoefficientNs(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->Ns[index];
}

float core::mtl::getCoefficientD(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->d[index];
}

int core::mtl::getIllum(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->illum[index];
}

std::string core::mtl::getPathToKdTexture(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->texturesPathesKd[index];
}

std::string core::mtl::getPathToNsTexture(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->texturesPathesNs[index];
}

std::string core::mtl::getPathToReflTexture(const std::string &nameResource)
{
    int index = vector::searchIndexFromValue(this->namesResourcesGroups, nameResource);
    return this->texturesPathesRefl[index];
}
