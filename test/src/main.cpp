    #include <continental/datamanagement/RasterFile.h>
#include <gtest/gtest.h>

using namespace continental::datamanagement;

TEST(ContinentalDataManagementTest, RasterFileRead)
{
    auto raster = RasterFile<short>::loadRasterByFile("D:\\Git\\continentaldatamanagement\\continentaldatamanagement\\assets\\rioSinos.asc");

    EXPECT_EQ(1546, raster.getCols());
    EXPECT_EQ(931, raster.getRows());
    EXPECT_DOUBLE_EQ(-51.428723629, raster.getXOrigin());
    EXPECT_DOUBLE_EQ(-30.01840635369, raster.getYOrigin());
    EXPECT_DOUBLE_EQ(0.00083333333333333, raster.getCellSize());
    EXPECT_EQ(-9999, raster.getNoDataValue());
    EXPECT_EQ(692, raster.getData(0));
    EXPECT_EQ(676, raster.getData(9));
    EXPECT_EQ(693, raster.getData(1, 0));
}

TEST(ContinentalDataManagementTest, RasterFileWrite)
{
    auto raster = RasterFile<short>::loadRasterByFile("D:\\Git\\continentaldatamanagement\\continentaldatamanagement\\assets\\rioSinos.asc");

    raster.setData(1, 0, 100);
    EXPECT_EQ(100, raster.getData(1, 0));
    EXPECT_EQ(100, raster.getData(1546));

    RasterFile<short>::writeData(raster, "D:\\Git\\continentaldatamanagement\\continentaldatamanagement\\assets\\rioSinosGenerated.asc");

    auto rasterGenerated = RasterFile<short>::loadRasterByFile("D:\\Git\\continentaldatamanagement\\continentaldatamanagement\\assets\\rioSinosGenerated.asc");
    EXPECT_EQ(100, rasterGenerated.getData(1, 0));
    EXPECT_EQ(100, rasterGenerated.getData(1546));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
