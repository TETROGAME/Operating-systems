//Arrange - Act - Assert
#include "ArrayHandler.h"
#include "ThreadHandler.h"
#include <gtest/gtest.h>
#include <sstream>
using std::stringstream;
using std::cout;
using std::cin;

TEST(FindMinMaxTest, NormalBehaviour) {
    //Arrange
    Holder holder({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, -1, -1, 0);

    //Act
    ArrayHandler::findMinMax(holder);

    //Assert
    ASSERT_EQ(holder.min_index, 0);
    ASSERT_EQ(holder.max_index, 9);
}
TEST(FindMinMaxTest, OneElement) {
    //Arrange
    Holder holder({1}, -1, -1, 0);

    //Act
    ArrayHandler::findMinMax(holder);

    //Assert
    ASSERT_EQ(holder.min_index, 0);
    ASSERT_EQ(holder.max_index, 0);
}
TEST(FindMinMaxTest, EmptyArray) {
    //Arrange
    Holder holder({}, -1, -1, 0);

    //Act
    ArrayHandler::findMinMax(holder);

    //Assert
    ASSERT_EQ(holder.min_index, -1);
    ASSERT_EQ(holder.max_index, -1);

}

TEST(FindAverageTest, NormalBehaviour) {
    //Arrange
    Holder holder({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, -1, -1, 0);

    //Act
    ArrayHandler::findAverage(holder);

    //Assert
    ASSERT_EQ(holder.average, 5.5);
}
TEST(FindAverageTest, OneElement) {
    //Arrange
    Holder holder({1}, -1, -1, 0);

    //Act
    ArrayHandler::findAverage(holder);

    //Assert
    ASSERT_EQ(holder.average, 1);
}
TEST(FindAverageTest, EmptyArray) {
    //Arrange
    Holder holder({}, -1, -1, 0);

    //Act
    ArrayHandler::findAverage(holder);

    //Assert
    ASSERT_EQ(holder.average, 0);
}