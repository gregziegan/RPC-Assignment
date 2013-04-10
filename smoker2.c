#include "smoking.h"
#define SMOKER_ID 2
#include "smoke.h"

/* Beginning supplies */
int tobacco = 10;
int paper = 8;
int matches = 10;

void smoke();
int canSmoke();
char getEmptySup();
void updateSupplies(struct supplyReq newSupplies);

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
char getEmptySup() {
    if (tobacco == 0)
        return 't';
    else if (paper == 0)
        return 'p';
    else
        return 'm';
}

/* Updates the supply values based on the struct received by the agent */
void updateSupplies(struct supplyReq newSupplies) {
    if (newSupplies.supplyType == 't') 
        tobacco += newSupplies.supplyAmount;
    else if (newSupplies.supplyType == 'p') 
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

    int suppliesLeft = 1; // 1 = true
    while (suppliesLeft != 0) {
        if (canSmoke() == 1)
            smoke();
        else {
            printf("Need more supplies!\n");

            // Creates a struct with an empty supply type to send to the agent 
            struct supplyReq request = {getEmptySup(), 1, SMOKER_ID, 0};

            printf("Supply Type of request: %c\n", request.supplyType);
            // Creates a struct to receive the supplies from the agent 
            int* result = getmemysupply_1(&request, cl);

            // Checks the result struct and whether resources are available //
            if (*result == REQUEST_GRANTED) { 
                printf("Tobacco supplies are received, and now smoking!\n");
                updateSupplies(request); // Will update resources and smoke on next iteration
            }
            else if (*result == INSUFFICIENT_SUPPLIES)
                suppliesLeft = 0;
            else if (*result == CHANGE_SMOKERS) {
                printf("Waiting to request again after 3 seconds\n");
                sleep(3);
            } else if (*result == TERMINATE) {
                printf("I am the last smoker and there are no more supplies... I will take the agent down with me!\n");
                exit_1(&request, cl);
                clnt_destroy(cl);
                exit(0);
            }
            else
                printf("No communication\n"); 
        } 
    } 

    printf("Sadly no more smoking supplies, now I will go kill myself.\n");

    clnt_destroy(cl);
    return 0;
}
