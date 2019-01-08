#ifndef CONTINENTALDATAMANAGEMENT_RESAMPLEDATA_H
#define CONTINENTALDATAMANAGEMENT_RESAMPLEDATA_H

#include <cmath>
#include <stdexcept>
#include "continentaldatamanagement/raster.h"

namespace ContinentalDataManagement
{
/// <summary>
/// Classe que ajusta as matrizes de dados
/// </summary>
namespace ResampleData
{
    int identifyColumn(const double x, const double xOrigin, const double cellSize);

    int identifyRow(const double y, const double yOrigin, const double cellSize, const int rows);

    //Reajusta os atributos de um raster através de um raster de referência
    template<class T>
    void adjustSpatialData(Raster<T> &rasterToAdjust, const Raster<T> &referenceRaster)
    {
        //Pega as informações do Raster de Referencia
        const double xOriginReference = referenceRaster.getXOrigin();
        const double yOriginReference = referenceRaster.getYOrigin();
        const double cellSizeReference = referenceRaster.getCellSize();
        const size_t rowsReference = referenceRaster.getRows();
        const size_t colsReference = referenceRaster.getCols();
        const size_t limitReference = rowsReference * colsReference;

        const std::vector<T> oldData = rasterToAdjust.getData();

        //Coordenada y upper left
        const double yUpperLeftReference = yOriginReference + (cellSizeReference * rowsReference);

        for (size_t pos = 0; pos < limitReference; ++pos)
        {
            const size_t i = pos % colsReference;
            const size_t j = pos / colsReference;

            //Calcula a coordenada de latitude do ponto desejado
            const double posY = yUpperLeftReference - (i * cellSizeReference) - (cellSizeReference / 2);

            //Calcula a coordenada de longitude do ponto desejado
            const double posX = xOriginReference + (j * cellSizeReference) + (cellSizeReference / 2);

            const size_t row = identifyRow(posY, rasterToAdjust.getYOrigin(), rasterToAdjust.getCellSize(), rasterToAdjust.getRows());
            const size_t column = identifyColumn(posX, rasterToAdjust.getXOrigin(), rasterToAdjust.getCellSize());

            //Se a linha ou coluna ficarem fora dos limites do raster a ser ajustado, escreve NoData
            if (row > (rasterToAdjust.getCols() - 1) || row < 0 || column > (rasterToAdjust.getCols() - 1) || column < 0)
                rasterToAdjust.setData(pos, rasterToAdjust.getNoDataValue());
            else {
                const double newPos = row * colsReference + column;
                rasterToAdjust->setData(pos, oldData[newPos]);
            }
        }
    }

    template<class T>
    void excludeNoDataFromRaster(Raster<T> &rasterToAdjust, const Raster<T> &referenceRasterArea, const short switchValue, const double xYtolerance)
    {

        if (rasterToAdjust.getCols() != referenceRasterArea.getRows() || rasterToAdjust.getCols() != referenceRasterArea.getCols())
        {
            throw std::runtime_error("Number of lines or columns from the raster to be adjusted is different from the reference raster!");
        }
        else if (std::abs(rasterToAdjust.getXOrigin() - referenceRasterArea.getXOrigin()) > xYtolerance || std::abs(rasterToAdjust.getYOrigin() - referenceRasterArea.getYOrigin()) > xYtolerance)
        {
            throw std::runtime_error("XY limits from raster to be ajusted is different from the reference raster!");
        }

        for (auto pos = 0; pos < rasterToAdjust.getRows(); ++pos)
        {
            //Verifica se o raster de referência possui neste ponto um valor diferente de NoDATA
            //Verifica se neste ponto o raster a ser ajustado possui NoData
            //Se tiver, substitui pelo valor indicado
            if (referenceRasterArea.getData(pos) != referenceRasterArea->getNoDataValue() && rasterToAdjust.getData(pos) == rasterToAdjust->getNoDataValue())
                    rasterToAdjust.setData(pos, switchValue);
        }
    }
}
}

#endif // CONTINENTALDATAMANAGEMENT_RESAMPLEDATA_H
