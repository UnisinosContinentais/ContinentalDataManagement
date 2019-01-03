#include "continentaldatamanagement/resampledata.h"

namespace ContinentalDataManagement
{
    namespace ResampleData {
        int identifyColumn(double x, double xOrigin, double cellSize)
        {
            // Transformação das coordenadas do ponto em linhas e colunas
            // Lembrando que a posição (1,1) equivale a (0,0)
            // Calcula a coordenada X do centro da célula no canto esquerdo
            double xllCenter = xOrigin + (cellSize / 2);

            return static_cast<int>(std::round((x - xllCenter) / cellSize));
        }

        int identifyRow(double y, double yOrigin, double cellSize, int numberRows)
        {
            // Transformação das coordenadas do ponto em linhas e colunas
            // Lembrando que a posição (1,1) equivale a (0,0)
            // Calcula a coordenada Y do centro da célula no canto esquerdo
            double yllCenter = yOrigin + (cellSize / 2);

            return static_cast<int>(std::round(numberRows - 1 - (y - yllCenter) / cellSize));
        }
    }
}
