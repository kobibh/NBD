#include <thread>
#include <iostream>

int g_counter;

static void count_to_bilion(int *cnt);

int main()
{
    std::thread t(count_to_bilion, &g_counter);

    while(1000000 > g_counter)
    {
        ++g_counter;
        std::cout << "main thread count" << g_counter << std::endl;
    }
    t.join();

    return 0;
}


static void count_to_bilion(int *cnt)
{
    while(1000000 > *cnt)
    {
        std::cout << "t thread count" << *cnt << std::endl;
        ++*cnt;
    }
}