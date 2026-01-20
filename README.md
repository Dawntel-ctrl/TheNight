# Night Sky Object Tracker

This is a simple C++ console program for tracking amateur astronomy observations.  
Users can log night sky sessions by entering a date, time, and location, then view a summary report and see which objects are best to observe based on the time of night.

## What the Program Does
- Displays a welcome banner and menu
- Lets the user add multiple observation sessions
- Stores data using an array of structs
- Uses an enum to classify night phases (early, mid, late)
- Shows object suggestions based on time of night
- Generates a formatted summary report
- Saves the report to `report.txt`

## Inputs
Each observation uses exactly three inputs:
- Date (string)
- Time (hour in 24-hour format)
- Location (string with spaces allowed)


## How to Run
1. Open the project in Microsoft Visual Studio  
2. Build the solution (`Ctrl + Shift + B`)  
3. Run without debugging (`Ctrl + F5`)

The saved report can be found in the project’s Debug folder.

## Author
Dawntel Norwood
