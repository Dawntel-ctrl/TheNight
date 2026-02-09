#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>

class AstronomyTracker {
public:
    // Enum
    enum NightPhase { EARLY_NIGHT = 1, MID_NIGHT = 2, LATE_NIGHT = 3, DAYTIME = 4 };

    // Struct
    struct Observation {
        std::string date;
        int hour24;
        std::string location;
        NightPhase phase;
    };

    AstronomyTracker() : count(0) {}

    // ----------------------------
    // PUBLIC METHODS (TESTED)
    // ----------------------------
    NightPhase computePhase(int hour24) const {
        if (hour24 >= 18 && hour24 <= 21) return EARLY_NIGHT;
        if (hour24 >= 22 || hour24 <= 1)  return MID_NIGHT;
        if (hour24 >= 2 && hour24 <= 5)   return LATE_NIGHT;
        return DAYTIME;
    }

    std::string phaseToString(NightPhase p) const {
        if (p == EARLY_NIGHT) return "Early Night";
        if (p == MID_NIGHT)   return "Mid Night";
        if (p == LATE_NIGHT)  return "Late Night";
        return "Daytime";
    }

    double computeAverageHour(const Observation a[], int n) const {
        if (n <= 0) return 0.0;
        int total = 0;
        for (int i = 0; i < n; i++) total += a[i].hour24;
        return static_cast<double>(total) / n;
    }

    int countPhase(const Observation a[], int n, NightPhase p) const {
        int c = 0;
        for (int i = 0; i < n; i++) {
            if (a[i].phase == p) c++;
        }
        return c;
    }

    NightPhase mostCommonPhase(int earlyCount, int midCount, int lateCount) const {
        if (earlyCount >= midCount && earlyCount >= lateCount) return EARLY_NIGHT;
        if (midCount >= earlyCount && midCount >= lateCount)   return MID_NIGHT;
        return LATE_NIGHT;
    }

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
    static const int MAX_OBS = 7;
    Observation obs[MAX_OBS];
    int count;

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

    void addObservation() {
        if (count >= MAX_OBS) return;

        Observation o;
        o.date = getNonEmptyLine("Enter date: eg: 1991-02-28 ");
        o.hour24 = getIntInRange("Enter hour (0-23): ", 0, 23);
        o.location = getNonEmptyLine("Enter location: ");
        o.phase = computePhase(o.hour24);

        obs[count++] = o;
    }

    void showVisibleObjectsMenu() const {}
    void showReport() const { printReport(std::cout); }

    void saveReportToFile(const std::string& filename) const {
        std::ofstream fout(filename);
        if (!fout) return;
        printReport(fout);
    }

    void printReport(std::ostream& out) const {
        out << "Sessions recorded: " << count << "\n";
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
