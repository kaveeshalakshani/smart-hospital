#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4

// Global System Configuration Constants
const char SPECIALTIES[NUM_SPECIALTIES][30] = {"General Practice", "Paediatrics", "Cardiology", "Neurology"};
const float BASE_FEES[NUM_SPECIALTIES] = {1500.0, 2500.0, 4500.0, 5000.0};
const int AVG_TIMES[NUM_SPECIALTIES] = {15, 20, 30, 30};

const char WARDS[NUM_WARDS][20] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const float WARD_RATES[NUM_WARDS] = {3000.0, 6000.0, 12000.0, 25000.0};

// Parallel Data Storage Arrays
char patientNames[MAX_PATIENTS][50];
int patientAges[MAX_PATIENTS];
int urgencyLevels[MAX_PATIENTS];
int selectedSpecialties[MAX_PATIENTS];
int assignedWards[MAX_PATIENTS];
int stayDays[MAX_PATIENTS];
int patientCount = 0;

// Helper function to clear standard input stream
void clearBuffer() {
    while (getchar() != '\n');
}

// Custom UI Section Header Formatter
void printHeader(const char* title) {
    printf("\n========================================\n");
    printf("   %s\n", title);
    printf("========================================\n");
}

// Requirement 2: Patient Intake and Form Processing
void registerPatient() {
    if (patientCount >= MAX_PATIENTS) {
        printf("\n[ERROR] Hospital capacity reached!\n");
        return;
    }

    printHeader("PATIENT INTAKE & REGISTRATION");
    printf("Enter Patient Name: ");
    scanf(" %[^\n]s", patientNames[patientCount]);

    printf("Enter Age: ");
    while (scanf("%d", &patientAges[patientCount]) != 1 || patientAges[patientCount] <= 0) {
        printf("Invalid age! Please enter a valid age: ");
        clearBuffer();
    }

    printf("Enter Urgency Level (1=Normal, 2=Urgent, 3=Critical): ");
    while (scanf("%d", &urgencyLevels[patientCount]) != 1 || urgencyLevels[patientCount] < 1 || urgencyLevels[patientCount] > 3) {
        printf("Invalid level! Enter 1, 2, or 3: ");
        clearBuffer();
    }

    printf("Select Specialty (1-OPD, 2-Paediatrics, 3-Cardiology, 4-Neurology): ");
    while (scanf("%d", &selectedSpecialties[patientCount]) != 1 || selectedSpecialties[patientCount] < 1 || selectedSpecialties[patientCount] > NUM_SPECIALTIES) {
        printf("Invalid choice! Enter between 1 and %d: ", NUM_SPECIALTIES);
        clearBuffer();
    }

    assignedWards[patientCount] = -1; // Default: No ward assigned
    stayDays[patientCount] = 0;

    printf("\n[SUCCESS] Patient %s Registered Successfully! Assigned ID: %d\n", patientNames[patientCount], patientCount);
    patientCount++;
}

// Search Subroutine by Patient Substring Match
void searchPatient() {
    if (patientCount == 0) {
        printf("\n[INFO] No patients registered yet!\n");
        return;
    }

    char query[50];
    int found = 0;
    printHeader("PATIENT SEARCH");
    printf("Enter Patient Name to Search: ");
    scanf(" %[^\n]s", query);

    printf("\n--- Search Results ---\n");
    for (int i = 0; i < patientCount; i++) {
        if (strstr(patientNames[i], query) != NULL) {
            printf("ID: %d | Name: %s | Age: %d | Specialty: %s\n",
                   i, patientNames[i], patientAges[i], SPECIALTIES[selectedSpecialties[i] - 1]);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching patient found.\n");
    }
}

// Requirement 6: Ward Bed Allocation Core Module
void allocateWard() {
    if (patientCount == 0) {
        printf("\n[INFO] No patients registered yet!\n");
        return;
    }

    printHeader("WARD BED ALLOCATION");
    int id;
    printf("Enter Patient ID (0 to %d): ", patientCount - 1);
    scanf("%d", &id);

    if (id < 0 || id >= patientCount) {
        printf("[ERROR] Invalid Patient ID!\n");
        return;
    }

    printf("\nSelect Ward Category:\n");
    for (int i = 0; i < NUM_WARDS; i++) {
        printf("%d. %s (LKR %.2f/day)\n", i + 1, WARDS[i], WARD_RATES[i]);
    }
    printf("Enter Ward Choice (1-%d): ", NUM_WARDS);
    scanf("%d", &assignedWards[id]);
    assignedWards[id] -= 1; // Array Index Shift

    printf("Enter Number of Days for Stay: ");
    scanf("%d", &stayDays[id]);

    printf("\n[SUCCESS] Ward %s allocated to Patient %s for %d days.\n",
           WARDS[assignedWards[id]], patientNames[id], stayDays[id]);
}

// Requirement 3 & 6: Automated Invoice Generation Logic
void generateBill() {
    if (patientCount == 0) {
        printf("\n[INFO] No patients registered yet!\n");
        return;
    }

    printHeader("BILLING & INVOICE GENERATION");
    int id;
    printf("Enter Patient ID (0 to %d): ", patientCount - 1);
    scanf("%d", &id);

    if (id < 0 || id >= patientCount) {
        printf("[ERROR] Invalid Patient ID!\n");
        return;
    }

    int specIdx = selectedSpecialties[id] - 1;
    float baseFee = BASE_FEES[specIdx];
    float discount = 0.0;

    // Senior and Pediatric Concession Logic
    if (patientAges[id] >= 60) {
        discount = baseFee * 0.15;
    } else if (patientAges[id] <= 12) {
        discount = baseFee * 0.10;
    }

    float finalConsultationFee = baseFee - discount;
    float wardFee = 0.0;

    if (assignedWards[id] != -1) {
        wardFee = WARD_RATES[assignedWards[id]] * stayDays[id];
    }

    float totalBill = finalConsultationFee + wardFee;

    printf("\n================ INVOICE ================\n");
    printf("Patient Name: %s | Age: %d\n", patientNames[id], patientAges[id]);
    printf("Specialty: %s\n", SPECIALTIES[specIdx]);
    printf("Base Consultation Fee: LKR %.2f\n", baseFee);
    printf("Discount Applied: LKR %.2f\n", discount);
    printf("Net Consultation Fee: LKR %.2f\n", finalConsultationFee);

    if (assignedWards[id] != -1) {
        printf("Ward: %s (%d days @ LKR %.2f/day): LKR %.2f\n",
               WARDS[assignedWards[id]], stayDays[id], WARD_RATES[assignedWards[id]], wardFee);
    } else {
        printf("Ward Charges: None\n");
    }

    printf("-----------------------------------------\n");
    printf("TOTAL PAYABLE AMOUNT: LKR %.2f\n", totalBill);
    printf("=========================================\n");
}

// Requirement 4: Priority Queue Sorting Display Engine
void displayTriageQueue() {
    if (patientCount == 0) {
        printf("\n[INFO] No patients in the queue!\n");
        return;
    }

    printHeader("EMERGENCY TRIAGE QUEUE");
    printf("Priority Order: Level 3 (Critical) -> Level 2 (Urgent) -> Level 1 (Normal)\n\n");

    for (int level = 3; level >= 1; level--) {
        for (int i = 0; i < patientCount; i++) {
            if (urgencyLevels[i] == level) {
                printf("[%s] ID: %d | Name: %s | Age: %d | Specialty: %s\n",
                    (level == 3 ? "CRITICAL" : (level == 2 ? "URGENT" : "NORMAL")),
                    i, patientNames[i], patientAges[i], SPECIALTIES[selectedSpecialties[i] - 1]);
            }
        }
    }
}

// Requirement 5: Statistical Analytics Aggregator
void displayAnalytics() {
    if (patientCount == 0) {
        printf("\n[INFO] No data available for analytics!\n");
        return;
    }

    printHeader("HOSPITAL ANALYTICS SUMMARY");
    int specialtyCounts[NUM_SPECIALTIES] = {0};
    int totalWaitTime = 0;

    for (int i = 0; i < patientCount; i++) {
        int specIdx = selectedSpecialties[i] - 1;
        specialtyCounts[specIdx]++;
        totalWaitTime += AVG_TIMES[specIdx];
    }

    printf("Total Patients Registered: %d\n", patientCount);
    printf("Total Estimated Waiting Time: %d mins\n\n", totalWaitTime);
    printf("Patient Volume by Specialty:\n");
    for (int i = 0; i < NUM_SPECIALTIES; i++) {
        printf("- %s: %d patients\n", SPECIALTIES[i], specialtyCounts[i]);
    }
}

// System Status Metrics Reporter
void displaySystemStatus() {
    printHeader("SYSTEM OVERVIEW REPORT");
    printf("Total Registered Patients : %d\n", patientCount);

    int wardAllocatedCount = 0;
    int criticalCount = 0;
    for (int i = 0; i < patientCount; i++) {
        if (assignedWards[i] != -1) wardAllocatedCount++;
        if (urgencyLevels[i] == 3) criticalCount++;
    }

    printf("Active Ward Admissions    : %d\n", wardAllocatedCount);
    printf("Critical Priority Cases  : %d\n", criticalCount);
}

// Emergency & Support Contact Information (New Feature)
void displayContactInfo() {
    printHeader("HOSPITAL HELPLINE & EMERGENCY CONTACTS");
    printf("Emergency Hot-line : 1990 (24/7 Service)\n");
    printf("Hospital Reception : +94 11 2345678\n");
    printf("Ambulance Support  : +94 11 8765432\n");
    printf("Email Query Desk   : help@smarthospital.lk\n");
}

// System Entry point
int main() {
    int choice;
    do {
        printHeader("SMART HOSPITAL MANAGEMENT SYSTEM");
        printf("1. Register New Patient\n");
        printf("2. Search Patient Record\n");
        printf("3. Allocate Ward Bed\n");
        printf("4. Generate Patient Bill\n");
        printf("5. View Triage Queue\n");
        printf("6. View Analytics Summary\n");
        printf("7. System Status Overview\n");
        printf("8. Emergency & Help Contacts\n");
        printf("9. Exit\n");
        printf("Enter Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("[ERROR] Invalid input! Exiting program.\n");
            break;
        }

        if (choice == 1) registerPatient();
        else if (choice == 2) searchPatient();
        else if (choice == 3) allocateWard();
        else if (choice == 4) generateBill();
        else if (choice == 5) displayTriageQueue();
        else if (choice == 6) displayAnalytics();
        else if (choice == 7) displaySystemStatus();
        else if (choice == 8) displayContactInfo();

    } while(choice != 9);

    printf("\nExiting System. Thank you!\n");
    return 0;
}
