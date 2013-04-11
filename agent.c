#include "smoking.h"
#include "smoke.h"

/* The Agent's supplies */
int tobacco = 20;
int paper = 15;
int matches = 15;
int smokersKilled = 0;
int lastSmokerID = 0;

/* Function Headers */
int checkSupply(char type, int amount);
int getSupply(char type);
void updateSupply(char type, int amount);

int *getmemysupply_1_svc(struct supplyReq *in, struct svc_req *rqstp) {

    /* Will tell a smoker to wait until another smoker has requested supplies */
    if (in->smokerID == lastSmokerID && smokersKilled < 2) {
        in->done = CHANGE_SMOKERS;
        printf("The Agent responds: Another smoker%d? You wait.\n", in->smokerID);
        return(&in->done);
    }

    lastSmokerID = in->smokerID; // Update the smoker ID

    /* Evaluates whether the agent has adequate resources for the request and 
     * then updates the struct to reflect the agent's status
     */
    if (checkSupply(in->supplyType, in->supplyAmount) == 1) {
        in->done = REQUEST_GRANTED;
    } else {
        in->done = INSUFFICIENT_SUPPLIES;
        in->supplyAmount = 0;
        smokersKilled++;
    }

    // Checks if two smokers have been killed and terminates server if there are no supplies left for the last smoker 
    if (smokersKilled == 2 && in->done == INSUFFICIENT_SUPPLIES) {
        in->done = TERMINATE;
    }

    char* returnMessage = "Agent Received {SupplyType: %c, SupplyAmount: %d, SmokerID: %d, Done: %d} and sent the result: %d\n";
                                                                                                        
    printf(returnMessage, in->supplyType, in->supplyAmount, in->smokerID, in->done, in->done);
    fflush(NULL);
    return(&in->done);
}

void *exit_1_svc(struct supplyReq *in, struct svc_req *rqstp) {
    printf("All smokers killed, therefore agent will terminate\n");
    fflush(NULL);
    exit(0);
}

/* Checks the supply of a certain resources and makes sure there is enough 
 * to satisfy the amount requested and then updates the supply accordingly 
 */
int checkSupply(char type, int amount) {
    if ((getSupply(type) - amount) >= 0) {
        updateSupply(type, amount);
        return 1;
    } else
        return 0;
}

/* Gets the amount left of a resource, either tobacoo, paper, or matches */
int getSupply(char type) {
    if (type == 't')
        return tobacco;
    else if (type == 'p')
        return paper;
    else
        return matches;
}

/* Change the value of the supply accordingly */
void updateSupply(char type, int amount) {
    if (type == 't')
        tobacco -= amount;
    else if (type == 'p')
        paper -= amount;
    else 
        matches -= amount;
}
