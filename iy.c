#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

int main() {
    CURL *curl;
    CURLcode res;
    json_object *json;
    json_object *story;

    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        // Set the URL and request method
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.duck.ai/story");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "prompt=Tell%20me%20a%20story");

        // Send the request and get the response
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
        } else {
            // Get the response data
            char *response;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
            } else {
                // Parse the response
                json = json_tokener_parse(response);
                if(json) {
                    // Extract the story
                    story = json_object_object_get(json, "story");
                    if(story) {
                        printf("%s\n", json_object_get_string(story));
                    } else {
                        fprintf(stderr, "Error: Could not find story in response\n");
                    }
                    json_object_put(json);
                } else {
                    fprintf(stderr, "Error: Could not parse response\n");
                }
                free(response);
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
