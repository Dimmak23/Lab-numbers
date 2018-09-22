#include <iostream>
#include <vector>
#include <math.h> 
#include <time.h>

template <typename T>
void pOL(T word){
	std::cout << word;
}
template <typename T, typename... Args>
void pOL(T first, Args... args){
	std::cout << first;
	pOL(args...);
}

class storage {
    private:
	std::string modeHeader = "\nChoose your goal of research: ";
    std::string incorrect = "\nThis input is incorrect.\n";
    std::string comeAgain = "\nThat's all for now, run again.\n";
	char answer;	

	public:
	bool mode = true;
	bool approved = true;

	void openDialog(){
		modeHeader += "\n\n(s/S) - for varifing single number";
		modeHeader += " being lab,";
		modeHeader += "\n(r/R) - for showing all lab numbers";
		modeHeader += " in the given range.\n\n";
		pOL(modeHeader);
		std::cin >> answer;
	}

	void validateDialog(){
		if ( (answer == 's') || (answer == 'S') ) mode = true;
		else if ( (answer == 'r') || (answer == 'R') ) mode = false;
		else approved = false;
		if (!approved) pOL(incorrect);
	}

	virtual void getNumber(){}
	virtual void getRange(){}
	virtual void closeDialog(){}

	~storage(){
		pOL(comeAgain);
	}
};

class numberResearcher : public storage {
    private:
    std::string enterNumber = "\nEnter number for research: ";
    std::string lab = "\nYou enter a lab number.\n";
    std::string notlab = "\nYou enter not a lab number.\n";
	int number;

	protected:
	bool labCheck(int x){
	    for (int i=2; i<=(int)sqrt((x/2)); i++)
	        if ( !(x%(i*i)) ) return true;
	    return false;
	}

	public:
	void openDialog(){}
	void validateDialog(){}
	void getNumber(){
		pOL(enterNumber);
		std::cin >> number;
	}
	virtual void getRange(){}
	void closeDialog(){
		if (labCheck(number)) pOL(lab);
		else pOL(notlab);
	}
};

class rangeResearcher : public numberResearcher {

	private:
    std::string enterLrange = "\nEnter start number in the range: ";
    std::string enterRrange = "\nEnter end number in the range: ";
    std::string labRange = "\nAll lab numbers in the given range [";
    std::string back = "] are:\n\n[\n";
    std::string notlabRange = "\nNot a single number in the given range((\n";
	int lRange;
	int rRange;
	std::vector<int> lab_numbers;

	void operate(){
		for(int next=lRange; next<=rRange; next++)
			if ( labCheck(next) )
				lab_numbers.push_back(next);
	}

	public:
	void openDialog(){}
	void validateDialog(){}
	void getRange(){
		pOL(enterLrange);
		std::cin >> lRange;
		pOL(enterRrange);
		std::cin >> rRange;
	}
	void closeDialog(){
		int i=0;
		operate();
		if (lab_numbers.empty()) pOL(notlabRange);
		else {
			pOL(labRange, lRange, ',', ' ', rRange, back);
			for (std::vector<int>::iterator it = lab_numbers.begin();
			     it!=lab_numbers.end(); it++){
				pOL(*it, '\t');
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

int main() {
	clock_t t;

	storage *entry = new storage;

	entry -> openDialog();

	entry -> validateDialog();

	if ( (entry -> approved) && (entry -> mode) ) {
		storage *check = new numberResearcher;
		check -> getNumber();
		t = clock();
		check -> closeDialog();
	}

	if ( (entry -> approved) && !(entry -> mode) ) {
		storage *check = new rangeResearcher;
		check -> getRange();
		t = clock();
		check -> closeDialog();
	}

	delete entry;

	t = clock() - t;

	pOL("\nTime spend: ", t*1.0/CLOCKS_PER_SEC, " seconds.\n");

    return 0;
}
