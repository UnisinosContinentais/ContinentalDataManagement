#ifndef CONTINENTALDATAMANAGEMENT_RECLASSPARAMETERS_H
#define CONTINENTALDATAMANAGEMENT_RECLASSPARAMETERS_H

#include "continental/datamanagement/continentaldatamanagement_export.h"

namespace Continental
{
namespace DataManagement
{
class CONTINENTALDATAMANAGEMENT_EXPORT ReclassParameters
{
private:
    double m_lowerBound = 0;
    double m_upperBound = 0;
    double m_newValue = 0;

    /// <summary>
    /// Creates an instance for this class
    /// </summary>
    /// <param name="LB">The Lower bound (greater or equal) </param>
    /// <param name="UB">The Upper bound (less than)</param>
    /// <param name="NewVal">The new value to be assigned.</param>
public:
    ReclassParameters(double lowerBound, double upperBound, double newValue);
    /// <summary>
    /// Retorna o valor Limite inferior
    /// </summary>
    double getLowerBound() const;
    /// <summary>
    /// Retorna o valor Limite superior
    /// </summary>
    double getUpperBound() const;
    /// <summary>
    /// Retorna o novo valor
    /// </summary>
    double getNewValue() const;

};
}
}

#endif // CONTINENTALDATAMANAGEMENT_RECLASSPARAMETERS_H
