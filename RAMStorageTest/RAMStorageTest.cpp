#include <iostream>

#include "RAMStorage.hpp"
#include "DriverData.hpp" 

using namespace HRD18;

const size_t MESSAGE_LEN = 4;

static bool test_read(std::vector<char>& res_vec, std::vector<char>& expected);

int main()
{
    RAMStorage storage(1000);
    std::vector<char> vec(MESSAGE_LEN);
    vec[0] = 'a';
    vec[1] = 'b';
    vec[2] = 'c';
    vec[3] = 'd';

    char handle[8] = {0};
    std::shared_ptr<DriverData> ddsp = std::make_shared<DriverData>(handle, DriverData::RequestType::WRITE, 0, 4);
    ddsp.get()->m_data = vec;
    std::shared_ptr<DriverData> ddsp2 = std::make_shared<DriverData>(handle, DriverData::RequestType::READ, 0, 4);
    
    storage.Write(ddsp);
    storage.Read(ddsp2);

    if (test_read(ddsp2->m_data, vec))
    {
        std::cout << "Test PASSED\n" << std::endl;
    }
    else
    {
        std::cout << "Test FAILED\n" << std::endl;
    }

    return (0);
}


static bool test_read(std::vector<char>& res_vec, std::vector<char>& expected)
{
    std::vector<char>::iterator i;
    std::vector<char>::iterator j;
    bool res = true;
    for (i = res_vec.begin(), j = expected.begin();
         i != res_vec.end(), j != expected.end();
        ++i, ++j)
    {
        if(*j != *i)
        {
            std::cout << *i << " ";
            std::cout << *i << " ";
            res = false;
        }
    }

    return (res);
}
