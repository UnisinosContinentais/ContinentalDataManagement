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
        int identifyColumn(double x, double xOrigin, double cellSize);

        int identifyRow(double y, double yOrigin, double cellSize, int rows);

		//Reajusta os atributos de um raster através de um raster de referência
        template<class T>
        void adjustSpatialData(Raster<T> &rasterToAdjust, const Raster<T> &referenceRaster)
        {
            //Pega as informações do Raster de Referencia
            double xOriginReference = referenceRaster.getXOrigin();
            double yOriginReference = referenceRaster.getYOrigin();
            double cellSizeReference = referenceRaster.getCellSize();
            size_t rowsReference = referenceRaster.getRows();
            size_t colsReference = referenceRaster.getCols();
            size_t limitReference = rowsReference * colsReference;

            std::vector<T> oldData = rasterToAdjust.getCopyData();

            //Coordenada y upper left
            double yUpperLeftReference = yOriginReference + (cellSizeReference * rowsReference);

            for (size_t pos = 0; pos < limitReference; pos++)
            {
                size_t i = pos % colsReference;
                size_t j = pos / colsReference;

                //Calcula a coordenada de latitude do ponto desejado
                double posY = yUpperLeftReference - (i * cellSizeReference) - (cellSizeReference / 2);

                //Calcula a coordenada de longitude do ponto desejado
                double posX = xOriginReference + (j * cellSizeReference) + (cellSizeReference / 2);

                size_t row = identifyRow(posY, rasterToAdjust.getYOrigin(), rasterToAdjust.getCellSize(), rasterToAdjust.getRows());
                size_t column = identifyColumn(posX, rasterToAdjust.getXOrigin(), rasterToAdjust.getCellSize());

                //Se a linha ou coluna ficarem fora dos limites do raster a ser ajustado, escreve NoData
                if (row > (rasterToAdjust.getCols() - 1) || row < 0 || column > (rasterToAdjust.getCols() - 1) || column < 0)
                    rasterToAdjust.setData(pos, rasterToAdjust.getNoDataValue());
                else {
                    double newPos = row * colsReference + column;
                    rasterToAdjust->setData(pos, oldData[newPos]);
                }
            }
        }

        template<class T>
        void excludeNoDataFromRaster(Raster<T> &rasterToAdjust, const Raster<T> &referenceRasterArea, short switchValue, double xYtolerance)
        {

            if (rasterToAdjust.getCols() != referenceRasterArea.getRows() || rasterToAdjust.getCols() != referenceRasterArea.getCols())
            {
                throw std::runtime_error("Number of lines or columns from the raster to be adjusted is different from the reference raster!");
            }
            else if (std::abs(rasterToAdjust.getXOrigin() - referenceRasterArea.getXOrigin()) > xYtolerance || std::abs(rasterToAdjust.getYOrigin() - referenceRasterArea.getYOrigin()) > xYtolerance)
            {
                throw std::runtime_error("XY limits from raster to be ajusted is different from the reference raster!");
            }

            for (auto pos = 0; pos < rasterToAdjust.getRows(); pos++)
            {
                //Verifica se o raster de referência possui neste ponto um valor diferente de NoDATA
                //Verifica se neste ponto o raster a ser ajustado possui NoData
                //Se tiver, substitui pelo valor indicado
                if (referenceRasterArea.getData(pos) != referenceRasterArea->getNoDataValue() && rasterToAdjust.getData(pos) == rasterToAdjust->getNoDataValue())
                        rasterToAdjust.setData(pos, switchValue);
            }
        }
    };
}

#endif // CONTINENTALDATAMANAGEMENT_RESAMPLEDATA_H
