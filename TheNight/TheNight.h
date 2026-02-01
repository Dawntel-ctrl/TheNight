#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>

class AstronomyTracker {
public:
    // Enum: excellent for time direction
    enum NightPhase { EARLY_NIGHT = 1, MID_NIGHT = 2, LATE_NIGHT = 3, DAYTIME = 4 };

    // Struct: groups related info for one observation session
    struct Observation {
        std::string date;      // user input (non-empty)
        int hour24;            // user input (0-23)
        std::string location;  // user input (non-empty, can include spaces)
        NightPhase phase;      // derived from hour24 (enum)
    };

    AstronomyTracker() : count(0) {}

    void run() {
        showBanner();

        int choice;
        do {
            showMenu();
            choice = getIntInRange("Choose an option please (1-5): ", 1, 5);

            switch (choice) {
            case 1:
                addObservation();
                break;
            case 2:
                showReport();
                break;
            case 3:
                showVisibleObjectsMenu();
                break;
            case 4:
                saveReportToFile("report.txt");
                break;
            case 5:
                std::cout << "\nGoodbye. Clear skies!\n";
                break;
            }
        } while (choice != 5);
    }

private:
    static const int MAX_OBS = 7;
    Observation obs[MAX_OBS];
    int count;

private:
    void showBanner() const {
        std::cout << "============================================================\n";
        std::cout << "            OBSERVING WHILE LATE-NIGHT(OWL)\n";
        std::cout << "============================================================\n";
        std::cout << "Enter DATE, TIME (hour), and LOCATION to log a sky session.\n";
        std::cout << "Then view a weekly report, get object suggestions, and save.\n";
        std::cout << "------------------------------------------------------------\n\n";
    }

    void showMenu() const {
        std::cout << "\n======================= MENU =======================\n";
        std::cout << "1) Add observation (date, time, location)\n";
        std::cout << "2) View weekly summary report\n";
        std::cout << "3) Show visible objects (based on time phase)\n";
        std::cout << "4) Save report to file (report.txt)\n";
        std::cout << "5) Quit\n";
        std::cout << "====================================================\n";
    }

    void addObservation() {
        if (count >= MAX_OBS) {
            std::cout << "You have reached the max of " << MAX_OBS << " observations.\n";
            std::cout << "View/save the report or quit.\n";
            return;
        }

        Observation o;

        o.date = getNonEmptyLine("Enter date (ex: 2026-01-18): ");
        o.hour24 = getIntInRange("Enter time hour (0-23): ", 0, 23);
        o.location = getNonEmptyLine("Enter location (spaces allowed): ");
        o.phase = computePhase(o.hour24);

        obs[count] = o;
        count++;

        std::cout << "\nSaved observation (" << count << "/" << MAX_OBS << ").\n";

        if ((o.hour24 >= 18 && o.hour24 <= 23) && (o.location.length() >= 3)) {
            std::cout << "Tip: Good evening window for planets and bright stars.\n";
        }
        else if ((o.hour24 >= 0 && o.hour24 <= 5) && (o.location.length() >= 3)) {
            std::cout << "Tip: Late hours can be darker—great for fainter objects.\n";
        }
        else {
            std::cout << "Tip: This time may be bright/daylight—try after 18:00.\n";
        }
    }

    void showVisibleObjectsMenu() const {
        if (count == 0) {
            std::cout << "No observations recorded yet. Add one first.\n";
            return;
        }

        std::cout << "\nShow objects for which session?\n";
        for (int i = 0; i < count; i++) {
            std::cout << "  " << (i + 1) << ") " << obs[i].date << " at " << obs[i].hour24
                << ":00 - " << obs[i].location << "\n";
        }

        int pick = getIntInRange("Pick a session number: ", 1, count);
        NightPhase p = obs[pick - 1].phase;

        std::cout << "\n--- Visible Objects Suggestions (Time Phase Based) ---\n";
        std::cout << "Session: " << obs[pick - 1].date << "  " << obs[pick - 1].hour24
            << ":00  " << obs[pick - 1].location << "\n";
        std::cout << "Phase: " << phaseToString(p) << "\n\n";

        switch (p) {
        case EARLY_NIGHT:
            printFixedObjectList(getEarlyNightObjects(), 5);
            break;
        case MID_NIGHT:
            printFixedObjectList(getMidNightObjects(), 5);
            break;
        case LATE_NIGHT:
            printFixedObjectList(getLateNightObjects(), 5);
            break;
        default:
            std::cout << "It may be daylight/bright sky. Try observing after 18:00.\n";
            break;
        }

        int hr = obs[pick - 1].hour24;
        if ((hr >= 22 && hr <= 23) && (p == MID_NIGHT)) {
            std::cout << "\nRecommendation: Try fainter targets and star clusters.\n";
        }
        else if ((hr >= 0 && hr <= 2) && (p == LATE_NIGHT)) {
            std::cout << "\nRecommendation: Great time for deep-sky attempts if the sky is dark.\n";
        }
        else {
            std::cout << "\nRecommendation: Start with bright objects (Moon/planets/bright stars).\n";
        }
    }

    void printFixedObjectList(const std::string list[], int size) const {
        std::cout << "Try spotting:\n";
        for (int i = 0; i < size; i++) {
            std::cout << "  - " << list[i] << "\n";
        }
    }

    const std::string* getEarlyNightObjects() const {
        static const std::string early[5] = {
            "Moon (if up)",
            "Venus (if visible)",
            "Jupiter (if visible)",
            "Bright constellation (Orion / Leo / Scorpius)",
            "Pleiades (M45) / bright cluster (seasonal)"
        };
        return early;
    }

    const std::string* getMidNightObjects() const {
        static const std::string mid[5] = {
            "Milky Way band (dark sites)",
            "Andromeda Galaxy (M31) / seasonal galaxy",
            "Double Cluster (h+chi) / seasonal cluster",
            "Ring Nebula (M57) / seasonal nebula",
            "Saturn or Mars (if visible)"
        };
        return mid;
    }

    const std::string* getLateNightObjects() const {
        static const std::string late[5] = {
            "Globular cluster attempt (seasonal)",
            "Faint nebula attempt (seasonal)",
            "Meteor watch (any season)",
            "Detailed Moon viewing (craters/terminator)",
            "Star-hopping practice to a target"
        };
        return late;
    }

    void showReport() const {
        printReport(std::cout);
    }

    void saveReportToFile(const std::string& filename) const {
        std::ofstream fout(filename.c_str());
        if (!fout) {
            std::cout << "Error: could not open " << filename << " for writing.\n";
            return;
        }

        printReport(fout);
        fout.close();

        std::cout << "Report saved to " << filename << "\n";
    }

    void printReport(std::ostream& out) const {
        out << "\n==================== WEEKLY OBSERVATION REPORT ====================\n";
        out << "Sessions recorded: " << count << "/" << MAX_OBS << "\n\n";

        if (count == 0) {
            out << "No observations yet.\n";
            return;
        }

        out << std::left
            << std::setw(14) << "Date"
            << std::setw(8) << "Hour"
            << std::setw(28) << "Location"
            << std::setw(16) << "Phase"
            << "\n";

        out << std::string(66, '-') << "\n";

        for (int i = 0; i < count; i++) {
            out << std::left
                << std::setw(14) << obs[i].date
                << std::setw(8) << obs[i].hour24
                << std::setw(28) << shorten(obs[i].location, 27)
                << std::setw(16) << phaseToString(obs[i].phase)
                << "\n";
        }

        out << std::string(66, '-') << "\n";

        double avgHr = computeAverageHour(obs, count);
        out << std::fixed << std::setprecision(1);
        out << "Derived value: Average observation hour = " << avgHr << "\n";

        int earlyCount = countPhase(obs, count, EARLY_NIGHT);
        int midCount = countPhase(obs, count, MID_NIGHT);
        int lateCount = countPhase(obs, count, LATE_NIGHT);

        out << "Phase counts: Early=" << earlyCount
            << "  Mid=" << midCount
            << "  Late=" << lateCount << "\n";

        NightPhase best = mostCommonPhase(earlyCount, midCount, lateCount);
        out << "Most common phase: " << phaseToString(best) << "\n";
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
        if (midCount >= earlyCount && midCount >= lateCount) return MID_NIGHT;
        return LATE_NIGHT;
    }

    std::string getNonEmptyLine(const std::string& prompt) const {
        std::string s;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, s);
            if (s.length() > 0) return s;
            std::cout << "Invalid input: cannot be empty.\n";
        }
    }

    int getIntInRange(const std::string& prompt, int minVal, int maxVal) const {
        int value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid number. Try again.\n";
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (value < minVal || value > maxVal) {
                std::cout << "Out of range. Enter " << minVal << " to " << maxVal << ".\n";
            }
            else {
                return value;
            }
        }
    }

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

    std::string shorten(const std::string& s, int maxLen) const {
        if (static_cast<int>(s.length()) <= maxLen) return s;
        return s.substr(0, maxLen - 3) + "...";
    }
};
