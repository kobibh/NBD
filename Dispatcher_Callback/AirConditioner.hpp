#ifndef ILRD_HRD18_AIRCONDITIONER_HPP
#define ILRD_HRD18_AIRCONDITIONER_HPP

#include "Dispatch_Callback.hpp"

namespace HRD18
{

const int ROOM_TEMP = 25;

class AirConditioner
{
public:
    explicit AirConditioner(Dispatcher<int>* dispatcher, int temp = ROOM_TEMP);
    ~AirConditioner() = default;
    int GetTemperture();
    void RunAirConditioner(const int& new_temp);
private:
    int m_temp;
    Callback<int, AirConditioner> m_cb;
}; //class AirConditioner

} //namespace HRD18

#endif //ILRD_HRD18_AIRCONDITIONER_HPP