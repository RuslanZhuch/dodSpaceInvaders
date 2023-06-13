#include "EnemiesContext.h"

#include <dod/BufferUtils.h>

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>

Game::Context::Enemy::Data Game::Context::Enemy::Data::load(Dod::MemPool& pool, int32_t& header) noexcept
{

    Data data;

    std::ifstream contextFile("resources/contexts/enemiesContext.json");
    assert(contextFile.is_open());
    const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

    rapidjson::Document json;
    json.Parse(fileRawData.c_str());
    const auto root{ json.GetObject() };

    for (const auto& element : root)
    {
        if (element.name == "contextName")
        {
            const auto contextName{ element.value.GetString() };
            std::cout << "Loading context: " << contextName << "\n";
        }
        else if (element.name == "data")
        {
            for (const auto& dataElement : element.value.GetObject())
            {
                if (dataElement.name == "enemiesParameters" && dataElement.value.IsObject())
                {
                    const auto enemiesParametersObj{ dataElement.value.GetObject() };
                    const auto numOfEnemiesPerRow{ enemiesParametersObj["numOfEnemiesPerRow"].GetInt() };
                    data.numOfEnemiesPerRow = numOfEnemiesPerRow;
                    const auto numOfEnemiesPerCol{ enemiesParametersObj["numOfEnemiesPerCol"].GetInt() };
                    data.numOfEnemiesCols = numOfEnemiesPerCol;
                    const auto enemiesXStride{ enemiesParametersObj["enemiesXStride"].GetFloat() };
                    data.enemiesXStride = enemiesXStride;
                    const auto enemiesYStride{ enemiesParametersObj["enemiesYStride"].GetFloat() };
                    data.enemiesYStride = enemiesYStride;
                    const auto width{ enemiesParametersObj["width"].GetFloat() };
                    data.width = width;
                    const auto height{ enemiesParametersObj["height"].GetFloat() };
                    data.height = height;
                    const auto weaponCooldownTime{ enemiesParametersObj["weaponCooldownTime"].GetFloat() };
                    data.weaponCooldownTime = weaponCooldownTime;
                    const auto startCoordX{ enemiesParametersObj["startCoordX"].GetFloat() };
                    data.batchCoordX = startCoordX;
                    const auto startCoordY{ enemiesParametersObj["startCoordY"].GetFloat() };
                    data.batchCoordY = startCoordY;
                    const auto direction{ enemiesParametersObj["direction"].GetInt() };
                    data.direction = direction;
                }
                else if (dataElement.name == "enemiesWeaponContext" && dataElement.value.IsObject())
                {
                    const auto enemiesWeaponContextObj{ dataElement.value.GetObject() };
                    data.enemyWeaponCooldownTimeLeft = enemiesWeaponContextObj["cooldownTimeLeft"].GetInt();
                }
                else if (dataElement.name == "enemiesUnitsContext" && dataElement.value.IsObject())
                {
                    const auto enemiesUnitsContextObj{ dataElement.value.GetObject() };

                    const auto toRemove{ enemiesUnitsContextObj["toRemove"].GetObject() };
                    const auto toRemoveType{ toRemove["type"].GetString() };
                    const auto toRemoveDataType{ toRemove["dataType"].GetString() };
                    const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
                    const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(data.toRemove, Dod::MemUtils::stackAquire(pool, toRemoveCapacityBytes, header));
                }
            }
        }
    }

    return data;

}