#include "Singleton.hpp"
#include "Factory.hpp"
#include "Animal.hpp"
#include "Cat.hpp"
// #include "Dog.hpp"

using namespace HRD18;

int main()
{
    Factory<Animal, int, void*> *factory = Singleton<Factory<Animal, int, void*>>::GetInstance();
    factory->Add(3, Cat::Create);
    Cat::Args cat_args;
    cat_args.m_param1 = "cat";
    factory->Create(3, &cat_args);
    // factory->Add(4, Dog::Create);
    // factory->Create(4,122);

    return (0);
}