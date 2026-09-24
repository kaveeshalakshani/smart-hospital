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
int assignedWards[100];
int stayDays[100];
int patientCount = 0;

// Requirement 2: Patient Registration with Input Validation
void registerPatient() {
    if (patientCount >= 100) {
        printf("\n[ERROR] Hospital capacity reached! Cannot register more patients.\n");
        return;
    }

    printf("\n--- Patient Intake & Registration ---\n");
    printf("Enter Patient Name: ");
    scanf(" %[^\n]s", patientNames[patientCount]);

    printf("Enter Age: ");
    while (scanf("%d", &patientAges[patientCount]) != 1 || patientAges[patientCount] <= 0) {
        printf("Invalid age! Please enter a valid age: ");
        while(getchar() != '\n'); // clear input buffer
    }

    printf("Enter Urgency Level (1=Normal, 2=Urgent, 3=Critical): ");
    while (scanf("%d", &urgencyLevels[patientCount]) != 1 || urgencyLevels[patientCount] < 1 || urgencyLevels[patientCount] > 3) {
        printf("Invalid level! Enter 1, 2, or 3: ");
        while(getchar() != '\n');
    }

    printf("Select Specialty (1-OPD, 2-Paediatrics, 3-Cardiology, 4-Neurology): ");
    while (scanf("%d", &selectedSpecialties[patientCount]) != 1 || selectedSpecialties[patientCount] < 1 || selectedSpecialties[patientCount] > 4) {
        printf("Invalid choice! Enter between 1 and 4: ");
        while(getchar() != '\n');
    }

    assignedWards[patientCount] = -1;
    stayDays[patientCount] = 0;

    printf("\n[SUCCESS] Patient %s Registered Successfully! Assigned ID: %d\n", patientNames[patientCount], patientCount);
    patientCount++;
}

// Requirement 6: Ward Bed Allocation Engine
void allocateWard() {
    if (patientCount == 0) {
        printf("\n[INFO] No patients registered yet!\n");
        return;
    }

    int id;
    printf("\nEnter Patient ID (0 to %d): ", patientCount - 1);
    scanf("%d", &id);

    if (id < 0 || id >= patientCount) {
        printf("[ERROR] Invalid Patient ID!\n");
        return;
    }

    printf("\nSelect Ward Category:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s (LKR %.2f/day)\n", i + 1, WARDS[i], WARD_RATES[i]);
    }
    printf("Enter Ward Choice (1-4): ");
    scanf("%d", &assignedWards[id]);
    assignedWards[id] -= 1;

    printf("Enter Number of Days for Stay: ");
    scanf("%d", &stayDays[id]);

    printf("\n[SUCCESS] Ward %s allocated to Patient %s for %d days.\n",
           WARDS[assignedWards[id]], patientNames[id], stayDays[id]);
}

// Requirement 3 & 6: Billing & Discount Engine
void generateBill() {
    if (patientCount == 0) {
        printf("\n[INFO] No patients registered yet!\n");
        return;
    }

    int id;
    printf("\nEnter Patient ID (0 to %d): ", patientCount - 1);
    scanf("%d", &id);

    if (id < 0 || id >= patientCount) {
        printf("[ERROR] Invalid Patient ID!\n");
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

// Requirement 4: Emergency Triage Display
void displayTriageQueue() {
    if (patientCount == 0) {
        printf("\n[INFO] No patients in the queue!\n");
        return;
    }

    printf("\n--- EMERGENCY TRIAGE QUEUE ---\n");
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

// Requirement 5: Hospital Analytics
void displayAnalytics() {
    if (patientCount == 0) {
        printf("\n[INFO] No data available for analytics!\n");
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
        printf("2. Allocate Ward Bed\n");
        printf("3. Generate Patient Bill\n");
        printf("4. View Triage Queue\n");
        printf("5. View Analytics Summary\n");
        printf("6. Exit\n");
        printf("Enter Choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("[ERROR] Invalid input! Exiting program.\n");
            break;
        }

        if (choice == 1) registerPatient();
        else if (choice == 2) allocateWard();
        else if (choice == 3) generateBill();
        else if (choice == 4) displayTriageQueue();
        else if (choice == 5) displayAnalytics();

    } while(choice != 6);

    printf("\nExiting System. Thank you!\n");
    return 0;
}
