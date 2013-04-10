#include "smoking.h"
#include <string.h>
#define COMP_LIMIT 7

int tobacco = 20;
int paper = 15;
int matches = 15;
int smokersKilled = 0;

int checkSupply(char* type, int amount);
int getSupply(char* type);

int *getmemysupply_1_svc(struct supplyReq *in, struct svc_req *rqstp) {

    /* Evaluates whether the agent has adequate resources for the request and 
     * then updates the struct to reflect the agent's status
     */

    static int response;
    if (checkSupply(in->supplyType, in->supplyAmount) == 1) {
        in->done = 1;
        response = 1;
    } else {
        in->done = -1;
        in->supplyAmount = 0;
        smokersKilled++;
        response = -1;
    }

    // Checks if all three smokers have been killed and terminates server if all smokers are killed 
    if (smokersKilled == 3) {
        in->done = -2;
        response = -2;
    }

    char* returnMessage = "Server Received {SupplyType: %s, SupplyAmount: %d, SmokerID: %d, Done: %d} and sent the result: %d\n";
                                                                                                        
    printf(returnMessage, in->supplyType, in->supplyAmount, in->smokerID, in->done, response);
    fflush(NULL);
    return(&response);
}

void *exit_1_svc(struct supplyReq *in, struct svc_req *rqstp) {
    printf("All smokers killed, therefore agent will terminate\n");
    fflush(NULL);
    exit(0);
}

/* Checks the supply of a certain resources and makes sure there is enough to satisfy the amount requested */
int checkSupply(char* type, int amount) {
    if ((getSupply(type) - amount) >= 0)
        return 1;
    else
        return 0;
}

/* Gets the amount left of a resource, either tobacoo, paper, or matches */
int getSupply(char* type) {
    if (strncmp(type, "tobacco", COMP_LIMIT) == 0)
        return tobacco;
    else if (strncmp(type, "paper", COMP_LIMIT) == 0)
        return paper;
    else if (strncmp(type, "matches", COMP_LIMIT) == 0)
        return matches;
    else {
        perror("Not a valid resource");
        return -1;
    }
}

