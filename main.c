#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDS 80
#define BUS_SEATING_CAPACITY 50

struct BusRecord {
    char date[20];
    char time[20];
    int passengerCount;
    char gender[10];
    int age;
    int getOnBusStopID;
    int getOffBusStopID; 
};

void generateRandomRecords(struct BusRecord records[], int count) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < count; ++i) {
        int hour = 8 + i / 7;
        int minute = 30 + (i % 7) * 7;
        
        if (hour >= 20) {
            break;  
        }

        if (minute >= 60) {
            minute = minute % 60;
            hour += 1;
        }

        sprintf(records[i].date, "2023-01-01");
        sprintf(records[i].time, "%02d:%02d", hour, minute);

        records[i].passengerCount = 1;

        sprintf(records[i].gender, rand() % 2 == 0 ? "Male" : "Female");

        records[i].age = rand() % 64 + 6;
        records[i].getOnBusStopID = rand() % 9 + 1;  
        records[i].getOffBusStopID = rand() % (10 - records[i].getOnBusStopID) + records[i].getOnBusStopID + 1; 
    }
}




void writeRecordsToFiles(struct BusRecord records[], int count) {
    FILE *outputFile = fopen("simulated_data.txt", "w");

    if (outputFile == NULL) {
        perror("Error opening file for writing");
        exit(1);
    }

    for (int i = 0; i < count; ++i) {
        fprintf(outputFile, "%s,%s,%d,%s,%d,%d,%d\n",
                records[i].date, records[i].time, records[i].passengerCount,
                records[i].gender, records[i].age, records[i].getOnBusStopID,
                records[i].getOffBusStopID);
    }

    fclose(outputFile);
}

void findPeakTravelTime(struct BusRecord records[], int count) {
    int hourCounts[24] = {0};
    for (int i = 0; i < count; ++i) {
        int hour;
        sscanf(records[i].time, "%d", &hour);
        hourCounts[hour] += records[i].passengerCount;
    }

    int maxHour = 0;
    for (int i = 1; i < 24; ++i) {
        if (hourCounts[i] > hourCounts[maxHour]) {
            maxHour = i;
        }
    }

    printf("Peak Travel Time: %02d:00-%02d:00, Peak Passenger Count: %d\n",
           maxHour, (maxHour + 1) % 24, hourCounts[maxHour]);
}

void calculateOccupancyRate(struct BusRecord records[], int count) {
    int occupancyCounts[24] = {0};
    int workingHoursStart = 8;  
    int workingHoursEnd = 20;   

  for (int i = 0; i < count; ++i) {
    int hour;
    sscanf(records[i].time, "%d", &hour);

    if (hour >= workingHoursStart && hour < workingHoursEnd) {
        occupancyCounts[hour] += records[i].passengerCount;
    }
}


    for (int i = workingHoursStart; i < workingHoursEnd; ++i) {
        float occupancyRate = (float)occupancyCounts[i] / BUS_SEATING_CAPACITY;
        printf("%02d:00-%02d:00 Occupancy Rate: %.2f%%\n", i, (i + 1) % 24, occupancyRate * 100);
    }
}

void countPassengersByGender(struct BusRecord records[], int count) {
    int maleCounts[12] = {0}; 
    int femaleCounts[12] = {0};

    const char *hourRanges[12] = {
        "08:00-09:00", "09:00-10:00", "10:00-11:00", "11:00-12:00", "12:00-13:00",
        "13:00-14:00", "14:00-15:00", "15:00-16:00", "16:00-17:00", "17:00-18:00",
        "18:00-19:00", "19:00-20:00"
    };

    for (int i = 0; i < count; ++i) {
        int hour, minute;
        sscanf(records[i].time, "%d:%d", &hour, &minute);
        int timeInMinutes = hour * 60 + minute;

        if (timeInMinutes < 480 || timeInMinutes >= 1200) {
            continue;
        }
        int workingHour = (timeInMinutes - 480) / 60;
        workingHour = workingHour >= 12 ? workingHour - 12 : workingHour;

        if (strcmp(records[i].gender, "Male") == 0) {
            maleCounts[workingHour] += records[i].passengerCount;
        } else if (strcmp(records[i].gender, "Female") == 0) {
            femaleCounts[workingHour] += records[i].passengerCount;
        }
    }

    printf("Hour Range      Male Count      Female Count\n");
    for (int i = 0; i < 12; ++i) {
        printf("%s     %d               %d\n", hourRanges[i], maleCounts[i], femaleCounts[i]);
    }
}


void findAgeRange(struct BusRecord records[], int count) {
    const int AGE_0_17 = 0;
    const int AGE_18_35 = 1;
    const int AGE_36_ABOVE = 2;

    int ageRanges[3] = {0}; 

    for (int i = 0; i < count; ++i) {
        if (records[i].age <= 17) {
            ageRanges[AGE_0_17] += records[i].passengerCount;
        } else if (records[i].age <= 35) {
            ageRanges[AGE_18_35] += records[i].passengerCount;
        } else {
            ageRanges[AGE_36_ABOVE] += records[i].passengerCount;
        }
    }

    printf("0-17 Age Range Count: %d\n", ageRanges[AGE_0_17]);
    printf("18-35 Age Range Count: %d\n", ageRanges[AGE_18_35]);
    printf("36 and above Age Range Count: %d\n", ageRanges[AGE_36_ABOVE]);
}

void findMostCommonStops(struct BusRecord records[], int count) {
    int getOnStopCounts[BUS_SEATING_CAPACITY] = {0};
    int getOffStopCounts[BUS_SEATING_CAPACITY] = {0};

    for (int i = 0; i < count; ++i) {
        int getOnStop = records[i].getOnBusStopID;
        int getOffStop = records[i].getOffBusStopID;

        getOnStopCounts[getOnStop] += records[i].passengerCount;
        getOffStopCounts[getOffStop] += records[i].passengerCount;
    }

    int maxGetOnStop = 0, maxGetOffStop = 0;
    int maxGetOnCount = getOnStopCounts[0];
    int maxGetOffCount = getOffStopCounts[0];

    for (int i = 0; i < BUS_SEATING_CAPACITY; ++i) {
        if (getOnStopCounts[i] > maxGetOnCount) {
            maxGetOnCount = getOnStopCounts[i];
            maxGetOnStop = i;
        }
    }

    for (int i = 0; i < BUS_SEATING_CAPACITY; ++i) {
        if (getOffStopCounts[i] > maxGetOffCount) {
            maxGetOffCount = getOffStopCounts[i];
            maxGetOffStop = i;
        }
    }

    printf("Most Common Get-On Stop: %d\n", maxGetOnStop);
    printf("Most Common Get-Off Stop: %d", maxGetOffStop);
}


void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Find Peak Travel Time\n");
    printf("2. Calculate Bus Occupancy Rate\n");
    printf("3. Count Passengers by Gender\n");
    printf("4. Find Age Range\n");
    printf("5. Find Most Common Bus Stops\n");
    printf("6. Exit\n");
}

int main() {
    struct BusRecord records[MAX_RECORDS];

    generateRandomRecords(records, MAX_RECORDS);
    writeRecordsToFiles(records, MAX_RECORDS);


    int choice;

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                findPeakTravelTime(records, MAX_RECORDS);
                break;
            case 2:
                calculateOccupancyRate(records, MAX_RECORDS);
                break;
            case 3:
                countPassengersByGender(records, MAX_RECORDS);
                break;
            case 4:
                findAgeRange(records, MAX_RECORDS);
                break;
            case 5:
                findMostCommonStops(records, MAX_RECORDS);
                break;
            case 6:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);

    return 0;
}
