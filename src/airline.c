#include "../lib/flightlib.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void menu() {
   // load the menu
   const char *start = "\n| Airline Database System |\n";
   while (1) {
      printf("\n");
      for (int i = 0; i <= strlen(start); i++) {
         printf("=");
      }
      printf(start);
      for (int i = 0; i <= strlen(start); i++) {
         printf("=");
      }
      printf("\n");
      printf("1. View Flights\n");
      printf("2. Add Flight\n");
      printf("3. Delete Flight\n");
      printf("4. Update Flight\n");
      printf("5. View Reservation\n");
      printf("6. Book Reservation\n");
      printf("7. Cancel Reservation\n");
      printf("8. Update Reservation\n");
      printf("9. Exit\n");
      printf("Enter your choice: ");
      int choice;
      scanf("%d", &choice);

      if (choice == 1) {
         viewFlight();
      } else if (choice == 2) {
         addFlight();
      } else if (choice == 3) {
         deleteFlight();
      } else if (choice == 4) {
         updateFlight();
      } else if (choice == 5) {
         viewReservation();
      } else if (choice == 6) {
         makeReservation();
      } else if (choice == 7) {
         cancelReservation();
      } else if (choice == 8) {
         updateReservation();
      } else if (choice == 9) {
         printf("Exiting. Goodbye\n");
         break;
      } else {
         printf("Please input number from range 1-9\n");
         sleep(1);
      }
   }
}

void initFile(const char file[]) {
   // create the file
   FILE *dbFile = fopen(file, "w");
   if (dbFile == NULL) {
      perror("Error creating file");
   }
   fclose(dbFile);
}

int main() {
   // load the file (if not exist then create it using initFile())
   FILE *flight = fopen(flightFile, "r");
   FILE *reserve = fopen(reserveFile, "r");

   if (flight == NULL) {
      perror("\nError opening flight database");

      printf("Creating database file\n");
      printf("Initalizing DB file\n");
      initFile(flightFile);
   }

   if (reserve == NULL) {
      perror("\nError opening reservation database");

      // printf("\033[0;33m");
      printf("Creating database file\n");
      printf("Initalizing DB file\n");
      // printf("\033[0m");
      initFile(reserveFile);
   }

   menu();

   if (flight != NULL && fclose(flight) != 0) {
      perror("Error closing file");
   }
   if (reserve != NULL && fclose(reserve) != 0) {
      perror("Error opening file");
   }

   return 0;
}