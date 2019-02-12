#include <cmath>

#include "continental/datamanagement/SpatialAnalyst.h"
#include "continental/datamanagement/Raster.h"
#include "continental/datamanagement/ResampleData.h"

namespace continental
{
namespace datamanagement
{
double SpatialAnalyst::areaCell(const size_t row, const size_t column, const double xll, const double yll, const double cellsize, const size_t rows, const size_t /*cols*/)
{
    const double xMin = xll;
    const double yMin = yll;
    const double xLeft = xMin + (column - 1) * cellsize;
    const double xRight = xLeft + cellsize;
    const double yBottom = yMin + (rows - row) * cellsize;
    const double yTop = yBottom + cellsize;

    const double yLatitude = (yBottom + yTop) / 2;
    const double valueF = (SpatialAnalyst::EARTH_SEMI_MAJOR_AXIS - SpatialAnalyst::EARTH_SEMI_MINOR_AXIS) / SpatialAnalyst::EARTH_SEMI_MAJOR_AXIS;
    const double valueE2 = 2 * valueF - std::pow(valueF, 2);
    const double yLatD = M_PI * yLatitude / 180;
    const double valueRN = SpatialAnalyst::EARTH_SEMI_MAJOR_AXIS / std::pow(1 - valueE2 * (std::pow(std::sin(yLatD), 2)), 0.5);
    double finalArea = ((M_PI * std::pow(valueRN, 2)) / 180) * std::abs(std::sin(yBottom * M_PI / 180) - std::sin(yTop * M_PI / 180));
    finalArea = (finalArea * std::abs(xLeft - xRight)) * 1000000;
    return finalArea;
}

double SpatialAnalyst::cellLength(const double yLatitude, const double /*xLongitude*/, const short relY, const short relX, const double cellSize)
{
    //CALCULA DISTANCIAS SOBRE A SUPERFICIE CONSIDERANDO O ELIPSOIDE wgs84
    //AS EQUAÇÕES UTILIZADAS AQUI FORAM OBTIDAS EM UMA HOMEPAGE DE PETER DANA
    //DA UNIVERSIDADE DO COLORADO

    //!ACHATAMENTO DO ELIPSÓIDE
    const double valueF = (SpatialAnalyst::EARTH_SEMI_MAJOR_AXIS - SpatialAnalyst::EARTH_SEMI_MINOR_AXIS) / SpatialAnalyst::EARTH_SEMI_MAJOR_AXIS;
    //!QUADRADO DA EXCENTRICIDADE
    const double valueE2 = (2 * valueF) - (std::pow(valueF, 2));
    //!RAIO DE CURVATURA DA TERRA NA LATITUDE
    const double valueRN = SpatialAnalyst::EARTH_SEMI_MAJOR_AXIS / std::pow(1 - valueE2 * (std::pow(std::sin(yLatitude * M_PI / 180), 2)), 0.5);

    //!CALCULA RAIO DA CIRCUNFERENCIA DE UM CIRCULO DETERMINADO PELO PLANO
    //!QUE CORTA O ELIPSÓIDE NA LATITUDE YLAT
    const double valueRCIRC = valueRN * std::cos(yLatitude * M_PI / 180);

    //Dependendo da direção relativa a partir de qual a célula veio através do Flowdirection (relX, relY), calcula o comprimento
    //Vertical
    if (relX == 0 && relY != 0)
    {
        return (valueRN * cellSize * (M_PI / 180.0)) * 0.96194;
    }

    //Horizontal
    if (relX != 0 && relY == 0)
    {
        return (valueRCIRC * cellSize * (M_PI / 180.0)) * 0.96194;
    }

    //Diagonal
    if (relX != 0 && relY != 0)
    {
        return std::sqrt((std::pow(valueRN * cellSize * (M_PI / 180.0), 2) + std::pow(valueRCIRC * cellSize * (M_PI / 180.0), 2))) * (1.36039 / 1.414);
    }

    return 0;
}
}
}
