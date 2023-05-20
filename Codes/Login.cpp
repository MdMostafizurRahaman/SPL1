#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_EMAIL_LENGTH 50
#define MAX_PASSWORD_LENGTH 20

struct User {
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char gender;
    int age;
};

struct User users[MAX_USERS];
int userCount = 0;

void saveUser(struct User user) {
    FILE *file = fopen("users.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %c %d\n", user.email, user.password, user.gender, user.age);
        fclose(file);
    }
}

void registerUser() {
    struct User newUser;
    
    printf("Email: ");
    scanf("%s", newUser.email);
    
    // Check if email already exists
    for (int i = 0; i < userCount; i++) {
        if (users[i].email == newUser.email) {
            printf("Email already registered!\n");
            return;
        }
    }
    
    printf("Password: ");
    scanf("%s", newUser.password);
    
    printf("Gender (M/F): ");
    scanf(" %c", &newUser.gender);
    
    printf("Age: ");
    scanf("%d", &newUser.age);
    
    saveUser(newUser);
    users[userCount++] = newUser;
    printf("Registration successful!\n");
}

int findUserIndex(char *email) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].email == email) {
            return i;
        }
    }
    return -1;
}

void login() {
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    
    printf("Email: ");
    scanf("%s", email);
    
    int attempts = 5;
    int userIndex = findUserIndex(email);
    
    if (userIndex == -1) {
        printf("Invalid email!\n");
        return;
    }
    
    while (attempts > 0) {
        printf("Password: ");
        scanf("%s", password);
        
        if (users[userIndex].password == password) {
            printf("Login successful!\n");
            return;
        } else {
            attempts--;
            printf("Incorrect password! Attempts remaining: %d\n", attempts);
        }
    }
    
    printf("SORRY!!! Too many incorrect attempts. Account blocked.\n");
}

int main() {
    printf("\n\nWelcome to CHAT SERVER\n\n");
    // Read existing user data from the file
    FILE *file = fopen("users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s %c %d %s", users[userCount].email, users[userCount].password, &users[userCount].gender, &users[userCount].age) != EOF) {
            userCount++;
        }
        fclose(file);
    }

    int choice;
    do {
        printf("\n1. Register\n");
        printf("2. Login\n");
        printf("3. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 3);

    return 0;
}