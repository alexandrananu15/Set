#include <iostream>
#include "Set.h"
#include "Comparator.h"
#include <string>
#include <cassert>

void testSet()
{
    Set<std::string>set1;                               //Test set1
    set1.insert("alexandra");
    set1.insert("otilia");
    set1.insert("luceafarul");
    set1.insert("zoe");
    set1.insert("eva");
    set1.insert("otilia");
    std::cout << set1 << std::endl << std::endl;

    Set<std::string>set2(set1);                         //Test set2 - copy constructor
    std::cout << set2 << std::endl << std::endl;
    
    Set<std::string>set3;                               //Test set3 = set1 - operatorul egal
    set3 = set1;
    std::cout << set3 << std::endl << std::endl;

    assert(set1.isPresent("zoe"));
    assert(!set2.isPresent("diana"));

    set1.erase("eva");
    std::cout << set1 << std::endl;

    try
    {
        set2.erase("diana");
        assert(false);
    }
    catch(const char* e)
    {
        if (e != "Nu exista valoarea in set")
            assert(false);
    }

    assert(set3.noElem() == 5); 

    std::cout << std::endl;
}



int main()
{
    testSet();
    std::cout << "Toate testele au trecut! :)";
    std::cout << std::endl;
}

