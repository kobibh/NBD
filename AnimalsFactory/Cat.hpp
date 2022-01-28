#ifndef ILRD_HRD18_CAT
#define ILRD_HRD18_CAT

#include "Animal.hpp"


namespace HRD18
{

class Cat : public Animal
{
public:
    struct Args;
    explicit Cat(Args* args);
    virtual ~Cat() = default;
    static std::shared_ptr<Animal> Create(void* args);
    struct Args
    {
        std::string m_param1;
    };
private:
    std::string m_sound;
};


Cat::Cat(Args* args) : m_sound(args->m_param1)
{
    std::cout << "cat created" << std::endl;
}

std::shared_ptr<Animal> Cat::Create(void* args)
{
    return std::make_shared<Cat>(reinterpret_cast<Args *>(args));
}

}
#endif // ILRD_HRD18_CAT
