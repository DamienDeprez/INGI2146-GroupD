/**
 * \file routing.c
 * \brief All function for the routing
 * \author Damien Deprez
 */

#include "routing.h"

/** \brief Print in the console the route
 *
 * \param r - route to print
 */
void print_route(struct route r){
    printf("route to %hu via %hu - metric : %hu\n",r.dst, r.via, r.metric);
}

/** \brief Create a routing table with an initial capacity
 *
 * \param initial_capacity - initial capacity of the routing table
 *
 * \return a pointer to the allocate routing table
 */
struct routing_table * create_routing_table(int initial_capacity){
    struct routing_table * routing_table = (struct routing_table*)(malloc(sizeof(struct routing_table)));
    routing_table->max_length = initial_capacity;
    routing_table->cursor = 0;
    routing_table->top = 0;
    routing_table->table=(struct route*)(malloc(sizeof(struct route)*initial_capacity));
    int i = 0;
    for(i=0;i<routing_table->max_length;i++){
        routing_table->table[i].dst=0;
        routing_table->table[i].via=0;
        routing_table->table[i].metric=0;
    }
    return routing_table;
}

/**\brief Destroy the routing table
 *
 * \param table - pointer to the routing table
 *
 */
void delete_routing_table(struct routing_table* table){
    free(table->table);
    free(table);
    table = NULL;
}

/** \brief Print in the console the routing table
 *
 * \param routing_table - pointer to the routing table
 *
 */
void print_routing_table(struct routing_table* table){
    int i = 0;
    printf("******** Table de routage ********\n");
    if(routing_table->top == 0 && routing_table->cursor == 0){
        printf("EMPTY\n");
        return;
    }
    for(i=0;i<=routing_table->top;i++){
        struct route r = routing_table->table[i];
        if(r.dst != 0 && r.via != 0){
            print_route(r);
        }
    }
}

/** \brief Add a route in the routing table
 *
 * \param table - pointer to the routing table
 * \param r - route to add
 *
 */
void add_route(struct routing_table* table, struct route r){
    int i = 0;
    //printf("******** Add route to %hu via %hu ********\n",r.dst, r.via);
    i = table->cursor;
    table->table[i].dst = r.dst;
    table->table[i].via = r.via;
    table->table[i].metric = r.metric;
    i++;
    // si le cursor n'est pas au bout
    if(table->cursor == 0 && table->top == 0){
        table->cursor = 1;
    }
    else if(table->cursor == table->top + 1){
        table->cursor++;
        table->top++;
    }
    else{
        for(;(i<table->table[i].dst) != 0 && (table->table[i].via) != 0 && i <= table->top;i++);
        table->cursor = i;
    }
    if (table->top + 1 == table->max_length){
        printf("-- Reach max size -> increase size of the table --\n");
        table->table = realloc(table->table, table->max_length*2*sizeof(struct route));
        table->max_length=table->max_length*2;
    }
}

/** \brief Remove an existing route in the routing table
 *
 * \param routing_table - pointer to the routing table
 * \param dest - destination of the route to remove
 * \return 0 if the route is successfully removed and 1 otherwise
 *
 */
int remove_route(struct routing_table* routing_table, unsigned short dest){
    int i = 0;
    int find=0;
    //printf("******** Remove route to %hu ********\n",dest);
    for(i=0;!find && i<=routing_table->top;i++){
        find = (routing_table->table[i].dst == dest);
    }
    i--;
    if(!find){
        return 1;
    }
    routing_table->table[i].dst=0;
    routing_table->table[i].via=0;
    routing_table->table[i].metric=0;
    // si le curseur est à la dernière position
    if (i == routing_table->top)
    {
        routing_table->cursor--;
        routing_table->top = routing_table->top == 0 ? 0 : routing_table->top-1;
    }
    else
    {
        if(routing_table->cursor > i)
        {
            routing_table->cursor = i;
        }
    }
    return 0;
}

/** \brief Find a route in the routing table
 *
 * \param  routing_table - pointer to the routing table
 * \param dest - destination of the route
 * \return the route if it exist otherwise, the route is all 0
 *
 */
struct route find_route(struct routing_table* routing_table, unsigned short dest){
    int i, find;
    find=0;
    for(i=0;!find && i<=routing_table->top;i++){
        find = (routing_table->table[i].dst == dest);
    }
    //print_route(routing_table->table[i-1]);
    struct route return_route;
    if(find){
        return_route.dst=routing_table->table[i-1].dst;
        return_route.via=routing_table->table[i-1].via;
        return_route.metric=routing_table->table[i-1].metric;
    }
    else{
        return_route.dst=0;
        return_route.via=0;
        return_route.metric=0;
    }
    return return_route;
}

/** \brief Change a route in the routing table
 *
 * \param routing_table - pointer to the routing table
 * \param r - route information to change
 * \param dest - destination of the route to change
 *
 */
void change_route(struct routing_table* routing_table, struct route r, unsigned short dest){
    int i = 0;
    int find = 0;
    for(i=0;!find && i<=routing_table->top;i++){
        find = (routing_table->table[i].dst == dest);
    }
    i--;
    if(find){
        routing_table->table[i].via = r.via;
        routing_table->table[i].metric = r.metric;
    }
}


/** \brief update the routing table using a message
 *
 * \param table - pointer to the routing table
 * \param msg - message to process
 *
 */
void update_message (struct routing_table* table, struct message msg){
    printf("-- update message \tflags - %hu\troute - %hu\tsource - %hu\thop - %hu\n",msg.flags,msg.route,msg.source,msg.hop);
    switch(msg.flags){
    // msg add a route
    case 1:
        ;
        struct route r;
        r = find_route(table,msg.route);
        if(r.dst == 0 && r.via == 0 && r.metric == 0){
            r.dst=msg.route;
            r.via=msg.source;
            r.metric=msg.hop;
            add_route(table,r);
        }
        else{
            if(r.metric>msg.hop){
                r.via=msg.source;
                r.metric=msg.hop;
                change_route(table,r,r.dst);
            }
        }
        break;
    // msg remove a route
    case 2:
        ;
        //r = find_route(table,msg.route);
        //if(r.dst != 0 && r.via != 0 && r.metric != 0){
            remove_route(table,msg.route);
        //}
        break;
    }
}
