#ifndef __FLIGHTLIB_H_
#define __FLIGHTLIB_H_

#define MAX_YEAR 9999
#define MIN_YEAR 1800

typedef struct {
   char identifier[10];
   char departureCity[50];
   char destinationCity[50];
   char departureTime[50];
   char destinationTime[50];
   int firstClassAvail;
   int firstClassSold;
   int coachAvail;
   int coachSold;
} Flights;

typedef struct {
   int reserveID;
   char flightIdentifier[10];
   char name[512];
   char seatClass[256];
   int seatNum;
} Reservation;

extern const char header[256];
extern const char reserveHeader[256];
extern const char reserveFile[256];
extern const char flightFile[256];

int isLeap(int yy);
int validateDate(char *str);
int validateTime(char *str);

void viewFlight();
void addFlight();
void deleteFlight();
void updateFlight();

void viewReservation();
void makeReservation();
void cancelReservation();
void updateReservation();

#endif