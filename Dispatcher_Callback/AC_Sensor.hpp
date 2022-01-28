#ifndef ILRD_HRD18_AC_SENSOR_HPP
#define ILRD_HRD18_AC_SENSOR_HPP

#include "Dispatch_Callback.hpp"

namespace HRD18
{

class AC_Sensor
{
public:
    explicit AC_Sensor() = default;
    ~AC_Sensor() = default;
    void SetTemperture(int new_temp);
    Dispatcher<int>* GetDispatcher();
private:
    int m_temp;
    Dispatcher<int> m_dispatcher;
};//class AC_Sensor

} //namespace HRD18

#endif //ILRD_HRD18_AC_SENSOR_HPP
