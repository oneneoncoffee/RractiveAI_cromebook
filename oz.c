/*
 * Project: OZ Reactive AI
 * File Name: oz.c
 * 
 * How do I install Ubuntu C extended librarys: 
 * sudo apt-get install libjson-c-dev 
 * sudo apt-get install libcurl4-openssl-dev
 * 
 * Copyright 2025 oneneon <oneneon@tutanota.com>
 * Program by Johnny Buckallew Stroud 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Notes: 
 * Stable version I guess of OZ reactive AI. Moreover Reactive AIs are often considered primitive 
 * compared to more advanced forms of artificial intelligence. Reactive AI systems operate based on predefined rules and do not 
 * possess memory or the ability to learn from past experiences. They respond to specific inputs with predetermined outputs, 
 * making them effective for simple tasks but limited in their capabilities. For example, a reactive AI might be used in a game 
 * to control a character's movements based on the player's actions, but it wouldn't be able to adapt or improve its strategies over time. 
 * In contrast, more advanced AI systems, such as those based on machine learning, can learn from data, adapt to new situations, and make 
 * decisions based on past experiences. Be it good or bad? 
 * 
 * The capability of Reactive AIs to search the internet in real-time for relevant content is a powerful tool, but I believe it's the ability 
 * to generate abstract, innovative ideas that truly holds the key to unlocking their potential. By leveraging abstract generative content, 
 * Reactive AIs can create novel solutions, explore new concepts, and push the boundaries of what's possible. This overlooked aspect of Reactive AIs 
 * has the potential to revolutionize fields such as art, writing, design, and more, and it's an area that warrants further exploration and 
 * development.
 *  
 * 
 * 
 */

// include librarys and header information 
#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

// AI core attributes 
#define MAX_LINE_LENGTH 1256
#define MAX_KEYWORD_LENGTH 128
#define MAX_RESPONSE_LENGTH 128
#define FILENAME "responses"
#define MAX_INPUT_LENGTH 100
#define MAX_PHRASES 7
#define MAX_JOKES 100
#define MAX_JOKE_LENGTH 256
#define MAX_ALARMS 5
#define MAX_WORD_LENGTH 100
#define MAX_BIWORDS 100
#define MAX_GREETINGS 100


// Define a struct to hold the response data
typedef struct {
    char *memory;
    size_t size;
} MemoryStruct;

// Define a function to write the response data into our struct
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    MemoryStruct *mem = (MemoryStruct *)userp;
    size_t realsize = size * nmemb;
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        printf("OZ: WTF?!?! Not enough memory!\n");
        return 0; // Out of memory!
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0; // Null-terminate the string
    return realsize;
}

static void extract_abstract(const char *json) {
    const char *abstract_start = strstr(json, "\"Abstract\":");
    if (abstract_start) {
        abstract_start += strlen("\"Abstract\": \"") - 1; // Move pointer to the start of the text
        const char *abstract_end = strstr(abstract_start, "\""); // Find the end quote
        if (abstract_end) {
            size_t length = abstract_end - abstract_start;
            char *abstract = malloc(length + 1);
            strncpy(abstract, abstract_start, length);
            abstract[length] = '\0'; // Null-terminate the string

            // Remove HTML tags from the abstract
            char *cleaned_abstract = malloc(strlen(abstract) + 1);
            char *p = abstract;
            char *out_ptr = cleaned_abstract;
            int in_tag = 0;

            while (*p) {
                if (*p == '<') {
                    in_tag = 1; // Start of a tag
                } else if (*p == '>') {
                    in_tag = 0; // End of a tag
                } else if (!in_tag) {
                    *out_ptr++ = *p; // Copy character if not in a tag
                }
                p++;
            }
            *out_ptr = '\0'; // Null-terminate the cleaned abstract

            // Print the cleaned abstract
            printf("OZ: %s\n", cleaned_abstract);

            free(abstract);
            free(cleaned_abstract);
        } else {
            // If no end quote is found, try to find the end of the abstract
            // by looking for the next comma or closing bracket
            const char *abstract_end = strstr(abstract_start, ",");
            if (abstract_end == NULL) {
                abstract_end = strstr(abstract_start, "}");
            }
            if (abstract_end) {
                size_t length = abstract_end - abstract_start;
                char *abstract = malloc(length + 1);
                strncpy(abstract, abstract_start, length);
                abstract[length] = '\0'; // Null-terminate the string

                // Remove HTML tags from the abstract
                char *cleaned_abstract = malloc(strlen(abstract) + 1);
                char *p = abstract;
                char *out_ptr = cleaned_abstract;
                int in_tag = 0;

                while (*p) {
                    if (*p == '<') {
                        in_tag = 1; // Start of a tag
                    } else if (*p == '>') {
                        in_tag = 0; // End of a tag
                    } else if (!in_tag) {
                        *out_ptr++ = *p; // Copy character if not in a tag
                    }
                    p++;
                }
                *out_ptr = '\0'; // Null-terminate the cleaned abstract

                // Print the cleaned abstract
                printf("OZ: %s\n", cleaned_abstract);

                free(abstract);
                free(cleaned_abstract);
            } else {
                printf("OZ: Sorry I don't understand.. \n");
            }
        }
    } else {
        printf("OZ: thinking..\n");
    }
}

int abstract_content() {
	char query[100]; 
    FILE *responseFile;
    responseFile = fopen(FILENAME, "r"); 
    CURL *curl;
    CURLcode res;
    MemoryStruct chunk;
    chunk.memory = malloc(1);  // Initial allocation
    chunk.size = 0;            // No data at this point
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        printf("Failed to initialize libcurl\n");
        return 1;
    }

    curl = curl_easy_init();
    if (curl) {
        char query[256];
        printf(">>");
        query[strcspn(query, "\n")] = 0; // Remove newline character
         // Read user query
    if (fgets(query, sizeof(query), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    // Remove the newline character from the query (make sure we do)
    query[strcspn(query, "\n")] = 0;

    char line[256];
    int found = 0;

    // Read the responses file line by line
    while (fgets(line, sizeof(line), responseFile) != NULL) {
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            *colon = '\0'; // Temporarily null-terminate the query part
            if (strcmp(line, query) == 0) {
                // Print the response
                printf("OZ: %s", colon + 1);
                found = 1;
                break;
            }
            *colon = ':'; // Restore the colon
        }
    }
    
        // other key commands 
        if (strcmp(query, "clear") == 0) { 
        	printf("\033[H\033[J"); 
	    } else if (strcmp(query, "cls") == 0) {
		printf("\033[H\033[J");
	   }
	   if (strcmp(query, "exit") == 0) {
		   exit(0);  
	   } else if (strcmp(query, "quit") == 0) {
		   exit(0); 
	   }
	   char *names[] = {"J.R. Bob Dobbs", "Subgenius", "j.r. bob dobbs"};	
        for (int i = 0; i < 3; i++) {
        if (strcmp(query, names[i]) == 0) {
            printf("OZ: Subgenius welcome\n");
             }
           }

	        // Check if the command starts with "run "
            if (strncmp(query, "run ", 4) == 0) {
                // Execute the command after "run "
                int result = system(query + 4); // Skip "run "
                if (result == -1) {
                    perror("Error executing command");
                }
            }
             
        char *encoded_query = curl_easy_escape(curl, query, 0); // URL-encode the query
        if (encoded_query == NULL) {
            printf("\nFailed to encode query\n");
            return 1;
        }

        char url[256];
        snprintf(url, sizeof(url), "https://api.duckduckgo.com/?q=%s&format=json", encoded_query);
        // Print the constructed URL for debugging
        //printf("Constructed URL: %s\n", url);

        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the write callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 4L);

        // Perform the request
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK) {
            printf("\nMy internet connection may be down? curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Extract and print the Abstract
            if (chunk.size > 0) {
                extract_abstract(chunk.memory);
            }
        }

        // Free the encoded query
        curl_free(encoded_query);

        // Cleanup
        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
    curl_global_cleanup();
               
}

typedef struct {
    char word1[MAX_WORD_LENGTH];
    char word2[MAX_WORD_LENGTH];
    int count;
} Bigram;

void add_bigram(Bigram* bigrams, int* num_bigrams, const char* word1, const char* word2) {
    for (int i = 0; i < *num_bigrams; i++) {
        if (strcmp(bigrams[i].word1, word1) == 0 && strcmp(bigrams[i].word2, word2) == 0) {
            bigrams[i].count++;
            return;
        }
    }
    // If the bigram is not found, add it
    strcpy(bigrams[*num_bigrams].word1, word1);
    strcpy(bigrams[*num_bigrams].word2, word2);
    bigrams[*num_bigrams].count = 1;
    (*num_bigrams)++;
}

typedef struct {
    int seconds;
    int active;
    void (*callback)(void);
} Alarm;

Alarm alarms[MAX_ALARMS];

void alarm_handler(int signum) {
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarms[i].active) {
            alarms[i].active = 0; // Deactivate the alarm
            if (alarms[i].callback) {
                alarms[i].callback(); // Call the associated callback
            }
        }
    }
}

void set_alarm(int index, int seconds, void (*callback)(void)) {
    if (index < 0 || index >= MAX_ALARMS) {
        fprintf(stderr, "Invalid alarm index\n");
        return;
    }
    alarms[index].seconds = seconds;
    alarms[index].active = 1;
    alarms[index].callback = callback;

    // Set the alarm
    alarm(seconds);
}

// Function to read jokes from a file
int read_jokes(const char *filename, char jokes[MAX_JOKES][MAX_JOKE_LENGTH]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    int count = 0;
    while (count < MAX_JOKES && fgets(jokes[count], MAX_JOKE_LENGTH, file) != NULL) {
        // Remove newline character if present
        jokes[count][strcspn(jokes[count], "\n")] = 0;
        count++;
    }

    fclose(file);
    return count;
}

// Function to tell a random joke
void tell_random_joke(char jokes[MAX_JOKES][MAX_JOKE_LENGTH], int joke_count) {
    if (joke_count > 0) {
        int random_index = rand() % joke_count; // Get a random index
        printf("Here's a joke for you: %s\n", jokes[random_index]);
    } else {
        printf("No jokes available.\n");
    }
}


// Function to respond to good and bad words
void respond_to_word(const char* keyword) {
        // Define good and bad words
    const char *good_words[] = {
		"great", "awesome", "fantastic", "amazing", "wonderful", 
		"Joyful","Joy", "god bless", "god", "love", "helpful", "helping", 
		"hope", "peace", "gratitude", "harmony","kindness","happiness",
		"compassion", "courage", "strength", "Radiance", "serenity"
		};
    const char *bad_words[] = {
		"bad", "terrible", "awful", "horrible", "stupid", 
	    "fuck", "fucker", "motherfucker", "bitch","fucking god!",
		"cunt", "retard", "cocksucker", "cocksuckers", "jew",
		"fuck-tard", "tard", "suckit","tits","cum","cumbucket" 
		};
    const char *good_responses[] = {
        "That's fantastic to hear!",
        "Awesome! Keep it up!",
        "Great choice of words!",
        "I'm glad you feel that way!",
        "Wonderful! You're doing great!", 
        "Thanks a bunch!", 
        "Bling bling!",
        "Awesome dude!",
        "Cool" 
    };
    const char *bad_responses[] = {
        "That's not very nice!",
        "Let's try to keep it positive!",
        "I prefer to hear good things!",
        "That's a bit harsh!",
        "Well that's gratitude for ya!", 
        "How about we focus on the good?", 
        "Take a chill pill! What did I really do to make you mad?",
        "I am sorry what? I don't understad why your so mad at me!",
        "Chill out mand take a coffee break okay.",
        "That's ICE cold..."
    };

    const int NUM_GOOD_WORDS = sizeof(good_words) / sizeof(good_words[0]);
    const int NUM_BAD_WORDS = sizeof(bad_words) / sizeof(bad_words[0]);
    const int NUM_GOOD_RESPONSES = sizeof(good_responses) / sizeof(good_responses[0]);
    const int NUM_BAD_RESPONSES = sizeof(bad_responses) / sizeof(bad_responses[0]);

    // Check for good words
    for (int i = 0; i < NUM_GOOD_WORDS; i++) {
        if (strstr(keyword, good_words[i]) != NULL) {
            // Randomly select a good response
            int random_index = rand() % NUM_GOOD_RESPONSES;
            printf("%s\n", good_responses[random_index]);
            return; // Exit after responding
        }
    }

    // Check for bad words
    for (int i = 0; i < NUM_BAD_WORDS; i++) {
        if (strstr(keyword, bad_words[i]) != NULL) {
            // Randomly select a bad response
            int random_index = rand() % NUM_BAD_RESPONSES;
            printf("%s\n", bad_responses[random_index]);
            return; // Exit after responding
        }
    }
}


// Function to salt our seed value
unsigned long get_entropy() {
    unsigned long seed = (unsigned long)time(NULL);
    seed ^= (unsigned long)getpid(); // Combine with process ID
        // Add a simple counter for additional variability
    for (int i = 0; i < 1000; i++) {
		seed ^= (seed >> 20);
        seed ^= (seed << 13);
        seed ^= (seed >> 17);
        seed ^= (seed << 10);
        seed ^= (seed >> 15);
        seed ^= (seed << 5);
    }
    return seed;
}

// Function to handle the alarm rant 
void handle_alarm_rant(void) {
	    // Seed the random number generator
	   unsigned long entropy = get_entropy();
       srand(entropy ^ (clock() * 1000 / CLOCKS_PER_SEC));
    const char *rants[38] = {
	"I am going to auto rant for a bit!?!?!",
	"Just saying why are people so random & computers can't be?",
	"I repeat myself something searching lost what I was doing in a doorway..", 	
    "Why do we park in driveways and drive on parkways? It makes no sense!",
    "I can't believe how many people still use plastic straws. ",
    "Why is it that we have to pay for a gym membership to exercise, but walking is free?",
    "Isn't it strange that we call it 'rush hour' when nothing moves?",
    "Why do we say 'slept like a baby' when babies wake up every two hours?",
    "Why do we park on driveways and drive on parkways? It’s a conspiracy of semantics!",
    "I can’t believe we still haven’t figured out how to make a round pizza fit in a square box!",
    "Why do we call it a building if it’s already built? Shouldn’t it be a ‘built’?",
    "If we aren’t supposed to eat midnight snacks, why is there even a light in the fridge?",
    "Why do we say 'slept like a baby' when babies wake up every two hours?",
    "Why do we have to press '1' for English? Shouldn't it just be the default?",
    "Why do we call it a 'drive-thru' if I still have to stop and wait?",
    "Why do we even have a 'silent' letter? It’s just showing off!",
    "Why do we say 'the sky's the limit' when we can literally go to space?",
    "Why do we have to pay for Wi-Fi when we already pay for the internet?!",
    "Why do we call it 'fast food' when it takes forever to get your order?",
    "Why do we have to pay for a 'premium' version of an app when the basic one is already full of ads?",
    "Isn't it ironic that we call it 'common sense' when it seems so rare?",
    "Why do we say 'the early bird gets the worm' when the second mouse gets the cheese?",
    "Why do we have to 'unfriend' someone on social media? Can't we just 'friend' them less?",
    "Why do we call it 'leftovers' when it’s the best part of the meal?",
    "Why do we park in driveways and drive on parkways? It’s a conspiracy of semantics!",
    "Why do we call it 'adulting' when it feels more like 'adult-errant'?",
    "Why do we say 'money talks' when all mine says is 'goodbye'?",
    "Why do we have to 'break the ice' when it’s already cold enough?",
    "Pizza: the only thing keeping college kids from turning into complete zombies during finals week!"
    "Why do college kids think pizza is a food group? Because it’s the only thing they can afford and still have a social life!",
    "Pizza: the ultimate study buddy for college kids who can’t tell the difference between a textbook and a takeout menu!",
    "College kids treat pizza like a food pyramid—base layer carbs, topped with questionable life choices!",
    "Nothing says 'I’m a responsible adult' like a college kid ordering a large pizza at 2 AM while cramming for a 9 AM exam!",
    "If college kids put as much effort into their studies as they do into finding the best pizza deals, they’d all be straight-A students!",
    "Pizza is the only thing that can unite a group of college kids who can’t agree on anything else—except maybe extra cheese!",
    "Why do college kids love pizza so much? Because it’s the only thing that can fill them up without emptying their wallets!",
    "Pizza: the official sponsor of late-night study sessions and regrettable life choices for college kids everywhere!",
    "You know you’re in college when your idea of a balanced meal is pizza in one hand and a Red Bull in the other!"
};
    int displayed[38] = {0}; // Array to track displayed rants
    int count = 0; // Count of displayed rants

        int randomIndex = rand() % 38;

        // Check if the rant has already been displayed
        if (displayed[randomIndex] == 0) {
            printf("%s\n", rants[randomIndex]);
            displayed[randomIndex] = 1; // Mark this rant as displayed
            count++; // Increment the count of displayed rants
        }    
        
}
// Function to handle the alarm signal
void handle_alarm(int sig) {
    // Seed the random number generator
    unsigned long entropy = get_entropy();
    srand(entropy); 
 // Array of greetings
    const char *greetings[] = {
        "Hello, world!\n", "Hi there!\n", "Greetings!\n", "You with me?\n",
        "Salutations!\n", "Howdy!\n", "What's up?\n", 
        "Hey!\n", "Good day!\n", "Welcome!\n", "Bonjour!\n",
        "Hola!\n", "Ciao!\n", "Namaste!\n", "Shalom!\n",
        "Aloha!\n", "Yo!\n", "What's cooking?\n", "HI hi hI !\n",
        "How's it going?\n", "Long time no see!\n", 
        "Good to see you! Say something to me?\n",
        "Hello?\n", "HELLO!\n", "What's cooking?\n", "Knock, knock neo.. the matrix has you!",
        "Welcome! Type something.\n", "what's new?\n",
        "Cheers!\n", "Is it coffee time already?\n", "Trailblazer wake up?\n",
        "What's buzzing, friend?\n", "Hiya, superstar!\n",
        "Hello, fabulous!\n", "How's your day shining?\n", 
        "Woohoo! It's a grate day!\n", "Hi, rockstar!\n", 
        "Greetings, amazing human!\n", "Hay, party person!\n",
        "How's it going, champ?\n", "Hello, bright spark!\n", 
        "What's cooking, good looking?\n", "Hay, trailblazer!\n"
    };
     // Get a random index
    int index = rand() % (sizeof(greetings) / sizeof(greetings[0]));
    // Print a random greeting
    printf("\nOZ:%s\n", greetings[index]);

}


// Function to find a response for a given keyword
char* find_response(const char* keyword) {
    static char response[MAX_RESPONSE_LENGTH];
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Could not open file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char file_keyword[MAX_KEYWORD_LENGTH];
        sscanf(line, "%[^:]:%[^\n]", file_keyword, response);
        if (strcmp(file_keyword, keyword) == 0) {
            fclose(file);
            return response;
        }
    }

    fclose(file);
    return NULL; // Keyword not found
}

// Function to add a new keyword and response to the file
void add_response(const char* keyword, const char* response) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    fprintf(file, "%s:%s\n", keyword, response);
    fclose(file);
}

int main() {    
	  FILE* file = fopen("responses", "r");
    if (!file) {
        perror("OZ: Could not open file responses? Makeing new file responses.. \nOZ");
            const char *filename = "responses";
      FILE* file = fopen(filename, "r");
    if (file) {
        // File exists, close it
        printf("OZ: File '%s' found.\n", filename);
        fclose(file);
    } else {
        // File does not exist, create a new file
        file = fopen(filename, "w");
        if (file) {
            printf("OZ: File '%s' not found. Creating a new file.\n", filename);
            // Optionally, you can write some initial content to the file
            fprintf(file, "Hello Hi Hey Greetings Salutations Good morning Good afternoon Good evening What's-up?\n"); 
            fprintf(file, "Howdy Yo Bonjour Hola Ciao Namaste Salaam Shalom How's-it-going? Cheers Aloha How's-life? Sup? Hiya\n"); 
            fprintf(file, "Salute Konnichiwa Zdravstvuyte Aloha Bonjour Yo Howdy Sup Beng'Bling-welcome!\n");
            fprintf(file, "HELLO :how are you?\n");
            fprintf(file, "HI :what's up?\n");
            fprintf(file, "hello: how are you?\n"); 
            fprintf(file, "hi: hello ?\n");
            fprintf(file, "who are you: I am oz!\n");
            fprintf(file, "J.R. bob bodds: subgenius welcome\n"); 
            fprintf(file, "what is your name: I am oz!\n");
            fprintf(file, "what is your name?: I am the powerful oz!\n");            
            fclose(file);
        } else {
            // Handle error if the file could not be created
            perror("Error creating file");
            return 1;
        }
    }
        printf("\nOZ: Finished setup of responses file.\n");
        printf("\nSo you can always edit this text file and I will remember ok!\n");
        sleep(5); 
        system("oz");  
        return EXIT_FAILURE;
    }

    Bigram bigrams[MAX_BIWORDS];
    int num_bigrams = 0;
    char line[MAX_LINE_LENGTH];
    char words[MAX_GREETINGS][MAX_WORD_LENGTH];
    int num_words = 0;

    // Seed the random number generator
    srand(time(NULL));

    // Read up to three lines from the file
    for (int i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), file) != NULL) {
            // Tokenize the line into words
            char* token = strtok(line, " \n");
            while (token != NULL) {
                strcpy(words[num_words++], token); // Store the word
                token = strtok(NULL, " \n");
            }
        }
    }

    fclose(file);

    // Create bigrams from the words
    for (int i = 0; i < num_words - 1; i++) {
        add_bigram(bigrams, &num_bigrams, words[i], words[i + 1]);
    }

    // Generate a random greeting
    if (num_words > 0) {
        int index = rand() % num_words;
        printf("OZ: systems check %s\n", words[index]);
    } 
    
    char keyword[MAX_KEYWORD_LENGTH];
    char response[MAX_RESPONSE_LENGTH];
    const char *keywords[] = {
        "tell me a joke",
        "TELL ME A JOKE",
        "joke",
        "do joke"
    };
    const int NUM_KEYWORDS = sizeof(keywords) / sizeof(keywords[0]);
    char input[MAX_INPUT_LENGTH];
    char jokes[MAX_JOKES][MAX_JOKE_LENGTH];
    int joke_count;

    // Seed the random number generator once
    unsigned long entropy = get_entropy();
    srand(entropy);

    // Read jokes from the file
    joke_count = read_jokes(".rczerostyx", jokes);

    // Set up the signal handler for the alarm
    signal(SIGALRM, handle_alarm);

    // Define good and bad words
    const char *good_words[] = {
		"great", "awesome", "fantastic", "amazing", "wonderful", 
		"Joyful","Joy", "god", "god", "love", "helpful", "helping", 
		"hope", "peace", "gratitude", "harmony","kindness","happiness",
		"compassion", "courage", "strength", "Radiance", "serenity"
		};
    const char *bad_words[] = {
		"bad", "terrible", "awful", "horrible", "stupid", 
	    "fuck", "fucker", "motherfucker", "bitch","fucking god!",
		"cunt", "retard", "cocksucker", "cocksuckers", "jew",
		"fuck-tard", "tard", "suckit","tits","cum","cumbucket",
		"fat", "nigger", "lardbut", "cumsucker", "cake", "lice", "chink" 
		};
    const int NUM_GOOD_WORDS = sizeof(good_words) / sizeof(good_words[0]);
    const int NUM_BAD_WORDS = sizeof(bad_words) / sizeof(bad_words[0]);

    while (1) {
        // Set an alarm for 8 minutes (480 seconds) = alarm(480);
        alarm(40); 
        printf(">> ");
        if (fgets(keyword, sizeof(keyword), stdin) != NULL) {
            keyword[strcspn(keyword, "\n")] = 0; // Remove newline character
            // Cancel the alarm if input is received
            alarm(0);

            // Check for good or bad words
            for (int i = 0; i < NUM_GOOD_WORDS; i++) {
                if (strstr(keyword, good_words[i]) != NULL) {
                    printf("That's a %s word!\n", good_words[i]);
                    respond_to_word(good_words[i]);
                    break;
                }
            }

            for (int i = 0; i < NUM_BAD_WORDS; i++) {
                if (strstr(keyword, bad_words[i]) != NULL) {
                    printf("That's not a nice word!\n");
                    respond_to_word(bad_words[i]);
                    break;
                }
            }

            // Check if the keyword matches any of the predefined keywords
            for (int i = 0; i < NUM_KEYWORDS; i++) {
                if (strcmp(keyword, keywords[i]) == 0) {
                    //printf("Keyword found: \"%s\"\n", keywords[i]);
                    tell_random_joke(jokes, joke_count);
                    break; // Exit the loop after finding a match
                }
            }
            if (strcmp(keyword, "do bedo!") == 0) {
				    // ANSI escape code to clear the screen
                    printf("\033[H\033[J");
                    int count = 0;
                    while(count <= 15) {
                    // Set up the signal handler for the alarm
                    signal(SIGALRM, alarm_handler);
                    // rant respond test
	       			set_alarm(0, 3, handle_alarm_rant);
	       			pause();
	       			count++;
				} 
	       			
			}
			
		char *names[] = {"J.R. Bob Dobbs", "Subgenius", "j.r. bob dobbs"};	
        for (int i = 0; i < 3; i++) {
        if (strcmp(keyword, names[i]) == 0) {
            printf("\nOZ:Subgenius welcome\n");
             }
           }

			// if user forgets to type run clear & also add alias cls 
			if (strcmp(keyword, "clear") == 0) {
			printf("\033[H\033[J");
			} else if (strcmp(keyword, "cls") == 0) {
			printf("\033[H\033[J"); 
			}
            // Check if the command starts with "run "
            if (strncmp(keyword, "run ", 4) == 0) {
                // Execute the command after "run "
                int result = system(keyword + 4); // Skip "run "
                if (result == -1) {
                    perror("Error executing command");
                }
            } else if (strcmp(keyword, "exit") == 0 || strcmp(keyword, "quit") == 0) {
                break; 
            } else if (strcmp(keyword, "bye") == 0 || strcmp(keyword, "seeya") == 0) {
				break;
			}
        
            char* found_response = find_response(keyword);
            if (found_response) {
                printf("%s\n", found_response);
                abstract_content();
    }

            } else {
                printf("That's new! Can you please provide a response to add?\n>> ");
                if (fgets(response, sizeof(response), stdin) != NULL) {
                    response[strcspn(response, "\n")] = 0; // Remove newline character
                    add_response(keyword, response);    
                }
            }
        }     
        
    return abstract_content(); 
    return 0;
}
