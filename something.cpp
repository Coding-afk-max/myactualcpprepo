#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <limits>
#include <iomanip>
#include <cstdlib>

//GENERAL CLASS
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

//UNDERGRADUATE CLASS
class undergraduate : public student{
    private:
    std::string major;
    int credits;

public:
undergraduate(const std::string& name, int age, int id, const std::string& major, int credits) : student(name, age, id), major(major), credits(credits) {}

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
//GRADUATE CLASS
class graduate : public student{
private:
std::string thesisTopic;

public:
graduate(const std::string& name, int age, int id, const std::string& thesisTopic) : student(name, age, id), thesisTopic(thesisTopic) {}

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


//EXCHANGE CLASS
class exchange : public student{
private:
std::string homeCountry;
public:
exchange(const std::string& name, int age, int id, const std::string& homeCountry) : student(name, age, id), homeCountry(homeCountry) {}

std::string getType() const override {
    return "Exchange";
}

double calculateTuition() const override {
    return 1500.0;
}

void printInfo() const override {
    student::printInfo();
    std::cout << "Home country: " << homeCountry << std::endl;
}
};

//DATABASE CLASS
class database {
private:
std::vector<std::unique_ptr<student>> students;
int nextId = 1;
public:
int generateId(){
    return nextId++;
}
void add(std::unique_ptr<student> student1) {
    students.push_back(std::move(student1));
}
void listAll() const {
if (students.empty()) {
std::cout << "The database is empty /:" << std::endl;
return;
}
for (const auto& s : students) {
    s->printInfo();
}
}
const student* findById(int id) const {
    for(const auto& s : students) {
        if(s->getId() == id){
            return s.get();
        }
    }
    return nullptr;
}

bool removeById(int id){
    for(auto it = students.begin(); it != students.end(); ++it){
if((*it)->getId() == id){
    students.erase(it);
    return true;
}
    }
    return false;
}
double totalTuition() const{
    double total = 0.0;
    for(const auto& s : students){
        total += s->calculateTuition();
    }
    return total;
}
};
//ADDSTUDENT FUNCTION
void addStudent(database& db){
std::cout << std::endl << "Student type: " << std::endl;
std::cout << std::endl << "1. Undergraduate" << std::endl;
std::cout << std::endl << "2. Graduate" << std::endl;
std::cout << std::endl << "3. Exchange" << std::endl;

int type;
std::cout << "Type: ";
std::cin >> type;

if(type < 1 || type > 3){
    std::cout << "Invalid type";
    return;
}

std::string name;
std::cout << "Name: ";
std::getline(std::cin, name);
int age;
std::cout << "Age: ";
std::cin >> age;
int id = db.generateId();

std::unique_ptr<student> student;
if(type == 1){
    std::string major;
    std::cout << "Major: ";
    std::getline(std::cin, major);
    int credits;
    std::cout << "Credits: ";
    std::cin >> credits;

    student = std::make_unique<undergraduate>(name, age, id, major, credits);
}
else if(type == 2){
std::string thesis;
std::getline(std::cin, thesis);
student = std::make_unique<graduate>(name,age,id,thesis);
}else{
std::string country;
std::cout << "Home country: ";
std::getline(std::cin, country);
student = std::make_unique<exchange>(name,age,id,country);
}
db.add(std::move(student));
std::cout << "Added! Student ID is" << id << std::endl;
}



//MAIN FUNCTION
int main(){
database db;

bool running = true;
while(running){
    std::cout << std::endl << "Welcome to the student database!" << std::endl;
std::cout << std::endl << "1. Add student" << std::endl;
std::cout << std::endl << "2. List all students" << std::endl;
std::cout << std::endl << "3. Find student by ID" << std::endl;
std::cout << std::endl << "4. Remove student by ID" << std::endl;
std::cout << std::endl << "5. Show total tuition" << std::endl;
std::cout << std::endl << "0. Quit" << std::endl;

int choice;
std::cout << "Choice: ";
std::cin >> choice;

switch(choice){
case 1:
addStudent(db);
break;
case 2:
std::cout << std::endl;
db.listAll();
break;
case 3:{
    int id;
    std::cout << "Id to find: ";
    std::cin >> id;
    const student* found = db.findById(id);
    if(found != nullptr){
        found->printInfo();
    }else{
        std::cout << "No student with this ID";
    }
}
break;
case 4:{
    int id;
    std::cout << "Id to remove: ";
    std::cin >> id;

    if(db.removeById(id)){
        std::cout << "Removed" << std::endl;
    }else{
        std::cout << "No student with this ID";
    }
}
break;
case 5:
std::cout << "Total tuition: €" << std::fixed << std::setprecision(2) << db.totalTuition() << std::endl;
break;
case 0:
running = false;
break;
default:
std::cout << "No choice such as the user input exists." << std::endl;
}
}
    return 0;
}
