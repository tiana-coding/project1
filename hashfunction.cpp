#include "hashfunction.h"

unsigned int hashFunction(const std::string& str, size_t tableSize){

unsigned int hash=0;
//syntax for (ElementTyp Variable : Container) char, c, str(durchlaufende datenstruktur)
    for(char c:str){
        
        hash = (hash << 5) - hash + c;//hash = (hash*2*2*2*2*2-hash) + c;
        
    }
    return hash % tableSize;




}