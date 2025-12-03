#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

#define MAX_INPUT_LENGTH 2048
#define MAX_RESPONSE_LENGTH 8192

/**
 * @brief Escapes special characters in a string for JSON formatting
 * @param input The input string to escape
 * @param output Buffer to store the escaped string
 * @param max_len Maximum length of the output buffer
 */
void escape_json_string(const char *input, char *output, size_t max_len) {
    size_t j = 0;
    for (size_t i = 0; input[i] != '\0' && j < max_len - 2; i++) {
        if (input[i] == '"' || input[i] == '\\') {
            output[j++] = '\\';
        }
        if (input[i] == '\n') {
            output[j++] = '\\';
            output[j++] = 'n';
        } else if (input[i] == '\r') {
            output[j++] = '\\';
            output[j++] = 'r';
        } else if (input[i] == '\t') {
            output[j++] = '\\';
            output[j++] = 't';
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

/**
 * @brief Sends a message to Ollama API and displays the response
 * @param message The message to send to the AI
 * @return 0 on success, -1 on failure
 */
int chat_with_ollama(const char *message) {
    char escaped_message[MAX_INPUT_LENGTH * 2];
    char curl_command[MAX_INPUT_LENGTH * 3];
    FILE *fp;
    char response[MAX_RESPONSE_LENGTH];
    
    // Escape the message for JSON
    escape_json_string(message, escaped_message, sizeof(escaped_message));
    
    // Write JSON payload to a temporary file to avoid shell escaping issues
    FILE *json_file = fopen("/tmp/ollama_request.json", "w");
    if (json_file == NULL) {
        printf("Error: Failed to create temporary file.\n");
        return -1;
    }
    
    // Add system prompt to instruct AI to use plain text format without LaTeX
    fprintf(json_file, 
            "{\"model\": \"deepseek-r1:8b\", "
            "\"prompt\": \"You are a helpful engineering assistant. "
            "Please answer in plain text format suitable for terminal display. "
            "Do NOT use LaTeX or markdown math notation like \\\\[, \\\\], $, $$, \\\\(, \\\\). "
            "Use simple text like 'I = V/R' instead of LaTeX formulas. "
            "Keep formatting simple and terminal-friendly.\\n\\n%s\", "
            "\"stream\": false}",
            escaped_message);
    fclose(json_file);
    
    // Build the curl command using the temporary file
    snprintf(curl_command, sizeof(curl_command),
             "curl -s -X POST http://localhost:11434/api/generate "
             "-H 'Content-Type: application/json' "
             "-d @/tmp/ollama_request.json");
    
    printf("\n[AI is thinking...]\n");
    
    // Execute the curl command
    fp = popen(curl_command, "r");
    if (fp == NULL) {
        printf("Error: Failed to communicate with Ollama service.\n");
        printf("Please ensure Ollama is running (try: ollama serve)\n");
        remove("/tmp/ollama_request.json");
        return -1;
    }
    
    // Read the response
    size_t total_read = 0;
    size_t bytes_read;
    while ((bytes_read = fread(response + total_read, 1, 
            sizeof(response) - total_read - 1, fp)) > 0) {
        total_read += bytes_read;
    }
    response[total_read] = '\0';
    
    int status = pclose(fp);
    remove("/tmp/ollama_request.json");
    
    if (status != 0) {
        printf("Error: Ollama service returned an error.\n");
        return -1;
    }
    
    // Parse the JSON response to extract the "response" field
    // Simple parsing - look for "response":"..." pattern
    char *response_start = strstr(response, "\"response\":\"");
    if (response_start != NULL) {
        response_start += 12; // Skip past "response":"
        char *response_end = response_start;
        
        // Find the end of the response (accounting for escaped quotes)
        int escape_next = 0;
        while (*response_end != '\0') {
            if (escape_next) {
                escape_next = 0;
            } else if (*response_end == '\\') {
                escape_next = 1;
            } else if (*response_end == '"') {
                break;
            }
            response_end++;
        }
        
        // Print the response character by character, handling escape sequences and markdown
        printf("\n[DeepSeek R1 Response]:\n");
        printf("----------------------------------------\n");
        escape_next = 0;
        int in_latex = 0;  // Track if we're inside LaTeX markup
        int skip_chars = 0;  // Number of characters to skip
        int in_think_tag = 0; // Track if we're in <think> tags
        int in_bold = 0;  // Track markdown bold **
        
        for (char *p = response_start; p < response_end; p++) {
            // Skip characters if needed
            if (skip_chars > 0) {
                skip_chars--;
                continue;
            }
            
            // Check for <think> and </think> tags (skip these sections)
            if (*p == '<' && p + 6 < response_end && strncmp(p, "<think>", 7) == 0) {
                in_think_tag = 1;
                skip_chars = 6;  // Skip the rest of "<think>"
                continue;
            }
            if (*p == '<' && p + 7 < response_end && strncmp(p, "</think>", 8) == 0) {
                in_think_tag = 0;
                skip_chars = 7;  // Skip the rest of "</think>"
                printf("\n"); // Add newline after think section
                continue;
            }
            
            // Skip content inside <think> tags
            if (in_think_tag) {
                continue;
            }
            
            if (escape_next) {
                switch (*p) {
                    case 'n': printf("\n"); break;
                    case 'r': break; // Skip carriage return
                    case 't': printf("\t"); break;
                    case '"': printf("\""); break;
                    case '\\': printf("\\"); break;
                    case 'u': {
                        // Handle Unicode escape sequences like \u003c
                        if (p + 4 < response_end) {
                            char hex[5] = {p[1], p[2], p[3], p[4], '\0'};
                            int unicode_val = (int)strtol(hex, NULL, 16);
                            if (unicode_val < 128) {
                                printf("%c", (char)unicode_val);
                            }
                            p += 4;
                        }
                        break;
                    }
                    default: printf("%c", *p); break;
                }
                escape_next = 0;
            } else if (*p == '\\') {
                // Check for LaTeX delimiters \[ \] \( \)
                if (p + 1 < response_end) {
                    if (p[1] == '[' || p[1] == '(') {
                        in_latex = 1;
                        skip_chars = 1;
                        continue;
                    } else if ((p[1] == ']' || p[1] == ')') && in_latex) {
                        in_latex = 0;
                        skip_chars = 1;
                        printf(" ");  // Add space after math
                        continue;
                    }
                }
                escape_next = 1;
            } else if (*p == '$') {
                // Skip LaTeX $ or $$ delimiters
                if (p + 1 < response_end && p[1] == '$') {
                    in_latex = !in_latex;
                    skip_chars = 1;  // Skip the second $
                } else {
                    in_latex = !in_latex;
                }
                continue;
            } else if (in_latex) {
                // Inside LaTeX: keep only basic chars (numbers, operators, letters)
                if ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'z') || 
                    (*p >= 'A' && *p <= 'Z') || *p == ' ' || *p == '=' || 
                    *p == '+' || *p == '-' || *p == '*' || *p == '/' || 
                    *p == ',' || *p == '.') {
                    printf("%c", *p);
                }
            } else if (*p == '*' && p + 1 < response_end && p[1] == '*') {
                // Remove markdown bold **
                skip_chars = 1;  // Skip the second *
                in_bold = !in_bold;
                continue;
            } else {
                printf("%c", *p);
            }
        }
        printf("\n----------------------------------------\n");
    } else {
        printf("\nError: Could not parse AI response.\n");
        printf("Raw response: %s\n", response);
        return -1;
    }
    
    return 0;
}

/**
 * @brief Main menu item for AI chat functionality
 */
void menu_item_5(void) {
    char input[MAX_INPUT_LENGTH];
    
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║      Chat with DeepSeek R1 (Local AI Model)       ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    printf("\nAI-powered assistant for engineering questions and support.\n");
    printf("Model: deepseek-r1:8b (running locally via Ollama)\n");
    printf("\nFeatures:\n");
    printf("  ✓ Interactive AI chat interface\n");
    printf("  ✓ Engineering problem solving assistance\n");
    printf("  ✓ Circuit analysis help\n");
    printf("  ✓ Programming and debugging support\n");
    printf("\nInstructions:\n");
    printf("  - Type your question or message\n");
    printf("  - Type 'exit', 'quit', 'b', or 'B' to return to main menu\n");
    printf("  - Type 'help' for usage tips\n");
    printf("\n════════════════════════════════════════════════════\n");
    
    while (1) {
        printf("\nYou: ");
        
        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Remove trailing newline
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        // Check for empty input
        if (len == 0) {
            continue;
        }
        
        // Check for exit commands (including 'b' or 'B')
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0 ||
            strcmp(input, "b") == 0 || strcmp(input, "B") == 0) {
            printf("\nExiting AI chat. Returning to main menu...\n");
            break;
        }
        
        // Check for help command
        if (strcmp(input, "help") == 0) {
            printf("\n[Help - AI Chat Tips]\n");
            printf("----------------------------------------\n");
            printf("Example questions you can ask:\n");
            printf("  • Explain how RC circuits work\n");
            printf("  • What is Ohm's law?\n");
            printf("  • Help me debug my C code\n");
            printf("  • Calculate the time constant of an RC circuit\n");
            printf("  • Explain signal processing concepts\n");
            printf("----------------------------------------\n");
            continue;
        }
        
        // Send message to AI
        if (chat_with_ollama(input) != 0) {
            printf("\nNote: If the service is not running, start it with:\n");
            printf("      $ ollama serve\n");
            printf("\nOr check if the model is available with:\n");
            printf("      $ ollama list\n");
        }
    }
}
