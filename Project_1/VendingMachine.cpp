/**
 * Title: VendingMachine.cpp
 * Auther: William Svea-Lochert
 * Abstract: This is a vending machine simulator. a user can by a bottle of water,
 *           a bottle of coffee, a chip, and a chocolate bar. The user can then
 *           choose to pay with either debit (pin 7777) or with cash.
 * 
 *          a admin can refill the machine and reset the machine, but he can not reset
 *          the machine id or the name of the machine
 * ID: 1112
 * Date: 10.11.2018
 * */

#include <iostream>
#include <string>
#include <list>
#include <map>
#include<limits>
#include<sstream>
#include <iomanip>
using namespace std;

const int QUEUE_CAPACITY = 10;
typedef int QueueElement;

class VendingMachine
{
public:
// Constructors //
VendingMachine();
VendingMachine(int number, string name);

// return type string //
string display();
string doublePrinter(double value);

// Void //
void displayMenu();
void status();
void setNumber(int setNumber);
void setName(string setName);
void fill();
void deselectNoPrint();
void displayReceipt();
void deselect();
void productSold();
void reset();
void clearCin();

// Return type bool //
bool refill(int water, int coffee, int chip, int chocolate);
bool selectItem();
bool payment();
bool selectCheck();
bool select(string productName, int index, bool checker, int amount);

//Product structure //
struct product;

private:
    int machineNumber;
    string machineName;
    const int tax = 10;
    list<product> products;
        // index 0 = water, index 1 = Coffee, index 2 = chip, index 3 = Chocolate //
    int selectList[4] = {0, 0, 0, 0};
    double total = 0.0;
    double totalEarnings = 0;
};

VendingMachine::VendingMachine()
: machineNumber(0), machineName("UNKNOWN")
{
    fill();
}

VendingMachine::VendingMachine(int number, string name)
: machineNumber(number), machineName(name)
{
    fill();
}

//product struct
struct VendingMachine::product
{
    string pName;
    int remaining;
    int sold;
    double price;
};

//product defult filler method
void VendingMachine::fill()
{
    product water = {"Water bottle", 0, 0, 1.50};
    product coffee = {"Regular coffee", 0, 0, 2.00};
    product chip = {"Sun Chip", 0, 0, 1.00};
    product chocolate = {"Chocolate Bar", 0, 0, 2.50};
    
    products.push_back(water);
    products.push_back(coffee);
    products.push_back(chip);
    products.push_back(chocolate);
}

//resets the vending machine//
void VendingMachine::reset()
{
    totalEarnings = 0;   
    for (product &product : products)
    {
        product.remaining = 0;
        product.sold = 0;
        
    }
}

//set Machine number//
void VendingMachine::setNumber(int setNumber)
{
    machineNumber = setNumber;
}

//set Machine Name//
void VendingMachine::setName(string setName)
{
    machineName = setName;
}

//display name and number of machine//
string VendingMachine::display()
{
    return "Number: " + to_string(machineNumber) + ", Name: " + machineName + "\n";
}

//display the status of the machine//
void VendingMachine::status()
{
    
    cerr << "Sold: ";
    for (auto v : products)
    {
        cerr << v.pName << ": " << v.sold << " / ";
    }
    cerr << "\nRemaining: ";
    for (auto v : products)
    {
        cerr << v.pName << ": " << v.remaining << " / ";
    }
    cerr << endl;
    cerr << "Total Earning: $" << doublePrinter(totalEarnings) << endl; 
}

//function to refill machine//
bool VendingMachine::refill(int water, int coffee, int chip, int chocolate)
{
    int refiller[4] = {water, coffee, chip, chocolate};
    int itterator = 0;
    for (product &product : products)
    {
        product.remaining += refiller[itterator];
        itterator++;
    }
    return true;
}

//Changes remaining and sold values in structs//
void VendingMachine::productSold(){
    for (product &product : products)
    {
        if(product.pName == "Water bottle")
        {
            product.remaining -= selectList[0];
            product.sold += selectList[0];
        }
        else if(product.pName == "Regular coffee")
        {
            product.remaining -= selectList[1];
            product.sold += selectList[1];
        }
        else if(product.pName == "Sun Chip")
        {
            product.remaining -= selectList[2];
            product.sold += selectList[2];
        }
        else if(product.pName == "Chocolate Bar")
        {
            product.remaining -= selectList[3];
            product.sold += selectList[3];
        }
    }
}

//displays machine menu//
void VendingMachine::displayMenu()
{
    cerr << "===== Vending Machine Menu =====" << endl;
    cerr << "   1.  Water............$1.50\n" << "   2.  Regular Coffee...$2.00\n"
         << "   3.  Sun Chip.........$1.00\n" << "   4.  Chocolate Bar....$2.50" << endl;
}

bool VendingMachine::select(string productName, int index, bool checker, int amount)
{
    clearCin();
    cout << "How many do you want to buy? ";
    cin >> amount;
    clearCin();
    
    //--itterate through products list--//
    for (product &product : products)
    {
        if(product.pName == productName)
        {
            if(amount > 0 && amount <= product.remaining && selectList[index] < product.remaining)
            {
                selectList[index] += amount;
                total += product.price*amount;
                cout << "You have seleted " << productName <<" (" << amount << ")" << endl;
                checker = true;
                return checker;
            }
            else if(amount > product.remaining || selectList[index] >= product.remaining )
            {
                cerr << "You selected " << productName << " (" << amount << ") – Sorry. We don’t have enough " << productName << "s\n" << endl;
                checker = true;
                return checker;
            } 
            else
            {
                cerr << "invalid input" << endl;
            }
        }
    }
    return false;
}

//let user select items//
bool VendingMachine::selectItem()
{
    bool inputCheck = false;
    bool contCheck = false;
    bool check = false;
    int selectedItem;
    int amount = 0;
    total = 0.0;
    string continueOption = {0};
    
    while(!inputCheck)
    {

        while(!check)
        {
            cout << "Select Item: ";
            cin >> selectedItem;
            
            //--Check if the item selected is water--//
            if(selectedItem == 1)
            {
                string water = "Water bottle";
                select(water, 0, check, amount);
                break;
            }
            //--Check if the item selected is Coffee--//
            else if(selectedItem == 2)
            {
                string coffee = "Regular coffee";
                select(coffee, 1, check, amount);
                break;
            }
            
            //--Check if the item selected is sun chips--//
            else if(selectedItem == 3)
            {
                string chip = "Sun Chip";
                select(chip, 2, check, amount);
                break;
            }
            
            //--Check if the item selected is Chocolate bar--//
            else if(selectedItem == 4)
            {
                string chocolateBar = "Chocolate Bar";
                select(chocolateBar, 3, check, amount);
                break;
            } 
            else
            {
                cerr << "error enter valid product\n" << endl;
                clearCin();
            }  
        }
        
        while(!contCheck)
        {
            cerr << "Continue? (Y/N) ";
            cin >> continueOption;
            clearCin();
            cout << endl;
            
            if(continueOption.length() == 1 && toupper(continueOption[0]) == 'N')
            {
                inputCheck = true;
                contCheck = true;
                break;
            }
            
            else if(continueOption.length() == 1 && toupper(continueOption[0]) == 'Y')
            {
                check = false;
                inputCheck = false;
                contCheck = true;
                break;
            }
        }
        contCheck = false;
    }
    return false;
}

bool VendingMachine::selectCheck()
{
    if(selectList[0] == 0 && selectList[1] == 0 && selectList[2] == 0 && selectList[3] == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//runs the payment sequence//
bool VendingMachine::payment()
{
    string paymentOption = {0};
    string pin ="";
    double amountPayed;
    double taxOnPrice = (total/100)*tax;
    double sum = total + taxOnPrice;
    
    if(selectCheck() == true)
    {
        return false;
    }
    
    cout << "Payment Option – Debit(1) / Cash(2): ";
    cin >> paymentOption;
    clearCin();
    
    if(paymentOption.length() == 1 && paymentOption[0] == '1')
    {
        cout << "Enter PIN: ";
        cin >> pin;
        clearCin();
        
        if(pin.length() == 4 && pin == "7777")
        {
            cout << "Valid" << endl;
            return true;
        }
        else
        {
           //deselects every item without printing what was deselected//
           deselectNoPrint();
            return false;
        }
        
    } 
    else if(paymentOption.length() == 1 && paymentOption[0] == '2')
    {
            cout << "Total Price with tax: $" << doublePrinter(sum) << endl;
            cout << "Enter Money Amount: ";
            
            char dollar;
            cin >> dollar;
            
            if(dollar != '$')
            {
                return false;
            }
            
            cin >> amountPayed;
            clearCin();
            
            if(amountPayed >= sum)
            {
                double change = amountPayed - sum;
                cout << "Change: $" <<doublePrinter(change) << endl;
                return true;
            }
            else
            {
                return false;
            }
    }
    
}

//deselects everything without printing anything to the console//
void VendingMachine::deselectNoPrint()
{
    selectList[0] = 0;
    selectList[1] = 0;
    selectList[2] = 0;
    selectList[3] = 0;
    total = 0.0;
}

//clear Cin
void VendingMachine::clearCin()
{
        cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

//displays a receipt for the customer//
void VendingMachine::displayReceipt()
{
    double taxOnPrice = (total/100)*tax;
    double sum = total + taxOnPrice;
    
    for (product &product : products)
    {
        if(selectList[0] > 0 && product.pName == "Water bottle")
        {
            cout << product.pName << ": $" << doublePrinter(product.price) << " X " << selectList[0] << " = $" << doublePrinter(product.price*selectList[0]) << endl;
        }
        if(selectList[1]> 0 && product.pName == "Regular coffee")
        {
            cout << product.pName << ": $" << doublePrinter(product.price) << " X " << selectList[0] << " = $" << doublePrinter(product.price*selectList[0]) << endl;
        }
        if(selectList[2]> 0 && product.pName == "Sun Chip")
        {
            cout << product.pName << ": $" << doublePrinter(product.price) << " X " << selectList[0] << " = $" << doublePrinter(product.price*selectList[0]) << endl;
        }
        if(selectList[3]> 0 && product.pName == "Chocolate Bar")
        {
            cout << product.pName << ": $" << doublePrinter(product.price) << " X " << selectList[0] << " = $" << doublePrinter(product.price*selectList[0]) << endl;
        }
    }
    cout << "Subtotal: $" << doublePrinter(total) << endl;
    cout << "Tax (" << tax << ".0%) : $" << doublePrinter(taxOnPrice) << endl;
    cout << "Total: $" << doublePrinter(sum) << endl;
    totalEarnings += sum;
    productSold();
    deselectNoPrint();
}

//deselects item and prints a message to the console//
void VendingMachine::deselect()
{
    int counter = 0;
    cout << "You de-selected" << endl;
    for(int i = 0; i < 4; i++)
    {
        if(selectList[i] > 0)
        {
            if(i == 0)
            {
                cout << "Water (" << selectList[i] << ")" << endl;
                selectList[i] = 0;
            }
            else if(i == 1)
            {
                cout << "Coffee (" << selectList[i] << ")" << endl;
                selectList[i] = 0;
            }
            else if(i == 2)
            {
                cout << "Sun Chip (" << selectList[i] << ")" << endl;
                selectList[i] = 0;
            }
            else if( i == 3)
            {
                cout << "Chocolate Bar (" << selectList[i] << ")" << endl;
                selectList[i] = 0;
            }
            else
            {
                counter++;
            }
        }
    }
    if(counter == 3){
            cout << "Nothing to de-select" << endl;
        }
}

//formating for printing doubles with 2 decimales//
string VendingMachine::doublePrinter(double value) 
{
    stringstream stream;
    stream << fixed << setprecision(2) << value;
    return stream.str();
}


int main()
{
    cout << "===== Welcome to CSUMB Vending Machine =====\n";

    VendingMachine machine1;
    cout << machine1.display();  // Display the ID and name of machine1.

    VendingMachine machine2 (200, "Library"); 
    cout << machine2.display();

    cout << "\n===== Updated machine1 Info =====\n";
    machine1.setNumber(100);
    machine1.setName("BIT117");
    cout << machine1.display(); 

    cout << "\n===== machine1 Status (1) =====\n";
    machine1.status();  // status() function displays current status of the machine.

    machine1.refill(5, 7, 3, 10); // A machine’s admin can refill the items.


    cout << "\n===== machine1 Status (2) =====\n";
    machine1.status();

    cout << endl;
    machine1.displayMenu();

    cout << endl;
    machine1.selectItem();   // A user can select item(s).

    cout << "\n===== machine1 Payment (1) =====\n";
    if(machine1.payment() == true)   // Pay with a debit card or cash.
    {
        cout << "\n===== Receipt (1) =====\n";
        machine1.displayReceipt();
    }
    else 
    {
        // Note that if a user entered incorrect payment information,
        // all selections should be deselected.
        cout << "Invalid payment. All selections are cancelled.\n";
    }

    cout << "\n===== machine1 Status (3) =====\n";
    machine1.status();

    cout << endl;
    machine1.selectItem();

    cout << endl;
    machine1.deselect( );  // A user can de-select items.

    cout << endl;
    machine1.selectItem();

    cout << "\n===== machine1 Payment (2) =====\n";
    if(machine1.payment())
    {
        cout << "\n===== Receipt (2) =====\n";
        machine1.displayReceipt();
    }

    cout << "\n===== machine1 Status (4) =====\n";
    machine1.status();

    machine1.reset();  // An admin resets the machine to the initial 
                       // status, except the ID and name of the machine.
    cout << "\n===== machine1 Status (5) =====\n";
    machine1.status();

    cout << "\n===== Thank you! =====\n";

    return 0;
}