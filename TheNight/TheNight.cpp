#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

class AstronomyTracker {
public:
    // Enum: excellent for time direction 
    enum NightPhase { EARLY_NIGHT = 1, MID_NIGHT = 2, LATE_NIGHT = 3, DAYTIME = 4 };

    // Struct: groups related info for one observation session
    struct Observation {
        string date;      // user input (non-empty)
        int hour24;       // user input (0-23)
        string location;  // user input (non-empty, can include spaces)
        NightPhase phase; // derived from hour24 (enum)
    };

    AstronomyTracker() : count(0) {
        // safe starting state: count = 0
    }

    void run() {
        showBanner();

        int choice;
        do { // do while loop show menu and options
            showMenu();
            choice = getIntInRange("Choose an option please (1-5): ", 1, 5);

            switch (choice) { // menu implemented with switch
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
                cout << "\nGoodbye. Clear skies!\n";
                break;
            }
        } while (choice != 5);
    }
    // session count here
private:
    static const int MAX_OBS = 7;      // set array size
    Observation obs[MAX_OBS];          // array of structs (they stored in class)
    int count;                         // how many have been filled

private:
    //  menu 
    void showBanner() const {
        cout << "============================================================\n";
        cout << "            NIGHT SKY OBJECT TRACKER\n";
        cout << "============================================================\n";
        cout << "Enter DATE, TIME (hour), and LOCATION to log a sky session.\n";
        cout << "Then view a weekly report, get object suggestions, and save.\n";
        cout << "------------------------------------------------------------\n\n";
    }

    void showMenu() const {
        cout << "\n======================= MENU =======================\n";
        cout << "1) Add observation (date, time, location)\n";
        cout << "2) View weekly summary report\n";
        cout << "3) Show visible objects (based on time phase)\n";
        cout << "4) Save report to file (report.txt)\n";
        cout << "5) Quit\n";
        cout << "====================================================\n";
    }

    // add Observation function 
    void addObservation() {
        // Array-based validation: prevent exceeding array size
        if (count >= MAX_OBS) {
            cout << "You have reached the max of " << MAX_OBS << " observations.\n";
            cout << "View/save the report or quit.\n";
            return;
        }

        Observation o; // local instance

        o.date = getNonEmptyLine("Enter date (ex: 2026-01-18): ");
        o.hour24 = getIntInRange("Enter time hour (0-23): ", 0, 23);
        o.location = getNonEmptyLine("Enter location (spaces allowed): ");
        o.phase = computePhase(o.hour24); // derived enum value

        obs[count] = o;
        count++;

        cout << "\nSaved observation (" << count << "/" << MAX_OBS << ").\n";

        // Compound if/else block #1 
        if ((o.hour24 >= 18 && o.hour24 <= 23) && (o.location.length() >= 3)) {
            cout << "Tip: Good evening window for planets and bright stars.\n";
        }
        else if ((o.hour24 >= 0 && o.hour24 <= 5) && (o.location.length() >= 3)) {
            cout << "Tip: Late hours can be darker—great for fainter objects.\n";
        }
        else {
            cout << "Tip: This time may be bright/daylight—try after 18:00.\n";
        }
    }

    //  Visible Objects for user 
    void showVisibleObjectsMenu() const {
        // Array-based validation: ensure at least one entry exists before showing phase-based objects
        if (count == 0) {
            cout << "No observations recorded yet. Add one first.\n";
            return;
        }

        cout << "\nShow objects for which session?\n";
        for (int i = 0; i < count; i++) {
            cout << "  " << (i + 1) << ") " << obs[i].date << " at " << obs[i].hour24
                << ":00 - " << obs[i].location << "\n";
        }

        int pick = getIntInRange("Pick a session number: ", 1, count);
        NightPhase p = obs[pick - 1].phase;

        cout << "\n--- Visible Objects Suggestions (Time Phase Based) ---\n";
        cout << "Session: " << obs[pick - 1].date << "  " << obs[pick - 1].hour24
            << ":00  " << obs[pick - 1].location << "\n";
        cout << "Phase: " << phaseToString(p) << "\n\n";

        // Enum used in a decision structure (switch)
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
            cout << "It may be daylight/bright sky. Try observing after 18:00.\n";
            break;
        }

        // Compound if/else block #2 
        int hr = obs[pick - 1].hour24;
        if ((hr >= 22 && hr <= 23) && (p == MID_NIGHT)) {
            cout << "\nRecommendation: Try fainter targets and star clusters.\n";
        }
        else if ((hr >= 0 && hr <= 2) && (p == LATE_NIGHT)) {
            cout << "\nRecommendation: Great time for deep-sky attempts if the sky is dark.\n";
        }
        else {
            cout << "\nRecommendation: Start with bright objects (Moon/planets/bright stars).\n";
        }
    }

    // Fixed number for loop requirement: prints exactly 5 items
    void printFixedObjectList(const string list[], int size) const {
        cout << "Try spotting:\n";
        for (int i = 0; i < size; i++) { // for loop fixed number of times
            cout << "  - " << list[i] << "\n";
        }
    }

    // Provide fixed object lists as C++ arrays
    const string* getEarlyNightObjects() const {
        static const string early[5] = {
            "Moon (if up)",
            "Venus (if visible)",
            "Jupiter (if visible)",
            "Bright constellation (Orion / Leo / Scorpius)",
            "Pleiades (M45) / bright cluster (seasonal)"
        };
        return early;
    }

    const string* getMidNightObjects() const {
        static const string mid[5] = {
            "Milky Way band (dark sites)",
            "Andromeda Galaxy (M31) / seasonal galaxy",
            "Double Cluster (h+chi) / seasonal cluster",
            "Ring Nebula (M57) / seasonal nebula",
            "Saturn or Mars (if visible)"
        };
        return mid;
    }

    const string* getLateNightObjects() const {
        static const string late[5] = {
            "Globular cluster attempt (seasonal)",
            "Faint nebula attempt (seasonal)",
            "Meteor watch (any season)",
            "Detailed Moon viewing (craters/terminator)",
            "Star-hopping practice to a target"
        };
        return late;
    }

    // Reports / File Output
    void showReport() const {
        printReport(cout);
    }

    void saveReportToFile(const string& filename) const {
        ofstream fout(filename.c_str());
        if (!fout) {
            cout << "Error: could not open " << filename << " for writing.\n";
            return;
        }

        printReport(fout);
        fout.close();

        cout << "Report saved to " << filename << "\n";
    }

    // Uses array in calculations + formatting; prints to screen or file
    void printReport(ostream& out) const {
        out << "\n==================== WEEKLY OBSERVATION REPORT ====================\n";
        out << "Sessions recorded: " << count << "/" << MAX_OBS << "\n\n";

        if (count == 0) {
            out << "No observations yet.\n";
            return;
        }

        out << left
            << setw(14) << "Date"
            << setw(8) << "Hour"
            << setw(28) << "Location"
            << setw(16) << "Phase"
            << "\n";

        out << string(66, '-') << "\n";

        for (int i = 0; i < count; i++) {
            out << left
                << setw(14) << obs[i].date
                << setw(8) << obs[i].hour24
                << setw(28) << shorten(obs[i].location, 27)
                << setw(16) << phaseToString(obs[i].phase)
                << "\n";
        }

        out << string(66, '-') << "\n";

        // Derived value tied to hobby: average observation hour 
        double avgHr = computeAverageHour(obs, count); // function accepts array parameter
        out << fixed << setprecision(1);
        out << "Derived value: Average observation hour = " << avgHr << "\n";

        // Another derived summary using enum counts 
        int earlyCount = countPhase(obs, count, EARLY_NIGHT);
        int midCount = countPhase(obs, count, MID_NIGHT);
        int lateCount = countPhase(obs, count, LATE_NIGHT);

        out << "Phase counts: Early=" << earlyCount
            << "  Mid=" << midCount
            << "  Late=" << lateCount << "\n";

        NightPhase best = mostCommonPhase(earlyCount, midCount, lateCount);
        out << "Most common phase: " << phaseToString(best) << "\n";
    }

    // Functions Using Arrays 
    // Function that accepts an array as a parameter
    double computeAverageHour(const Observation a[], int n) const {
        if (n <= 0) return 0.0;

        int total = 0;
        for (int i = 0; i < n; i++) {
            total += a[i].hour24;
        }
        return static_cast<double>(total) / n;
    }

    // Another array-parameter function (counts enum occurrences)
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

    //  Input & VALIDATION
    string getNonEmptyLine(const string& prompt) const {
        string s;
        while (true) { // while loop (validation)
            cout << prompt;
            getline(cin, s);
            if (s.length() > 0) return s;
            cout << "Invalid input: cannot be empty.\n";
        }
    }

    int getIntInRange(const string& prompt, int minVal, int maxVal) const {
        int value;

        while (true) { // (validation) with while loop  
            cout << prompt;
            cin >> value;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid number. Try again.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (value < minVal || value > maxVal) {
                cout << "Out of range. Enter " << minVal << " to " << maxVal << ".\n";
            }
            else {
                return value;
            }
        }
    }

    // used to switch statements from enum
    NightPhase computePhase(int hour24) const {
        // from time input
        if (hour24 >= 18 && hour24 <= 21) return EARLY_NIGHT;
        if (hour24 >= 22 || hour24 <= 1)  return MID_NIGHT;
        if (hour24 >= 2 && hour24 <= 5)  return LATE_NIGHT;
        return DAYTIME;
    }

    string phaseToString(NightPhase p) const {
        if (p == EARLY_NIGHT) return "Early Night";
        if (p == MID_NIGHT)   return "Mid Night";
        if (p == LATE_NIGHT)  return "Late Night";
        return "Daytime";
    }

    // Keep table neat if location is long
    string shorten(const string& s, int maxLen) const {
        if (static_cast<int>(s.length()) <= maxLen) return s;
        return s.substr(0, maxLen - 3) + "...";
    }
};

int main() {
    // object tracker created locally in main
    AstronomyTracker tracker;
    tracker.run();
    return 0;
}
