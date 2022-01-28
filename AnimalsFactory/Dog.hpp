#ifndef ILRD_HRD18_DOG
#define ILRD_HRD18_DOG

#include "Animal.hpp"

namespace HRD18
{

class Dog : public Animal
{
public:
    explicit Dog(int key, std::string sound);
    virtual ~Dog() = default;
    static std::shared_ptr<Animal> Create(int key, std::string sound);
private:
    std::string m_sound;
};

Dog::Dog(int key, std::string sound) : Animal(), m_sound(sound)
{
    std::cout << "dog created" << std::endl;
}

std::shared_ptr<Animal> Dog::Create(int key, std::string sound)
{
    return std::make_shared<Dog>(key);
}
}
#endif // ILRD_HRD18_DOG
