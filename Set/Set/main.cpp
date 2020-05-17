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

    Set<int>set4;                               //Test set4
    set4.insert(11);
    set4.insert(22);
    set4.insert(33);
    set4.insert(44);
    std::cout << set4 << std::endl << std::endl;

    Set<int>set5(set4);                         //Test set5 - copy constructor
    std::cout << set5 << std::endl << std::endl;

    Set<int>set6;                               //Test set6 = set5 - operatorul egal
    set6 = set5;
    std::cout << set6 << std::endl << std::endl;

    assert(set5.isPresent(11));
    assert(set5.isPresent(44));

    //tinem cont ca pe cazul cu int-uri, sunt considerate egale numerele cu aceeasi paritate
    
    //astfel, daca vrem sa il stergem pe 11, putem la fel de bine sa dam ca valoare 101, ptr ca au aceeasi paritate
    set4.erase(101);
    std::cout << set4 << std::endl;

    try
    {
        set4.erase(33);
        assert(false);
    }
    catch (const char* e)
    {
        if (e != "Nu exista valoarea in set")
            assert(false);
    }

    assert(set6.noElem() == 2);

    std::cout << std::endl;

}



int main()
{
    testSet();
    std::cout << "Toate testele au trecut! :)";
    std::cout << std::endl;
}

