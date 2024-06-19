#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME3 "C:\\Users\\NEW\\Desktop\\Food Delivery App\\restaurants_ratings.csv"
#define BUFFER_SIZE 1024

// Function to update the review in the CSV file
void update_review(const char* restaurant_name, double new_rating) {
    FILE *file = fopen(FILENAME3, "r+");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char buffer[BUFFER_SIZE];
    long pos = 0;  // To keep track of the position in the file
    int found = 0;

    while (fgets(buffer, BUFFER_SIZE, file)) {
        char name[100];
        double avg_rating;
        int num_reviews;
        long line_start = pos;  // Starting position of the current line

        // Parse the line
        sscanf(buffer, "%99[^,],%lf,%d", name, &avg_rating, &num_reviews);

        // Calculate the position right after reading the line
        pos = ftell(file);

        // Check if it's the restaurant we're looking for
        if (strcasecmp(name, restaurant_name) == 0) {
            num_reviews++;
            avg_rating = ((avg_rating * (num_reviews - 1)) + new_rating) / num_reviews;
            found = 1;

            // Move file pointer back to the start of the current line
            fseek(file, line_start, SEEK_SET);
            fprintf(file, "%s,%.1f,%d\n", name, avg_rating, num_reviews);

            // Padding in case the new line is shorter than the old
            long new_pos = ftell(file);
            int padding = pos - new_pos;
            for(int i = 0; i < padding; i++) {
                fputc(' ', file);
            }
            break;
        }
    }

    if (!found) {
        // If no matching restaurant found, add a new entry
        fprintf(file, "%s,%.1f,1\n", restaurant_name, new_rating);
    }

    fclose(file);
    if (found) {
        printf("Updated \"%s\" successfully.\n", restaurant_name);
    } else {
        printf("Added \"%s\" successfully.\n", restaurant_name);
    }
}

int main() {
    char restaurant_name[100];
    double stars;

    printf("Enter restaurant name: ");
    fgets(restaurant_name, sizeof(restaurant_name), stdin);
    restaurant_name[strcspn(restaurant_name, "\n")] = 0;  // Remove newline character

    printf("Enter new rating (0.0 to 10.0): ");
    scanf("%lf", &stars);

    update_review(restaurant_name, stars);

    return 0;
}
