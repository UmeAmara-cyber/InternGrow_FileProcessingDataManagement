#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<iomanip>
#include<ctime>
#include<cstdio>
using namespace std;
// =========================
// STUDENT STRUCTURE
// =========================
struct Student {
    int id;
    string name;
    string department;
    int semester;
    double marks;
};
// =========================
// FILE NAMES
// =========================
const string DATA_FILE = "students.txt";
const string CSV_FILE = "students.csv";
const string LOG_FILE = "error.log";
const string BACKUP_FILE = "students_backup.txt";
// =========================
// ERROR LOGGING
// =========================
void logError(string message) {
    ofstream file(LOG_FILE.c_str(), ios::app);
    if (file.is_open()) {
        time_t now = time(0);

        file << "[" << ctime(&now) << "] "
             << message << endl;

        file.close();
    }
}

// =========================
// CREATE FILE
// =========================

void createFile() {

    ifstream check(DATA_FILE.c_str());

    if (check.is_open()) {
        cout << "\nFile already exists.\n";
        check.close();
        return;
    }

    check.close();

    ofstream file(DATA_FILE.c_str());

    if (!file.is_open()) {
        cout << "\nError: Could not create file.\n";
        logError("Unable to create students.txt");
        return;
    }

    file.close();

    cout << "\nFile created successfully!\n";
}

// =========================
// ADD RECORD
// =========================

void addRecord() {

    ofstream file(DATA_FILE.c_str(), ios::app);

    if (!file.is_open()) {
        cout << "\nError: Could not open file.\n";
        logError("Unable to open students.txt while adding record");
        return;
    }

    Student s;

    cout << "\nEnter Student ID: ";
    cin >> s.id;

    cin.ignore();

    cout << "Enter Student Name: ";
    getline(cin, s.name);

    cout << "Enter Department: ";
    getline(cin, s.department);

    cout << "Enter Semester: ";
    cin >> s.semester;

    cout << "Enter Marks: ";
    cin >> s.marks;

    file << s.id << "|"
         << s.name << "|"
         << s.department << "|"
         << s.semester << "|"
         << s.marks << endl;

    file.close();

    cout << "\nRecord added successfully!\n";
}

// =========================
// READ ALL RECORDS
// =========================

vector<Student> readRecords() {

    vector<Student> students;

    ifstream file(DATA_FILE.c_str());

    if (!file.is_open()) {
        logError("Unable to open students.txt for reading");
        return students;
    }

    string line;

    while (getline(file, line)) {

        if (line.empty())
            continue;

        stringstream ss(line);

        string id;
        string name;
        string department;
        string semester;
        string marks;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, department, '|');
        getline(ss, semester, '|');
        getline(ss, marks, '|');

        try {

            Student s;

            s.id = atoi(id.c_str());
            s.name = name;
            s.department = department;
            s.semester = atoi(semester.c_str());
            s.marks = atof(marks.c_str());

            students.push_back(s);

        }
        catch (...) {

            logError("Invalid record found in students.txt");
        }
    }

    file.close();

    return students;
}

// =========================
// DISPLAY RECORDS
// =========================

void displayRecords() {

    vector<Student> students = readRecords();

    if (students.empty()) {

        cout << "\nNo records found.\n";
        return;
    }

    cout << "\n";
    cout << "============================================================\n";
    cout << "                    STUDENT RECORDS\n";
    cout << "============================================================\n";

    cout << left
         << setw(8) << "ID"
         << setw(25) << "Name"
         << setw(20) << "Department"
         << setw(12) << "Semester"
         << setw(10) << "Marks"
         << endl;

    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < students.size(); i++) {

        cout << left
             << setw(8) << students[i].id
             << setw(25) << students[i].name
             << setw(20) << students[i].department
             << setw(12) << students[i].semester
             << setw(10) << students[i].marks
             << endl;
    }

    cout << "============================================================\n";
}

// =========================
// UPDATE RECORD
// =========================

void updateRecord() {

    vector<Student> students = readRecords();

    if (students.empty()) {

        cout << "\nNo records available.\n";
        return;
    }

    int searchID;

    cout << "\nEnter Student ID to update: ";
    cin >> searchID;

    bool found = false;

    for (int i = 0; i < students.size(); i++) {

        if (students[i].id == searchID) {

            found = true;

            cin.ignore();

            cout << "Enter New Name: ";
            getline(cin, students[i].name);

            cout << "Enter New Department: ";
            getline(cin, students[i].department);

            cout << "Enter New Semester: ";
            cin >> students[i].semester;

            cout << "Enter New Marks: ";
            cin >> students[i].marks;

            break;
        }
    }

    if (!found) {

        cout << "\nRecord not found.\n";
        logError("Update failed: Student ID not found");
        return;
    }

    ofstream file(DATA_FILE.c_str());

    if (!file.is_open()) {

        cout << "\nError opening file.\n";
        logError("Unable to open students.txt during update");
        return;
    }

    for (int i = 0; i < students.size(); i++) {

        file << students[i].id << "|"
             << students[i].name << "|"
             << students[i].department << "|"
             << students[i].semester << "|"
             << students[i].marks << endl;
    }

    file.close();

    cout << "\nRecord updated successfully!\n";
}

// =========================
// DELETE RECORD
// =========================

void deleteRecord() {

    vector<Student> students = readRecords();

    if (students.empty()) {

        cout << "\nNo records available.\n";
        return;
    }

    int searchID;

    cout << "\nEnter Student ID to delete: ";
    cin >> searchID;

    bool found = false;

    ofstream file("temp.txt");

    if (!file.is_open()) {

        cout << "\nError creating temporary file.\n";
        logError("Unable to create temp.txt during delete");
        return;
    }

    for (int i = 0; i < students.size(); i++) {

        if (students[i].id == searchID) {

            found = true;
            continue;
        }

        file << students[i].id << "|"
             << students[i].name << "|"
             << students[i].department << "|"
             << students[i].semester << "|"
             << students[i].marks << endl;
    }

    file.close();

    if (!found) {

        remove("temp.txt");

        cout << "\nRecord not found.\n";
        logError("Delete failed: Student ID not found");
        return;
    }

    remove(DATA_FILE.c_str());
    rename("temp.txt", DATA_FILE.c_str());

    cout << "\nRecord deleted successfully!\n";
}

// =========================
// SEARCH RECORD
// =========================

void searchRecord() {

    vector<Student> students = readRecords();

    if (students.empty()) {

        cout << "\nNo records available.\n";
        return;
    }

    int searchID;

    cout << "\nEnter Student ID to search: ";
    cin >> searchID;

    bool found = false;

    for (int i = 0; i < students.size(); i++) {

        if (students[i].id == searchID) {

            found = true;

            cout << "\n";
            cout << "========== RECORD FOUND ==========\n";
            cout << "ID         : " << students[i].id << endl;
            cout << "Name       : " << students[i].name << endl;
            cout << "Department : " << students[i].department << endl;
            cout << "Semester   : " << students[i].semester << endl;
            cout << "Marks      : " << students[i].marks << endl;
            cout << "==================================\n";

            break;
        }
    }

    if (!found) {

        cout << "\nRecord not found.\n";
    }
}

// =========================
// GENERATE REPORT
// =========================

void generateReport() {

    vector<Student> students = readRecords();

    if (students.empty()) {

        cout << "\nNo records available.\n";
        return;
    }

    double totalMarks = 0;
    double highest = students[0].marks;
    double lowest = students[0].marks;

    for (int i = 0; i < students.size(); i++) {

        totalMarks += students[i].marks;

        if (students[i].marks > highest)
            highest = students[i].marks;

        if (students[i].marks < lowest)
            lowest = students[i].marks;
    }

    double average = totalMarks / students.size();

    cout << "\n";
    cout << "========================================\n";
    cout << "             STUDENT REPORT\n";
    cout << "========================================\n";

    cout << "Total Students : " << students.size() << endl;
    cout << fixed << setprecision(2);
    cout << "Average Marks  : " << average << endl;
    cout << "Highest Marks  : " << highest << endl;
    cout << "Lowest Marks   : " << lowest << endl;

    cout << "========================================\n";
}

// =========================
// CSV FILE PROCESSING
// =========================

void exportCSV() {

    vector<Student> students = readRecords();

    if (students.empty()) {

        cout << "\nNo records available for CSV export.\n";
        return;
    }

    ofstream file(CSV_FILE.c_str());

    if (!file.is_open()) {

        cout << "\nError creating CSV file.\n";
        logError("Unable to create students.csv");
        return;
    }

    file << "ID,Name,Department,Semester,Marks\n";

    for (int i = 0; i < students.size(); i++) {

        file << students[i].id << ","
             << "\"" << students[i].name << "\","
             << "\"" << students[i].department << "\","
             << students[i].semester << ","
             << students[i].marks << endl;
    }

    file.close();

    cout << "\nCSV file created successfully!\n";
    cout << "File: " << CSV_FILE << endl;
}

// =========================
// DATA BACKUP
// =========================

void backupData() {

    ifstream source(DATA_FILE.c_str());

    if (!source.is_open()) {

        cout << "\nNo data file found for backup.\n";
        logError("Backup failed: students.txt not found");
        return;
    }

    ofstream backup(BACKUP_FILE.c_str());

    if (!backup.is_open()) {

        cout << "\nUnable to create backup file.\n";
        logError("Unable to create students_backup.txt");
        source.close();
        return;
    }

    string line;

    while (getline(source, line)) {

        backup << line << endl;
    }

    source.close();
    backup.close();

    cout << "\nData backup created successfully!\n";
    cout << "Backup file: " << BACKUP_FILE << endl;
}

// =========================
// VIEW ERROR LOG
// =========================

void viewErrorLog() {

    ifstream file(LOG_FILE.c_str());

    if (!file.is_open()) {

        cout << "\nNo error log exists yet.\n";
        return;
    }

    cout << "\n";
    cout << "================ ERROR LOG ================\n";

    string line;

    while (getline(file, line)) {

        cout << line << endl;
    }

    cout << "============================================\n";

    file.close();
}

// =========================
// MAIN MENU
// =========================

int main() {

    int choice;

    do {

        cout << "\n";
        cout << "================================================\n";
        cout << "       FILE PROCESSING & DATA MANAGEMENT\n";
        cout << "================================================\n";

        cout << "1.  Create File\n";
        cout << "2.  Add Record\n";
        cout << "3.  Read Records\n";
        cout << "4.  Update Record\n";
        cout << "5.  Delete Record\n";
        cout << "6.  Search Records\n";
        cout << "7.  Generate Report\n";
        cout << "8.  Export to CSV\n";
        cout << "9.  Backup Data\n";
        cout << "10. View Error Log\n";
        cout << "0.  Exit\n";

        cout << "================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                createFile();
                break;

            case 2:
                addRecord();
                break;

            case 3:
                displayRecords();
                break;

            case 4:
                updateRecord();
                break;

            case 5:
                deleteRecord();
                break;

            case 6:
                searchRecord();
                break;

            case 7:
                generateReport();
                break;

            case 8:
                exportCSV();
                break;

            case 9:
                backupData();
                break;

            case 10:
                viewErrorLog();
                break;

            case 0:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
                logError("Invalid menu choice entered");
        }
    } while (choice != 0);
    return 0;
}