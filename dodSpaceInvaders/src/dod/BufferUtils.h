#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

#include "Buffers.h"

#include <type_traits>

namespace Dod::BufferUtils
{

	void initFromMemory(auto& dbBuffer, const auto& memSpan) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, 0, static_cast<int32_t>(memSpan.dataEnd - memSpan.dataBegin)) };

		//TODO: Is it legal?
		dbBuffer.dataBegin = reinterpret_cast<decltype(dbBuffer.dataBegin)>(actualData.dataBegin);
		dbBuffer.dataEnd = reinterpret_cast<decltype(dbBuffer.dataEnd)>(actualData.dataEnd);
		if constexpr (requires {dbBuffer.numOfFilledEls; })
			dbBuffer.numOfFilledEls = 0;

	}

	void initFromMemory(auto& dbBuffer, const auto& memSpan, MemTypes::capacity_t beginIndex, MemTypes::capacity_t endIndex) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, beginIndex, endIndex) };

		//TODO: Is it legal?
		dbBuffer.dataBegin = reinterpret_cast<decltype(dbBuffer.dataBegin)>(actualData.dataBegin);
		dbBuffer.dataEnd = reinterpret_cast<decltype(dbBuffer.dataEnd)>(actualData.dataEnd);
		if constexpr (requires {dbBuffer.numOfFilledEls; })
			dbBuffer.numOfFilledEls = 0;

	}

	template <typename T>
	[[nodiscard]] static auto initFromArray(DBBuffer<T>& dbBuffer, auto& src) noexcept
	{

		struct MemSpan
		{
			MemTypes::dataPoint_t dataBegin{};
			MemTypes::dataPoint_t dataEnd{};
		};
		const MemSpan memSpan(
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()),
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data() + src.size())
		);

		initFromMemory(dbBuffer, memSpan);

	}

	template <typename T>
	[[nodiscard]] static auto initFromArray(ImBuffer<T>& imBuffer, const auto& src) noexcept
	{

		struct MemSpan
		{
			MemTypes::dataConstPoint_t dataBegin{};
			MemTypes::dataConstPoint_t dataEnd{};
		};
		const MemSpan memSpan(
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(src.data()),
			reinterpret_cast<Dod::MemTypes::dataConstPoint_t>(src.data() + src.size())
		);

		initFromMemory(imBuffer, memSpan);

	}

	template <typename T>
	[[nodiscard]] static auto initFromArray(MutBuffer<T>& buffer, auto& src) noexcept
	{

		struct MemSpan
		{
			MemTypes::dataPoint_t dataBegin{};
			MemTypes::dataPoint_t dataEnd{};
		};
		const MemSpan memSpan(
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()),
			reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data() + src.size())
		);

		initFromMemory(buffer, memSpan);

	}

	template <typename T>
	[[nodiscard]] auto getNumFilledElements(const Dod::DBBuffer<T>& buffer) noexcept
	{
		return buffer.numOfFilledEls;
	}

	template<typename BufferType>
	[[nodiscard]] auto getNumFilledElements(BufferType buffer) noexcept
	{
		return static_cast<int32_t>(buffer.dataEnd - buffer.dataBegin);
	}

	template<typename T>
	void populate(DBBuffer<T>& buffer, T value, bool strobe) noexcept
	{

		const auto capacity{ buffer.dataEnd - buffer.dataBegin };
		const auto bCanAddValue{ (Dod::BufferUtils::getNumFilledElements(buffer) + 1 < capacity) && strobe };

		buffer.numOfFilledEls += size_t(1) * bCanAddValue;
		buffer.dataBegin[Dod::BufferUtils::getNumFilledElements(buffer) * bCanAddValue] = value;

	}

	template<typename T>
	[[nodiscard]] T& get(const DBBuffer<T>& buffer, int32_t elId) noexcept
	{
		return buffer.dataBegin[elId + 1];
	}

	[[nodiscard]] auto& get(const auto& buffer, int32_t elId) noexcept
	{
		return buffer.dataBegin[elId];
	}

	template<typename T>
	void remove(DBBuffer<T>& buffer, const ImBuffer<int32_t> indicesToRemove) noexcept
	{

		auto targetIdx{ Dod::BufferUtils::getNumFilledElements(buffer) };
		for (int32_t idx{ 0 }; idx < Dod::BufferUtils::getNumFilledElements(indicesToRemove); ++idx)
		{
			const auto removeId{ indicesToRemove.dataBegin[idx] };
			std::swap(buffer.dataBegin[removeId + 1], buffer.dataBegin[targetIdx]);
			--targetIdx;
		}
		buffer.numOfFilledEls -= Dod::BufferUtils::getNumFilledElements(indicesToRemove);

	}

	template<typename BufferType>
	void initFromBuffer(BufferType& dest, const DBBuffer<typename BufferType::type_t>& src) noexcept
	{
		dest.dataBegin = src.dataBegin + 1;
		dest.dataEnd = src.dataEnd;
	}

	template<typename BufferType>
	void initFromBuffer(BufferType& dest, const DBBuffer<typename BufferType::type_t>& src, int32_t elementBeginId, int32_t elementEndId) noexcept
	{
		dest.dataBegin = src.dataBegin + 1 + elementBeginId;
		dest.dataEnd = src.dataBegin + 1 + elementEndId;
	}

	[[nodiscard]] auto createImFromBuffer(const auto& srcBuffer) noexcept
	{
	
		using type_t = std::remove_pointer_t<decltype(srcBuffer.dataBegin)>;

		Dod::ImBuffer<type_t> imBuffer;
		initFromBuffer(imBuffer, srcBuffer, 0, Dod::BufferUtils::getNumFilledElements(srcBuffer));

		return imBuffer;

	}

	template<typename BufferType>
	[[nodiscard]] auto getBufferBounds(const BufferType& buffer) noexcept
	{

		struct Output
		{
			decltype(buffer.dataBegin) dataBegin{};
			decltype(buffer.dataEnd) dataEnd{};
		};

		using type_t = std::remove_pointer_t<decltype(buffer.dataBegin)>;
		if constexpr (std::is_same_v<BufferType, DBBuffer<type_t>>)
			return Output(buffer.dataBegin + 1, buffer.dataEnd);
		else 
			return Output(buffer.dataBegin, buffer.dataEnd);

	}

};
