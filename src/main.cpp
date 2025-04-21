#include "main.h"

int main(int argc, char *argv[])
{
    Listener listener("./config/config.json");
    boost::thread listener_thread(&Listener::run, &listener);
}