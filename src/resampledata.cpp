#include "continental/datamanagement/resampledata.h"

#include <assert.h>

namespace Continental
{
namespace DataManagement
{
    int ResampleData::identifyColumn(const double x, double xOrigin, double cellSize)
    {
        assert(cellSize > static_cast<double>(0));

        // Transformação das coordenadas do ponto em linhas e colunas
        // Lembrando que a posição (1,1) equivale a (0,0)
        // Calcula a coordenada X do centro da célula no canto esquerdo
        const double xllCenter = xOrigin + (cellSize / 2);

        return static_cast<int>(std::round((x - xllCenter) / cellSize));
    }

    int ResampleData::identifyRow(const double y, const double yOrigin, const double cellSize, const int numberRows)
    {
        assert(cellSize > 0.0);

        // Transformação das coordenadas do ponto em linhas e colunas
        // Lembrando que a posição (1,1) equivale a (0,0)
        // Calcula a coordenada Y do centro da célula no canto esquerdo
        const double yllCenter = yOrigin + (cellSize / 2);

        return static_cast<int>(std::round(numberRows - 1 - (y - yllCenter) / cellSize));
    }
}
}
