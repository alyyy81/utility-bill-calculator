#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

int main() {
   
    const double BASE_SERVICE_FEE = 18.50;
    const double TIER_1_RATE      = 0.08;
    const double TIER_2_RATE      = 0.12;
    const double TIER_3_RATE      = 0.16;
    const double TAX_RATE         = 0.055; // 5.5% Municipal Clean Energy Tax

    const double TIER_1_LIMIT     = 500.0;
    const double TIER_2_LIMIT     = 500.0; // Captures the 501 to 1000 tier frame

    string accountNumber;
    string customerName;
    double previousReading = -1.0;
    double currentReading  = -1.0;

    cout << " Orange Walk BEL Agency MONTHLY BILLING CALCULATOR \n";
    cout << "Enter Customer Account Number : ";
    getline(cin, accountNumber);

    cout << "Enter Customer Full Name     : ";
    getline(cin, customerName);

   
    while (true) {
        cout << "Enter Previous Reading (kWh) : ";
        if (cin >> previousReading && previousReading >= 0) {
            break;
        }
        cout << "[ERROR] Invalid entry. Reading must be a non-negative numeric value.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    
    while (true) {
        cout << "Enter Current Reading (kWh)  : ";
        if (cin >> currentReading) {
            if (currentReading < 0) {
                cout << "[ERROR] Invalid entry. Reading must be a non-negative numeric value.\n";
            } else if (currentReading < previousReading) {
                cout << "[ERROR] Inverted reading sequence. Current value cannot be less than previous (" 
                     << previousReading << " kWh).\n";
            } else {
                break; // Input is fully valid
            }
        } else {
            cout << "[ERROR] Invalid numeric type structure.\n";
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "\nComputing charges... Done.\n\n";

   
    double totalConsumption = currentReading - previousReading;
    double unallocatedUnits = totalConsumption;

    double tier1Units = 0.0, tier2Units = 0.0, tier3Units = 0.0;
    double tier1Cost  = 0.0, tier2Cost  = 0.0, tier3Cost  = 0.0;

   
    if (unallocatedUnits > 0) {
        tier1Units = (unallocatedUnits > TIER_1_LIMIT) ? TIER_1_LIMIT : unallocatedUnits;
        tier1Cost = tier1Units * TIER_1_RATE;
        unallocatedUnits -= tier1Units;
    }

   
    if (unallocatedUnits > 0) {
        tier2Units = (unallocatedUnits > TIER_2_LIMIT) ? TIER_2_LIMIT : unallocatedUnits;
        tier2Cost = tier2Units * TIER_2_RATE;
        unallocatedUnits -= tier2Units;
    }

   
    if (unallocatedUnits > 0) {
        tier3Units = unallocatedUnits;
        tier3Cost = tier3Units * TIER_3_RATE;
    }

    double consumptionSubtotal = tier1Cost + tier2Cost + tier3Cost;
    double cleanEnergyTax      = consumptionSubtotal * TAX_RATE;
    double totalBalanceDue     = BASE_SERVICE_FEE + consumptionSubtotal + cleanEnergyTax;

  
    cout << fixed << setprecision(2);
    cout << "               ITEMIZED UTILITY INVOICE               \n";
    cout << left << setw(20) << "Account Number"   << " : " << accountNumber << "\n";
    cout << left << setw(20) << "Customer Name"    << " : " << customerName << "\n";
    cout << left << setw(20) << "Total Consumption" << " : " << totalConsumption << " kWh\n";
    cout << left << setw(38) << "Line Item Breakdown" << right << setw(16) << "Amount ($)" << "\n";
    
    cout << left << setw(38) << "Base Customer Charge" 
         << "$ " << right << setw(14) << BASE_SERVICE_FEE << "\n";
    
    cout << left << setw(38) << ("Tier 1 Usage (First " + to_string((int)TIER_1_LIMIT) + ".00 kWh)")
         << "$ " << right << setw(14) << tier1Cost << "\n";
         
    cout << left << setw(38) << ("Tier 2 Usage (Next " + to_string((int)TIER_2_LIMIT) + ".00 kWh)")
         << "$ " << right << setw(14) << tier2Cost << "\n";
         
    
    stringstream tier3Label;
    tier3Label << "Tier 3 Usage (Excess " << totalConsumption - tier1Units - tier2Units << " kWh)";
    cout << left << setw(38) << tier3Label.str()
         << "$ " << right << setw(14) << tier3Cost << "\n";
         
    cout << left << setw(38) << "Total Consumption Subtotal" 
         << "$ " << right << setw(14) << consumptionSubtotal << "\n";
         
    cout << left << setw(38) << "Municipal Clean Energy Surcharge (5.5%)" 
         << "$ " << right << setw(14) << cleanEnergyTax << "\n";
         
    cout << left << setw(38) << "TOTAL BALANCE DUE" 
         << "$ " << right << setw(14) << totalBalanceDue << "\n";
    cout << "Payment Due Date: 21 Days From Statement Generation.\n";
    cout << "Thank you for being a valued customer!\n";

    return 0;
}
