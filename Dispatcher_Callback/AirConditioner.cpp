#include "AirConditioner.hpp"

namespace HRD18
{

AirConditioner::AirConditioner(Dispatcher<int>* dispatcher, int temp) :  m_temp(temp),
                m_cb(dispatcher, *this, &AirConditioner::RunAirConditioner)
{}

void AirConditioner::RunAirConditioner(const int& new_temp)
{
    m_temp = new_temp;
}

int AirConditioner::GetTemperture()
{
    return m_temp;
}

} //namespace HRD18