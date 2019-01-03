#ifndef CONTINENTALDATAMANAGEMENT_SPATIALANALYST_H
#define CONTINENTALDATAMANAGEMENT_SPATIALANALYST_H

#include <vector>
#include <cmath>

#include "continentaldatamanagement/raster.h"
#include "continentaldatamanagement/resampledata.h"
#include "continentaldatamanagement/reclassparameters.h"

namespace ContinentalDataManagement
{
	class SpatialAnalyst
	{
        public:
        //Realiza a operação de Reclass, com base nos parâmetros específicados
        template<class T>
        void reclassRaster(Raster<T> &rasterToReclass, std::vector<ReclassParameters> &parameters)
        {
            size_t limit = rasterToReclass.getRows() * rasterToReclass.getCols();
            for (size_t position = 0; position < limit; ++position)
            {
                for (auto &parameter : parameters) {
                    if (rasterToReclass.getData(position) >= parameter.getLowerBound() && rasterToReclass->getData(position) < parameter.getUpperBound())
                    {
                        rasterToReclass.setData(position, parameter.getNewValue());
                        break;
                    }
                }
            }
        }

        static double areaCell(size_t row, size_t column, double xOrigin, double yOrigin, double cellSize, size_t rows, size_t cols);
        static double cellLength(double yLatitude, double xLongitude, short relY, short relX, double cellSize);

        //Extrair um raster a partir de outro
        template<class T>
        Raster<T> extractRasterByRaster(Raster<T> &bigRaster, Raster<T> &referenceRaster)
        {
            //Compatibiliza as informações do raster grande, com o espaço do raster pequeno
            ResampleData::adjustSpatialData(bigRaster, referenceRaster);

            auto limit = referenceRaster.getRows() * referenceRaster.getCols();
            for (size_t position = 0; position < limit; ++position)
            {
                //Extrai a informação somente para o intervalo onde houverem valores para o raster de referência
                if (referenceRaster.getData(position) != referenceRaster.getNoDataValue())
                    referenceRaster.setData(position, bigRaster.getData(position));
            }

            return referenceRaster;
        }
    };
}

#endif // CONTINENTALDATAMANAGEMENT_SPATIALANALYST_H
