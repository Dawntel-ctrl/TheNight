#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>

// add (for polymorphism + manager)
#include "NightManager.h"
#include "NightDerived.h"
#include "NightOver.h"
#include "NightComp.h"

class AstronomyTracker {
public:

    //  (conflicts with NightBase enum)
    /*
    enum NightPhase { EARLY_NIGHT = 1, MID_NIGHT = 2, LATE_NIGHT = 3, DAYTIME = 4 };
    */

    // (old storage model)
    /*
    struct Observation {
        std::string date;
        int hour24;
        std::string location;
        NightPhase phase;
    };
    */

    //  OLD CONSTRUCTOR
    // AstronomyTracker() : count(0) {}

    // new construter (manager handles size)
    AstronomyTracker() {}

    // ----------------------------
    // PUBLIC METHODS (TESTED)
    // ----------------------------

    //  UPDATED RETURN TYPE to use global NightPhase
    ::NightPhase computePhase(int hour24) const {
        if (hour24 >= 18 && hour24 <= 21) return EARLY_NIGHT;
        if (hour24 >= 22 || hour24 <= 1)  return MID_NIGHT;
        if (hour24 >= 2 && hour24 <= 5)   return LATE_NIGHT;
        return DAYTIME;
    }

    std::string phaseToString(::NightPhase p) const {
        if (p == EARLY_NIGHT) return "Early Night";
        if (p == MID_NIGHT)   return "Mid Night";
        if (p == LATE_NIGHT)  return "Late Night";
        return "Daytime";
    }

    // DELETE OLD ARRAY-BASED FUNCTIONS (they use Observation[])
    /*
    double computeAverageHour(const Observation a[], int n) const { ... }

    int countPhase(const Observation a[], int n, NightPhase p) const { ... }

    NightPhase mostCommonPhase(int earlyCount, int midCount, int lateCount) const { ... }
    */

    // ----------------------------
    // PROGRAM ENTRY (NOT TESTED)
    // ----------------------------
    void run() {
        showBanner();

        int choice;
        do {
            showMenu();
            choice = getIntInRange("Choose an option please (1-5): ", 1, 5);

            switch (choice) {
            case 1: addObservation(); break;
            case 2: showReport(); break;
            case 3: showVisibleObjectsMenu(); break;
            case 4: saveReportToFile("report.txt"); break;
            case 5: std::cout << "\nGoodbye. Clear skies!\n"; break;
            }
        } while (choice != 5);
    }

private:

    // OLD FIXED STORAGE
    /*
    static const int MAX_OBS = 7;
    Observation obs[MAX_OBS];
    int count;
    */

    // new, for dynamic storage
    NightManager manager;

    // ----------------------------
    // PRIVATE UI / I/O
    // ----------------------------
    void showBanner() const {
        std::cout << "============================================================\n";
        std::cout << "            OBSERVING WHILE LATE-NIGHT(OWL)\n";
        std::cout << "============================================================\n";
    }

    void showMenu() const {
        std::cout << "\n1) Add observation\n2) View report\n3) Show objects\n4) Save\n5) Quit\n";
    }

    // had a rewrite now creates dynamic objects
    void addObservation() {

        std::string date = getNonEmptyLine("Enter date: eg: 1991-02-28 ");
        int hour24 = getIntInRange("Enter hour (0-23): ", 0, 23);
        std::string location = getNonEmptyLine("Enter location: ");

        ::NightPhase phase = computePhase(hour24);

        NightComp comp(location);

        // added , dynamically allocate derived object
        manager.add(new NightDerived(date, hour24, phase, comp, 0));
    }

    void showVisibleObjectsMenu() const {}

    void showReport() const {
        printReport(std::cout);
    }

    void saveReportToFile(const std::string& filename) const {
        std::ofstream fout(filename);
        if (!fout) return;
        printReport(fout);
    }

    // updated to use manager
    void printReport(std::ostream& out) const {
        out << "Sessions recorded: " << manager.getSize() << "\n";
        manager.printAll(out);  // polymorphic print
    }

    std::string getNonEmptyLine(const std::string& prompt) const {
        std::string s;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, s);
            if (!s.empty()) return s;
        }
    }

    int getIntInRange(const std::string& prompt, int minVal, int maxVal) const {
        int v;
        while (true) {
            std::cout << prompt;
            std::cin >> v;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (v >= minVal && v <= maxVal) return v;
        }
    }
};

