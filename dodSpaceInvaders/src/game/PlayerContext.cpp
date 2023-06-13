#include "PlayerContext.h"

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>

Game::Context::Player::Data Game::Context::Player::Data::load(Dod::MemPool& pool, int32_t& header) noexcept
{

    Data data;

    std::ifstream contextFile("resources/contexts/playerContext.json");
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
                if (dataElement.name == "playerParameters" && dataElement.value.IsObject())
                {
                    const auto playerParametersObj{ dataElement.value.GetObject() };
                    const auto width{ playerParametersObj["width"].GetFloat() };
                    data.width = width;
                    const auto height{ playerParametersObj["height"].GetFloat() };
                    data.height = height;
                }
                else if (dataElement.name == "playerPositionContext" && dataElement.value.IsObject())
                {
                    const auto playerPositionContextObj{ dataElement.value.GetObject() };
                    const auto positionX{ playerPositionContextObj["positionX"].GetFloat() };
                    const auto positionY{ playerPositionContextObj["positionY"].GetFloat() };
                    data.xCoord = positionX;
                    data.yCoord = positionY;
                }
                else if (dataElement.name == "playerLifetimeContext" && dataElement.value.IsObject())
                {
                    const auto playerLifetimeContextObj{ dataElement.value.GetObject() };
                    const auto lifes{ playerLifetimeContextObj["lifes"].GetInt() };
                    data.lifes = lifes;
                }
                else if (dataElement.name == "playerInputsContext" && dataElement.value.IsObject())
                {
                    const auto playerInputsContextObj{ dataElement.value.GetObject() };
                    const auto inputs{ playerInputsContextObj["inputs"].GetInt() };
                    const auto prevInputs{ playerInputsContextObj["prevInputs"].GetInt() };
                }
                else if (dataElement.name == "playerMovement" && dataElement.value.IsObject())
                {
                    const auto playerMovementObj{ dataElement.value.GetObject() };
                    const auto move{ playerMovementObj["move"].GetInt() };
                }
                else if (dataElement.name == "playerFire" && dataElement.value.IsObject())
                {
                    const auto playerFireObj{ dataElement.value.GetObject() };
                    const auto move{ playerFireObj["fire"].GetInt() };
                }
            }
        }
    }

    return data;

}
