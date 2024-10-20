
// #include <argp.h>
#include <signal.h>
#include <stdlib.h>

#include "cusSocket.h"
#include "trace.h"

// Signal handler function
void handle_sigint(int sig)
{
        print_i("\nCaught signal %d (Ctrl+C)\n", sig);
        // Perform any cleanup and exit if needed
        cus_socket_close();
        exit(0);
}

int main()
{
        // Set up the signal handler
        signal(SIGINT, handle_sigint);
        cus_socket_create();
        return 0;
}
