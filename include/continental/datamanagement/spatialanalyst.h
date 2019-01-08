#ifndef CONTINENTALDATAMANAGEMENT_SPATIALANALYST_H
#define CONTINENTALDATAMANAGEMENT_SPATIALANALYST_H

#include <vector>
#include <cmath>

#include "continental/datamanagement/raster.h"
#include "continental/datamanagement/resampledata.h"
#include "continental/datamanagement/reclassparameters.h"

#include "continental/datamanagement/continentaldatamanagement_export.h"

namespace Continental
{
namespace DataManagement
{
class CONTINENTALDATAMANAGEMENT_EXPORT SpatialAnalyst
{
    public:
        static constexpr double EARTH_SEMI_MAJOR_AXIS = 6378.137;
        static constexpr double EARTH_SEMI_MINOR_AXIS = 6356.752;

        //Realiza a operação de Reclass, com base nos parâmetros específicados
        template<class T>
        void reclassRaster(Raster<T> &rasterToReclass, const std::vector<ReclassParameters> &parameters)
        {
            const size_t limit = rasterToReclass.getRows() * rasterToReclass.getCols();
            for (size_t position = 0; position < limit; ++position)
            {
                for (const auto &parameter : parameters) {
                    if (rasterToReclass.getData(position) >= parameter.getLowerBound() && rasterToReclass->getData(position) < parameter.getUpperBound())
                    {
                        rasterToReclass.setData(position, parameter.getNewValue());
                        break;
                    }
                }
            }
        }

        static double areaCell(const size_t row, const size_t column, const double xOrigin, const double yOrigin, const double cellSize, const size_t rows, const size_t cols);
        static double cellLength(const double yLatitude, const double xLongitude, const short relY, const short relX, const double cellSize);

        //Extrair um raster a partir de outro
        template<class T>
        Raster<T> extractRasterByRaster(Raster<T> &bigRaster, const Raster<T> &referenceRaster)
        {
            //Compatibiliza as informações do raster grande, com o espaço do raster pequeno
            ResampleData::adjustSpatialData(bigRaster, referenceRaster);

            const auto limit = referenceRaster.getRows() * referenceRaster.getCols();
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
}

#endif // CONTINENTALDATAMANAGEMENT_SPATIALANALYST_H
