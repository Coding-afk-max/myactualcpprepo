#include <iostream>
#include <string>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>

template <typename T>
class myVector{
    private:
    T* data;
    std::size_t size;
    std::size_t capacity;

void reallocate(std::size_t newCapacity){
    T* newData = new T[newCapacity];

    for(std::size_t i =0; i < size; ++i){
        newData[i] = std::move(data[i]);
    }
    delete[] data;

    data = newData;
    capacity = newCapacity;
}
public:
myVector() : data(nullptr), size(0), capacity(0) {}

explicit myVector(std::size_t count, const T& value =T()) : data(count > 0 ? new T[count] : nullptr), size(count), capacity(count){
        for(std::size_t i = 0; i < size; ++i){
            data[i] = value;
        }
    }
myVector(std::initializer_list<T> init) : data(init.size() > 0 ? new T[init.size()] : nullptr), size(init.size()), capacity(init.size()){
    std::size_t i = 0;
    for(const T& value : init){
data[i++] = value;
    }
}
~myVector(){
    delete[] data;
}

void swap(myVector& other) noexcept{
    std::swap(data, other.data);
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
}

myVector(const myVector& other) : data(other.size > 0 ? new T[other.size] : nullptr), size(other.size), capacity(other.size){
    for(std::size_t i = 0; i < size; ++i){
        data[i] = other.data[i];
    }
}
myVector(myVector&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity){
    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
}
myVector& operator=(myVector other) noexcept{
    swap(other);
    return *this;
}
void push_back(T value){
    if(size == capacity){
        std::size_t newCap = (capacity == 0) ? 1 : capacity * 2; reallocate(newCap);
    }
    data[size] = std::move(value);
    ++size;
}
T& operator[](std::size_t index){
    return data[index];
}
const T& operator[](std::size_t index) const{
    return data[index];
}
T& at(std::size_t index){
if(index >= size){
    throw std::out_of_range("myVector::at - index out of range");
}
return data[index];
}
T& front(){return data[0];}
T& back(){return data[size - 1];}

std::size_t size1() const {return size;}
std::size_t capacity1() const {return capacity;}
bool empty() const {return size == 0;}

void pop_back(){
    if(size == 0){
        throw std::out_of_range("myVector::pop_back - Vector is empty");
    }
    --size;
    data[size] = T();
}
void clear(){
    for(std::size_t i = 0; i < size; ++i){
        data[i] = T();
    }
    size = 0;
}
void reserve(std::size_t newCapacity){
    if(newCapacity > capacity){
        reallocate(newCapacity);
    }
}
void shrink_to_fit(){
    if(capacity == size) return;
    if(size == 0){
        delete[] data;
        data = nullptr;
        capacity = 0;
    }else{
        reallocate(size);
    }
}

void insert(std::size_t index, T value){
    if(index > size){
        throw std::out_of_range("myVector::insert - Index out of range.");
    }
    if(size == capacity){
        reallocate(capacity == 0 ? 1 : capacity * 2);
    }
    for(std::size_t i = size; i > index; --i){
        data[i] == std::move(data[i - 1]);
    }
    data[index] = std::move(value);
    ++size;
}

void erase(std::size_t index){
    if(index >= size){
    throw std::out_of_range("myVector::erase - Index out of range");
    }
    for(std::size_t i = index; i + 1 < size; ++i){
        data[i] = std::move(data[i + 1]);
    }
    --size;
    data[size] = T();
}
T* begin(){return data;}
T* end(){return data + size;}
const T* begin() const{return data;}
const T* end() const{return data + size;}
};

template<typename T>
void print(const std::string& label, const myVector<T>& v){
    std::cout << label << ": [ ";
    for(const auto& item : v){
        std::cout << item << " ";
    }
    std::cout << "] (size=" << v.size1() << ", capacity=" << v.capacity1() << ")" << std::endl;
}

int main(){
std::cout << "--- push_back: watch the capacity DOUBLE ---" << std::endl;
myVector<int> numbers;
for(int i = 1; i <= 9; ++i){
    numbers.push_back(i * 10);
    std::cout << "Size=" << numbers.size1() << " Capacity=" << numbers.capacity1() << std::endl;
}
print("numbers", numbers);

std::cout << std::endl << "--- access ---" << std::endl;
std::cout << "numbers[2] = " << numbers[2] << std::endl;
std::cout << "front = " << numbers.front() << ", back = " << numbers.back() << std::endl;
numbers[0] = 999;
try{
    std::cout << numbers.at(100) << std::endl;
}catch (const std::out_of_range& e){
    std::cout << "Caught: " << e.what() << std::endl;
}
std::cout << std::endl << "--- insert, erase, pop_back ---" << std::endl;
numbers.insert(1, 555);
print("After insert(1, 555)", numbers);
numbers.erase(0);
print("After erase(0)      ", numbers);
numbers.pop_back();
print("After pop_back:     ", numbers);

std::cout << std::endl << "--- reserve, shrink_to_fit, clear ---" << std::endl;
numbers.reserve(100);
print("Numbers after reserve(100)  ", numbers);
numbers.shrink_to_fit();
print("After shrink_to_fit         ", numbers);

std::cout << std::endl << "--- strings and initializer lists ---" << std::endl;
myVector<std::string> words = {"Hello", "Templates", "World"};
words.push_back("!");
print("Words", words);

myVector<double> filled(4, 2.5);
print("Filled(4, 2.5)", filled);
    return 0;
}
