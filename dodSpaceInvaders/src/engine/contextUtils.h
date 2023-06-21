#pragma once

#include <dod/MemPool.h>
#include <dod/MemUtils.h>
#include <dod/Buffers.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>

#include <string_view>
#include <optional>

namespace Engine::ContextUtils
{
	
	[[nodiscard]] rapidjson::Document loadFileDataRoot(const std::string_view filename) noexcept;
	[[nodiscard]] std::optional<rapidjson::GenericArray<true, rapidjson::Value>> gatherContextData(const rapidjson::Document& doc, size_t numOfExpectedElements) noexcept;
	
	[[nodiscard]] static void loadVariable(auto& dest, rapidjson::GenericArray<true, rapidjson::Value> src, size_t id) noexcept
	{

        using type_t = std::decay_t<decltype(dest)>;

        if (!src[id].IsObject())
            return;

        const auto& dataObject{ src[id].GetObject() };

        if constexpr (std::is_same_v<type_t, int32_t>)
            dest = dataObject["initial"].GetInt();
        else if constexpr (std::is_same_v<type_t, uint32_t>)
            dest = dataObject["initial"].GetUint();
        else if constexpr (std::is_same_v<type_t, float>)
            dest = dataObject["initial"].GetFloat();

	}

    template <typename T>
    [[nodiscard]] static void loadBuffer(
        Dod::DBBuffer<T>& dest,
        rapidjson::GenericArray<true, rapidjson::Value> src, 
        size_t id, 
        Dod::MemPool& pool, 
        int32_t& header
    ) noexcept
    {

        /*
        * 
        const auto toRemove{ enemiesUnitsContextObj["toRemove"].GetObject() };
        const auto toRemoveType{ toRemove["type"].GetString() };
        const auto toRemoveDataType{ toRemove["dataType"].GetString() };
        const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
        const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t) };
        Dod::BufferUtils::initFromMemory(data.toRemove, Dod::MemUtils::stackAquire(pool, toRemoveCapacityBytes, header));
        
        */

        using type_t = T;

        if (!src[id].IsObject())
            return;

        const auto& dataObject{ src[id].GetObject() };

        constexpr auto dataTypeSize{ sizeof(type_t) };
        const auto capacity{ dataObject["capacity"].GetInt() };
        const auto capacityBytes{ capacity * dataTypeSize };

        Dod::BufferUtils::initFromMemory(dest, Dod::MemUtils::stackAquire(pool, capacityBytes, header));

    }

}
