#ifndef CONTINENTALDATAMANAGEMENT_RASTERFILE_H
#define CONTINENTALDATAMANAGEMENT_RASTERFILE_H

#include <QString>
#include <vector>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

#include "continentaldatamanagement/raster.h"

namespace ContinentalDataManagement
{
/// <summary>
/// Classe de Raster para trabalhar com dados inteiros
/// </summary>
template<class T>
    class RasterFile
    {
        private:
            //Lê um raster e retorna dados em inteiro
            static Raster<T> getASCData(const QString &fileName, bool onlyHeader)
            {
                const QRegularExpression regex("\\d+");
                QFile fs(fileName);

                //Lê a linha do número de colunas
                size_t cols = regex.match(fs.readLine()).captured(0).toULongLong();
                //Lê a linha do número de linhas
                size_t rows = regex.match(fs.readLine()).captured(0).toULongLong();
                //Lê a linha da coordenada inferior em X
                double xOrigin = regex.match(fs.readLine()).captured(0).toDouble();
                //Lê a linha da coordenada inferior em Y
                double yOrigin = regex.match(fs.readLine()).captured(0).toDouble();
                //Lê a linha do cellsize
                double cellSize = regex.match(fs.readLine()).captured(0).toDouble();
                //Lê a linha do NODATA
                int noData = regex.match(fs.readLine()).captured(0).toInt();

                Raster<T> raster(rows, cols, xOrigin, yOrigin, cellSize, noData);

                //Lê apenas as informações do cabeçalho, caso ativado
                if (onlyHeader == true)
                {
                    size_t position = 0;
                    for (size_t i = 0; i < rows; ++i)
                    {
                        const auto line = fs.readLine().split(' ');
                        for (size_t j = 0; j < cols; ++j)
                        {
                            raster.setData(position, static_cast<T>(line[j].toFloat()));
                            position++;
                        }
                    }
                }

                return raster;
            }

            /// <summary>
            /// Escreve os dados da matriz para um arquivo ASC especificado.
            /// </summary>
            /// <param name="ArquivoDestino"></param>
            static void writeASCData(const Raster<T> &raster, const QString &fileName)
            {
                QFile fs(fileName);
                if (!fs.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    throw std::invalid_argument("Error open file!");
                }

                QTextStream out(&fs);

                QString lineBreaker("\n");
                QString space(" ");

                //Escreve o cabeçalho
                out << "ncols " << raster.getCols() << lineBreaker;
                out << "nrows " << raster.getRows() << lineBreaker;
                out << "xllcorner " << raster.getXOrigin() << lineBreaker;
                out << "yllcorner " << raster.getYOrigin() << lineBreaker;
                out << "cellsize " << raster.getCellSize() << lineBreaker;
                out << "NODATA_value " << raster.getNoDataValue() << lineBreaker;

                size_t cols = raster.getCols();
                size_t limit = raster.getRows() * cols;
                for (size_t i = 0; i < limit; i++)
                    out << raster.getData(i) << ((i + 1) % cols == 0 ? lineBreaker : space);
            }

        public:
            /// <summary>
            /// Lê os dados a partir de um arquivo extero
            /// </summary>
            static Raster<T> loadRasterByFile(const QString &fileName, bool onlyHeader = false)
            {
                const QRegularExpression regexAsc("\\.asc$", QRegularExpression::CaseInsensitiveOption);

                //Extensão .ASC
                if (regexAsc.match(fileName).hasMatch())
                    return RasterFile<T>::getASCData(fileName, onlyHeader);
                else
                    throw std::invalid_argument("Select data format is not compatible!");
            }

            /// <summary>
            /// Escreve os dados no disco
            /// </summary>
            static void writeData(const Raster<T> &raster, const QString &fileName)
            {
                const QRegularExpression regexAsc("\\.asc$", QRegularExpression::CaseInsensitiveOption);

                //Extensão .ASC
                if (regexAsc.match(fileName).hasMatch())
                {
                    RasterFile<T>::writeASCData(raster, fileName);
                }
                else
                {
                    throw std::invalid_argument("Select data format is not compatible!");
                }
            }
    };
}

#endif // CONTINENTALDATAMANAGEMENT_RASTERFILE_H
