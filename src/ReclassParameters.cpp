#include "continental/datamanagement/ReclassParameters.h"

namespace continental
{
namespace datamanagement
{
ReclassParameters::ReclassParameters(const double lowerBound, const double upperBound, const double newValue):
    m_lowerBound(lowerBound), m_upperBound(upperBound + 0.001), m_newValue(newValue)
{
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
