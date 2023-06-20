#include "pch.h"

#include <engine/contextUtils.cpp>

#include <rapidjson/document.h>

TEST(Context, LoadJsonFile)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };

	ASSERT_TRUE(doc.IsObject());
	const auto obj{ doc.GetObject() };

	EXPECT_NE(obj.FindMember("whoIAm"), obj.end());
	EXPECT_NE(obj.FindMember("version"), obj.end());
	EXPECT_NE(obj.FindMember("contextType"), obj.end());
	EXPECT_NE(obj.FindMember("data"), obj.end());

}

TEST(Context, LoadJsonFileFailed)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileBroken.json") };

	ASSERT_FALSE(doc.IsObject());

}

TEST(Context, LoadJsonFileNoFile)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileDontExist.json") };

	ASSERT_FALSE(doc.IsObject());

}

TEST(Context, LoadData)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };

	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	EXPECT_EQ(data.Size(), 3);

}

TEST(Context, LoadDataFailed)
{

	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 4) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}
	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}
	{
		const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFileNotExist.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

		EXPECT_FALSE(inputDataOpt.has_value());
	}

}

TEST(Context, LoadVariable)
{

	const auto doc{ Engine::ContextUtils::loadFileDataRoot("assets/sampleFile.json") };
	const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };
	ASSERT_TRUE(inputDataOpt.has_value());

	const auto& data{ inputDataOpt.value() };
	ASSERT_EQ(data.Size(), 3);

	{
		float dst{};
		Engine::ContextUtils::loadVariable(dst, data, 0);
		EXPECT_EQ(dst, 50.f);
	}

	{
		int32_t dst{};
		Engine::ContextUtils::loadVariable(dst, data, 1);
		EXPECT_EQ(dst, 25);
	}

	{
		uint32_t dst{};
		Engine::ContextUtils::loadVariable(dst, data, 2);
		EXPECT_EQ(dst, 134217728U);
	}

}

TEST(Context, LoadBuffer)
{
	ASSERT_TRUE(false);
}
