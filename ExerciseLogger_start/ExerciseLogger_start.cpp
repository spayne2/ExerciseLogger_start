#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
const string FILENAME = "exercise_logger.txt";
enum ExerciseType //ENUM for labelling records int the rext file
{
	RUN,
	WEIGHT_LIFTING
};
//Abstract exercise class
class Exercise
{
public:
	virtual void display() = 0; //for displaying the exercises
	virtual string getDataForFile() = 0;//to get the data formatted for the file. When overiding this function, write the data to one line all to string
	virtual void addData() = 0; //for accepting user input
	virtual void loadDataFromFile(ifstream&) = 0; //for reading the data from the file
};
//run class to represent a run object.
class Run : public Exercise
{
protected:
	double distanceInKm; //overall distnace of the run
	int timeMins; // time in minutes 
	int timeSeconds; //time in seconds
	string terrain; //the terrain type of the run e.g. road, track, trail

public:
	Run() { distanceInKm = 0; timeMins = 0; timeSeconds = 0; terrain = ""; }
	//function to ask user to add data to run, called from wherever a new run object is created
	void addData()
	{ //get data from user input
		cout << "Type in distance" << endl;
		cin >> distanceInKm;
		cout << "Type in minutes" << endl;
		cin >> timeMins;
		cout << "Type in seconds" << endl;
		cin >> timeSeconds;
		cout << "Type in terrain type" << endl;
		cin.ignore(1000, '\n');
		cin.clear();
		getline(cin, terrain);
	}
	//function to caclculate the average pace of the run, to be called when the runs are displayed
	string calculateAvgPace()
	{
		int fullSeconds = (timeMins * 60) + timeSeconds;

		int secondsPerKm = fullSeconds / distanceInKm;

		double minutesPerKm = secondsPerKm / 60;
		int leftOverSeconds = secondsPerKm % 60;

		return to_string(minutesPerKm) + "'" + to_string(leftOverSeconds) + "\"";
	}
	//function to display the run information
	void display()
	{
		cout << "Run: Disance: " << distanceInKm << "Kms, Time " << timeMins << "mm:" << timeSeconds << "ss, Pace: " << calculateAvgPace() << " Terain: " << terrain << endl;
	}

	string getDataForFile()
	{				//record is labelled with the enum
		return to_string(ExerciseType::RUN) + " " + to_string(distanceInKm) + " " + to_string(timeMins) + " " + to_string(timeSeconds) + " " + terrain;
	}

	void loadDataFromFile(ifstream& file)
	{
		file >> distanceInKm; //get the distance from the file
		file >> timeMins; //get the minutes from the file
		file >> timeSeconds; //get the time in seconds from the file
		getline(file, terrain); //get the terrain (getline so it can have spaces)
	}
};

class Weightlifting : public Exercise
{
private:
	int weight; //weights being lifted
	int reps; //reps per set
	int sets; //number of sets
	string type; //type of exercise

	float calculateTotalWeight() //cumalative total weight
	{
		return weight * (reps * sets);
	}
public:
	Weightlifting() { weight = 0; reps = 0; sets = 0; type = ""; }; //default constructor
	void addData() //function to allow user to type in information about the weightlifting
	{
		cout << "Type in weight in KG's" << endl;
		cin >> weight;
		cout << "Type in reps" << endl;
		cin >> reps;
		cout << "Type in sets" << endl;
		cin >> sets;
		cout << "Type in type" << endl;
		cin.ignore(1000, '\n');
		cin.clear();
		getline(cin, type);
	}
	void display() //function to display the weight lifting stats
	{
		cout << "WeightLifting: Weight: " << weight << " KG's reps: " << reps << " sets:" << sets << " Total Weight: " << calculateTotalWeight() << "KG's Type: " << type << endl;
	}
	string getDataForFile() //function to get the data in the file format ready to be written to the file
	{					//record is labelled with the enum
		return to_string(ExerciseType::WEIGHT_LIFTING) + " " + to_string(weight) + " " + to_string(reps) + " " + to_string(sets) + " " + type;
	}
	void loadDataFromFile(ifstream& file)
	{
		file >> weight; //get the weight from the file
		file >> reps; //get the reps from the file
		file >> sets; //get the sets from the file
		getline(file, type); //get the type
	}
};
//class to represent a person object
class Person
{
private:
	string name; //store the name
	int age; //store the age
	vector<Exercise*> exercises; //store a list of exercises; //composition
public:
	Person() { age = 0; }

	void addExercise() //function to add exercises for a user
	{
		cout << "What type of exercise ?" << endl;
		cout << "1. Run " << endl;
		cout << "2. Weight Lifting" << endl;
		int userInput;
		cin >> userInput;
		if (userInput == 1)
		{
			cout << "Add info of most recent run" << endl;
			Run* run = new Run; //initiliase
			run->addData(); //add data
			exercises.push_back(run); //add to the vector
		}
		else if (userInput == 2)
		{
			cout << "Add info of most recent weightlifting" << endl;
			Weightlifting* wl = new Weightlifting; //initiliase
			wl->addData(); //add data
			exercises.push_back(wl); //add to the vector
		}
	}

	void addData() //function to add data from the user to the person object
	{
		cout << "Type in persons name" << endl;
		cin.ignore(1000, '\n');
		cin.clear();
		getline(cin, name);
		cout << "Type in persons age" << endl;
		cin >> age;
		addExercise();
	}
	//function to return the name of the person
	string getName()
	{
		return name;
	}
	//function to return the age of the person
	int getAge()
	{
		return age;
	}
	//function to return the exercises that the person has stored
	vector<Exercise*> getExercises()
	{
		return exercises;
	}
	//function to print the ecercises to the console;
	void printExercises()
	{
		for (int i = 0; i < exercises.size(); i++)
		{
			cout << "Exercise " << (i + 1) << endl;
			exercises[i]->display();
		}
		cout << endl;
	}
	//display function to show all user infor
	void display()
	{
		cout << "Name: " << name << " Age: " << age << endl;
		cout << "Exercises: " << endl;
		printExercises();
	}
	//edit function to edit the users info
	void edit()
	{
		cout << "Type in new name" << endl;
		cin.ignore(1000, '\n'); //clears the input buffer before using getline
		cin.clear();
		getline(cin, name);

		cout << "Type in new age" << endl;
		cin >> age;
	}

	void loadDataFromFile(ifstream& file)
	{
		getline(file, name); //get the name, allows for spaces
		file >> age; //get the age


		int exerciseCount;
		file >> exerciseCount; //get the number of exercises from the file
		file.ignore(1000, '\n');
		file.clear();
		for (int i = 0; i < exerciseCount; i++)
		{

			int exerciseType;
			file >> exerciseType;
			switch (exerciseType)
			{
			case ExerciseType::RUN:
			{
				Run* run = new Run;
				run->loadDataFromFile(file); //call the load data from file for run
				exercises.push_back(run); //add the run to the vector
				break;
			}
			case ExerciseType::WEIGHT_LIFTING:
			{
				Weightlifting* wl = new Weightlifting;
				wl->loadDataFromFile(file); //call the load data from file for run
				exercises.push_back(wl); //add the run to the vector
				break;
			}

			default:
				break;
			}
			//assume there will always be one run

		}
	}
};

//Menu class, controls the program running, holds a vector of persons
class Menu
{
private:
	vector<Person> persons; //vector to hold all the persons // composition (sort of)
	bool keepMenuGoing = true;
	//function to print person record
	void printPersonVector()
	{
		for (int i = 0; i < persons.size(); i++)
		{
			cout << (i + 1) << " " << persons[i].getName() << endl;
		}
	}
	//function to add a new person
	void addNewPerson()
	{
		Person person; //initialize a person object
		person.addData(); //call the add data function;
		persons.push_back(person); // add to the vector stored in the menu object
	}

	//function to add a new run for a user, called from the main menu
	void addNewExercise()
	{
		cout << "Add exercise for which person?" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].addExercise();
	}

	//function to show person, called from the main menu
	void showPerson() {
		cout << "Display which person" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].display();
	}
	//function to edit person, called from the main menu
	void editPerson() {
		cout << "Edit which person" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].edit();
	}
	//function for writing all the info to a file
	void writeToFile()
	{
		ofstream outPutFile; //create output file objec
		outPutFile.open(FILENAME); //open the file wanted
		outPutFile << persons.size() << endl; //write the number of people to the file
		for (int i = 0; i < persons.size(); i++) //loop over all the person records
		{
			outPutFile << persons[i].getName() << endl; //send the name to the file 
			outPutFile << persons[i].getAge() << endl; //send the age to the file
			outPutFile << persons[i].getExercises().size() << endl; //write the number of exercises to the file
			for (auto exercise : persons[i].getExercises()) //send all the runs to the file
			{
				outPutFile << exercise->getDataForFile() << endl;;
			}
		}
	}

	void loadFromFile()
	{
		ifstream inputFile;
		inputFile.open(FILENAME); //open  file needed
		if (inputFile)
		{
			int personCount = 0;
			inputFile >> personCount; //get the number of people from the file
			inputFile.ignore(1000, '\n');
			inputFile.clear();
			for (int i = 0; i < personCount; i++)
			{
				Person person; //create a person object
				person.loadDataFromFile(inputFile);  //load data from file
				persons.push_back(person); //add the person to the vector
			}
		}
	}

	void saveAndQuit()
	{
		writeToFile();
		keepMenuGoing = false;
		cout << "Goodbye" << endl;
	}

public:
	//function to display the start menu
	void start()
	{
		loadFromFile();

		while (keepMenuGoing)
		{
			cout << "Welcome to the Exercise logger, please choose an option" << endl;
			cout << "1. Add a new person" << endl;
			cout << "2. Add a new Exercise" << endl;
			cout << "3. Show Person" << endl;
			cout << "4. Edit Person" << endl;
			cout << "5. Write to File" << endl;
			cout << "6. Load from file" << endl;
			cout << "7. Save & Quit" << endl;

			int input;
			cin >> input;

			switch (input)
			{
			case 1:
				addNewPerson();
				break;
			case 2:
				addNewExercise();
				break;
			case 3:
				showPerson();
				break;
			case 4:
				editPerson();
				break;
			case 5:
				writeToFile();
				break;
			case 6:
				loadFromFile();
				break;
			case 7:
				saveAndQuit();
				break;
			}
		}
	}
};

int main()
{
	//initilize menu object
	Menu m;
	m.start(); //start the menu;
}


