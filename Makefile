CC = clang
# -g : allows use of GNU Debugger
# -Wall : show all warnings
CFLAGS = -g -Wall
LIBS = # None yet...
CSOURCE1 = smoker1.c smoking_clnt.c smoking_xdr.c
CSOURCE2 = smoker2.c smoking_clnt.c smoking_xdr.c
CSOURCE3 = smoker3.c smoking_clnt.c smoking_xdr.c
SSOURCE = agent.c smoking_svc.c smoking_xdr.c
RPCSRC = smoking.x
RPCGEN = smoking.h smoking_clnt.c smoking_svc.c smoking_xdr.c
CLIENT1 = smoker1 
CLIENT2 = smoker2 
CLIENT3 = smoker3
SERVER = agent

all: $(CLIENT1) $(CLIENT2) $(CLIENT3) $(SERVER)

client: $(CSOURCE) $(RPCGEN)
	$(CC) $(LIBS) $(CFLAGS) -o $(CLIENT1) $(CSOURCE1)
	$(CC) $(LIBS) $(CFLAGS) -o $(CLIENT2) $(CSOURCE2)
	$(CC) $(LIBS) $(CFLAGS) -o $(CLIENT3) $(CSOURCE3)

server: $(SSOURCE) $(RPCGEN)
	$(CC) $(LIBS) $(CFLAGS) -o $(SERVER) $(SSOURCE)

$(RPCGEN): $(RPCSRC)
	rpcgen $(RPCSRC)

# 'clean' rule for remove non-source files
# To use, call 'make clean'
# Note: you don't have to call this in between every
#       compilation, it's only in case you need to
#       clean out your directory for some reason.
clean:
	@# Using the '@' sign suppresses echoing
	@# the line while the command is run
	rm -f $(CLIENT) $(SERVER) $(RPCGEN)
