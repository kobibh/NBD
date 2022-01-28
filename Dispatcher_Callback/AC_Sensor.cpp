#include "AC_Sensor.hpp"

namespace HRD18
{

Dispatcher<int>* AC_Sensor::GetDispatcher()
{
    return &m_dispatcher;
}

void AC_Sensor::SetTemperture(int new_temp)
{
    m_temp = new_temp;
    m_dispatcher.NotifyAll(m_temp);
}

} // namespace HRD18