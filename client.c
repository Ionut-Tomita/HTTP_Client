#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include <ctype.h>

#define HOST "34.246.184.49"
#define PORT 8080

int login_status = 0;
int library_access = 0;



int contains_spaces(char *str) {

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            return 1;
        }
    }

    return 0;
}
int contain_only_spaces(char *str) {
    int count = 0;

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            count++;
        }
    }

    if (count == strlen(str)) {
        return 1;
    }

    return 0;
}

int contains_digits(char *str) {

    for (int i = 0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            return 1;
        }
    }

    return 0;
}

// functie care trimite mesajul de eroare primit de la server
void error_message(char *response) {

    char *rez = calloc(LINELEN, sizeof(char));

    const char *start = strstr(response, "\"error\":");
    start += strlen("\"error\":\"");
    char *end = strstr(start, "\"}");
    strncpy(rez, start, end - start);
    rez[end - start] = '\0';
    printf("ERROR: %s\n", rez);
    free(rez);
    
}


void register_user(int sockfd, char *message, char *response) {

    if (login_status) {
        printf("ERROR: User already logged in.\n");
        return;
    }

    char *username = calloc(LINELEN, sizeof(char));
    
    printf("username=");
    fgets(username, LINELEN, stdin);
    username[strcspn(username, "\n")] = 0;

    char *password = calloc(LINELEN, sizeof(char));
    printf("password=");
    fgets(password, LINELEN, stdin);
    password[strcspn(password, "\n")] = 0;


    if (contains_spaces(username)) {
        printf("ERROR: Username can not contain spaces.\n");
        return;
    }

    if (strlen(username) == 0) {
        printf("ERROR: Username can not be empty.\n");
        return;
    }

    if (contains_spaces(password)) {
        printf("ERROR: Password can not contain spaces.\n");
        return;
    }

    if (strlen(password) == 0) {
        printf("ERROR: Password can not be empty.\n");
        return;
    }
    

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);
    char *serialized_string = json_serialize_to_string_pretty(root_value);

    message = compute_post_request(HOST, "/api/v1/tema/auth/register", "application/json", &serialized_string, 1, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);

    if (status_code == 201) {
        printf("SUCCES: User registered successfully.\n");

    } else if (status_code >= 400) {
        error_message(response);
    }


    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    free(message);
    free(response);
    free(username);
    free(password);
}


void login_user(int sockfd, char *message, char *response, char **cookies){

    if (login_status) {
        printf("ERROR: User already logged in.\n");
        return;
    }

    char *username = calloc(LINELEN, sizeof(char));
    char *password = calloc(LINELEN, sizeof(char));

    printf("username=");
    fgets(username, LINELEN, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("password=");
    fgets(password, LINELEN, stdin);
    password[strcspn(password, "\n")] = 0;

    if (contains_spaces(username)) {
        printf("ERROR: Username can not contain spaces.\n");
        return;
    }

    if (strlen(username) == 0) {
        printf("ERROR: Username can not be empty.\n");
        return;
    }

    if (contains_spaces(password)) {
        printf("ERROR: Password can not contain spaces.\n");
        return;
    }

    if (strlen(password) == 0) {
        printf("ERROR: Password can not be empty.\n");
        return;
    }

    // creeaza json cu datele userului
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);
    char *serialized_string = json_serialize_to_string_pretty(root_value);

    message = compute_post_request(HOST, "/api/v1/tema/auth/login", "application/json", &serialized_string, 1, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);    

    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);

    
    if (status_code == 200) {

        // extrage cookie-ul din raspuns
        const char *set_cookie_header = "Set-Cookie: ";
        char *start = strstr(response, set_cookie_header);
        start += strlen(set_cookie_header);
        char *end = strstr(start, "\r\n");

        if (end) {
            strncpy(cookies[0], start, end - start);
            cookies[0][end - start] = '\0';
        }

        login_status = 1;

        printf("SUCCES: User logged in successfully.\n");

    } else if (status_code >= 400) {
        error_message(response);
    }

    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    free(message);
    free(response);
    free(username);
    free(password);
}

void enter_library(int sockfd, char *message, char *response, char **cookies) {

    if (!login_status) {
        printf("ERROR: User not logged in.\n");
        return;
    }

    if (library_access) {
        printf("ERROR: You already have access to the library.\n");
        return;
    }

    message = compute_get_request(HOST, "/api/v1/tema/library/access", NULL, cookies[0], NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);


    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);
    
    if (status_code == 200) {

        // extrage tokenul din raspuns
        const char *start = strstr(response, "\"token\":");
        start += strlen("\"token\":\"");
        char *end = strstr(start, "\"}");
        strncpy(cookies[1], start, end - start);
        cookies[1][end - start] = '\0';

        library_access = 1;

        printf("SUCCES: You have access to the library.\n");
    } else {
        error_message(response);
    }

    free(message);
    free(response);
}

void get_books(int sockfd, char *message, char *response, char **cookies) {

    if (!login_status) {
        printf("ERROR: User not logged in.\n");
        return;
    }

    if (!library_access) {
        printf("ERROR: You do not have access to the library.\n");
        return;
    }

    message = compute_get_request(HOST, "/api/v1/tema/library/books", NULL, NULL, cookies[1]);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);

    
    if (status_code == 200) {
        printf("SUCCES: \n");

        // afiseaza cartile
        char *json_start = strstr(response, "\r\n\r\n");
        if (json_start) {
            json_start += 4;
        } else {
            json_start = response;
        }

        JSON_Value *root_value = json_parse_string(json_start);
        JSON_Array *books_array = json_value_get_array(root_value);
        int size = json_array_get_count(books_array);

        for (int i = 0; i < size; i++) {
            JSON_Object *book = json_array_get_object(books_array, i);
            JSON_Value *book_value = json_object_get_wrapping_value(book);
            char *book_string = json_serialize_to_string_pretty(book_value);
            printf("%s\n", book_string);

            json_free_serialized_string(book_string);
        }

        json_value_free(root_value);


    } else {
        error_message(response);
    }

    free(message);
    free(response);
}

void get_book(int sockfd, char *message, char *response, char **cookies) {

    if (!login_status) {
        printf("ERROR: User not logged in.\n");
        return;
    }

    if (!library_access) {
        printf("ERROR: You do not have access to the library.\n");
        return;
    }

    char *id = calloc(LINELEN, sizeof(char));
    printf("id=");
    fgets(id, LINELEN, stdin);
    id[strcspn(id, "\n")] = 0;

    if (strlen(id) == 0) {
        printf("ERROR: Id can not be empty.\n");
        return;
    }

    // Verifica dacă id-ul este un numar intreg
    char *endptr;
    strtol(id, &endptr, 10);
    if (*endptr != '\0') {
        printf("ERROR: Id must be an integer.\n");
        return;
    }

    // verifica daca contine spatii
    if (contains_spaces(id)) {
        printf("ERROR: Id can not contain spaces.\n");
        return;
    }


    char *url = calloc(LINELEN, sizeof(char));
    sprintf(url, "/api/v1/tema/library/books/%s", id);

    message = compute_get_request(HOST, url, NULL, NULL, cookies[1]);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);

    
    if (status_code == 200) {
        printf("SUCCES: \n");

        // afiseaza cartea
        char *json_start = strstr(response, "\r\n\r\n");
        if (json_start) {
            json_start += 4;
        } else {
            json_start = response;
        }

        JSON_Value *root_value = json_parse_string(json_start);
        JSON_Object *book_object = json_value_get_object(root_value);
        JSON_Value *book_value = json_object_get_wrapping_value(book_object);
        char *book = json_serialize_to_string_pretty(book_value);
        printf("%s\n", book);

        json_free_serialized_string(book);
        json_value_free(root_value);



    } else {
        error_message(response);
    }

    free(message);
    free(response);
    free(id);
    free(url);
}

void add_book(int sockfd, char *message, char *response, char **cookies) {

    // verificare daca userul este logat
    if (!login_status) {
        printf("ERROR: User not logged in.\n");
        return;
    }

    // verificare daca userul are acces la biblioteca
    if (!library_access) {
        printf("ERROR: You do not have access to the library.\n");
        return;
    }

    char *title = calloc(LINELEN, sizeof(char));
    char *author = calloc(LINELEN, sizeof(char));
    char *genre = calloc(LINELEN, sizeof(char));
    char *publisher = calloc(LINELEN, sizeof(char));
    char *page_count = calloc(LINELEN, sizeof(char));

    printf("title=");
    fgets(title, LINELEN, stdin);
    title[strcspn(title, "\n")] = 0;

    printf("author=");
    fgets(author, LINELEN, stdin);
    author[strcspn(author, "\n")] = 0;
    
    printf("genre=");
    fgets(genre, LINELEN, stdin);
    genre[strcspn(genre, "\n")] = 0;

    printf("publisher=");
    fgets(publisher, LINELEN, stdin);
    publisher[strcspn(publisher, "\n")] = 0;

    printf("page_count=");
    fgets(page_count, LINELEN, stdin);
    page_count[strcspn(page_count, "\n")] = 0;

    if (strlen(title) == 0 || contain_only_spaces(title)) {
        printf("ERROR: Title can not be empty.\n");
        return;
    }

    if (strlen(author) == 0 || contain_only_spaces(author)) {
        printf("ERROR: Author can not be empty.\n");
        return;
    }

    if (strlen(genre) == 0 || contain_only_spaces(genre)) {
        printf("ERROR: Genre can not be empty.\n");
        return;
    }

    if (strlen(publisher) == 0 || contain_only_spaces(publisher)) {
        printf("ERROR: Publisher can not be empty.\n");
        return;
    }

    if (strlen(page_count) == 0) {
        printf("ERROR: Page count can not be empty.\n");
        return;
    }

    char *endptr;
    strtol(page_count, &endptr, 10);
    if (*endptr != '\0') {
        printf("ERROR: Page count must be an integer.\n");
        return;
    }

    if (contains_spaces(page_count)) {
        printf("ERROR: Page count can not contain spaces.\n");
        return;
    }

    if (contains_digits(genre)) {
        printf("ERROR: Genre can not contain digits.\n");
        return;
    }

    // creeaza json cu datele cartii
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_string(root_object, "publisher", publisher);
    json_object_set_string(root_object, "page_count", page_count);
    char *serialized_string = json_serialize_to_string_pretty(root_value);

    // creeaza mesajul de post
    message = compute_post_request(HOST, "/api/v1/tema/library/books", "application/json", &serialized_string, 1, cookies[0], cookies[1]);

    // trimite mesajul la server
    send_to_server(sockfd, message);

    // primeste raspunsul de la server
    response = receive_from_server(sockfd);

    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);

    
    if (status_code == 200) {
        printf("SUCCES: Carte adaugata cu succes.\n");
    } else {
        error_message(response);
    }

    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    free(message);
    free(response);
    free(title);
    free(author);
    free(genre);
    free(publisher);
    free(page_count);
}

void delete_book(int sockfd, char *message, char *response, char **cookies) {

    if (!login_status) {
        printf("ERROR: User not logged in.\n");
        return;
    }

    if (!library_access) {
        printf("ERROR: You do not have access to the library.\n");
        return;
    }

    char *id = calloc(LINELEN, sizeof(char));
    printf("id=");
    fgets(id, LINELEN, stdin);
    id[strcspn(id, "\n")] = 0;

    // Verifica dacă id-ul este un numar intreg
    char *endptr;
    strtol(id, &endptr, 10);
    if (*endptr != '\0') {
        printf("ERROR: Id must be an integer.\n");
        return;
    }

    // Verifica daca id-ul este gol
    if (strlen(id) == 0) {
        printf("ERROR: Id can not be empty.\n");
        return;
    }

    // verifica daca contine spatii
    if (contains_spaces(id)) {
        printf("ERROR: Id can not contain spaces.\n");
        return;
    }


    char route[256];
    sprintf(route, "%s%s", "/api/v1/tema/library/books/", id);

    message = compute_delete_request(HOST, route, NULL, cookies[0], cookies[1]);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);

    if (status_code == 200) {
        printf("SUCCES \n");
    } else {
        error_message(response);
    }

    free(message);
    free(response);
    free(id);
}


void logout_user(int sockfd, char *message, char *response, char **cookies) {

    if (!login_status) {
        printf("ERROR: User not logged in.\n");
        return;
    }

    message = compute_get_request(HOST, "/api/v1/tema/auth/logout", NULL, cookies[0], cookies[1]);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    int status_code;
    sscanf(response, "HTTP/1.1 %d", &status_code);

    if (status_code == 200) {

        // reseteaza variabilele de login
        login_status = 0;
        library_access = 0;
        cookies[0] = calloc(LINELEN, sizeof(char));
        cookies[1] = calloc(LINELEN, sizeof(char));
        printf("SUCCES: User logged out successfully!\n");
        
    } else {
        error_message(response);
    }

    free(message);
    free(response);
}


int main(int argc, char *argv[])
{ 
    char *message = calloc(LINELEN, sizeof(char));
    char *response = calloc(LINELEN, sizeof(char));
    char command[LINELEN];
    char **cookies = calloc (3, sizeof(char*));

    for (int i = 0; i < 3; i++) {
        cookies[i] = calloc(LINELEN, sizeof(char));
    }
        
    while (1) {

        fgets(command, LINELEN, stdin);
        command[strcspn(command, "\n")] = 0;
    
        int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
        
        if (strcmp(command, "register") == 0) {
            register_user(sockfd, message, response);

        } else if (strcmp(command, "login") == 0) {
            login_user(sockfd, message, response, cookies);         

        } else if (strcmp(command, "enter_library") == 0) {
            enter_library(sockfd, message, response, cookies);

        } else if (strcmp(command, "get_books") == 0) {
            get_books(sockfd, message, response, cookies);

        } else if (strcmp(command, "get_book") == 0) {
            get_book(sockfd, message, response, cookies);

        } else if (strcmp(command, "add_book") == 0) {
            add_book(sockfd, message, response, cookies);

        } else if (strcmp(command, "delete_book") == 0) {
            delete_book(sockfd, message, response, cookies);

        } else if (strcmp(command, "logout") == 0) {
            logout_user(sockfd, message, response, cookies);

        } else if (strcmp(command, "exit") == 0) {
            close(sockfd);
            break;
        } else {
            printf("Comanda invalida\n");
        }

        close_connection(sockfd);


    }

    return 0;
}
