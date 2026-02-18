#include<gtest/gtest.h>
#include"ecs/Types.hh"

using namespace ECS;

TEST(TypesTest, EntityIDSize) {
  EXPECT_EQ(sizeof(EntityID), sizeof(uint32_t));
}

TEST(TypesTest, InvalidEntityValue) {
  EXPECT_EQ(INVALID_ENTITY, 0xFFFFFFFF);
}

