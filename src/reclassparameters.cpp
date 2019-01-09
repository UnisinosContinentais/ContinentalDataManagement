#include "continental/datamanagement/reclassparameters.h"

namespace Continental
{
<<<<<<< HEAD
namespace DataManagement
{
ReclassParameters::ReclassParameters(const double lowerBound, const double upperBound, const double newValue):
    m_lowerBound(lowerBound), m_upperBound(upperBound + 0.001), m_newValue(newValue)
{
=======
    ReclassParameters::ReclassParameters(const double lowerBound, const double upperBound, const double newValue):
        m_lowerBound(lowerBound), m_upperBound(upperBound + 0.001), m_newValue(newValue)
    {

    }
>>>>>>> branch 'feature/task-1-converter-iphdatamanagement' of https://continentais.unisinos.br:8000/continentais/continentaldatamanagement.git

}

double ReclassParameters::getLowerBound() const
{
    return m_lowerBound;
}

double ReclassParameters::getUpperBound() const
{
    return m_upperBound;
}

double ReclassParameters::getNewValue() const
{
    return m_newValue;
}
}
}
