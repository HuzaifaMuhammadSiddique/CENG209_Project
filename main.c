#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>  

#define _POSIX_C_SOURCE 200809L
#define MAX_ITEMS 15
#define MAX_CREATURES 7
#define MAX_INPUT_LEN 200

struct Player{
    int health;
    int strength;
    int money;
    char *items[20];
};

struct Creature{
    char* name;
    int health;
    int damage;
    char* room;
};

struct Room{
    char* name;
    char *creatures[7];
    char* description;
    char *items[15];
    char* right;
    char* left;
    char* up;
    char* down;
};

void startingMessage(){
    FILE *file = fopen("welcome_message.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file named 'welcome_message.txt'\n");
    }else{
        char line[256];
        while(fgets(line, sizeof(line), file)){
            printf("%s", line); 
        }
        fclose(file);
    }

    printf("\n");
    
}


char *strdup(const char *source) {
    if (source == NULL) return NULL;

    size_t len = strlen(source) + 1;
    char *copy = malloc(len);
    if (copy != NULL) {
        memcpy(copy, source, len);
    }
    return copy;
}

void loadRoomStatsFromFile(struct Room *room, FILE *file) {
    char line[320];

    if (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        room->name = strdup(line);
    }

    if (fgets(line, sizeof(line), file) != NULL) {
    }

    if (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        room->description = strdup(line); 
    }

    if (fgets(line, sizeof(line), file) != NULL) {
    }


    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        room->items[i] = strdup(line);
        i++;
    }
    


}

void loadCreaturesInRoom(struct Room *room, FILE *file){
    char line[100];
    int j = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; 
        if (strcmp(line, "") == 0) {
            break;
        }
        room->creatures[j] = strdup(line);
        j++;
    }
}




void loadCreatureStatsFromFile(struct Creature *creature, FILE *file){
    char line[20];

    if (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        creature->name = strdup(line);
    }

    if (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0; 
        int number = atoi(line);
        creature->health = number; 
    }

    if (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0; 
        int number = atoi(line);
        creature->damage = number;
    }
}

void alignRooms(struct Room rooms[]){
    rooms[0].up = "Chamber";
    rooms[0].right = "Treasure Vault";
    rooms[0].down = "Enterance Hall";
    rooms[0].left = "Guardroom";

    rooms[1].right = "Spider Nest";
    rooms[1].down = "Arena";
    rooms[1].left = "Potion Laboratory";
    rooms[1].up = NULL;

    rooms[2].up = "Arena";
    rooms[2].right = "Trophy Room";
    rooms[2].left = "Library of the Lost";
    rooms[2].down = NULL;

    rooms[3].up = "Potion Laboratory";
    rooms[3].right = "Arena";
    rooms[3].down = "Library of the Lost";
    rooms[3].left = NULL;

    rooms[4].up = "Guardroom";
    rooms[4].right = "Enterance Hall";
    rooms[4].down = NULL;
    rooms[4].left = NULL;

    rooms[5].up = NULL;
    rooms[5].right = "Chamber";
    rooms[5].down = "Guardroom";
    rooms[5].left = NULL;

    rooms[6].up = NULL;
    rooms[6].down = "Treasure Vault";
    rooms[6].left = "Chamber";
    rooms[6].right = NULL;

    rooms[7].up = "Spider Nest";
    rooms[7].down = "Trophy Room";
    rooms[7].left = "Arena";
    rooms[7].right = NULL;

    rooms[8].up = "Treasure Vault";
    rooms[8].left = "Enterance Hall";
    rooms[8].down = NULL;
    rooms[8].right = NULL;
}


struct Room getRoomByName(char* name, struct Room allRooms[]){
    for(int i = 0; i < 9; i++){
        if(strcmp(allRooms[i].name, name) == 0){
            return allRooms[i];
        }
    }
    struct Room emptyRoom = {0};  // Return an empty room structure
    return emptyRoom;
}

struct Creature getCreatureByName(char* name, struct Creature allCreatures[]){
    for(int i = 0; i < 7; i++){
        if(strcmp(allCreatures[i].name, name) == 0){
            return allCreatures[i];
        }
    }
    struct Creature emptyCreature = {0};  // Return an empty creature structure
    return emptyCreature;
}

void displayAllItems(struct Room room){
    printf("Items in this Room: \n");

    for(int i=0; i<15; i++){
        if(room.items[i] != NULL){
            printf("%s \n", room.items[i]);
        }
    }
    printf("\n");

}

void displayAllCreatures(struct Room room){
    printf("Creatures in this Room: \n");
    
    for(int i=0; i<7; i++){
        if(room.creatures[i] != NULL){
            printf("%s \n", room.creatures[i]);
        }
    }
    printf("\n");
}

bool itemInRoom(char* item, struct Room room){

    for(int i=0; i<15; i++){
        if(room.items[i] != NULL){
            if(strcmp(room.items[i], item) == 0){
            return true;
            }
        }
    }
    return false;
}

bool creatureInRoom(char* creature, struct Room room){
    for(int i =0; i<7; i++){
        if(room.creatures[i] != NULL){
            if(strcmp(room.creatures[i], creature) == 0){
            return true;
            }
        }
    }
    return false;
}



void addToInventory(char* item){
    FILE *inventory = fopen("inventory.txt", "a");
    if(inventory == NULL){
        printf("Error: Could not open file named 'inventory.txt'\n");
    }else{
        fprintf(inventory, "%s\n", item);
        fclose(inventory);
        printf("%s was successfully added to the inventory. \n", item); 
    }
}

bool battle(struct Player *player, struct Creature *creature){
    while(1){
        int playerPower = player->strength;
        int creaturePower = creature->damage;

        creature->health -= playerPower;
        if(creature->health <= 0){
            return true;
        }

        int randomNumber = (rand() % 11) - 5;
        player->health -= (creaturePower + randomNumber);

        if(player->health <= 0){
            return false;
        }
    }
}


void removeItemFromRoom(struct Room *room, char* item){
    for(int i =0; i <15; i++){
        if(room->items[i] != NULL){
        if(strcmp(room->items[i], item) == 0){
            room->items[i] = NULL;
            break;
        }}
    }
}

void removeCreatureFromRoom(struct Room *room, char* creature){
    for(int i =0; i <7; i++){
        if(room->creatures[i] != NULL){
        if(strcmp(room->creatures[i], creature) == 0){
            room->creatures[i] = NULL;
            break;
        }}
    }
}


bool potionInInventory(char *inventory[20]) {
    for (int i = 0; i < 20; i++) {
        if (inventory[i] != NULL && strcmp(inventory[i], "Potion") == 0) {
            return true;
        }
    }
    return false;
}



void overwriteInventory(char* inventory[20], FILE *file){
    for(int i = 0; i<20; i++){
        if(inventory[i] != NULL){
            fprintf(file, "%s\n", inventory[i]); 
        }else{
            continue;
        }
    }
}


bool itemInInventory(char* item, char* inventory[20]){
    for(int i=0; i<20; i++){
        if(inventory[i] != NULL){
            if(strcmp(inventory[i], item) == 0){
                return true;
            }
        }
    }
    return false;
}


int countItemsInInventory(char *inventory[]) {
    int count = 0;
    for (int i = 0; i < 20; i++) {
        if (inventory[i] != NULL) {
            count++;
        }
    }
    return count;
}


int getItemPrice(char* item, FILE *file) {
    char element[100];
    int price = 0;
    
    while (fgets(element, sizeof(element), file)) { 
        element[strcspn(element, "\n")] = '\0'; 

        if (strcmp(element, item) == 0) {  
            if (fgets(element, sizeof(element), file) != NULL) {
                price = atoi(element);  
            }
            break;
        }
    }
    return price;
}


void updateRoom(struct Room *currentRoom, struct Room allRooms[]) {
    for (int i = 0; i < 9; i++) {
        if (strcmp(allRooms[i].name, currentRoom->name) == 0) {
            allRooms[i] = *currentRoom;
            break;
        }
    }
}

void saveGame(struct Player *player, struct Room allRooms[], struct Room *currentRoom, char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf("Error: Could not open file '%s' for saving (%s).\n", filepath, strerror(errno));
        return;
    }

    (void)currentRoom;

    fprintf(file, "Player\n");
    fprintf(file, "Health: %d\n", player->health);
    fprintf(file, "Strength: %d\n", player->strength);
    fprintf(file, "Money: %d\n", player->money);
    fprintf(file, "Items:\n");
    for (int i = 0; i < 20; i++) {
        if (player->items[i] != NULL) {
            fprintf(file, "%s\n", player->items[i]);
        }
    }

    fprintf(file, "\nRooms\n");
    for (int i = 0; i < 9; i++) {
        fprintf(file, "Room: %s\n", allRooms[i].name);
        fprintf(file, "Items:\n");
        for (int j = 0; j < 15; j++) {
            if (allRooms[i].items[j] != NULL) {
                fprintf(file, "%s\n", allRooms[i].items[j]);
            }
        }
        fprintf(file, "Creatures:\n");
        for (int j = 0; j < 7; j++) {
            if (allRooms[i].creatures[j] != NULL) {
                fprintf(file, "%s\n", allRooms[i].creatures[j]);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);

    FILE *listFile = fopen("saved_games.txt", "a");
    if (listFile) {
        fprintf(listFile, "%s\n", filepath);
        fclose(listFile);
    }

    printf("Game saved to '%s'.\n", filepath);
}

void loadGame(struct Player *player, struct Room allRooms[], struct Room *currentRoom, char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Error: Could not open file '%s' for loading (%s).\n", filepath, strerror(errno));
        return;
    }

    char line[256];
    bool inPlayer = false, inRoom = false, inItems = false, inCreatures = false;
    struct Room *currentLoadingRoom = NULL;

    for (int i = 0; i < 20; i++) player->items[i] = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "Player") == 0) {
            inPlayer = true;
            inRoom = false;
            inItems = false;
            inCreatures = false;
        } else if (strcmp(line, "Rooms") == 0) {
            inPlayer = false;
            inRoom = true;
            inItems = false;
            inCreatures = false;
        } else if (strncmp(line, "Room: ", 6) == 0) {
            char *roomName = line + 6;
            for (int i = 0; i < 9; i++) {
                if (strcmp(allRooms[i].name, roomName) == 0) {
                    currentLoadingRoom = &allRooms[i];
                    break;
                }
            }

            if (currentLoadingRoom) {
                for (int i = 0; i < 15; i++) currentLoadingRoom->items[i] = NULL;
                for (int i = 0; i < 7; i++) currentLoadingRoom->creatures[i] = NULL;
            }

            inItems = false;
            inCreatures = false;
        } else if (strcmp(line, "Items:") == 0) {
            inItems = true;
            inCreatures = false;
        } else if (strcmp(line, "Creatures:") == 0) {
            inItems = false;
            inCreatures = true;
        } else if (inPlayer) {
            if (sscanf(line, "Health: %d", &player->health) == 1) continue;
            if (sscanf(line, "Strength: %d", &player->strength) == 1) continue;
            if (sscanf(line, "Money: %d", &player->money) == 1) continue;
            if (strcmp(line, "Items:") == 0) continue;
            if (strlen(line) > 0) {
                for (int i = 0; i < 20; i++) {
                    if (player->items[i] == NULL) {
                        player->items[i] = strdup(line);
                        break;
                    }
                }
            }
        } else if (inRoom && currentLoadingRoom != NULL) {
            if (inItems && strlen(line) > 0) {
                for (int i = 0; i < 15; i++) {
                    if (currentLoadingRoom->items[i] == NULL) {
                        currentLoadingRoom->items[i] = strdup(line);
                        break;
                    }
                }
            } else if (inCreatures && strlen(line) > 0) {
                for (int i = 0; i < 7; i++) {
                    if (currentLoadingRoom->creatures[i] == NULL) {
                        currentLoadingRoom->creatures[i] = strdup(line);
                        break;
                    }
                }
            }
        } else if (strncmp(line, "CurrentRoom: ", 13) == 0) {
            char *currentRoomName = line + 13;
            for (int i = 0; i < 9; i++) {
                if (strcmp(allRooms[i].name, currentRoomName) == 0) {
                    *currentRoom = allRooms[i];
                    break;
                }
            }
        }
    }

    FILE *inventoryFile = fopen("inventory.txt", "w");
    if (inventoryFile) {
        for (int i = 0; i < 20; i++) {
            if (player->items[i] != NULL) {
                fprintf(inventoryFile, "%s\n", player->items[i]);
            }
        }
        fclose(inventoryFile);
    }

    

    fclose(file);
    printf("Game loaded from '%s'.\n", filepath);
}

void listSavedGames() {
    FILE *file = fopen("saved_games.txt", "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char line[256];
    printf("Saved Games:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}


int calculateRemainingCreatures(struct Room allRooms[], int numRooms) {
    int remaining = 0;
    for (int i = 0; i < numRooms; i++) {
        for (int j = 0; j < 7; j++) {
            if (allRooms[i].creatures[j] != NULL) {
                remaining++;
            }
        }
    }
    return remaining;
}



// THE MAIN FUNCTION //

int main(){
    startingMessage();

    int itemsInInventory = 0;
    int creaturesLeft = 33;

    struct Room allRooms[9];
    struct Creature allCreatures[7];

    for(int j =0; j<9; j++){
        for (int i = 0; i < 15; i++) {
            allRooms[j].items[i] = NULL;
        }
        for (int i = 0; i < 7; i++) {
            allRooms[j].creatures[i] = NULL;
        }
    }

    char *creatureFiles[7] = {
        "barbarian.txt",
        "dragon.txt",
        "goblin.txt",
        "spider.txt",
        "troll.txt",
        "warrior.txt",
        "zombie.txt"
    };

    for(int i = 0; i < 7; i++){
        FILE *currentFile = fopen(creatureFiles[i], "r");
        loadCreatureStatsFromFile(&allCreatures[i], currentFile);
        fclose(currentFile);
    }

    char *roomFiles[9] = {
        "arena.txt",
        "chamber.txt",
        "enterance_hall.txt",
        "guardroom.txt",
        "library.txt",
        "potion_laboratory.txt",
        "spider_nest.txt",
        "treasure_vault.txt",
        "trophy_room.txt"
    };

    for(int i=0; i<9; i++){
        FILE *currentFile = fopen(roomFiles[i], "r");
        loadRoomStatsFromFile(&allRooms[i], currentFile);
        fclose(currentFile);
    }

    char *roomCreatureFiles[9] = {
        "creatures_arena.txt",
        "creatures_chamber.txt",
        "creatures_hall.txt",
        "creatures_guardroom.txt",
        "creatures_library.txt",
        "creatures_laboratory.txt",
        "creatures_spider.txt",
        "creatures_vault.txt",
        "creatures_trophy.txt"
    };


    for(int i=0; i<9; i++){
        FILE *currentFile = fopen(roomCreatureFiles[i], "r");
        loadCreaturesInRoom(&allRooms[i], currentFile);
        fclose(currentFile);
    }


    // This decides the position of all rooms with respect to other rooms
    // Now we have a map-like structure virtually
    alignRooms(allRooms);

    bool displayed = false;

    struct Player newPlayer;
    newPlayer.health = 3000;
    newPlayer.strength = 50;
    newPlayer.money = 500;
    struct Room currentRoom = allRooms[0];

    
    for(int i=0; i<20; i++){
        newPlayer.items[i] = NULL;
    }

    printf("Health: %d \n", newPlayer.health);
    printf("Strength: %d \n", newPlayer.strength);
    printf("Money: %d \n", newPlayer.money);

    printf("%s \n", currentRoom.description);
    displayed = true;



    while(1){

    if(creaturesLeft == 0){
        printf("CONGRATULATIONS! You have defeated all of the creatures in the game! \n");
        printf("YOU HAVE WON THE GAME!");
        return 0;
    }

        printf("You are in %s \n", currentRoom.name);

        if(!displayed){
            printf("%s \n", currentRoom.description);
            displayed = true;
        }

        char input[MAX_INPUT_LEN];
        char firstWord[MAX_INPUT_LEN];
        char secondWord[MAX_INPUT_LEN];

        printf("Enter command: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; 


        if (sscanf(input, "%s %99[^\n]", firstWord, secondWord) == 2) {
            
        } else if (sscanf(input, "%s", firstWord) == 1) {
            
        } else {
            printf("Invalid input. Try again.\n");
            continue;
        }
        if(strcmp(firstWord, "move") == 0){
            if(strcmp(secondWord, "right") == 0){
                if(currentRoom.right == NULL){
                    printf("You have encountered a wall! There is no room to the right! \n");
                }else{
                    char* newRoom = currentRoom.right;
                    currentRoom = getRoomByName(newRoom, allRooms);
                    displayed = false;
                }
            }else if(strcmp(secondWord, "left") == 0){
                if(currentRoom.left == NULL){
                    printf("You have encountered a wall! There is no room to the left! \n");
                }else{
                    char* newRoom = currentRoom.left;
                    currentRoom = getRoomByName(newRoom, allRooms);
                    displayed = false;
                }
            }else if(strcmp(secondWord, "up") == 0){
                if(currentRoom.up == NULL){
                    printf("You have encountered a wall! There is no room upwards! \n");
                }else{
                    char* newRoom = currentRoom.up;
                    currentRoom = getRoomByName(newRoom, allRooms);
                    displayed = false;
                }
            }else if(strcmp(secondWord, "down") == 0){
                if(currentRoom.down == NULL){
                    printf("You have encountered a wall! There is no room downwards! \n");
                }else{
                    char* newRoom = currentRoom.down;
                    currentRoom = getRoomByName(newRoom, allRooms);
                    displayed = false;
                }
            }else{
                printf("Invalid Command! Please specify a direction: up, down, right or left. \n");
            }

        } else if (strcmp(firstWord, "look") == 0) {
            printf("%s \n\n", currentRoom.description);

            // Display creatures first
            printf("Creatures in this Room:\n");
            for (int i = 0; i < 7; i++) {
                if (currentRoom.creatures[i] != NULL) {
                    printf("%s\n", currentRoom.creatures[i]);
                }
            }

            printf("\n");

            printf("Items in this Room:\n");
            for (int i = 0; i < 15; i++) {
                if (currentRoom.items[i] != NULL) {
                    printf("%s\n", currentRoom.items[i]);
                }
            }

            printf("\n");
        }else if(strcmp(firstWord, "inventory") == 0){
            FILE *file = fopen("inventory.txt", "r");

            if (file == NULL) {
                printf("Error: Could not open file named 'inventory.txt'\n");
            }else{
                char item[100];
                printf("Items in the Inventory: \n");
                while(fgets(item, sizeof(item), file)){
                    printf("%s", item);
                }
                fclose(file);
            }

        } else if (strcmp(firstWord, "pickup") == 0) {
            char *item = secondWord;
            if (itemInRoom(item, currentRoom)) {
                if (countItemsInInventory(newPlayer.items) >= 20) {
                    printf("Inventory Full! Cannot add more items.\n");
                } else {
                    addToInventory(item);
                    for (int i = 0; i < 20; i++) {
                        if (newPlayer.items[i] == NULL) {
                            newPlayer.items[i] = strdup(item);
                            break;
                        }
                    }
                    itemsInInventory++;
                    removeItemFromRoom(&currentRoom, item);
                    updateRoom(&currentRoom, allRooms);
                }
            } else {
                printf("No item named %s present in room %s.\n", item, currentRoom.name);
            }
        }else if(strcmp(firstWord, "attack") == 0){
            char* creature = secondWord;
            if(creatureInRoom(creature, currentRoom)){
                struct Creature currentCreature = getCreatureByName(creature, allCreatures);
                bool result = battle(&newPlayer, &currentCreature);
                if(result){
                    creaturesLeft--;
                    printf("You won the battle! You successfully defeated %s. \n", creature);
                    printf("Your health is %d \n", newPlayer.health);
                    removeCreatureFromRoom(&currentRoom, creature);
                    updateRoom(&currentRoom, allRooms);
                }else{
                    printf("You were defeated by %s. Game ends! \n", creature);
                    return 0;
                }

            }else{
                printf("No creature named %s present in room %s. \n", creature, currentRoom.name);
            }
        }else if(strcmp(firstWord, "exit") == 0){
            // removing everything from the inventory
            FILE *file = fopen("inventory.txt", "w");
            fclose(file);

            return 0;

        }else if(strcmp(firstWord, "player") == 0){
            printf("Health: %d \n", newPlayer.health);
            printf("Strength: %d \n", newPlayer.strength);

        }else if(strcmp(firstWord, "use") == 0){
            if(strcmp(secondWord, "Potion") == 0){
                if(potionInInventory(newPlayer.items)){
                    itemsInInventory--;
                    newPlayer.health += 200;

                    for (int i = 0; i < 20; i++) {
                        if (newPlayer.items[i] != NULL && strcmp(newPlayer.items[i], "Potion") == 0) {
                            newPlayer.items[i] = NULL;  // Remove potion from inventory
                            break;
                        }
                    }
                    FILE *file = fopen("inventory.txt", "w");
                    overwriteInventory(newPlayer.items, file);
                    fclose(file);
                    printf("You have successfully used the Potion. Your health is now %d. \n", newPlayer.health);
                }else{
                    printf("You do not have any Potion to use! \n");
                }
            }else{
                printf("Wrong argument with 'use'. You can only enter 'Potion' with 'use'. \n");
            }
        }else if(strcmp(firstWord,"shop") == 0){
            while(1){
                char input2[MAX_INPUT_LEN];
                char command[MAX_INPUT_LEN];
                char argument[MAX_INPUT_LEN];

                printf("You are in the shop. \n");
                printf("Your money is %d \n", newPlayer.money);
                printf("Enter command for shop: ");
                fgets(input2, sizeof(input2), stdin);
                input2[strcspn(input2, "\n")] = '\0'; 


                if (sscanf(input2, "%s %99[^\n]", command, argument) == 2) {
            
                }else {
                    printf("Invalid input. Try again.\n");
                    continue;
                }

                if(strcmp(command, "buy") == 0){
                    if(strcmp(argument, "health") == 0){
                        if(newPlayer.money >= 50){
                            newPlayer.health += 200;
                            newPlayer.money -= 50;
                            printf("Your new health is %d and your remaining money is %d \n", newPlayer.health, newPlayer.money);
                        }else{
                            printf("You do not have enough money to buy health. Minimum money required is 50. \n");
                        }
                    }else if(strcmp(argument, "strength") == 0){
                        if(newPlayer.money >= 100){
                            newPlayer.strength += 20;
                            newPlayer.money -= 100;
                            printf("Your new strength is %d and your remaining money is %d \n", newPlayer.strength, newPlayer.money);

                        }else{
                            printf("You do not have enough money to buy strength. Minimum money required is 100. \n");
                        }
                    }else{
                        printf("Invalid argument with 'buy'. You can only buy health or strength. \n");
                    }
                }else if(strcmp(command, "sell") == 0){
                    char* item = argument;
                    if(itemInInventory(item, newPlayer.items)){
                        // code
                        FILE *items = fopen("items.txt", "r");
                        int price = getItemPrice(item, items);
                        itemsInInventory--;
                        for (int i = 0; i < 20; i++) {
                            if (newPlayer.items[i] != NULL && strcmp(newPlayer.items[i], item) == 0) {
                                newPlayer.items[i] = NULL;  // Remove item from inventory
                                break;
                            }
                        }
                        FILE *file = fopen("inventory.txt", "w");
                        overwriteInventory(newPlayer.items, file);
                        fclose(file);
                        newPlayer.money += price;

                        printf("You have successfully sold %s. Your money is %d. \n", item, newPlayer.money);                        
                    }else{
                        printf("You do not have %s in your inventory. \n", item);
                    }
                }else if(strcmp(command, "exit") == 0){
                    if(strcmp(argument, "shop") == 0){
                        break;
                    }
                }else{
                    printf("Invalid Command! \n");
                }
            }
        }else if(strcmp(firstWord, "equip") == 0){
            char* weapon = secondWord;
            if(itemInInventory(weapon, newPlayer.items)){
                bool equipped = false;
                if(strcmp(weapon, "Gun") == 0){
                    newPlayer.strength += 50;
                    printf("Your new strength is %d \n", newPlayer.strength);
                    equipped = true;
                }else if(strcmp(weapon, "Sword") == 0){
                    newPlayer.strength += 30;
                    printf("Your new strength is %d \n", newPlayer.strength);
                    equipped = true;
                }else if(strcmp(weapon, "Knife") == 0){
                    newPlayer.strength += 20;
                    printf("Your new strength is %d \n", newPlayer.strength);
                    equipped = true;
                }else if(strcmp(weapon, "Rope") == 0){
                    newPlayer.strength += 10;
                    printf("Your new strength is %d \n", newPlayer.strength);
                    equipped = true;
                }else if(strcmp(weapon, "Sheild") == 0){
                    newPlayer.health += 500;
                    printf("Your new health is %d \n", newPlayer.health);
                    equipped = true;
                }else if(strcmp(weapon, "Bow") == 0){
                    newPlayer.strength += 15;
                    printf("Your new strength is %d \n", newPlayer.strength);
                    equipped = true;
                }else if(strcmp(weapon, "Helmet") == 0){
                    newPlayer.health += 200;
                    printf("Your new health is %d \n", newPlayer.health);
                    equipped = true;
                }else{
                    printf("You cannot equip %s. \n", weapon);
                }

                if(equipped){
                    itemsInInventory--;
                    for (int i = 0; i < 20; i++) {
                        if (newPlayer.items[i] != NULL && strcmp(newPlayer.items[i], weapon) == 0) {
                            newPlayer.items[i] = NULL;  // Remove weapon from inventory
                            break;
                        }
                    }                    
                }
            }else{
                printf("You do not have %s in your inventory. \n", weapon);
            }
        }else if (strcmp(firstWord, "save") == 0) {
            saveGame(&newPlayer, allRooms, &currentRoom, secondWord);
        } else if (strcmp(firstWord, "load") == 0) {
            loadGame(&newPlayer, allRooms, &currentRoom, secondWord);
            creaturesLeft = calculateRemainingCreatures(allRooms, 9);
            displayed = false; // Refresh room description
        } else if (strcmp(firstWord, "list") == 0) {
             listSavedGames();
        }
        
        else{
            printf("Invalid Command! \n");
        }
        
    }
    return 0;
}