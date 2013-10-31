
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>

#include "anyio.h"

static int device_flag;
static int write_flag;
static int program_flag;
static int verify_flag;
static int info_flag;
static int verbose_flag;
static board_access_t access;

static struct option long_options[] = {
    {"device", required_argument, 0, 'd'},
    {"write", required_argument, 0, 'w'},
    {"program", required_argument, 0, 'p'},
    {"verify", required_argument, 0, 'v'},
    {"info", required_argument, 0, 'i'},
    {"help", no_argument, 0, 'h'},
    {"verbose", no_argument, &verbose_flag, 1},
    {0, 0, 0, 0}
};

void print_short_usage() {
    printf("Configuration and diagnostic tool for mesanet PCI(E)/ETH/LPT/USB boards\n");
    printf("Try `mesaflash --help' for more information\n");
}

void print_usage() {
    printf("Syntax:\n");
    printf("    mesaflash --device device_name [--verbose]\n");
    printf("    mesaflash --device device_name [--write filename] [--verbose]\n");
    printf("    mesaflash --device device_name [--program filename] [--verbose]\n");
    printf("    mesaflash --device device_name [--verify filename] [--verbose]\n");
    printf("    mesaflash --info filename [--verbose]\n");
    printf("Options:\n");
    printf("  --device      select active device name. If no other options is given it will detect board with given name and print info about it.\n");
    printf("  --write       write filename into onboard flash if available (IMPORTANT! filename must be VALID FPGA configuration file)\n");
    printf("  --program     write filename into FPGA (IMPORTANT! filename must be VALID FPGA configuration file)\n");
    printf("  --verify      verify if 'filename' is properly programmed in flash\n");
    printf("  --verbose     print detailed information while running commands\n");
    printf("  --info        print info about configuration in filename\n");
}

int process_cmd_line(int argc, char *argv[]) {
    int c;

    while (1) {
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1) break;
        switch (c) {
            case 0: {
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0) break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
            }
            break;

            case 'd': {
                int i;

                if (device_flag > 0) {
                    printf("Error: multiply --device option\n");
                    exit(-1);
                }
                access.device_name = optarg;
                for (i = 0; optarg[i] != '\0'; i++)
                    access.device_name[i] = toupper(access.device_name[i]);

                device_flag++;
            }
            break;

            case 'w': {
                if (write_flag > 0) {
                    printf("Error: multiply --write option\n");
                    exit(-1);
                }
            }
            break;

            case 'p': {
                if (program_flag > 0) {
                    printf("Error: multiply --program option\n");
                    exit(-1);
                }
            }
            break;

            case 'v': {
                if (verify_flag > 0) {
                    printf("Error: multiply --verify option\n");
                    exit(-1);
                }
            }
            break;

            case 'i': {
                if (info_flag > 0) {
                    printf("Error: multiply --info option\n");
                    exit(-1);
                }
            }
            break;

            case 'h': {
                print_usage();
                exit(0);
            }
            break;

            case '?':
                /* getopt_long already printed an error message. */
                return -1;
                break;

            default:
                abort();
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_short_usage();
        return 0;
    }
    process_cmd_line(argc, argv);

    access.verbose = verbose_flag;
    access.pci = 1;

    if (info_flag == 1) {
        return 0;
    }
    if (device_flag == 1) {
        boards_init(&access);
        boards_scan(&access);
        if (write_flag == 1) {
        } else if (program_flag == 1) {
        } else if (verify_flag == 1) {
        }
    }

    return 0;
}
