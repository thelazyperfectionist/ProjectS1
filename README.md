# Bus Passenger Data Analysis
This C program analyzes bus passenger data from a file and provides insights into peak travel times, bus occupancy rates, passenger counts by gender, age ranges, and the most common bus stops.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [File Format](#file-format)
- [Contributing](#contributing)
- [License](#license)

## Overview
This program reads bus passenger data from a file named `passenger_data.txt` and performs various analyses on the data. It provides statistics on peak travel times, bus occupancy rates, passenger counts by gender, age ranges, and identifies the most common bus stops.

## Features
1. **Find Peak Travel Time:** Identifies the peak travel time based on the highest passenger count during different hours.
2. **Calculate Bus Occupancy Rate:** Calculates the occupancy rate during working hours (8:30 to 20:00) for each hour.
3. **Count Passengers by Gender:** Counts the number of male and female passengers during specific time intervals.
4. **Find Age Range:** Categorizes passengers into age ranges (0-17, 18-35, 36 and above) and counts the number in each range.
5. **Find Most Common Bus Stops:** Identifies the most common bus stop pair where passengers get on and off.


## Usage
After running the program, a menu will be displayed allowing you to choose from different analyses. Follow the on-screen instructions to input your choice and view the results.

## File Format
The input file (`passenger_data.txt`) should follow the format:
date,time,passengerCount,gender,age,getOnBusStopID,getOffBusStopID
For example:
2023-12-11,08:45,20,Male,25,3,7
