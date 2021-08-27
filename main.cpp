#include <iostream>
#include "SkipList.h"
#define FILE_PATH "./store/dumpFile"

int main(){
    SkipList<int,std::string>skipList(6);
    skipList.insert_element(1, "我"); 
	skipList.insert_element(3, "叫"); 
	skipList.insert_element(7, "胡先颖"); 
	skipList.insert_element(8, "来自未来"); 
	skipList.insert_element(9, "请你们大声告诉我"); 
	skipList.insert_element(19, "我叫什么"); 
	skipList.insert_element(19, "大声告诉我！"); 

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();

    // skipList.load_file();

    skipList.search_element(9);
    skipList.search_element(18);


    skipList.display_list();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();

}