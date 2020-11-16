#include <iostream>
#include <deque>
#include <stack>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

//General Functions
string calcTime(int);

//Report Function List
int avgWaitingListLength(stack <int>);
double avgWaitingListTime(stack <int>);
void sortTransactions(vector <Transaction>);

class Customer
{
public:
  string firstName, lastName;
  int arrivalTime;
  Customer (string, string, int);
};

Customer::Customer (string firstName, string lastName, int arrivalTime) {
  this->firstName = firstName;
  this->lastName = lastName;
  this->arrivalTime = arrivalTime;

  cout << "Customer " << this->firstName << " " << this->lastName << " was added at " << calcTime(this->arrivalTime) << "\n";
}

class Teller
{
    public:
      Teller(string);
      string name;
      int freeTime;
      bool working;
      bool busy;
};

Teller::Teller(string name) {
    this->name = name;
    this->freeTime = -1;
    this->working = false;
    this->busy = false;
}

class Transaction
{
    public:
      Transaction(string, string, string, string, int, int);
      string fNameTransaction, lNameTransaction, typeTransaction, timeTransaction;
      void report();
      int timeTransInt;
      int amountTransaction;
};

Transaction::Transaction(string fNameTransaction, string lNameTransaction, string typeTransaction, string timeTransaction, int amountTransaction, int timeTransInt) {
    this->fNameTransaction = fNameTransaction;
    this->lNameTransaction = lNameTransaction;
    this->typeTransaction = typeTransaction;
    this->timeTransaction = timeTransaction;
    this->amountTransaction = amountTransaction;
    this->timeTransInt = timeTransInt;
    
    cout << "Transaction for " << this->fNameTransaction << " " << this->lNameTransaction << " will complete at " << this->timeTransaction << "\n";
}

bool compareTransaction(Transaction t1, Transaction t2) { 
    if (t1.lNameTransaction == t2.lNameTransaction){
        if (t1.fNameTransaction == t2.fNameTransaction){
            if (t1.amountTransaction <= t2.amountTransaction){
                return (t1.timeTransInt < t2.timeTransInt); 
            }
            return (t1.amountTransaction > t2.amountTransaction) ;
        }
        return (t1.fNameTransaction < t2.fNameTransaction); 
    }
    return (t1.lNameTransaction < t2.lNameTransaction); 
} 

void Transaction::report() {
    cout << this->fNameTransaction << " " << this->lNameTransaction << " " << this->typeTransaction << " " << this->timeTransaction << " $" << this->amountTransaction << "\n";
}

class Bank
{
    public:
      deque <Customer> customersWaiting;
      vector <Teller*> mainTellerStack;
      vector <Transaction> transactionList;
      stack <Teller*> workingTellerStack;
      stack <int> waitingListLength;
      stack <int> waitingListTime;
      int timeMarker;
      int tellerInterval;
      int lengthOfDay;
};

int main ()
{
  //Seed random   
  srand(time (0));
  
  //Create fName and lName lists and vectors
  static const string fNameList[] = { 
    "John", "Lisa", "Carl", "Frank",
    "Bob", "Jill", "Karen", "Walter",
    "Jim", "Jack", "Sally", "Julie",
    "Erica", "Francis", "Daesha", "Lukas",
    "Peter", "Mozart", "Moe", "Wes",
  };
  
  static const string lNameList[] = { 
    "Johnson", "Smith", "Gamel", "Donner",
    "Frankson", "Wooser", "Shephard", "Oreilly",
    "Jackson", "Thompson", "Booker", "Washington",
    "King", "Stewart", "Benjamin", "Vasquez",
    "Ramirez", "Doe", "Carlson", "Park",
  };

  vector<string> fNameVector (fNameList, fNameList + sizeof(fNameList) / sizeof(fNameList[0]));
  vector<string> lNameVector (lNameList, lNameList + sizeof(lNameList) / sizeof(lNameList[0]));
  
  //Initialize bank
  Bank b;
  
  //Create tellers and add to Bank
  Teller t1("t1"), t2("t2"), t3("t3");
  b.mainTellerStack.push_back(&t1);
  b.mainTellerStack.push_back(&t2);
  b.mainTellerStack.push_back(&t3);
  
  //Set Teller tellerInterval
  b.tellerInterval = 3;
	
  //Set length of day	
  b.lengthOfDay = 100;

  //Variable to determine if new customer has arrived
  int newCustomer = 0;
  int randInt = 0;
  
  //Variables for transactions
  int tmpTransaction = 0;
  int tmpTransTime = 0;
  int amountTrans = 0;
  string tmpTransType = "";
  
  //Name variables
  string randFName = "";
  string randLName = "";

  //Increment through day (10:00 AM - 1:00 PM = 180 minutes)
  for (int i = 0; i < b.lengthOfDay; i++)
  {
    cout << i << ":\n";

    //Check for new Customer
    newCustomer = 1 + rand () % 2;

    if (newCustomer == 2){
      //Create random name  
      randInt = rand () % fNameVector.size ();
      randFName = fNameVector[randInt];
      
      randInt = rand () % lNameVector.size ();
      randLName = lNameVector[randInt];
      
      Customer tmpCustomer (randFName, randLName, i);
      b.customersWaiting.push_back(tmpCustomer);
    }

    cout << "Size of waiting list: " << b.customersWaiting.size() << "\n";
    b.waitingListLength.push(b.customersWaiting.size());
    cout << "Teller stack size: " << b.workingTellerStack.size() << "\n\n";
	
    //Free up tellers 
    for(int j = 0; j < b.mainTellerStack.size(); j++) {
      if(b.mainTellerStack[j]->freeTime == i && b.mainTellerStack[j]->busy && b.mainTellerStack[j]->working){
        b.mainTellerStack[j]->busy = false;
        b.mainTellerStack[j]->freeTime = -1;
        cout << "Teller " << b.mainTellerStack[j]->name << " has finished transaction\n";
      }
    }
	
	  //Add Tellers
	  if((b.customersWaiting.size() > 0 && b.workingTellerStack.size() < 1) || (b.customersWaiting.size() > 3 && b.workingTellerStack.size() < 2) || (b.customersWaiting.size() > 7 && b.workingTellerStack.size() < 3)){
      //Check for first non-working teller
      for(int j = 0; j < b.mainTellerStack.size(); j++) {
        if(!b.mainTellerStack[j]->working){
          b.mainTellerStack[j]->working = true;
          b.workingTellerStack.push(b.mainTellerStack[j]);
          cout << "Teller " << b.workingTellerStack.top()->name << " was added\n";
          break;
        }
      }
    }

    //Remove Tellers if necessary and not busy
    if((b.customersWaiting.size() < 4 && b.workingTellerStack.size() >= 2) || (b.customersWaiting.size() < 8 && b.workingTellerStack.size() >= 3)){
      if(!b.workingTellerStack.top()->busy && b.workingTellerStack.top()->working && b.workingTellerStack.top()->freeTime == -1){
          b.workingTellerStack.top()->working = false;
          cout << "Teller " << b.workingTellerStack.top()->name << " was removed\n";
          b.workingTellerStack.pop();
      }
    }

    //Assign Customer to free teller 
    for(int j = 0; j < b.mainTellerStack.size(); j++) {
      if(b.mainTellerStack[j]->working && !b.mainTellerStack[j]->busy){
            
        //Skip if no customers waiting
        if(b.customersWaiting.size() == 0){
          cout << "No customers available at this time\n\n";
          break;
        }

        //Figure Customer transaction 
        tmpTransaction = newCustomer = 1 + rand () % 4;
        tmpTransTime = newCustomer = 1 + rand () % 10;
        
        if(i + tmpTransTime > b.lengthOfDay - 1){
          cout << "Not enough time for transaction\n";
          break;
        }
        
        //Set Teller to busy
        b.mainTellerStack[j]->busy = true;
        
        //Set customer wait time
        b.waitingListTime.push(i - b.customersWaiting[0].arrivalTime);
        
        b.mainTellerStack[j]->freeTime = i + tmpTransTime;
        
        cout << "Customer " << b.customersWaiting[0].firstName << " " << b.customersWaiting[0].lastName << " Attended by " << b.mainTellerStack[j]->name << "\n";
        
        switch(tmpTransaction) {
          case 1:
            cout << "\tOpen Account transaction\n";
            tmpTransType = "Open Account";
            break;
          case 2:
            cout << "\tClose Account transaction\n";
            tmpTransType = "Close Account";
            break;
          case 3:
            cout << "\tDeposit transaction\n";
            tmpTransType = "Deposit";
            amountTrans =  1 + rand () % 10000;
            break;
          case 4:
            cout << "\tWithdrawal transaction\n";
            tmpTransType = "Withdrawal";
            amountTrans = 1 + rand () % 10000;
            amountTrans *= -1;
            break;
          default:
            cout << "\tError\n";
        }
        
        cout << "\tWill finish on " << calcTime(b.mainTellerStack[j]->freeTime) << "\n";
        
        Transaction tmpTransaction(b.customersWaiting[0].firstName, b.customersWaiting[0].lastName, tmpTransType, calcTime(b.mainTellerStack[j]->freeTime), amountTrans, b.mainTellerStack[j]->freeTime);
        b.transactionList.push_back(tmpTransaction);
        b.customersWaiting.pop_front();
        amountTrans = 0;
      }
    }
     
      cout << "\n\n";
    }
    
    cout << "\n\nDay Ended with " << b.transactionList.size() << " Transactions\n";
    cout << "Average Waiting List Length: " << avgWaitingListLength(b.waitingListLength) << "\n";
    cout << "Average Waiting List Time: " << avgWaitingListTime(b.waitingListTime) << "\n";
    sortTransactions(b.transactionList);

  return 0;
}


string calcTime(int i) {
    string tmpString = "Time " + to_string(i);
    return tmpString;
}

int avgWaitingListLength(stack <int> waitingListLength){
    int length = waitingListLength.size();
    int total = 0;
    double average = 0;
    
    while(!waitingListLength.empty()){
        total += waitingListLength.top();
        waitingListLength.pop();
    }
    average = (double)total/length;
    return round(average);
}

double avgWaitingListTime(stack <int> waitingListTime){
    int length = waitingListTime.size();
    int total = 0;
    double average = 0;
    
    while(!waitingListTime.empty()){
        total += waitingListTime.top();
        waitingListTime.pop();
    }
    average = (double)total/length;
    return average;
}

void sortTransactions(vector <Transaction> transactionList){
    cout << "\n\nSorted Transactions:\n";
    sort(transactionList.begin(),transactionList.end(),compareTransaction);
    
    for(int i = 0; i < transactionList.size(); i++){
        transactionList[i].report();
    }
}
