#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to check if a filename ends with a specific extension
int ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr) return 0;
    
    // Compare the end of the string to the suffix
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

// Function to read a file, strip local includes, and write to the output
void process_file(const char *filename, FILE *out_fp) {
    FILE *in_fp = fopen(filename, "r");
    if (!in_fp) {
        fprintf(stderr, "Error: Could not open file '%s'. Skipping.\n", filename);
        return;
    }

    // Add a helpful comment to track where the code came from
    fprintf(out_fp, "// --- Start of %s ---\n", filename);

    char line[1024]; // Buffer to hold each line (max 1023 characters)
    
    // Read the file line-by-line
    while (fgets(line, sizeof(line), in_fp)) {
        
        // Check if the line starts with `#include "` (exactly 10 characters)
        if (strncmp(line, "#include \"", 10) == 0) {
            // Comment it out in the final file instead of writing it as code
            fprintf(out_fp, "// Stripped local include: %s", line);
            continue; // Skip the fputs below and move to the next line
        }
        
        // If it's a normal line, write it to the output file
        fputs(line, out_fp);
    }

    fprintf(out_fp, "\n// --- End of %s ---\n\n", filename);
    fclose(in_fp);
}

int main(int argc, char *argv[]) {
    // Check if the user provided any files
    if (argc < 2) {
        printf("Usage: %s <file1.c> <file2.h> ...\n", argv[0]);
        return 1;
    }

    // Hardcoded output file for simplicity
    const char *output_filename = "combined_output.c";
    FILE *out_fp = fopen(output_filename, "w");
    if (!out_fp) {
        perror("Error creating output file");
        return 1;
    }

    printf("Generating %s...\n", output_filename);

    // PASS 1: Loop through all arguments and process ONLY header files
    for (int i = 1; i < argc; i++) {
        if (ends_with(argv[i], ".h")) {
            printf("Adding header file: %s\n", argv[i]);
            process_file(argv[i], out_fp);
        }
    }

    // PASS 2: Loop through all arguments and process ONLY source files
    for (int i = 1; i < argc; i++) {
        if (ends_with(argv[i], ".c")) {
            printf("Adding source file: %s\n", argv[i]);
            process_file(argv[i], out_fp);
        }
    }

    fclose(out_fp);
    printf("Success! All files combined into '%s'.\n", output_filename);

    return 0;
}