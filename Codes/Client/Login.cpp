#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_EMAIL_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define USER_NAME 20

struct User {
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char userName[USER_NAME];
};

struct User users[MAX_USERS];
int userCount = 0;

void saveUser(struct User user) {
    FILE *file = fopen("userInfo.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s\n", user.email, user.password, user.userName);
        fclose(file);
    }
}

void registerUser() {
    struct User newUser;
    
    printf("Email: ");
    scanf("%s", newUser.email);
    
    // Check if email already exists
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, newUser.email) == 0) {
            printf("Email already registered!\n");
            return;
        }
    }
    
    printf("Password: ");
    scanf("%s", newUser.password);
    
    printf("User Name: ");
    scanf(" %s", newUser.userName);
    
    saveUser(newUser);
    users[userCount++] = newUser;
    printf("Registration successful!\n");
}

int findUserIndex(char *email) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, email) == 0) {
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
        
        if (strcmp(users[userIndex].password, password) == 0) {
            printf("Login successful!\n");
            return;
        } else {
            attempts--;
            printf("Incorrect password! Attempts remaining: %d\n", attempts);
        }
    }
    
    printf("Too many incorrect attempts. Account blocked.\n");
}

int main() {
    // Read existing user data from the file
    FILE *file = fopen("userInfo.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s %s", users[userCount].email, users[userCount].password, users[userCount].userName) != EOF) {
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

