#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct students {
    char First_name[50];
    char Last_name[50];
    int ID;
    char Gender;
    int Academic_year;
    float GPA;
};
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void add_student(struct students *num, char first_name[50], char last_name[50], int id, char gender, int academic_year, float gpa) {
    strcpy(num->First_name, first_name);
    strcpy(num->Last_name, last_name);
    num->ID = id;
    num->Gender = gender;
    num->Academic_year = academic_year;
    num->GPA = gpa;
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int contains_invalid_characters(const char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) || isspace(str[i]) || ispunct(str[i])) {
            return 1; // Invalid character found
        }
    }
    return 0; // No invalid character found
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int check_existing_id(int id) {
    FILE *file = fopen("student_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 0;
    }

    int existing_id;
    while (fscanf(file, "%*s %*s %d %*c %*d %*f", &existing_id) != EOF) {
        if (existing_id == id) {
            fclose(file);
            return 1; // ID exists
        }
    }

    fclose(file);
    return 0; // ID does not exist
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void update_student(int id, int option, float new_value, FILE *file) {
    struct students student;
    FILE *temp_file = fopen("temp_student_data.txt", "w");
    if (temp_file == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    int found = 0;
    // Rewind the file pointer to the beginning
    rewind(file);
    // Read and write the table header to the temporary file
    char header[100];
    fgets(header, sizeof(header), file);
    fprintf(temp_file, "%s", header);

    // Read each student record from the file
    while (fscanf(file, "%s %s %d %c %d %f", student.First_name, student.Last_name, &student.ID, &student.Gender, &student.Academic_year, &student.GPA) != EOF) {
        if (student.ID == id) {
            found = 1;
            if (option == 1) { // Update Academic Year
                student.Academic_year = (int)new_value;
            } else if (option == 2) { // Update GPA
                student.GPA = new_value;
            }
        }
        // Write the student record to the temporary file with proper formatting
        fprintf(temp_file, "%s\t%s\t%d\t%c\t%d\t%.2f\n", student.First_name, student.Last_name, student.ID, student.Gender, student.Academic_year, student.GPA);
    }

    fclose(file);
    fclose(temp_file);

    if (!found) {
        printf("ID not found. No student with ID %d exists in the system.\n", id);
        remove("temp_student_data.txt"); // Remove the temporary file
        return;
    }

    remove("student_data.txt"); // Remove the original file
    rename("temp_student_data.txt", "student_data.txt"); // Rename the temporary file to the original file

    printf("Operation Successful. Student with ID %d has been updated.\n", id);
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


void search_student(const char *query, FILE *file) {
    struct students student;
    int found = 0;

    printf("\nSearch Results:\n");
    printf("---------------------------------------------------------------------------\n");
    printf("First Name\tLast Name\tID\tGender\tYear\tGPA\n");
    printf("---------------------------------------------------------------------------\n");

    // Read each student record from the file and compare with the query
    while (fscanf(file, "%s %s %d %c %d %f", student.First_name, student.Last_name, &student.ID, &student.Gender, &student.Academic_year, &student.GPA) != EOF) {
        if (strcmp(query, student.First_name) == 0 || strcmp(query, student.Last_name) == 0 || atoi(query) == student.ID) {
            printf("%-12s\t%-11s\t%d\t%c\t%d\t%.2f\n", student.First_name, student.Last_name, student.ID, student.Gender, student.Academic_year, student.GPA);
            found = 1;
        }
    }

    if (!found) {
        printf("No student found with name or ID: %s\n", query);
    }
    printf("---------------------------------------------------------------------------\n");
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void remove_student(int id) {
    FILE *file = fopen("student_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    FILE *temp_file = fopen("temp_student_data.txt", "w");
    if (temp_file == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    int existing_id;
    int found = 0;
    char line[100]; // assuming maximum length of a line is 100 characters

    // Read each line from the file
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%*s %*s %d", &existing_id); // Extract ID from the line

        // If ID matches, skip writing this line to the temporary file
        if (existing_id == id) {
            found = 1;
            continue;
        }

        // Write the line to the temporary file
        fprintf(temp_file, "%s", line);
    }

    fclose(file);
    fclose(temp_file);

    if (!found) {
        printf("ID not found. No student with ID %d exists in the system.\n", id);
        remove("temp_student_data.txt"); // Remove the temporary file
        return;
    }

    remove("student_data.txt"); // Remove the original file
    rename("temp_student_data.txt", "student_data.txt"); // Rename the temporary file to the original file

    printf("Operation Successful. Student with ID %d has been removed from the system.\n", id);
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main()
{
    struct students student;
    char first_name[50];
    char last_name[50];
    int id;
    char gender;
    int academic_year;
    float gpa;
    int c=0;
    int choice;
    while(choice != 5)
    {
        printf("please enter one of the following numbers:\n\n1\tfor adding a student.\n2\tfor removing a student.\n3\tfor searching for a student.\n4\tfor editing student status(GPA & Acadmic year).\n5\tTo EXIT.\nYour choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice)
        {
            case 1:
            {
                FILE *file = fopen("student_data.txt", "a+"); // Open file in append mode

                if (file == NULL) {
                    printf("Error opening file!\n");
                    return 1;
                }

                // Check if file is empty
                fseek(file, 0, SEEK_END);
                if (ftell(file) == 0) {
                    // Write header if file is empty
                    fprintf(file, "First\tLast\tID\tGender\tYear\tGPA\n");
                }

                printf("Please enter the student info:\n");

                getchar();
                while(1){
                    puts("First Name: ");
                    gets(&first_name);
                    if (!contains_invalid_characters(first_name)) {
                        break;
                    }
                    else {
                        printf("Invalid input! Please enter a string without numbers, spaces, or special characters.\n");
                    }
                }

                puts("Last Name: ");
                while(1){
                    gets(&last_name);
                    if (!contains_invalid_characters(last_name)) {
                        break;
                    }
                    else {
                        printf("Invalid input! Please enter a string without numbers, spaces, or special characters.\n");
                    }
                }

                do {
                    printf("ID (must be exactly 7 digits): ");
                    scanf("%d", &id);

                    // Check if ID already exists
                    if (check_existing_id(id)) {
                        printf("ID %d already exists. Please enter a different ID.\n", id);
                        // Clear input buffer
                        while (getchar() != '\n');
                    }
                    else if (id < 1000000 || id > 9999999) {
                        printf("ID must be exactly 7 digits. Please enter a valid ID.\n");
                    }
                } while (check_existing_id(id) || id < 1000000 || id > 9999999);

                while(c==0){
                    printf("Gender (M for male & F for female): ");
                    scanf(" %c", &gender);

                    while (getchar() != '\n');

                    if (gender=='M' || gender=='m')
                    {
                        c++;
                    }
                    else if(gender=='F' || gender=='f')
                    {
                        c++;
                    }
                    else
                    {
                        printf("INVALID INPUT. enter M for male & F for female.\n");
                    }
                }
                float is_whole;
                while(1){
                    printf("Year (from 1 to 5 years): ");
                    scanf("%d", &academic_year);
                    is_whole = (int)academic_year;
                    if(academic_year >= 1 && academic_year <= 5)
                    {
                        break;
                    }
                    else
                    {
                        printf("enter a number between 1 and 5.\n");
                    }
                }

                while(1){
                    printf("GPA: ");
                    scanf("%f", &gpa);

                    if(gpa >= 0.0 && gpa <= 4.0)
                    {
                        break;
                    }
                    else
                    {
                        printf("your GPA must be between 1.00 and 4.00\n");
                    }
                }


                add_student(&student, first_name, last_name, id, gender, academic_year, gpa);

                // Write data to file in tabular form
                fprintf(file, "%s\t%s\t%d\t%c\t%d\t%.2f\n", student.First_name, student.Last_name, student.ID, student.Gender, student.Academic_year, student.GPA);

                fclose(file); // Close the file
                break;


            }

            case 2:
            {

                int ID;
                printf("please enter the ID of the student u want to remove: ");
                scanf("%d", &ID);
                printf("\n");
                remove_student(ID);
                break;

            }
            case 3:
            {

                FILE *file = fopen("student_data.txt", "r");
                if (file == NULL) {
                    printf("Error opening file!\n");
                    return 1;
                }

                char query[50];
                printf("Enter the name or ID of the student you want to search: ");
                scanf("%s", query);

                // Perform the search
                search_student(query, file);

                fclose(file);
                break;

            }

            case 4:
            {

                FILE *file = fopen("student_data.txt", "r+");
                if (file == NULL) {
                    printf("Error opening file!\n");
                    return 1;
                }

                int id;
                printf("Enter the ID of the student you want to update: ");
                scanf("%d", &id);

                // Check if ID exists
                int found = 0;
                struct students student;
                while (fscanf(file, "%s %s %d %c %d %f", student.First_name, student.Last_name, &student.ID, &student.Gender, &student.Academic_year, &student.GPA) != EOF) {
                    if (student.ID == id) {
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    printf("ID not found. No student with ID %d exists in the system.\n", id);
                    fclose(file);
                    return 1;
                }

                int option;
                printf("Choose the option you want to update:\n");
                printf("1. Update Academic Year\n");
                printf("2. Update GPA\n");
                printf("Enter your choice (1 or 2): ");
                scanf("%d", &option);

                float new_value;
                printf("Enter the new value: ");
                scanf("%f", &new_value);

                // Perform the update
                update_student(id, option, new_value, file);

                fclose(file);
                break;
            }


        }

    }


    return 0;
}
