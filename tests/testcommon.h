#pragma once

#define TEST(value, Type) \
do { \
        const auto json_##value = toJson(value); \
        const auto result_##value = fromJson<Type>(json_##value); \
        QCOMPARE(result_##value, value); \
} while (false)


class TestCommon
{
public:
    TestCommon();
};

