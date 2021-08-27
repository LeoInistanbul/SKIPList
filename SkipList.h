#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex.h>
#include <fstream>

#define STORE_FILE "store/dumpFile"

std::mutex mtx;     // mutex for critical section
std::string delimiter = ":";

template<typename Key,typename Value>
class Node{
public:
    Node(){}
    Node(Key, Value, int level);
    ~Node();
    Key get_key() const;
    Value get_value() const;
    void set_value(Value);
    Node<Key,Value> **forward;  
    int node_level;
private:
    int key;
    Value value;
};
template<typename K ,typename V>
Node<K,V>::Node(const K k,const V v,int level){
    this->k=k;
    this->v=v;
    this->node_level=level;

    this->forward=new Node<K,V>*[level+1];   //索引层从1开始的
    memset(this->forward,0,sizeof(Node<K,V>*)*(level+1));// 给前一层索引赋值0
}
template<typename K,typename V>
Node<K,V>::~Node(){ 
    delete[] forward;
}
template<typename K,typename V>
K Node<K,V>::get_key()const{
 return key;
}
template<typename K,typename V>
V Node<K,V>::get_value()const{
    return value;
}
template<typename K,typename V> 
void Node<K,V>::set_value(V value) {
    this->value=value;
};


//SkipList
template<typename Key, typename Value>
class SkipList{
public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    Node<Key,Value>* create_node(Key,Value,int);
    int insert_element(Key,Value);
    void display_list();
    bool search_element(Key);
    void delete_element(Key);
    void dump_file();
    void load_file();
    int size();

private:    
    int _max_level;          //跳表的最大层
    int _skip_list_level;      //当前索引层
    Node<Key,Value> *_header;       //头节点的指针
    std::ofstream _file_writer;
    std::ifstream _file_reader;
    int element_count;                  //统计当前元素个数
private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);
};

template<typename K, typename V>
Node<K,V>* SkipList<K,V>::create_node(const K k,const V v,int level){
    Node<V> *head = new Node<V>(k,v,level);
    return head;
}

//插入到跳表中 如果插入成功返回1 插入不成功（意味着已经有该元素）返回0
template<typename K,typename V>
int SkipList<K,V>::insert_element(const K key,const V value){
    mtx.lock();
    Node<K,V>* cur=this->_header;
    Node<K,V>* update[_max_level+1];
    memset(update,0,sizeof(Node<K,V> *)*(_max_level+1));

    for(int i=_skip_list_level;i>=0;i--){
        while(cur->forward[i]!=nullptr && cur->forward[i]->get_key()<key){
            cur=cur->forward[i];
        }
        update[i]=cur;
    }
    cur=cur->forward[0];
    if(cur!=nullptr && cur->get_key() == key){
        std::cout <<"key:"<<key<<",exists"<<std::endl;
        mtx.unlock();
        return 0;
    }
    if(cur==nullptr||cur->get_key()!=key){
        int random_level = get_random_level();
        if(random_level > _skip_list_level){
            for(int i=)_skip_list_level+1;i<random_level+1;i++)
                update[i]=_header;
        }
        _skip_list_level=random_level;
        Node<K,V>*insert_node=create_node(key,value,random_level);

        for(int i=0;i<=random_level;i++){
            insert_node->forward[i]=update[i]->forward[i];
            update[i]->forward[i]=insert_node;
        }
        std::cout<<"Succesfully insert key:"<<key<<",value:"<<value<<std::endl;
        element_count++;
    }
    mtx.unlock();
    return 1;
}

//显示表中的数据 
template<typename K,typename V>
void SkipList<K,V>::display_list(){
    std::cout <<"\n**** Skip List *****\n"<<std::endl;
    for(int i=0;i<=_skip_list_level;i++){
        Node<V>*node=this->_header->forward[i];
        std::cout<<"level"<<i<<":";
        while(node!=null){
            std::cout << node->get_key()<<":"<<node->get_value()<<";";
            node=node->forward[i];
        }
        std::cout <<std::endl;
    }
}

//将表中的数据输出到文件中
template<typename K,typename V>
void SkipList<K,V>::dump_file(){
    std::cout << "dump_file"<<std::endl;
    _file_writer.open(STORE_FILE);
    Node<K,V>* node=this->_header->forward[0];

    while(node!=nullptr){
        _file_writer<<node->get_key()<<":"<<node->get_value()<<'\n';
        std::cout <<node->get_key<<":"<<node->get_value()<<';\n';
        node=node->forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return;
}

//下载文件中的数据信息
template<typename K,typename V>
void SkipList<K,V>::load_file(){
    _file_reader.open(STORE_FILE);
    std:cout<<"load_file"<<std::endl;
    std:string lint;
    std::int *key = new std::int();
    std::string *value = new std::string();
    while(getline(_file_reader,line)){
        get_key_value_from_string(line,key,value);
        if(key->empty() || value->empty())continue;
        insert_element(*key,*value);
        std::cout <<"key:"<<*key<<"value:"<<*value<<std::endl;
    }
    _file_reader.close();
}

template<typename K, typename V>
int SkipList<K,V>::size(){
    return element_count;
}

template<typename K,typename V>
void SkipList<K,V>::get_key_value_from_string(const std::string&str,std::string*key,std::string*value){
    if(!is_valid_string(str)) return;
    *key=str.substr(0,str.find(delimiter));
    *value=str.substr(str.find(delimiter+1),str.length());
}

template<typename K,typename V>
bool SkipList<K,V>::is_valid_string(const std::string &str){
    if(str.empty())return false;
    if(str.find(delimiter)==std::string::npos)return false;
    return true;
}

template<typename K,typename V>
void SkipList<K,V>::delete_element(K key){
    mtx.lock();
    Node<V> *cur=this->_header;
    Node<V> *update[_max_level+1];
    memset(update,0,sizeof(Node<V>*)*(_max_level+1));

    for(int i=_skip_list_level;i>=0;i--){
        while(cur->forward[i]!=nullptr && cur->forward[i]->get_key() < key){
            cur=cur->forward[i];
        }
        update[i]=cur;
    }
    
    cur=cur->forward[0];
    if(cur!=nullptr && cur->get_key()==key){
        for(int i=0;i<=_skip_list_level;i++){
            if(update[i]->forward[i]!=cur)break;
            update[i]->forward[i]=cur->forward[i];
        }
        while(_skip_list_level>0 && _header->forward[_skip_list_level]==0){
            _skip_list_level--;
        }
        std::cout << "Sucessfully deleted key"<<key<<std::endl;
        element_count--;
    }
    mtx.unlock();
    return;
}
template<typename K,typename V>
bool SkipList<K,V>::search_element(K key){
    std::cout <<"search_element"<<std::endl;
    Node<V> *cur=_header;

    for(int i=_skip_list_level;i>=0;i--){
        while(cur->forward[i] && cur->forward[i]->get_key() <key){
            cur=cur->forward[i];
        }
    }

    cur=cur->forward[0];

    if(cur && cur->get_key()==key()){
        std::cout<<"Found Key:"<<key<<",value:"<<cur->get_value()<<std::endl;
        return true;
    }

    std::cout <<"Not Found Key:"<<key<<std::endl;
    return false;
}

template<typename K,typename V>
SkipList<K,V>::SkipList(int max_level){
    this->_max_level=max_level;
    this->_skip_list_level=0;
    this->element_count=0;
    int key;
    V v;
    this->_header=new Node<V>(key,v,_max_level);
}

template<typename K,typename V>
SkipList<K,V>::~SkipList() {
    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
    delete _header;
}

template<typename K,typename V>
int SkipList<K,V>::get_random_level(){
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
}
