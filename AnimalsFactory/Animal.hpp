#ifndef ILRD_HRD18_ANIMAL
#define ILRD_HRD18_ANIMAL

#include <iostream>
#include <memory>


namespace HRD18
{
class Animal
{
public:
    explicit Animal();
    virtual ~Animal() = 0;
private:
    Animal& operator=(const Animal& other);
};

Animal::Animal() 
{
    std::cout << "Animal created" << std::endl;
}

Animal::~Animal() = default;


}

#endif // ILRD_HRD18_ANIMAL