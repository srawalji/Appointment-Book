//-----------------------------------------------------------------
// CSC 252 Project 2
//
// File Name: CSC 252 Group 7 Project 2.cpp 
//
//This C++ programming project will ask a user to choose one of the five (5)
//different tasks to run by the program. The user will input D(or d), M (or m),
//O(or o),C(or c) , Q(or q). The prompt should be the following, which we will refer to as the
//“Main Menu” in this handout:
//
// Welcome to the CSC 252 Project 2 Appointment Book: 
//D(d): Add daily appointments
//M(m): Add monthly appointments
//O(o): Add a one-time appointment
//C(c): Check appointments
//Q(q): Quit program
//
//----------------------------------------------------------------- 


#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

// Function used in Time class
int remainder(int a, int n)
{
	if (a >= 0)
	{
		return a % n;
	}
	else
	{
		return n - 1 - (-a - 1) % n;
	}
}

// Time class used, but only for hours and minutes
class Time
{
public:
	Time(int hour, int min, int sec)
	{
		time_in_secs = 60L * 60 * hour + 60 * min + sec;
	}
	Time()
	{
		time_t now = time(0);
		struct tm t;
		localtime_s(&t, &now);
		time_in_secs = 60L * 60 * t.tm_hour + 60 * t.tm_min + t.tm_sec;
	}
	int get_hours() const { return time_in_secs / (60 * 60); }
	int get_minutes() const { return (time_in_secs / 60) % 60; }
	int get_seconds() const { return time_in_secs % 60; }
	int seconds_from(Time t) const { return time_in_secs - t.time_in_secs; }
	void add_seconds(int s) { time_in_secs = remainder(time_in_secs + s, 60 * 60 * 24); }
private:
	int time_in_secs;
};

// Date class
class Date
{
public:
	Date(int in_year, int in_month, int in_day)
	{
		day = in_day;
		month = in_month;
		year = in_year;
	}
	Date()
	{
		day = 1;
		month = 1;
		year = 2018;
	}
	void print() const { cout << year << "/" << month << "/" << day; }
	bool equals(Date other) const
	{
		return day == other.day && month == other.month && year == other.year;
	}
private:
	int day;
	int month;
	int year;
};

// Appointment class, will be superclass to specialized appointment classes
class Appointment
{
public:
	Appointment() {}
	Appointment(string description, Time* startTime, Time* endTime)
	{
		appointmentDescription = description;
		this->startTime = startTime;
		this->endTime = endTime;
	}

	void print2()
	{
		cout << setfill('0') << setw(2) << startTime->get_hours() << ":" << setfill('0') << setw(2) << startTime->get_minutes() << " - ";
		cout << setfill('0') << setw(2) << endTime->get_hours() << ":" << setfill('0') << setw(2) << endTime->get_minutes()
			<< "  " << appointmentDescription << endl;
	}

	//A read method for reading the start time, end time, and description of the appointment
	virtual void readAppointment()
	{
		// declare variables and prompt user for appointment details
		string details;
		string start_hours;
		string start_mins;
		string end_hours;
		string end_mins;
		string appointmentDescription1;
		string appointDescription2;
		cout << "Enter start_hours start_mins end_hours end_mins description: ";
		cin.ignore(256, '\n');
		getline(cin, details, '\n');

		//split the string to get individual details
		stringstream ss(details);
		string item;
		int pos = 1;

		ss >> start_hours >> start_mins >> end_hours >> end_mins;

		// while loop to assign post-time input into the appointment description
		while (!ss.eof())
		{
			ss >> appointmentDescription1;
			appointDescription2 += appointmentDescription1 + " ";
		}

		startTime = new Time(stoi(start_hours), stoi(start_mins), 0);
		endTime = new Time(stoi(end_hours), stoi(end_mins), 0);
		appointmentDescription = appointDescription2;
	}

	//method for  determine whether this appointment happens on this specific date. 
	// Returns true if given date is valid or not.
	virtual bool checkingDate(string &enteredDate);
private:
	string appointmentDescription;
	Time* startTime;
	Time* endTime;
};

bool Appointment::checkingDate(string &enteredDate)
{
	return true;
}



//derived class
//inherits from the base class Appointment and handles the daily appointments.
class Daily : public Appointment {
	//everything remains same as baseclass. No extra parameters
public:
	Daily(const Appointment& appointmentAddr)
	{}
	Daily() {}
	virtual bool checkingDate(string &enteredDate);
};

bool Daily::checkingDate(string &enteredDate)
{
	return true;
}




//derived class
// inherits from the base class Appointment and handles the monthly appointments.
class Monthly : public Appointment {
private:
	int dayOfAppointment;
public:
	Monthly(const Appointment& appointmentAddr) {}
	Monthly() {}
	void setDay()
	{
		cout << "Enter day of the month (dd): ";
		int dateInput;
		cin >> dateInput;
		dayOfAppointment = dateInput;
	}
	int getDay()
	{
		return dayOfAppointment;
	}
	virtual bool checkingDate(string &enteredDate);
};

bool Monthly::checkingDate(string &enteredDate)
{
	stringstream ss(enteredDate);
	string item;
	int pos = 1;

	// receives string and separates the individual variables of the date
	string y, m, d;
	ss >> y >> m >> d;

	// sees if day variable in comparison date matches a day in the appointment book
	if (dayOfAppointment == stoi(d))
	{
		return true;
	}
	else
	{
		return false;
	}
}




//derived class
//inherits from the base class Appointment and handles the one-time appointments.
class OneTime : public Appointment {
private:
	string sdate;
public:
	OneTime(const Appointment& appointmentAddr) {}
	OneTime() {}
	void setDate()
	{
		cout << "Enter the year, month, and date (yyyy mm dd): ";
		string inputDate;
		getline(cin, inputDate);
		sdate = inputDate;
	}
	string getDate()
	{
		return sdate;
	}
	virtual bool checkingDate(string &enteredDate);
};

bool OneTime::checkingDate(string &enteredDate)
{
	// checks to see if comparison date matches date of an appointment in the book
	if (sdate == enteredDate)
	{
		return true;
	}
	else
	{
		return false;
	}
}



// Main function
int main()
{
	// declare class variables
	vector<Appointment*> appointments;

	// declare switch statement variables
	char input;
	bool function = true;

	// welcome user and provide them with options for system use
	cout << "Welcome to the CSC 252 Project 2 Appointment Book\n" << "D(d): Add daily appointments\n" <<
		"M(m): Add monthly appointments\n" << "O(o): Add a one-time appointment\n" <<
		"C(c): Check appointments\n" << "Q(q): Quit program" << endl << endl;

	// use bool for parameter and use switch statement to act as menu for system
	while (function)
	{
		cout << "Daily Monthly Onetime Check Quit? (d/m/o/c/q): ";
		cin >> input;
		switch (input)
		{
		case 'D':
		case 'd':
		{
			Daily* daily = new Daily();
			daily->readAppointment();
			appointments.push_back(daily);
		}
		break;
		case 'M':
		case 'm':
		{
			Monthly* monthly = new Monthly();
			monthly->readAppointment();
			monthly->setDay();
			appointments.push_back(monthly);
		}
		break;
		case 'O':
		case 'o':
		{
			OneTime* oneTime = new OneTime();
			oneTime->readAppointment();
			oneTime->setDate();
			appointments.push_back(oneTime);
		}
		break;
		case 'C':
		case 'c':
		{
			// prompt user for a date to check appointments, and store it in a string variable
			cout << "Enter the year, month, and date (yyyy mm dd): ";
			string sdate1;
			cin.ignore();
			getline(cin, sdate1);

			cout << "The appointments on this day are: " << endl;

			for (unsigned i = 0; i < appointments.size(); i++)
			{
				// if comparison date matches any appointments in the book
				if (appointments[i]->checkingDate(sdate1) == true)
				{
					// print the appointment to the output
					appointments[i]->print2();
				}
			}
		}
		break;
		case 'Q':
		case 'q':
			cout << "Thank you for using the Appointment Book! Good Bye!" << endl;
			function = false;
			break;
		default: cout << "You have not entered a valid selection. Please try again!\n" << endl;
		}
	}
}