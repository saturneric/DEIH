//
//  main.c
//  DEIH
//
//  Created by Eric on 15-10-24.
//  Copyright (c) 2015年 Bakantu Eric. All rights reserved.
//

/*includes*/
#include <stdio.h>
#include <stdlib.h>
#include <mongoc.h>
#include <unistd.h>
#include <bson.h>
#include <bcon.h>

/*functions*/
int add_words(void);
int search_words(void);
int delete_words(void);
int review (void);
void init (void);
int dishes(void);
void end (void);

/*verb*/
int review_words = 0;
mongoc_client_t      *m_client;
mongoc_database_t    *m_database;
mongoc_collection_t  *m_collection;

struct m_word{
    char name[32];
    char meaning[64];
    char paofsp[32];
};

/*detail functions*/
void init (void){
    mongoc_init ();
    m_client = mongoc_client_new ("mongodb://localhost:27017");
    m_database = mongoc_client_get_database (m_client, "DEIH");
    m_collection = mongoc_client_get_collection (m_client, "DEIH", "words");
}
int main(void) {
    init();
    dishes();
    end();
    return 0;
}
int dishes(void){
    system("clear");
    printf("Welcome to DEIH which is a tool to help you to remember new words\n");
    if (review_words > 0){
        printf("Well, you have %d words to review, and \n",review_words);
        printf("do you want to review these words first? ");
        char ifnot = '\0';
        ifnot = getchar();
        getchar();
        if (ifnot == 'y')review();
        else system("clear");
    }
    while(1){
        printf("1.Add words\n");
        printf("2.review words\n");
        printf("3.search words\n");
        printf("4.delete words\n");
        printf("5.exit\n");
        printf("You want to?");
        int ifnot = 0;
        scanf("%d",&ifnot);
        getchar();
        switch (ifnot){
            case 1:
                add_words();
                break;
            case 2:
                review();
                break;
            case 3:
                search_words();
                break;
            case 4:
                delete_words();
                break;
            case 5:
                return 0;
                break;
            default:
                break;
        }
        system("clear");
    }
    return 0;
}
int delete_words(void){
    mongoc_cursor_t *cursor;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc;
    char f_words[32];
    bson_t *query;
    
    printf("The name of the world that you wante to delete: ");
    scanf("%s",f_words);
    BSON_APPEND_UTF8 (query, "Name", f_words);
    cursor = mongoc_collection_find (m_collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
    
    doc = bson_new ();
    BSON_APPEND_OID (doc, "Name", &oid);
    if (!mongoc_collection_remove (m_collection, MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) {
        printf ("Delete failed: %s\n", error.message);
    }
    bson_destroy (query);
    mongoc_cursor_destroy (cursor);
    return 0;
}
int add_words(void){
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc;
    int count = 1;
    int n_ifnot = 1;
    char ifnot[4], m_ifnot[4];
    
    while (n_ifnot) {
        printf("`````````````````````````````````````````````````````````````\n");
        struct m_word* p_word = malloc(sizeof(struct m_word));
        printf("[No. %d]\n",count);
        printf("Name: ");
        scanf("%s",p_word->name);
        printf("Paofsp: ");
        scanf("%s",p_word->paofsp);
        printf("Meaning: ");
        scanf("%s",p_word->meaning);
        printf("```````````````````````````````````````````````````````````\n");
        printf("Sure?");
        scanf("%s",ifnot);
        if (strcmp(ifnot,"y") == 0){
            doc = bson_new ();
            bson_oid_init (&oid, NULL);
            BSON_APPEND_OID (doc, "_id", &oid);
            BSON_APPEND_UTF8 (doc, "Name",p_word->name);
            BSON_APPEND_UTF8 (doc, "Meaning",p_word->meaning);
            BSON_APPEND_UTF8 (doc, "Paofsp",p_word->paofsp);
            BSON_APPEND_UTF8 (doc, "time",0);
            if (!mongoc_collection_insert (m_collection, MONGOC_INSERT_NONE, doc, NULL, &error)) {
                fprintf (stderr, "%s\n", error.message);
            }
            else {
                printf("Save done！\n");
                count++;
            }
            bson_destroy (doc);
        }
        printf("Done?");
        scanf("%s",m_ifnot);
        if (strcmp(m_ifnot,"`") == 0){
            n_ifnot = 0;
        }
        printf("`````````````````````````````````````````````````````````````\n");
        system("clear");
    }
    return 0;
}
int search_words(void){
    return 0;
}
int review(void){
    return 0;
}
void end(void){
    
}

