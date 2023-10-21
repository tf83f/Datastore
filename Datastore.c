#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 100000 // Maximum size of hash table
#define MAX_SIZE 100000 // Maximum size of the array for distinct count
#define FILE_SIZE 2000 // Number of lines from the file to be read
#define MINIMUM_YEAR 2020 // Minimum year in the file

const char* filename = "your_path/data.tsv"; // Replace "file.tsv" with the actual path to your TSV file

// This function converts a timestamp string in the format "YYYY-MM-DDTHH:MM:SS" to the total number of seconds since January 1, 2020.
long convert_timestamp_to_int(char *timestamp_str) {
    // Initialize variables to store the extracted year, month, day, hour, minute, and second from the timestamp string.
    int year, month, day, hour, minute, second;
    
    // Use sscanf to parse the timestamp string and extract the year, month, day, hour, minute, and second values.
    sscanf(timestamp_str, "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    
    // Calculate the total number of seconds since January 1, MINIMUM_YEAR, based on the extracted values.
    // The formula considers the seconds, minutes, hours, days, months, and years elapsed from January 1, MINIMUM_YEAR.
    long total_seconds = ((year-MINIMUM_YEAR) * 31536000) + (month * 2592000) + (day * 86400) + (hour * 3600) + (minute * 60) + second;
    
    // Return the total number of seconds since January 1, MINIMUM_YEAR
    return total_seconds;
}

// This function converts an event string to its corresponding integer code.
int convert_event_to_int(char *event) {
    // Compare the event string with different predefined events and return the corresponding integer code.
    // If the event string matches any of the predefined events, the corresponding integer code is returned.
    // If no match is found, 0 is returned as the default code.

    if (strcmp(event, "MOUSE_UP") == 0) { return 1; }
    if (strcmp(event, "LEFT_SWIPE") == 0) { return 2; }
    if (strcmp(event, "BLUR") == 0) { return 3; }
    if (strcmp(event, "DOUBLE_TAP") == 0) { return 4; }
    if (strcmp(event, "HOVER") == 0) { return 5; }
    if (strcmp(event, "TAP") == 0) { return 6; }
    if (strcmp(event, "FOCUS") == 0) { return 7; }
    if (strcmp(event, "SCROLL") == 0) { return 8; }
    if (strcmp(event, "MOUSE_RIGHT") == 0) { return 9; }
    if (strcmp(event, "MOUSE_LEFT") == 0) { return 10; }
    if (strcmp(event, "RESIZE") == 0) { return 11; }
    if (strcmp(event, "RIGHT_SWIPE") == 0) { return 12; }
    if (strcmp(event, "MOUSE_DOWN") == 0) { return 13; }
    if (strcmp(event, "CLICK") == 0) { return 14; }

    // If the event string does not match any predefined events, return 0 as the default code.
    return 0;
}

// Definition of a singly linked list node.
struct node {
    long user_id;         // Data: Represents a user ID (assuming it's a long integer).
    long timestamp;       // Data: Represents a timestamp (assuming it's a long integer).
    int event;            // Data: Represents an event (assuming it's an integer).
    int hash_unique;      // Data: Represents a value for hash table unique purposes (assuming it's an integer).
    int hash_exist;       // Data: Represents a value for hash table existence purposes (assuming it's an integer).
    struct node *next;    // Pointer to the next node in the linked list.
};

// Definition of a node in the hash table.
struct hash_node {
    int hash_unique;            // Data: Represents a unique value for hash table purposes (assuming it's an integer).
    struct node* node_ptr;     // Pointer to a node in the linked list associated with this hash entry.
    struct hash_node *next;    // Pointer to the next node in the hash table bucket (linked list of hash nodes).
};

// Hash table represented as an array of hash_node pointers.
struct hash_node* hash_table[HASH_SIZE+1]; // An array of pointers to the head of linked lists (buckets) in the hash table.

// Static pointer to the sentinel node of the hash table (z_hash).
struct hash_node* z_hash; // Points to the sentinel node of the hash table (used for boundary checking or other purposes).

// Initialize the hash table and the sentinel node (z_hash).
void hashinitialize() {
    int i;
    
    // Allocate memory for the sentinel node (z_hash) of the hash table.
    z_hash = (struct hash_node*)malloc(sizeof *z_hash);
    
    // Initialize the sentinel node (z_hash) to point to itself, indicating the end of the linked list.
    z_hash->next = z_hash;
    
    // Set a unique hash value for the sentinel node (z_hash), greater than the size of the hash table (HASH_SIZE).
    z_hash->hash_unique = HASH_SIZE + 10;
    
    // Loop through each bucket in the hash table.
    for (i = 0; i < HASH_SIZE; i++) {
        // Allocate memory for the head node of each bucket (hash_node).
        hash_table[i] = (struct hash_node*)malloc(sizeof *hash_table[i]);
        
        // Initialize the head node of each bucket to point to the sentinel node (z_hash).
        hash_table[i]->next = z_hash;
    }
}

// Insert a hash node into the hash table at the given hash_value.
int hash_insert(int hash_value, struct hash_node* hash_node) {
    // Update the pointers to insert the new hash_node at the beginning of the linked list (bucket) in the hash table.
    hash_node->next = hash_table[hash_value]->next;
    hash_table[hash_value]->next = hash_node;
    return 0;
}

// Static pointers to the head and a sentinel node (z) of the linked list.
static struct node *head; // Points to the first node of the linked list (head node).
static struct node *z;    // Points to a sentinel node (used for boundary checking or other purposes).

// Free the memory allocated for the linked list with the given head node.
void free_list(struct node* head) {
    struct node* current = head;
    
    // Traverse the linked list and free each node until the sentinel node (z) is reached.
    while (current != z) {
        struct node* temp = current;
        current = current->next;
        free(temp); // Free the memory for the current node.
    }
}

// Free the memory allocated for the hash table and its nodes.
void free_hash_table() {
    for (int i = 0; i < HASH_SIZE; i++) {
        struct hash_node* current = hash_table[i]->next;
        
        // Traverse the linked list (bucket) in the hash table and free each hash_node until the sentinel node (z_hash) is reached.
        while (current != z_hash) {
            struct hash_node* temp = current;
            current = current->next;
            free(temp); // Free the memory for the current hash_node.
        }
        
        free(hash_table[i]); // Free the memory for the head of the bucket in the hash table.
    }
    
    free(z_hash); // Free the memory for the sentinel node (z_hash) of the hash table.
}

// Initialize the linked list and set up the sentinel node (z).
void listinitialize() {
    head = (struct node*)malloc(sizeof *head); // Allocate memory for the head node.
    z = (struct node*)malloc(sizeof *z);       // Allocate memory for the sentinel node (z).
    
    head->next = z; // Set the next pointer of the head node to the sentinel node (z).
    z->next = z;    // Set the next pointer of the sentinel node to itself, indicating the end of the linked list.
    z->timestamp = __LONG_MAX__; // Set the timestamp of the sentinel node to a large value (maximum long integer).
}

// Check if a record with the given timestamp, user_id, and event is unique in the hash table.
int is_unique(char* timestamp, int user_id, char* event) {
    // Convert the timestamp string to a long integer using the convert_timestamp_to_int function.
    long timestamp_long = convert_timestamp_to_int(timestamp);
    
    // Convert the event string to an integer code using the convert_event_to_int function.
    int event_int = convert_event_to_int(event);
    
    // Calculate the hash_exist and hash_unique values for the record based on user_id, event, and timestamp.
    long hash_exist = ((user_id % HASH_SIZE) + event_int) % HASH_SIZE;
    long hash_unique = (hash_exist + timestamp_long) % HASH_SIZE;
    
    // Traverse the linked list (bucket) in the hash table corresponding to the hash_exist value.
    struct hash_node* current = hash_table[hash_exist];
    while (current->next != z_hash) {
        current = current->next;
        
        // Check if the current node in the hash table matches the given record using hash_unique, event, timestamp, and user_id.
        if ((current->hash_unique == hash_unique) && (current->node_ptr->event == event_int) && (current->node_ptr->timestamp == timestamp_long) && (current->node_ptr->user_id == user_id))
            return 0; // Return 0 if the record already exists in the hash table (not unique).
    }
    return 1; // Return 1 if the record is unique and doesn't exist in the hash table.
}

// Insert a new record into the linked list and the hash table if it's unique.
int list_insert(char* timestamp, long user_id, char* event) {
    // Check if the record is unique using the is_unique function. If it's not unique, return 0.
    if (!is_unique(timestamp, user_id, event)) {
        return 1;
    }
    
    // Allocate memory for a new node (x) to be inserted into the linked list.
    struct node *x, *t = head;
    long timestamp_long = convert_timestamp_to_int(timestamp);
    z->timestamp = timestamp_long;
    
    // Find the appropriate position to insert the new node in the linked list based on the timestamp.
    while (timestamp_long > t->next->timestamp) {
        t = t->next;
    }
    
    // Allocate memory for the new node (x) and set up its data and pointers.
    x = (struct node *)malloc(sizeof *x);
    x->next = t->next;
    t->next = x;
    x->user_id = user_id;
    x->timestamp = timestamp_long;
    x->event = convert_event_to_int(event);
    x->hash_exist = ((x->user_id % HASH_SIZE) + x->event) % HASH_SIZE;
    x->hash_unique = (x->hash_exist + timestamp_long) % HASH_SIZE;
    
    // Allocate memory for a new hash_node (y) to be inserted into the hash table.
    struct hash_node* y;
    y = (struct hash_node*)malloc(sizeof *y);
    y->hash_unique = x->hash_unique;
    y->node_ptr = x;
    y->next = NULL;
    
    // Insert the new hash_node into the hash table at the corresponding bucket based on hash_exist value.
    hash_insert(x->hash_exist, y);
    
    return 0; // Return 0 to indicate successful insertion.
}

// Count the number of records in the linked list between the given dates (inclusive).
int count(char* date_from, char* date_to) {
    int count = 0;
    struct node *t = head;
    long date_from_long = convert_timestamp_to_int(date_from);
    long date_to_long = convert_timestamp_to_int(date_to);
    
    // Find the node in the linked list where the date_from falls or the last node if date_from is after the last node.
    while (date_from_long > t->next->timestamp) {
        if (t->next == z) break;
        t = t->next;
    }
    
    // Count the number of nodes in the linked list between date_from and date_to (inclusive).
    while (t->next->timestamp <= date_to_long) {
        if (t->next == z) break;
        t = t->next;
        count++;
    }
    
    return count; // Return the count of records between the given dates.
}

// Count the number of records with a specific event type between the given dates (inclusive).
int count_with_event(char* date_from, char* date_to, char* event) {
    int count = 0;
    int event_int = convert_event_to_int(event);
    struct node *t = head;
    long date_from_long = convert_timestamp_to_int(date_from);
    long date_to_long = convert_timestamp_to_int(date_to);
    
    // Find the node in the linked list where the date_from falls or the last node if date_from is after the last node.
    while (date_from_long > t->next->timestamp) {
        if (t->next == z) break;
        t = t->next;
    }
    
    // Count the number of nodes with the specified event type in the linked list between date_from and date_to (inclusive).
    while (t->next->timestamp <= date_to_long) {
        if (t->next == z) break;
        t = t->next;
        if (t->event == event_int) {
            count++;
        }
    }
    
    return count; // Return the count of records with the specified event type between the given dates.
}

// Count the number of records associated with a specific user_id between the given dates (inclusive).
int count_with_user_id(char* date_from, char* date_to, long user_id) {
    int count = 0;
    struct node *t = head;
    long date_from_long = convert_timestamp_to_int(date_from);
    long date_to_long = convert_timestamp_to_int(date_to);
    
    // Find the node in the linked list where the date_from falls or the last node if date_from is after the last node.
    while (date_from_long > t->next->timestamp) {
        if (t->next == z) break;
        t = t->next;
    }
    
    // Count the number of nodes associated with the specified user_id in the linked list between date_from and date_to (inclusive).
    while (t->next->timestamp <= date_to_long) {
        if (t->next == z) break;
        t = t->next;
        if (t->user_id == user_id) {
            count++;
        }
    }
    
    return count; // Return the count of records associated with the specified user_id between the given dates.
}

// Count the number of records with both a specific event type and a specific user_id between the given dates (inclusive).
int count_with_event_and_user_id(char* date_from, char* date_to, char* event, long user_id) {
    int count = 0;
    int event_int = convert_event_to_int(event);
    struct node *t = head;
    long date_from_long = convert_timestamp_to_int(date_from);
    long date_to_long = convert_timestamp_to_int(date_to);
    
    // Find the node in the linked list where the date_from falls or the last node if date_from is after the last node.
    while (date_from_long > t->next->timestamp) {
        if (t->next == z) break;
        t = t->next;
    }
    
    // Count the number of nodes with both the specified event type and user_id in the linked list between date_from and date_to (inclusive).
    while (t->next->timestamp <= date_to_long) {
        if (t->next == z) break;
        t = t->next;
        if ((t->event == event_int) && (t->user_id == user_id)) {
            count++;
        }
    }
    
    return count; // Return the count of records with both the specified event type and user_id between the given dates.
}

// Add a value to the end of the array if it's not full.
void add_to_array(long array[], size_t* size, long value) {
    if (*size >= MAX_SIZE) {
        printf("Array is full. Cannot add more elements.\n");
        return;
    }
    array[*size] = value;
    (*size)++;
}

// Check if a value is present in the array.
bool is_value_in_array(const long array[], size_t size, long value) {
    for (size_t i = 0; i < size; i++) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

// Count the number of distinct users who have performed a specific event between the given dates.
int count_distinct_users_with_event(char* date_from, char* date_to, char* event) {
    int distinct_users = 0;
    int event_int = convert_event_to_int(event);
    long users_array[MAX_SIZE];
    size_t size = 0;
    struct node *t = head;
    long date_from_long = convert_timestamp_to_int(date_from);
    long date_to_long = convert_timestamp_to_int(date_to);
    
    // Find the node in the linked list where the date_from falls or the last node if date_from is after the last node.
    while (date_from_long > t->next->timestamp) {
        if (t->next == z) break;
        t = t->next;
    }
    
    // Count the number of distinct users who performed the specified event in the linked list between date_from and date_to (inclusive).
    while (t->next->timestamp <= date_to_long) {
        if (t->next == z) break;
        t = t->next;
        
        // Check if the user_id is not present in the users_array and the event matches the specified event_int.
        if (!is_value_in_array(users_array, size, t->user_id) && (t->event == event_int)) {
            add_to_array(users_array, &size, t->user_id);
            distinct_users++;
        }
    }
    
    return distinct_users; // Return the count of distinct users who performed the specified event.
}

// Count the number of distinct users who have not performed any event between the given dates.
int count_distinct_users_without_event(char* date_from, char* date_to) {
    int distinct_users = 0;
    long users_array[MAX_SIZE];
    size_t size = 0;
    struct node *t = head;
    long date_from_long = convert_timestamp_to_int(date_from);
    long date_to_long = convert_timestamp_to_int(date_to);
    
    // Find the node in the linked list where the date_from falls or the last node if date_from is after the last node.
    while (date_from_long > t->next->timestamp) {
        if (t->next == z) break;
        t = t->next;
    }
    
    // Count the number of distinct users who have not performed any event in the linked list between date_from and date_to (inclusive).
    while (t->next->timestamp <= date_to_long) {
        if (t->next == z) break;
        t = t->next;
        
        // Check if the user_id is not present in the users_array.
        if (!is_value_in_array(users_array, size, t->user_id)) {
            add_to_array(users_array, &size, t->user_id);
            distinct_users++;
        }
    }
    
    return distinct_users; // Return the count of distinct users who have not performed any event.
}

// Check if an event exists for a given user_id in the hash table.
int exist(char* event, long user_id) {
    int event_int = convert_event_to_int(event);
    long hash_exist = ((user_id % HASH_SIZE) + event_int) % HASH_SIZE;
    
    struct hash_node* current = hash_table[hash_exist];
    while (current->next != z_hash) {
        current = current->next;
        
        // Check if the event and user_id match the specified values.
        if ((current->node_ptr->event == event_int) && (current->node_ptr->user_id == user_id))
            return 1; // Return 1 if the event exists for the user_id in the hash table.
    }
    
    return 0; // Return 0 if the event does not exist for the user_id in the hash table.
}

// Process data from the file and insert records into the linked list and hash table.
int process_data() {
    // Open the file with read mode
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filename);
        return 1; // Return 1 to indicate an error occurred
    }

    // Define the maximum length of a line in the file
    int MAX_LINE_LENGTH = 100;
    char buffer[MAX_LINE_LENGTH];

    // Initialize a line counter
    int line = 0;

    // Read data line by line until the end of the file or a specified limit (FILE_SIZE) is reached
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line++;

        // Check if the line limit (FILE_SIZE) is reached and break the loop if necessary
        if (line > FILE_SIZE && FILE_SIZE != 0)
            break;

        // Declare variables to store the fields read from each line
        long field1;
        char field2[MAX_LINE_LENGTH];
        char field3[MAX_LINE_LENGTH];

        // Parse the line and extract the three fields (field1, field2, and field3)
        if (sscanf(buffer, "%ld\t%s\t%s", &field1, field2, field3) == 3) {
            // Insert the extracted data into the linked list
            list_insert(field2, field1, field3);
        } else {
            // Print an error message if the parsing fails
            printf("Error parsing line: %s", buffer);
        }
    }

    // Close the file after reading all the data
    fclose(file);

    // Return 0 to indicate successful data processing
    return 0;
}

// Define the run function which takes no arguments and returns an integer value
int run() {
    // Start an infinite loop (while true)
    while (1) {
        // Initialize variables to store user input and data
        char request[1000] = "";
        char endpoint[50] = "";
        char date_from[30] = "";
        char date_to[30] = "";
        char event[30] = "";
        long user_id = 0;

        // Prompt the user for input
        printf("(To exit enter : /q?) -> Enter the request: ");
        
        // Read the user input from the standard input and store it in the 'request' variable
        // Break the loop on EOF (end-of-file) or error
        if (fgets(request, sizeof(request), stdin) == NULL) {
            break;
        }
        
        // Extract the 'endpoint' from the 'request' using sscanf
        sscanf(request, "/%[^?]?", endpoint);

        // Check if the user wants to exit the program
        if (strcmp(endpoint, "q") == 0) {
            printf("Exit the program");
            return 0; // Return 0 to exit the function and terminate the program
        }
        // If the user requests the 'count' endpoint
        else if (strcmp(endpoint, "count") == 0) {

            int result;
            // Parse the request to extract 'endpoint', 'date_from', 'date_to', 'event', and 'user_id'
            sscanf(request, "/%[^?]?date_from=%[^&]&date_to=%[^&]&event=%[^&]&user_id=%ld", endpoint, date_from, date_to, event, &user_id);

            // Check different cases based on the presence of 'event' and 'user_id'
            if (strlen(event) > 2 && (user_id > 0)) {
                printf("Running count from : %s to : %s and event : %s and user_id : %ld\n", date_from, date_to, event, user_id);
                // Call the appropriate count function based on the provided data
                result = count_with_event_and_user_id(date_from, date_to, event, user_id);
            } else if (strlen(event) > 2 && !user_id) {
                printf("Running count from : %s to : %s and event : %s\n", date_from, date_to, event);
                result = count_with_event(date_from, date_to, event);
            } else if (!(strlen(event) > 2) && (user_id > 0)) {
                printf("Running count from : %s to : %s and user_id : %ld\n", date_from, date_to, user_id);
                result = count_with_user_id(date_from, date_to, user_id);
            } else {
                printf("Running count from : %s to : %s\n", date_from, date_to);
                result = count(date_from, date_to);
            }
            // Print the result of the count operation
            printf("Count result: %d\n", result);
            printf("\n");
        }
        // If the user requests the 'count_distinct_users' endpoint
        else if (strcmp(endpoint, "count_distinct_users") == 0) {
            int result;
            // Parse the request to extract 'endpoint', 'date_from', 'date_to', and 'event'
            sscanf(request, "/%[^?]?date_from=%[^&]&date_to=%[^&]&event=%[^&]&", endpoint, date_from, date_to, event);

            // Check if 'event' is provided to determine which count function to call
            if (strlen(event) > 2) {
                printf("Running count distinct from : %s to : %s and event : %s\n", date_from, date_to, event);
                result = count_distinct_users_with_event(date_from, date_to, event);
            } else {
                printf("Running count distinct from : %s to : %s\n", date_from, date_to);
                result = count_distinct_users_without_event(date_from, date_to);
            }
            // Print the result of the count_distinct_users operation
            printf("Distinct users count: %d\n", result);
            printf("\n");
        }
        // If the user requests the 'exists' endpoint
        else if (strcmp(endpoint, "exists") == 0) {
            // Parse the request to extract 'endpoint', 'event', and 'user_id'
            sscanf(request, "/%[^?]?event=%[^&]&user_id=%ld\n", endpoint, event, &user_id);
            printf("Running exist with event : %s and user_id : %ld\n", event, user_id);
            // Check if the event with the specified user_id exists
            int result = exist(event, user_id);
            // Print the result of the exist operation ('true' or 'false')
            printf("Exists: %s\n", result ? "true" : "false");
            printf("\n");
        }
        // If the user enters an invalid endpoint
        else {
            printf("Invalid endpoint.\n");
            printf("\n");
        }
    }

    return 0; // Return 0 to indicate successful completion of the function
}

int main()
{
    // Initialize the list data structure
    listinitialize();

    // Initialize the hash table data structure
    hashinitialize();

    // Process the data (perform some operations on the data structures)
    process_data();

    // Run the main functionality of the program using user input
    run();

    // Free the memory used by the hash table data structure
    free_hash_table();

    // Free the memory used by the linked list data structure
    free_list(head); // 'head' is likely a pointer to the first node of the linked list

    // Indicate successful program execution and return 0 to the operating system
    return 0;
}
