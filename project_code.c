//C PROJECT- MOVING TICKET BOOKING PROJECT
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROWS 10
#define COLS 10
#define MOVIES 4
//Pricing Function
int getPrice(int row){
    if (row < 3) return 150; // front rows
    if (row <7 ) return 200; // middle row
    return 250;  //back
}
typedef struct{
    char name[50];
    int seat[ROWS][COLS]; // 0= available , 1= booked
    int revenue;
} show;

typedef struct{
    char customerName[50];
    char phone[15];
    char movieName[50];
    int row,col,price;
} Booking;

show movies[MOVIES];

void initializeSeats(){
    char movieNames[MOVIES][50] = {"Avengers: Endgame","Inception","Interstellar","The Dark Knight"};
    for(int m = 0; m < MOVIES; m++){
        for(int i=0; i<ROWS; i++){
            for(int j = 0;j< COLS; j++){
                movies[m].seat[i][j] = 0; //All seats Available
            }
        }
        movies[m].revenue = 0;
        sprintf(movies[m].name,"%s", movieNames[m]);
    }
}
void displaySeats(int m){
    printf("\n-- Seating Arrangement for %s ---\n",movies[m].name);
    for(int i=0; i < ROWS; i++){
        printf("Row %2d: ", i + 1);
        for(int j = 0; j < COLS; j++){
            if(movies[m].seat[i][j] == 0)
            printf("%2d( )",j + 1); //Available
            else
            printf("%2d(X) ",j + 1); //Booked
        }
        printf("\n");
    }
}

void displayAvailableSeats(int m) {
    printf("\n--- Available Seats for %s ---\n", movies[m].name);
    for (int i = 0; i < ROWS; i++){
        for(int j = 0; j< COLS; j++){
            if (movies[m].seat[i][j] == 0){
                printf("Row %d Seat %d | Price Rs%d\n", i+1,j+1,getPrice(i));          
              }
        }
    }
}
void saveBookingToFile(Booking b){
    FILE *fp = fopen("bookings.txt","a");
    if (!fp){
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "Name: %s | Phone: %s | Movie: %s | Row: %d | Seat: %d | Price: Rs%d\n",
    b.customerName,b.phone,b.movieName,b.row,b.col,b.price);
    fclose(fp);
}

void bookTicket(int m){
    Booking b;
    int row,col;

    displayAvailableSeats(m);

    printf("\n Enter your name: ");
     scanf(" %[^\n]", b.customerName);
    printf("Enter your phone number: ");
    scanf("%s", b.phone);

    printf("Enter row (1-%d): ", ROWS);
    scanf("%d", &row);
    printf("Enter seat number (1-%d): ", COLS);
    scanf("%d", &col);

    row--; col--; // adjust index
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        printf("Invalid seat selection!\n");
        return;
    }
    if (movies[m].seat[row][col] == 1) {
        printf("Seat already booked!\n");
        return;
    }

// confirm booking
movies[m].seat[row][col] = 1;
int price = getPrice(row);
movies[m].revenue += price;

// save booking details
strcpy(b.movieName, movies[m].name);
b.row = row + 1;
b.col = col + 1;
b.price = price;

saveBookingToFile(b);

printf("\n Booking Confirmed!\n");
printf("Name: %s\nPhone: %s\nMovie: %s\nRow: %d Seat: %d\nPrice: Rs%d\n",
    b.customerName, b.phone, b.movieName, b.row, b.col,b.price);
    printf("\n Pay here: https://payment.expample.com/pay/%s%d%d\n",
    b.customerName,b.row,b.col); // mock payment link
}

void cancelTicket(int m){
    int row,col;
    displaySeats(m);
    printf("\nEnter row (1-%d): ",ROWS);
    scanf("%d",&row);
    printf("Enter seat number (1-%d): ",COLS);
    scanf("%d",&col);

    row--; col--; // adjust index
    if(row < 0 || row >= ROWS || col < 0 || col >= COLS){
        printf("Invalid seat selection!\n");
        return;
    }
   if  (movies[m].seat[row][col] == 0){
        printf("Seat is not booked!\n");
        return;
    }
    movies[m].seat[row][col] = 0;
    int price = getPrice(row);
    movies[m].revenue -= price;
    printf("Ticket canceled successfully! Refunded: Rs%d\n", price);
}

void saveToFile(int m){
    FILE *fp = fopen("daily_records.txt","a");
    if (!fp){
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "Movie: %s | Revenue: ₹%d\n", movies[m].name, movies[m].revenue);
    fclose(fp);
    printf("Daily record saved to file.\n");
}
int chooseMovie(){
    printf("\n-- Select a Movie ---\n");
    for (int i = 0; i < MOVIES; i++){
        printf("%d. %s\n", i + 1, movies[i].name);
    }
    printf("Enter choice: ");
    int choice;
    scanf("%d", &choice);
    if(choice < 1 || choice > MOVIES){
        printf("Invalid choice!\n");
        return -1;
    }
    return choice -1;
}
int main(){
    int choice,selectedMovie;
    initializeSeats();

    do{
        printf("\n-- Movie Ticket Booking system ---\n");
        printf("1. Display Seat Matrix\n");
        printf("2. Show Available Seats (List)\n");
        printf("3. Book Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. Show Revenue\n");
        printf("6. Save Daily Records\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(choice >= 1 && choice <= 6){
            selectedMovie = chooseMovie();
            if (selectedMovie == -1)continue;
        }
        switch (choice)
        {
            case 1: displaySeats(selectedMovie); break;
            case 2: displayAvailableSeats(selectedMovie); break;
            case 3: bookTicket(selectedMovie); break;
            case 4: cancelTicket(selectedMovie); break;
            case 5: printf("Revenue for %s: Rs%d\n",movies[selectedMovie].name,movies[selectedMovie].revenue); break;
            case 6: saveToFile(selectedMovie); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");   
        
        }
    } while (choice != 7);
    
    return 0;
}