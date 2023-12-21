#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "write_resources.h"

static const char* const description_line =
    "cesources - Small utility to embed files into a C program\n";

static const char* const usage_fmt =
    "usage: %s [options] [resource files]\n"
    "Options:\n"
    "--help                  Display this message and quit\n"
    "--input-dir             Directory to look for the resource files in\n"
    "--output-dir            Directory to place the generated C resources file\n"
    "--resources-filename    Name to give the generated C resources file\n";

static const char* const help_option_str = "--help";
static const char* const input_dir_option_str = "--input-dir";
static const char* const output_dir_option_str = "--output-dir";
static const char* const resources_filename_option_str = "--resources-filename";

int main(const int argc, const char** const argv) {
    int argi = 1;
    bool help_option_passed = false;
    const char* input_dir = NULL;
    const char* output_dir = NULL;
    const char* resources_filename = NULL;
    const char** resource_filenames = NULL;
    int resource_file_count = 0;
    int exit_status = EXIT_SUCCESS;
    while (argi < argc) {
        const char* const current_arg = argv[argi];

        if (!strcmp(current_arg, help_option_str)) {
            printf(description_line);
            printf(usage_fmt, argv[0]);
            help_option_passed = true;
            break;
        } else if (!strcmp(current_arg, input_dir_option_str)) {
            input_dir = argv[argi + 1];
            argi += 2;
        } else if (!strcmp(current_arg, output_dir_option_str)) {
            output_dir = argv[argi + 1];
            argi += 2;
        } else if (!strcmp(current_arg, resources_filename_option_str)) {
            resources_filename = argv[argi + 1];
            argi += 2;
        } else {
            resource_filenames = argv + argi;
            resource_file_count = argc - argi;
            break;
        }
    }

    if (!help_option_passed) {
        if (resource_file_count > 0) {
            bool resources_status = write_resources(
                input_dir,
                output_dir,
                resources_filename,
                resource_filenames,
                resource_file_count
            );
            if (!resources_status) {
                fprintf(stderr, "couldn't write resources file due to above errors\n");
                exit_status = EXIT_FAILURE;
            }
        } else {
            fprintf(stderr, "error: no resource files given\n");
            fprintf(stderr, usage_fmt, argv[0]);
            exit_status = EXIT_FAILURE;
        }
    }

    return exit_status;
}
