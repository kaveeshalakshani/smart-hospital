#include <stdio.h>
#include <string.h>

// Requirement 1: Constants
const char SPECIALTIES[4][30] = {"General Practice", "Paediatrics", "Cardiology", "Neurology"};
const float BASE_FEES[4] = {1500.0, 2500.0, 4500.0, 5000.0};
const int AVG_TIMES[4] = {15, 20, 30, 30};

const char WARDS[4][20] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const float WARD_RATES[4] = {3000.0, 6000.0, 12000.0, 25000.0};

// Requirement 1: Parallel Arrays
char patientNames[100][50];
int patientAges[100];
int urgencyLevels[100];
int selectedSpecialties[100];
int patientCount = 0;

// Requirement 2: Patient Registration
void registerPatient() {
    printf("\n--- Patient Intake & Registration ---\n");
    printf("Enter Patient Name: ");
    scanf(" %[^\n]s", patientNames[patientCount]);
    printf("Enter Age: ");
    scanf("%d", &patientAges[patientCount]);
    printf("Enter Urgency Level (1=Normal, 2=Urgent, 3=Critical): ");
    scanf("%d", &urgencyLevels[patientCount]);

    printf("Select Specialty (1-OPD, 2-Paediatrics, 3-Cardiology, 4-Neurology): ");
    scanf("%d", &selectedSpecialties[patientCount]);

    printf("\nPatient %s Registered Successfully!\n", patientNames[patientCount]);
    patientCount++;
}

// Requirement 3: Billing & Discount Engine
void generateBill() {
    if (patientCount == 0) {
        printf("\nNo patients registered yet!\n");
        return;
    }

    int id;
    printf("\nEnter Patient ID (0 to %d): ", patientCount - 1);
    scanf("%d", &id);

    if (id < 0 || id >= patientCount) {
        printf("Invalid Patient ID!\n");
        return;
    }

    int specIdx = selectedSpecialties[id] - 1;
    float baseFee = BASE_FEES[specIdx];
    float discount = 0.0;

    if (patientAges[id] >= 60) {
        discount = baseFee * 0.15;
    } else if (patientAges[id] <= 12) {
        discount = baseFee * 0.10;
    }

    float finalFee = baseFee - discount;

    printf("\n--- INVOICE ---\n");
    printf("Patient Name: %s\n", patientNames[id]);
    printf("Specialty: %s\n", SPECIALTIES[specIdx]);
    printf("Base Consultation Fee: LKR %.2f\n", baseFee);
    printf("Discount Applied: LKR %.2f\n", discount);
    printf("Final Payable Amount: LKR %.2f\n", finalFee);
}

// Requirement 4: Emergency Triage Display
void displayTriageQueue() {
    if (patientCount == 0) {
        printf("\nNo patients in the queue!\n");
        return;
    }

    printf("\n--- EMERGENCY TRIAGE QUEUE ---\n");
    printf("Level 3 (Critical) -> Level 2 (Urgent) -> Level 1 (Normal)\n\n");

    // Display Critical Patients
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

// Requirement 5: Hospital Analytics
void displayAnalytics() {
    if (patientCount == 0) {
        printf("\nNo data available for analytics!\n");
        return;
    }

    int specialtyCounts[4] = {0};
    int totalWaitTime = 0;

    for (int i = 0; i < patientCount; i++) {
        int specIdx = selectedSpecialties[i] - 1;
        specialtyCounts[specIdx]++;
        totalWaitTime += AVG_TIMES[specIdx];
    }

    printf("\n--- HOSPITAL ANALYTICS SUMMARY ---\n");
    printf("Total Patients Registered: %d\n", patientCount);
    printf("Total Estimated Waiting Time: %d mins\n\n", totalWaitTime);
    printf("Patient Volume by Specialty:\n");
    for (int i = 0; i < 4; i++) {
        printf("- %s: %d patients\n", SPECIALTIES[i], specialtyCounts[i]);
    }
}

int main() {
    int choice;
    do {
        printf("\n=== SMART HOSPITAL MANAGEMENT SYSTEM ===\n");
        printf("1. Register New Patient\n");
        printf("2. Generate Patient Bill\n");
        printf("3. View Triage Queue\n");
        printf("4. View Analytics Summary\n");
        printf("5. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            registerPatient();
        } else if (choice == 2) {
            generateBill();
        } else if (choice == 3) {
            displayTriageQueue();
        } else if (choice == 4) {
            displayAnalytics();
        }
    } while(choice != 5);

    return 0;
}
