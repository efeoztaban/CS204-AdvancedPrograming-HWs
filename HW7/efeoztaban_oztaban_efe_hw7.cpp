#include <iostream>
#include <string>

using namespace std;

class Human
{                   // this is the human class with age and name informations
public:

	Human(){}
	Human(string new_name, int new_age) : name(new_name), age(new_age){}


	virtual void printInfo()       // virtual because derived classes have the same function as well
	{
	cout<< endl << "Human informations" << endl;
	cout<< "Name: " << name << endl;
	cout<< "Age: " << age << endl;
	}

protected:

	string name;
	int age;

};

class Muggle: public Human
{                            // muggle class derived from human class which has job information as addition
public:

	Muggle(){}
	Muggle(string new_name, int new_age, string new_job): Human(new_name, new_age), job(new_job){}

	virtual void printInfo()
	{
	cout<< "Muggle Informations" << endl;
	cout<< "Name: " << name << endl;
	cout<< "Age: " << age << endl;
	cout<< "Job: " << job << endl;
	}

protected:

	string job;

};

class Wizardkind: public Human
{                                  // wizarkind class derived from human class which has wand information as addition
public:

	Wizardkind(){}
	Wizardkind(string new_name, int new_age, string new_wand): Human(new_name, new_age), wand(new_wand){}

	virtual void printInfo()
	{
	cout<< "Wizarkind Informations" << endl;
	cout<< "Name: " << name << endl;
	cout<< "Age: " << age << endl;
	cout<< "Wand: " << wand << endl;
	}

	void doSpell(string spell_name)        // all wizarkind objects or objects from derived classes of wizarkind can accses that function
	{
		cout<< name << " used spell: " << spell_name << endl;
	}

protected:

	string wand;

};

class Student: public Wizardkind
{                               // student class which is derived from wizarkind class that was derived from human class
public:

	Student(){}
	Student(string new_name, int new_age, string new_wand, string new_pet, string new_house): Wizardkind(new_name, new_age, new_wand), pet(new_pet), houseName(new_house){}

	virtual void printInfo()
	{
	cout<< "Student Wizarkind Informations" << endl;
	cout<< "Name: " << name << endl;
	cout<< "Age: " << age << endl;
	cout<< "House: " << houseName << endl;
	cout<< "Wand: " << wand << endl;
	cout<< "Pet: " << pet << endl;
	}

	void feedPet()
	{
		cout << name << " fed " << pet << endl;
	}

protected:   // has pet and house information in addition to wizarkind object informations

	string pet;
	string houseName;

};

class GrownUp: public Wizardkind
{                // grownup class which is derived from wizarkind class which was derived from human class
public:

	GrownUp(){}
	GrownUp(string new_name, int new_age, string new_wand, string new_job): Wizardkind(new_name, new_age, new_wand), job(new_job){}

	virtual void printInfo()
	{
	cout<< "Grownup Wizarkind Informations" << endl;
	cout<< "Name: " << name << endl;
	cout<< "Age: " << age << endl;
	cout<< "Wand: " << wand << endl;
	cout<< "Job: " << job << endl;
	}


protected:   // has job info in addition to wizarkind object informations

	string job;

};


int main(){
Muggle vernon("Vernon Dursley", 50, "Director at Grunnings");
vernon.printInfo();
cout << endl;
Student Harry("Harry Potter", 16, "Phoenix Feather", "Hedwig", "Gryffindor");
Harry.printInfo();
cout << endl;
GrownUp Dumbledore("Albus Dumbledore", 110, "Elder Wand", "Headmaster");
Dumbledore.printInfo();
cout << endl;
Dumbledore.doSpell("Expecto Patronum");
cout << endl;
Harry.doSpell("Expelliarmus");
cout << endl;
Harry.feedPet();
cout << endl;
Student Ginny("Ginny Weasley", 15, "Yew wood", "Arnold", "Gryffindor");
GrownUp Snape("Severus Snape", 35, "Dragon Heartstring", "Potion Master");
Student Hermione("Hermione Granger", 16, "Vine", "Crookshanks", "Gryffindor");

Human hArray[6];
hArray[0] = vernon;
hArray[1] = Harry;
hArray[2] = Dumbledore;
hArray[3] = Ginny;
hArray[4] = Snape;
hArray[5] = Hermione;
Human * hPtr;
for (int i = 0; i < 6; i++) {
cout << endl;
hPtr = &hArray[i];
hPtr->printInfo();
}
cout << endl;
system("pause");

return 0;
}