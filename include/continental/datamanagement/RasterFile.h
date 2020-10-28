#ifndef CONTINENTALDATAMANAGEMENT_RASTERFILE_H
#define CONTINENTALDATAMANAGEMENT_RASTERFILE_H

#include <QString>
#include <vector>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

#include "continental/datamanagement/Raster.h"
#include "continental/datamanagement/export.h"

namespace continental
{
namespace datamanagement
{
/// <summary>
/// Classe de Raster para trabalhar com dados inteiros
/// </summary>
template<class T>
class RasterFile
{
private:
    //Lê um raster e retorna dados em inteiro
    static Raster<T> getASCData(const QString &fileName, const bool onlyHeader)
    {
        const QRegularExpression regex("-?[0-9.,]+");
        QFile fs(fileName);
        if (!fs.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw std::invalid_argument("Error openning file!");
        }

        //Lê a linha do número de colunas
        const size_t cols = regex.match(fs.readLine()).captured(0).toULongLong();
        //Lê a linha do número de linhas
        const size_t rows = regex.match(fs.readLine()).captured(0).toULongLong();
        //Lê a linha da coordenada inferior em X
        const double xOrigin = regex.match(fs.readLine()).captured(0).replace(",", ".").toDouble();
        //Lê a linha da coordenada inferior em Y
        const double yOrigin = regex.match(fs.readLine()).captured(0).replace(",", ".").toDouble();
        //Lê a linha do cellsize
        const double cellSize = regex.match(fs.readLine()).captured(0).replace(",", ".").toDouble();
        //Lê a linha do NODATA
        const int noData = regex.match(fs.readLine()).captured(0).toInt();

        Raster<T> raster(rows, cols, xOrigin, yOrigin, cellSize, noData);

        //Lê apenas as informações do cabeçalho, caso ativado
        if (!onlyHeader)
        {
            QRegularExpression regexLine("\\s$");

            for (size_t i = 0; i < rows; ++i)
            {
                const auto line = QString::fromStdString(fs.readLine().toStdString()).replace(regexLine, "").split(' ');
                for (size_t j = 0; j < cols; ++j)
                {
                    raster.setData(i, j, static_cast<T>(line[static_cast<int>(j)].toDouble()));
                }
            }
        }

        return std::move(raster);
    }

    /// <summary>
    /// Escreve os dados da matriz para um arquivo ASC especificado.
    /// </summary>
    /// <param name="ArquivoDestino"></param>
    const static void writeASCData(const Raster<T> &raster, const QString &fileName)
    {
        QFile fs(fileName);
        if (!fs.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            throw std::invalid_argument("Error openning file!");
        }

        QTextStream out(&fs);

        const QString lineBreaker("\n");
        const QString space(" ");

        out << "ncols " << raster.getCols() << lineBreaker;
        out << "nrows " << raster.getRows() << lineBreaker;
        QRegularExpression regex("([.][1-9]+)0+$");
        QRegularExpression regex2("[.]0+$");
        QRegularExpression regex3("([.]\\d{1,6})\\d+$");
        out << "xllcorner "
            << QString()
                .setNum(raster.getXOrigin(), 'f', 42)
                .replace(regex, "\\1")
                .replace(regex2, "")
                .replace(regex3, "\\1")
                .replace(".", ",")
            << lineBreaker;
        out << "yllcorner "
            << QString()
                .setNum(raster.getYOrigin(), 'f', 42)
                .replace(regex, "\\1")
                .replace(regex2, "")
                .replace(regex3, "\\1")
                .replace(".", ",")
            << lineBreaker;
        out << "cellsize "
            << QString()
                .setNum(raster.getCellSize(), 'f', 42)
                .replace(regex, "\\1")
                .replace(regex2, "")
                .replace(regex3, "\\1")
                .replace(".", ",")
            << lineBreaker;
        out << "NODATA_value " << raster.getNoDataValue() << lineBreaker;

        const size_t cols = raster.getCols();
        const size_t limit = raster.getRows() * cols;
        for (size_t i = 0; i < limit; ++i)
        {
            out << raster.getData(i) << space;
            if ((i + 1) % cols == 0)
            {
                out << lineBreaker;
            }
        }
    }

public:
    /// <summary>
    /// Lê os dados a partir de um arquivo extero
    /// </summary>
    static Raster<T> loadRasterByFile(const QString &fileName, const bool onlyHeader = false)
    {
        const QRegularExpression regexAsc("\\.asc$", QRegularExpression::CaseInsensitiveOption);

        //Extensão .ASC
        if (regexAsc.match(fileName).hasMatch())
            return RasterFile<T>::getASCData(fileName, onlyHeader);
        else
            throw std::invalid_argument("Selected data format is not compatible!");
    }

    /// <summary>
    /// Escreve os dados no disco
    /// </summary>
    const static void writeData(const Raster<T> &raster, const QString &fileName)
    {
        const QRegularExpression regexAsc("\\.asc$", QRegularExpression::CaseInsensitiveOption);

        //Extensão .ASC
        if (regexAsc.match(fileName).hasMatch())
        {
            RasterFile<T>::writeASCData(raster, fileName);
        }
        else
        {
            throw std::invalid_argument("Selected data format is not compatible!");
        }
    }
};
}
}

#endif // CONTINENTALDATAMANAGEMENT_RASTERFILE_H
