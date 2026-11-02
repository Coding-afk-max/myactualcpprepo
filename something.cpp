#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <limits>
#include <iomanip>
#include <cstdlib>

class student{
protected: //protected basically means child and parent can access this
std::string name;
int age;
int id;

public: //anyone can use this
student(const std::string& name, int age, int id) : name(name), age(age), id(id) {} //constructor, dont forget the {}.

virtual ~student(){} //required virtual destructor
virtual std::string getType() const = 0; //returns data basically.
virtual double calculateTuition() const = 0; //cant forget the const = 0

virtual void printInfo() const { //every functions needs to be virtual, also, the const is to not change the student
std::cout << "id: " << id << std::endl;
std::cout << "name: " << name << std::endl;
std::cout << "type: " << getType() << std::endl; //invoking the gettype function from earlier
std::cout << "tuition: €" << std::fixed << std::setprecision(2) << calculateTuition() << std::endl;
}
int getId() const{return id;}
const std::string& getName() const{return name;} //this is a getter
};

class undergraduate : public student{
    private:
    std::string major;
    int credits;

public:
undergraduate(const std::string& name, int age, int id, const std::string major, int credits) : student(name, age, id), major(major), credits(credits) {}

std::string getType() const override {
    return "Undergraduate";
}

double calculateTuition() const override {
    return credits * 250.0;
}

void printInfo() const override {
    student::printInfo();
    std::cout << "major: " << major << std::endl;
    std::cout << "credits: " << credits << std::endl;

}
};

class graduate : public student{
private:
std::string thesisTopic;

public:
graduate(const std::string& name, int age, int id, const std::string thesisTopic) : student(name, age, id), thesisTopic(thesisTopic) {}

std::string getType() const override {
    return "Graduate";
}
double calculateTuition() const override{
    return 9000.0;
}
void printInfo() const override {
    student::printInfo();
    std::cout << "thesis: " << thesisTopic << std::endl;
}
};


int main(){

    return 0;
}
