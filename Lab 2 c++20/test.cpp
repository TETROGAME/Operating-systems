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

TEST(Lab2Test, EndToEnd) {
    //Arrange
    std::istringstream input("5\n7 2 9 4 6\n");
    std::ostringstream output;
    auto* origCinBuf = std::cin.rdbuf();
    auto* origCoutBuf = std::cout.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());

    //Act
    Holder holder (ArrayHandler::getArray(), -1, -1, 0);

    ThreadHandler::launchFindMinMaxThread(ArrayHandler::findMinMax, holder);
    std::cout << "\nMAX: " << holder.array[holder.max_index]
              << "\nMIN: " << holder.array[holder.min_index];
    std::cout << "\nMAX index: " << holder.max_index
              << "\nMIN index: " << holder.min_index << "\n";

    ThreadHandler::launchFindAverageThread(ArrayHandler::findAverage, holder);
    std::cout << "\nAverage: " << holder.average << "\n";

    ArrayHandler::setMinMaxToAverage(holder);
    for (const int i : holder.array) { std::cout << i << " "; }

    std::cin.rdbuf(origCinBuf);
    std::cout.rdbuf(origCoutBuf);

    //Assert
    std::string outStr = output.str();
    EXPECT_NE(outStr.find("MAX: 9"), std::string::npos);
    EXPECT_NE(outStr.find("MIN: 2"), std::string::npos);
    EXPECT_NE(outStr.find("MAX index: 2"), std::string::npos);
    EXPECT_NE(outStr.find("MIN index: 1"), std::string::npos);
    EXPECT_NE(outStr.find("Average:"), std::string::npos);
}