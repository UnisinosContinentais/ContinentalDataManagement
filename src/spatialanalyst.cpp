#define _USE_MATH_DEFINES
#include <cmath>

#include <continentaldatamanagement/spatialanalyst.h>
#include "continentaldatamanagement/raster.h"
#include "continentaldatamanagement/resampledata.h"

namespace ContinentalDataManagement
{
    double SpatialAnalyst::areaCell(size_t row, size_t column, double xll, double yll, double cellsize, size_t rows, size_t /*cols*/)
	{
        double xMin = xll;
        double yMin = yll;
        double xLeft = xMin + (column - 1) * cellsize;
        double xRight = xLeft + cellsize;
        double yBottom = yMin + (rows - row) * cellsize;
        double yTop = yBottom + cellsize;

        auto valueA = 6378.137;
        auto valueB = 6356.752;
        double yLatitude = (yBottom + yTop) / 2;
        double valueF = (valueA - valueB) / valueA;
        double valueE2 = 2 * valueF - std::pow(valueF, 2);
        double yLatD = M_PI * yLatitude / 180;
        double valueRN = valueA / std::pow(1 - valueE2 * (std::pow(std::sin(yLatD), 2)), 0.5);
        double finalArea = ((M_PI * std::pow(valueRN, 2)) / 180) * std::abs(std::sin(yBottom * M_PI / 180) - std::sin(yTop * M_PI / 180));
        finalArea = (finalArea * std::abs(xLeft - xRight)) * 1000000;
        return finalArea;

	}

    double SpatialAnalyst::cellLength(double yLatitude, double /*xLongitude*/, short relY, short relX, double cellSize)
	{
		//CALCULA DISTANCIAS SOBRE A SUPERFICIE CONSIDERANDO O ELIPSOIDE wgs84
		//AS EQUAÇÕES UTILIZADAS AQUI FORAM OBTIDAS EM UMA HOMEPAGE DE PETER DANA
		//DA UNIVERSIDADE DO COLORADO 

        //COMPRIMENTO DO SEMI EIXO MAIOR DO ELIPSÓIDE (KM)
        constexpr double valueA = 6378.137;
        //COMPRIMENTO DO SEMI EIXO MENOR DO ELIPSÓIDE (KM)
        constexpr double valueB = 6356.752;

        //!ACHATAMENTO DO ELIPSÓIDE
        double valueF = (valueA - valueB) / valueA;
        //!QUADRADO DA EXCENTRICIDADE
        double valueE2 = (2 * valueF) - (std::pow(valueF, 2));
        //!RAIO DE CURVATURA DA TERRA NA LATITUDE
        double valueRN = valueA / std::pow(1 - valueE2 * (std::pow(std::sin(yLatitude * M_PI / 180), 2)), 0.5);

		//!CALCULA RAIO DA CIRCUNFERENCIA DE UM CIRCULO DETERMINADO PELO PLANO 
		//!QUE CORTA O ELIPSÓIDE NA LATITUDE YLAT
        double valueRCIRC = valueRN * std::cos(yLatitude * M_PI / 180);

		//Dependendo da direção relativa a partir de qual a célula veio através do Flowdirection (relX, relY), calcula o comprimento
        //Vertical
        if (relX == 0 && relY != 0)
            return (valueRN * cellSize * (M_PI / 180.0)) * 0.96194;

        //Horizontal
        if (relX != 0 && relY == 0)
            return (valueRCIRC * cellSize * (M_PI / 180.0)) * 0.96194;

        //Diagonal
        if (relX != 0 && relY != 0)
            return std::sqrt((std::pow(valueRN * cellSize * (M_PI / 180.0), 2) + std::pow(valueRCIRC * cellSize * (M_PI / 180.0), 2))) * (1.36039 / 1.414);

		return 0;
    }
}
