#include <iostream>
#include <vector>
#include <math.h> 
#include <time.h>

// template function for simple message output
template <class messageType>
void pOL(messageType message){
	std::cout << message;
}

/*
Base class "storage" has private: messages for display, and private container
for answer from user;

public containers, methods and destructor: destructor simply send "come again"
message, boolean "approved" for validating input from user, methods for send
messages input validity or result, boolean "mode" for initilizing mode of
program - number research or range research, also created a virtual empty
methods for overloading it in the derrived class and using that overloaded
methods by the base class pointer to derrived class.
*/
class storage{

    // There is no need to share this containers and methods with derrived
    // class and main program
    private:
    // messages represent as strings, they kept private because doesn't uses
    // in other parts of programm
	std::string modeHeader = "\nChoose your goal of research: ";
    std::string incorrect = "\nThis input is incorrect.\n";
    std::string comeAgain = "\nThat's all for now, run again.\n";
    
    // char container for answer letter from user kept stored privetly
	char answer;	
	
	// Main program need this containers and methods
	public:
	// true - for number, false - for range
	bool mode = true;
	// approved = true - correct input, false - incorrect input
	bool approved = true;

	// Method that show to user options of the program
	void openDialog(){
		// Move to dialog header additional messages
		modeHeader += "\n\n(s/S) - for varifing single number";
		modeHeader += " being lab,";
		modeHeader += "\n(r/R) - for showing all lab numbers";
		modeHeader += " in the given range.\n\n";
		// output message for user
		pOL(modeHeader);
		// read mode from user
		std::cin >> answer;
	}
    // Method print corresponding message about "correct" or "incorrect" input
	void validateDialog(){
		// Setting range-search mode
		if ( (answer == 's') || (answer == 'S') ) mode = true;
		// Setting range-search mode
		else if ( (answer == 'r') || (answer == 'R') ) mode = false;
		// Not allow to program work when input invalid
		else approved = false;
		// Immidiately send error message in case wrong input
		if (!approved) pOL(incorrect);
	}
	// Prepared virtual empty method for collecting number for research
	virtual void getNumber(){}
	// Prepared virtual empty method for collecting range for research
	virtual void getRange(){}
	// Virtual empty method for sening result message if base class pointer
	// will initilize it - because of keyword "virtual" derrived class overload
	// method closeDialog() will be used	
	virtual void closeDialog(){}

	// Destructor simply send message to user asking run program again 
	~storage(){
		pOL(comeAgain);
	}
};

/*
Derrived class numberResearcher contain:
private messages for user, container for collecting number;

protected method labCheck() does actual research of single number;

public methods such as getNumber() for collecting number, closeDialog() for
sending result to user and bunch of empty methods openDialog() and
validateDialog() that accessible from main program. Some methods empty because
they is overloads base non-virtual methods so main program will call base
methods.
*/
class numberResearcher : public storage
{
    // There is no need to share this containers with derrived class
    // and main program
    private:
    // messages represent as strings for number research dialogs
    std::string enterNumber = "\nEnter number for research: ";
    std::string lab = "\nYou enter a lab number.\n";
    std::string notlab = "\nYou enter not a lab number.\n";
	
	// integer container for collected number from user
	int number;
	
	// research method with bool return shared with range research class
	protected:
	// method recieve number for research and return "true" - if number lab
	bool labCheck(int x){
		// iterator stops when get square root of half researched number
		// there is no need go after this point
	    for (int i=2; i<=(int)sqrt((x/2)); i++)
	        if ( !(x%(i*i)) ) return true;
	    return false;
	}

	// Main program need this methods
	public:
	// Empty method for calling bace class openDialog()
	void openDialog(){}
	// Empty method for calling bace class validateInput()
	void validateDialog(){}
	// Method collect number from user
	void getNumber(){
		// Send simple message to user
		pOL(enterNumber);
		std::cin >> number;
	}
	// Prepared virtual empty method for collecting range for research
	virtual void getRange(){}
	// Method sends result to user
	void closeDialog(){
		// Only here research method calls for this class
		if (labCheck(number)) pOL(lab);
		else pOL(notlab);
	}
};

/*
Derrived class rangeResearcher contain:
private method for operate with inherited labCheck(), messages for user,
containers for collecting range and vector for lab numbers in the range;

protected method labCheck() does actual research of single number;

public methods such as getNumber() for collecting number, closeDialog() for
sending result to user and bunch of empty methods openDialog() and
validateDialog() that accessible from main program. Some methods empty because
they is overloads base non-virtual methods so main program will call base
methods.
*/
class rangeResearcher : public numberResearcher
{
    // There is no need to share this containers and methods with derrived
    // class and main program
	private:
	// messages represent as strings for range research dialogs
    std::string enterLrange = "\nEnter start number in the range: ";
    std::string enterRrange = "\nEnter end number in the range: ";
    std::string labRange = "\nAll lab numbers in the given range [";
    std::string back = "] are:\n\n[\n";
    std::string notlabRange = "\nNot a single number in the given range((\n";

	// integer container for begin of range
	int lRange;
	// integer container for end of range
	int rRange;
	// integer vector container for all lab numbers in the range
	std::vector<int> lab_numbers;

	// Method called labCheck() and keep lab number
	void operate(){
		for(int next=lRange; next<=rRange; next++)
			if ( labCheck(next) )
				lab_numbers.push_back(next);
	}

	// Main program need this methods
	public:
	// Empty method for calling bace class openDialog()
	void openDialog(){}
	// Empty method for calling bace class validateInput()
	void validateDialog(){}
	// Method collect range from user
	void getRange(){
		pOL(enterLrange);
		std::cin >> lRange;
		pOL(enterRrange);
		std::cin >> rRange;
	}
	// Method sends result to user
	void closeDialog(){
		// temporary integer container
		int i=0;
		// research in the range starts here
		operate();
		// send empty result vector to user
		if (lab_numbers.empty()) pOL(notlabRange);
		else {
			// send find any lab number message to user
			pOL(labRange);
			pOL(lRange);
			pOL(',');
			pOL(' ');
			pOL(rRange);
			pOL(back);
			// printing by vector iterator in the for-loop
			for (std::vector<int>::iterator it = lab_numbers.begin();
			     it!=lab_numbers.end(); it++){
				pOL(*it);
				pOL('\t');
				if(++i==6){
					pOL('\n');
					i=0;
				}
			}
			if (i) pOL('\n');
			pOL("].\n");
		}
	}
};

// Main program starting point
int main() {

	// declaring clock variable
	clock_t t;
	// Initiliazing pointer to base class
	storage *entry = new storage;
	// Initiliazing dialog with user
	entry -> openDialog();
	// Initiliazing analisis of input from user
	entry -> validateDialog();

	// single-number mode
	if ( (entry -> approved) && (entry -> mode) )
	{
		// Initiliazing pointer to numberResearcher class
		storage *check = new numberResearcher;
		// Initiliazing collecting number from user 
		check -> getNumber();
		// start clock
		t = clock();
		// Closing dialog with user 
		check -> closeDialog();
	}

	// range of the numbers mode
	if ( (entry -> approved) && !(entry -> mode) )
	{
		// Initiliazing pointer to rangeResearcher class
		storage *check = new rangeResearcher;
		// Initiliazing collecting range from user
		check -> getRange();
		// start clock
		t = clock();
		// Closing dialog with user 
		check -> closeDialog();
	}

	// deleting base class pointer
	delete entry;

	// getting time mesuare
	t = clock() - t;

	// send elapsed time for calculating to user
	pOL("\nTime spend: ");
	pOL(t*1.0/CLOCKS_PER_SEC);
	pOL(" seconds.\n");

    return 0;
}
