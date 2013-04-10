#include "smoking.h"
#include <string.h>
#define COMP_LIMIT 7
#define SMOKER_ID 2

/* Beginning supplies */
int tobacco = 10;
int paper = 8;
int matches = 10;

/* Decrements the supplies */
void smoke() {
    tobacco--;
    paper--;
    matches--;
    printf("Smoker%d smoked... ahhh\n", SMOKER_ID);
}

/* Evaluates whether smoker can smoke */
int canSmoke() {
    if(tobacco > 0 && paper > 0 && matches > 0)
        return 1;
    else
        return 0;
}

/* Retrieves a string representation of an empty resource */
char* getEmptySup() {
    if (tobacco == 0)
        return "tobacco";
    else if (paper == 0)
        return "paper";
    else
        return "matches";
}

/* Updates the supply values based on the struct received by the agent */
void updateSupplies(struct supplyReq newSupplies) {
    if (strncmp(newSupplies.supplyType, "tobacco", COMP_LIMIT) == 0) 
        tobacco += newSupplies.supplyAmount;
    else if (strncmp(newSupplies.supplyType, "paper", COMP_LIMIT) == 0) 
        paper += newSupplies.supplyAmount;
    else
        matches += newSupplies.supplyAmount;
}

int main(int argc, char**argv) 
{
    
    CLIENT *cl;

    if(argc != 2) {
        printf("Usage: ./smoker%d HOSTNAME\n", SMOKER_ID);
        return 1;
    }

    char *server_hostname = argv[1];

    cl = clnt_create(server_hostname, SMOKER_PROG, SMOKER_VERS, "udp");

    if(cl == NULL) {
        clnt_pcreateerror("Error creating client\n");
        exit(EXIT_FAILURE);
    }
    
    char* emptySupply = (char *)malloc(sizeof(char[10]));
    int suppliesLeft = 1; // 1 = true
    while (suppliesLeft != 0) {
        if (canSmoke() == 1)
            smoke();
        else {
            printf("Need more supplies!\n");

            strcpy(emptySupply, getEmptySup());
            
            // Creates a struct with an empty supply type to send to the agent 
            struct supplyReq request = {getEmptySup(), 0, SMOKER_ID, 0};

            // Creates a struct to receive the supplies from the agent 
            int* result = getmemysupply_1(&request, cl);

            /*
            // Checks the result struct and whether resources are available //
            if (result == 1) { 
                printf("Tobacco supplies are received, and now smoking!\n");
                updateSupplies(request); // Will update resources and smoke on next iteration
            }
            else if (result == -1)
                suppliesLeft = 0;
            else if (result == -2) {
                printf("I am the last smoker and there are no more supplies... I will take the agent down with me!");
                exit_1(&request, cl);
                clnt_destroy(cl);
                exit(0);
            }
            else     
                printf("No communication\n"); */
            printf("%d", *result);
            suppliesLeft = 0;
        } 
    } 

    printf("Sadly no more smoking supplies, now I will go kill myself.\n");

    clnt_destroy(cl);
    return 0;
}
