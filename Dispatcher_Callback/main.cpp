#include <iostream>

#include "Dispatch_Callback.hpp"
#include "AC_Sensor.hpp"
#include "AirConditioner.hpp"


using namespace HRD18;

int main()
{    
    AC_Sensor ac_sensor;
    AirConditioner ac(ac_sensor.GetDispatcher());

    std::cout << ac.GetTemperture() << std::endl;
    ac_sensor.SetTemperture(ac.GetTemperture() + 1);
    std::cout << ac.GetTemperture() << std::endl;


    return (0);
}
