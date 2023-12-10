#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_RECORDS 1000
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
           maxHour, (maxHour + 1) % 24, hourCounts[maxHour]+1);
}

void calculateOccupancyRate(struct BusRecord records[], int count) {
    int occupancyCounts[24] = {0};
    int workingHoursStart = 8;  // Bus working hours start at 8:30
    int workingHoursEnd = 20;   // Bus working hours end at 20:00

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
    int passengersGottenOff[12] = {0}; 

    for (int i = 0; i < count; ++i) {
        int hour, minute;
        sscanf(records[i].time, "%d:%d", &hour, &minute);
        int timeInMinutes = hour * 60 + minute;

        if (timeInMinutes < 510 || timeInMinutes >= 1200) {
            continue;
        }

        int workingHour = (timeInMinutes - 510) / 60; 
        if (strcmp(records[i].gender, "Male") == 0) {
            maleCounts[workingHour] += records[i].passengerCount;
        } else if (strcmp(records[i].gender, "Female") == 0) {
            femaleCounts[workingHour] += records[i].passengerCount;
        }
        if (i < count - 1 && records[i].getOffBusStopID != records[i + 1].getOnBusStopID &&
            passengersGottenOff[workingHour] == 0) {
            int getOffHour, getOffMinute;
            sscanf(records[i + 1].time, "%d:%d", &getOffHour, &getOffMinute);
            int getOffTimeInMinutes = getOffHour * 60 + getOffMinute;

            if (getOffTimeInMinutes < 510 || getOffTimeInMinutes >= 1200) {
                continue; 
            }

            int getOffWorkingHour = (getOffTimeInMinutes - 510) / 60; 

            maleCounts[getOffWorkingHour] -= records[i + 1].passengerCount;
            femaleCounts[getOffWorkingHour] -= records[i + 1].passengerCount;

            passengersGottenOff[workingHour] = 1; 
        }
    }

    for (int i = 0; i < 12; ++i) {
        printf("%02d:30-%02d:30 Male Count: %d, Female Count: %d\n",
               i + 8, (i + 9) % 12 + 8, maleCounts[i], femaleCounts[i]);
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


void findMostCommonBusStops(struct BusRecord records[], int count) {
    int stopPairs[BUS_SEATING_CAPACITY][BUS_SEATING_CAPACITY] = {0};

    for (int i = 0; i < count; ++i) {
        int getOnStop = records[i].getOnBusStopID;
        int getOffStop = records[i].getOffBusStopID;

        stopPairs[getOnStop][getOffStop] += records[i].passengerCount;
    }

    int maxGetOnStop = 0, maxGetOffStop = 0;
    int maxCount = stopPairs[0][0];

    for (int i = 0; i < BUS_SEATING_CAPACITY; ++i) {
        for (int j = 0; j < BUS_SEATING_CAPACITY; ++j) {
            if (stopPairs[i][j] > maxCount) {
                maxCount = stopPairs[i][j];
                maxGetOnStop = i;
                maxGetOffStop = j;
            }
        }
    }

    printf("Most Common Bus Stops: Get On Stop %d, Get Off Stop %d, Passenger Count: %d\n",
           maxGetOnStop, maxGetOffStop, maxCount);
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
    FILE *file = fopen("passenger_data.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    struct BusRecord records[MAX_RECORDS];
    int recordCount = 0;

    while (fgets(line, sizeof(line), file) != NULL && recordCount < MAX_RECORDS) {
    if (sscanf(line, "%19[^,],%19[^,],%d,%9[^,],%d,%d,%d",
               records[recordCount].date, records[recordCount].time,
               &records[recordCount].passengerCount, records[recordCount].gender,
               &records[recordCount].age, &records[recordCount].getOnBusStopID,
               &records[recordCount].getOffBusStopID) != 7) {
        fprintf(stderr, "Error parsing line: %s", line);
        continue;
    }

    recordCount++;
}


    fclose(file);

    int choice;

    do {
    displayMenu();
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            findPeakTravelTime(records, recordCount);
            break;
        case 2:
            calculateOccupancyRate(records, recordCount);
            break;
        case 3:
            countPassengersByGender(records, recordCount);
            break;
        case 4:
            findAgeRange(records, recordCount);
            break;
        case 5:
            findMostCommonBusStops(records, recordCount);
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