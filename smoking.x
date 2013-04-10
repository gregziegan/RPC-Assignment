/* Structure of a supply request */
struct supplyReq {
    char* supplyType; 
    int supplyAmount; 
    int smokerID; 
    int done; 
};

program SMOKER_PROG {
    version SMOKER_VERS {
        int GETMEMYSUPPLY(struct supplyReq) = 1;
        void EXIT(struct supplyReq) = 2;
    } = 1;
} = 0x21250000;

