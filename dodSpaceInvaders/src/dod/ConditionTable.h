#pragma once

#include <dod/Buffers.h>
#include <cinttypes>
#include <span>

namespace Dod::CondTable
{

	enum class TriState : int8_t
	{
		SKIP = 0,
		FALSE, 
		TRUE,
	};

	struct Table
	{
		const ImBuffer<uint32_t> xOrMasks;
		const ImBuffer<uint32_t> ignoreMasks;
	};

	[[nodiscard]] static Table generate(const auto& tableSrc, const std::span<uint32_t> xOrMasksMem, const std::span<uint32_t> ignoreMasksMem) noexcept
	{

		Dod::DBBuffer<uint32_t> xOrMasks;
		Dod::BufferUtils::initFromArray(xOrMasks, xOrMasksMem);
		Dod::DBBuffer<uint32_t> ignoreMasks;
		Dod::BufferUtils::initFromArray(ignoreMasks, ignoreMasksMem);

		for (int32_t rowId{}; rowId < tableSrc.size(); ++rowId)
		{
			uint32_t xOr{};
			uint32_t ignore{};
			for (int32_t colId{}; colId < tableSrc[rowId].size(); ++colId)
			{
				if (tableSrc[rowId][colId] == TriState::FALSE)
					xOr |= (1 << colId);
				if (tableSrc[rowId][colId] == TriState::SKIP)
					ignore |= (1 << colId);
			}
			for (size_t colId{ tableSrc[rowId].size() }; colId < 32; ++colId)
				ignore |= (1 << colId);

			Dod::BufferUtils::populate(xOrMasks, xOr, true);
			Dod::BufferUtils::populate(ignoreMasks, ignore, true);
		}

		return Table(Dod::BufferUtils::createImFromBuffer(xOrMasks), Dod::BufferUtils::createImFromBuffer(ignoreMasks));

	}

};
