#include "checkinterface.h"

static void printVersion(const char *command)
{
    printf(
    "%s (" PACKAGE_NAME ") " PACKAGE_VERSION "\n\n"
    "Copyright (C) 2012-2015 Sander van der Burg\n"
    , command);
}

int main(int argc, char *argv[])
{
    return IFF_checkCmdLineInterface("iffcheck", "IFF", argc, argv, NULL, printVersion);
}
