#include <ctime>
#include <exception>
#include <iostream>

#include "App/Controller.hpp"

int main()
{
    srand(time(NULL));

    try
    {
        Controller controller;
        controller.run();
    }
    catch(std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
