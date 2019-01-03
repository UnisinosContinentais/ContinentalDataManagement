#include "continentaldatamanagement/reclassparameters.h"

namespace ContinentalDataManagement
{
    ReclassParameters::ReclassParameters(double lowerBound, double upperBound, double newValue)
    {
        m_lowerBound = lowerBound;
        m_upperBound = upperBound + 0.001;
        m_newValue = newValue;
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
