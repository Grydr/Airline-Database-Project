#include "flightlib.h"
#include <stdio.h>
#include <string.h>

const char header[256] = "Flight Database\n";
const char reserveHeader[256] = "Reservation Database\n";
const char reserveFile[256] = "data/reservationDB.txt";
const char flightFile[256] = "data/airlineDB.txt";

void removeNewLine(char *str) {
   int len = strlen(str);
   if (len > 0 && str[len - 1] == '\n') {
      str[len - 1] = '\0';
   }
}

// ex. 12/12/2024 19:00
/*
 * Date can’t be less than 1 and more than 31
 * Month can’t be less than 1 and more than 12
 * Year can’t be less than 1800 and more than 9999
 * When the months are April, June, September, November the date can’t be more than 30.
 * When the month is February we have to check whether,
 * If the year is leap year then the number of days can’t be more than 29
 * Else the number of days can’t be more than 28.
 */

int isLeap(int yy) {
   if (yy % 400 == 0) {
      return 1;
   } else if (yy % 100 == 0) {
      return 0;
   } else if (yy % 4 == 0) {
      return 1;
   }
   return 0;
}

int validateDate(char *str) {
   int dd, mm, yy;
   dd = mm = yy = 0;
   sscanf(str, "%2d/%2d/%4d", &dd, &mm, &yy);

   // check if date is within boundary
   if (yy < MIN_YEAR || yy > MAX_YEAR) {
      printf("Year must be between 1800 - 9999\n");
      return 0;
   }
   if (mm < 1 || mm > 12) {
      printf("Month must be between 1 - 12\n");
      return 0;
   }
   if (dd < 1 || dd > 31) {
      printf("Day must be between 1 - 31\n");
      return 0;
   }

   // check if leap year on february
   if (mm == 2) {
      if (isLeap(yy)) {
         if (dd <= 29) {
            return 1;
         } else {
            printf("Day must be between 1 - 29 (Leap Year)\n");
            return 0;
         }
      }
   }

   // check day on April, June, September, November
   if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
      if (dd <= 30) {
         return 1;
      } else {
         printf("Day must be between 1 - 30\n");
         return 0;
      }
      return 1;
   }
}

int validateTime(char *str) {
   int hour, min;
   hour = min = 0;

   char cutStr[256];
   // strncpy(cutStr, str + 11, 5);
   strcpy(cutStr, str + 11);
   sscanf(cutStr, "%2d:%2d", &hour, &min);

   // validate hour & min
   if (hour < 0 || hour > 23) {
      printf("Hour must be between 0 - 23\n");
      return 0;
   }
   if (min < 0 || min > 59) {
      printf("Minutes must be between 0 - 59\n");
      return 0;
   }
   return 1;
}

void viewFlight() {
   FILE *file = fopen(flightFile, "r");
   if (file == NULL) {
      perror("Error opening file");
      return;
   }

   // prettiefied formatting
   printf("\n");
   int strLen = strlen(header);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   printf(header);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");

   char ch = fgetc(file);
   if (ch == EOF) {
      printf("Database file is empty\n");
      return;
   } else {
      ungetc(ch, file);
   }

   while ((ch = fgetc(file)) != EOF) {
      printf("%c", ch);
   }
   printf("\n");
   fclose(file);
}

void addFlight() {
   int Count;
   printf("How many flight to add: ");
   scanf("%d", &Count);

   Flights flight[Count];

   FILE *file = fopen(flightFile, "a");
   if (file == NULL) {
      perror("Error opening file");
      fclose(file);
      return;
   }
   for (int i = 0; i < Count; i++) {
      printf("\nEnter flight number (XXX-XXX): ");
      scanf("%s", flight[i].identifier);

      printf("City of Departure (XXX): ");
      scanf("%s", flight[i].departureCity);

      printf("City of Destination (XXX): ");
      scanf("%s", flight[i].destinationCity);

      while ((getchar()) != '\n') // clear input buffer
         ;
      while (1) {
         printf("Date & Time of Departure (DD/MM/YYYY HH:MM): ");
         fgets(flight[i].departureTime, sizeof(flight[0].departureTime), stdin);
         removeNewLine(flight[i].departureTime);

         if (validateDate(flight[i].departureTime) && validateTime(flight[i].departureTime)) {
            break;
         } else {
            printf("Invalid Date or Time, please try again\n");
         }
      }

      while (1) {
         printf("Date & Time of Arrival (DD/MM/YYYY HH:MM): ");
         fgets(flight[i].destinationTime, sizeof(flight[0].destinationTime), stdin);
         removeNewLine(flight[i].destinationTime);

         if (validateDate(flight[i].destinationTime) && validateTime(flight[i].destinationTime)) {
            break;
         } else {
            printf("Invalid Date or Time, please try again\n");
         }
      }

      printf("First Class Seats Still Available: ");
      scanf("%d", &flight[i].firstClassAvail);

      printf("First Class Seats Sold: ");
      scanf("%d", &flight[i].firstClassSold);

      printf("Coach Seats Still Available: ");
      scanf("%d", &flight[i].coachAvail);

      printf("Coach Seats Sold: ");
      scanf("%d", &flight[i].coachSold);

      fprintf(file, "%s ", flight[i].identifier);
      fprintf(file, "%s ", flight[i].departureCity);
      fprintf(file, "%s ", flight[i].destinationCity);
      fprintf(file, "%s ", flight[i].departureTime);
      fprintf(file, "%s ", flight[i].destinationTime);
      fprintf(file, "%d ", flight[i].firstClassAvail);
      fprintf(file, "%d ", flight[i].firstClassSold);
      fprintf(file, "%d ", flight[i].coachAvail);
      fprintf(file, "%d \n", flight[i].coachSold);
      printf("\nFlights successfully Added\n");

      // flightCount++;
   }
   fclose(file);
}

void deleteFlight() {
   FILE *file = fopen(flightFile, "r");
   FILE *temp = fopen("temp.txt", "w");
   if (file == NULL || temp == NULL) {
      perror("Error opening file");
      fclose(file);
      fclose(temp);
      return;
   }

   // prettiefied formatting
   printf("\n");
   int strLen = strlen(header);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   printf(header);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   char ch = fgetc(file);

   if (ch == EOF) {
      printf("Database file is empty\n");
      return;
   } else {
      ungetc(ch, file);
   }

   while ((ch = fgetc(file)) != EOF) {
      printf("%c", ch);
   }
   rewind(file);

   int found = 0;
   char choice[10];
   printf("Enter Flight identifier to delete: ");
   scanf("%s", choice);

   char line[512];
   while (fgets(line, sizeof(line), file) != NULL) {
      char currentID[10];
      sscanf(line, "%s", currentID);

      if (strcmp(currentID, choice) != 0) {
         fputs(line, temp);
      } else {
         found = 1;
      }
   }

   fclose(temp);
   fclose(file);

   if (found) {
      remove(flightFile);
      rename("temp.txt", flightFile);
      printf("\nFlight with identifier %s successfully removed\n", choice);
   } else {
      remove("temp.txt");
      printf("\nFlight with identifier %s not found\n", choice);
   }
}

void updateFlight() {
   FILE *file = fopen(flightFile, "r");
   FILE *temp = fopen("temp.txt", "w");
   if (file == NULL || temp == NULL) {
      perror("Error opening file");
      fclose(file);
      fclose(temp);
      return;
   }

   // prettiefied formatting
   printf("\n");
   int strLen = strlen(header);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   printf(header);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");

   // check if database is empty
   char ch = fgetc(file);
   if (ch == EOF) {
      printf("Database file is empty\n");
      return;
   } else {
      ungetc(ch, file);
   }

   while ((ch = fgetc(file)) != EOF) {
      printf("%c", ch);
   }
   rewind(file);

   int found = 0;
   char choice[10];
   printf("Enter Flight identifier to update: ");
   scanf("%s", choice);

   char line[512];
   while (fgets(line, sizeof(line), file) != NULL) {
      char currentID[10];
      sscanf(line, "%s", currentID);

      if (strcmp(currentID, choice) == 0) {
         found = 1;

         Flights newFlight;
         printf("\nEnter new flight number (XXX-XXX): ");
         scanf("%s", newFlight.identifier);

         printf("Enter new City of Departure (XXX): ");
         scanf("%s", newFlight.departureCity);

         printf("Enter new City of Destination (XXX): ");
         scanf("%s", newFlight.destinationCity);

         while (1) {
            printf("Enter new Date & Time of Departure (DD/MM/YYYY HH:MM): ");
            while ((getchar()) != '\n')
               ;
            fgets(newFlight.departureTime, sizeof(newFlight.departureTime), stdin);
            removeNewLine(newFlight.departureTime);

            if (validateDate(newFlight.departureTime) && validateTime(newFlight.departureTime)) {
               break;
            } else {
               printf("Invalid Date or Time. Please try again.\n");
            }
         }

         while (1) {
            printf("Enter new Date & Time of Arrival (DD/MM/YYYY HH:MM): ");
            fgets(newFlight.destinationTime, sizeof(newFlight.destinationTime), stdin);
            removeNewLine(newFlight.destinationTime);

            if (validateDate(newFlight.destinationTime) && validateTime(newFlight.destinationTime)) {
               break;
            } else {
               printf("Invalid Date or Time. Please try again.\n");
            }
         }

         printf("Enter new First Class Seats Still Available: ");
         scanf("%d", &newFlight.firstClassAvail);

         printf("Enter new First Class Seats Sold: ");
         scanf("%d", &newFlight.firstClassSold);

         printf("Enter new Coach Seats Still Available: ");
         scanf("%d", &newFlight.coachAvail);

         printf("Enter new Coach Seats Sold: ");
         scanf("%d", &newFlight.coachSold);

         fprintf(temp, "%s ", newFlight.identifier);
         fprintf(temp, "%s ", newFlight.departureCity);
         fprintf(temp, "%s ", newFlight.destinationCity);
         fprintf(temp, "%s ", newFlight.departureTime);
         fprintf(temp, "%s ", newFlight.destinationTime);
         fprintf(temp, "%d ", newFlight.firstClassAvail);
         fprintf(temp, "%d ", newFlight.firstClassSold);
         fprintf(temp, "%d ", newFlight.coachAvail);
         fprintf(temp, "%d \n", newFlight.coachSold);
      } else {
         fputs(line, temp);
      }
   }

   fclose(temp);
   fclose(file);

   if (found) {
      remove(flightFile);
      rename("temp.txt", flightFile);
      printf("\nFlight with identifier %s successfully updated\n", choice);
   } else {
      remove("temp.txt");
      printf("\nFlight with identifier %s not found\n", choice);
   }
}

void viewReservation() {
   FILE *file = fopen(reserveFile, "r");
   if (file == NULL) {
      perror("Error opening file");
      fclose(file);
      return;
   }

   // prettiefied formatting
   printf("\n");
   int strLen = strlen(reserveHeader);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   printf(reserveHeader);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");

   char ch = fgetc(file);
   if (ch == EOF) {
      printf("Database file is empty\n");
      return;
   } else {
      ungetc(ch, file);
   }

   char line[256];
   while (fgets(line, sizeof(line), file) != NULL) {
      printf("%s", line);
   }
   fclose(file);
}

void makeReservation() {
   FILE *flight = fopen(flightFile, "r");
   FILE *reserve = fopen(reserveFile, "a");
   if (flight == NULL || reserve == NULL) {
      perror("Error opening file");
      fclose(flight);
      fclose(reserve);
      return;
   }

   int Count;
   printf("How many reservation to add: ");
   scanf("%d", &Count);

   Reservation reservation[Count];

   // prettiefied formatting
   printf("\n");
   char *availFlight = "Available Flight\n";
   int strLen = strlen(availFlight);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   printf(availFlight);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");

   // Show Available Flights
   char ch = fgetc(flight);
   if (ch == EOF) {
      printf("Database is empty\n");
   } else {
      ungetc(ch, flight);
   }

   while ((ch = fgetc(flight)) != EOF) {
      printf("%c", ch);
   }
   fclose(flight);

   for (int i = 0; i < Count; i++) {
      printf("\nEnter reservation ID: ");
      scanf("%d", &reservation[i].reserveID);

      printf("Enter flight number to reserve (XXX-XXX): ");
      scanf("%s", reservation[i].flightIdentifier);

      printf("Enter your name: ");
      while (getchar() != '\n')
         ;
      fgets(reservation[i].name, sizeof(reservation[0].name), stdin);
      removeNewLine(reservation[i].name);

      printf("Enter your seat class (First or Couch): ");
      scanf("%s", reservation[i].seatClass);

      printf("Number of seats: ");
      scanf("%d", &reservation[i].seatNum);

      fprintf(reserve, "%d ", reservation[i].reserveID);
      fprintf(reserve, "%s ", reservation[i].flightIdentifier);
      fprintf(reserve, "%s ", reservation[i].name);
      fprintf(reserve, "%s ", reservation[i].seatClass);
      fprintf(reserve, "%d \n", reservation[i].seatNum);

      printf("\nReservation successfully added\n");
   }
   fclose(reserve);
}

void cancelReservation() {
   FILE *file = fopen(reserveFile, "r");
   FILE *temp = fopen("temp.txt", "w");
   if (file == NULL || temp == NULL) {
      perror("Error opening file");
      fclose(file);
      fclose(temp);
      return;
   }

   // prettiefied formatting
   printf("\n");
   int strLen = strlen(reserveHeader);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   printf(reserveHeader);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");

   char ch = fgetc(file);
   if (ch == EOF) {
      printf("Database file is empty\n");
      return;
   } else {
      ungetc(ch, file);
   }

   while ((ch = fgetc(file)) != EOF) {
      printf("%c", ch);
   }
   rewind(file);

   int found = 0;
   int choice;
   printf("Enter Reservation identifier to cancel: ");
   scanf("%d", &choice);

   char line[256];
   while (fgets(line, sizeof(line), file) != NULL) {
      int currentID = 0;
      sscanf(line, "%1d", &currentID);

      if (currentID != choice) {
         fputs(line, temp);
      } else {
         found = 1;
      }
   }

   fclose(temp);
   fclose(file);

   if (found) {
      remove(reserveFile);
      rename("temp.txt", reserveFile);
      printf("\nReservation with identifier %d successfully removed\n", choice);
   } else {
      remove("temp.txt");
      printf("\nReservation with identifier %d not found\n", choice);
   }
}

void updateReservation() {
   FILE *file = fopen(reserveFile, "r");
   FILE *temp = fopen("temp.txt", "w");
   if (file == NULL || temp == NULL) {
      perror("Error opening file");
      fclose(file);
      fclose(temp);
      return;
   }

   // prettiefied formatting
   printf("\n");
   int strLen = strlen(reserveHeader);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");
   printf(reserveHeader);
   for (int i = 0; i <= strLen; i++) {
      printf("=");
   }
   printf("\n");

   char ch = fgetc(file);
   if (ch == EOF) {
      printf("Database file is empty\n");
      return;
   } else {
      ungetc(ch, file);
   }

   while ((ch = fgetc(file)) != EOF) {
      printf("%c", ch);
   }
   rewind(file);

   int found = 0;
   int choice;
   printf("Enter reservation identifier to update: ");
   scanf("%d", &choice);

   char line[512];
   while (fgets(line, sizeof(line), file) != NULL) {
      int currentID = 0;
      sscanf(line, "%1d", &currentID);

      if (currentID == choice) {
         found = 1;

         Reservation newReservation;
         printf("\nEnter new flight identifier to reserve (XXX-XXX): ");
         scanf("%s", newReservation.flightIdentifier);
         printf("Enter new name: ");
         while (getchar() != '\n')
            ;
         fgets(newReservation.name, sizeof(newReservation.name), stdin);
         removeNewLine(newReservation.name);

         printf("Enter new seat class (First or Couch): ");
         scanf("%s", newReservation.seatClass);

         printf("Enter new number of seats: ");
         scanf("%d", &newReservation.seatNum);

         fprintf(temp, "%d ", choice);
         fprintf(temp, "%s ", newReservation.flightIdentifier);
         fprintf(temp, "%s ", newReservation.name);
         fprintf(temp, "%s ", newReservation.seatClass);
         fprintf(temp, "%d \n", newReservation.seatNum);
      } else {
         fputs(line, temp);
      }
   }

   fclose(temp);
   fclose(file);

   if (found) {
      remove(reserveFile);
      rename("temp.txt", reserveFile);
      printf("\nReservation with identifier %d successfully updated\n", choice);
   } else {
      remove("temp.txt");
      printf("\nReservation with identifier %d not found\n", choice);
   }
}
