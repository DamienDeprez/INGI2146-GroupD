#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef ROUTING_H
#define ROUTING_H

#define ADD 1
#define UPDATE 2
#define REMOVE 3

/** \brief route entry in the routing table
 */
struct route {
    unsigned short dst; /**< destination of the route */
    unsigned short via; /**< via to reach the destination */
    unsigned short metric; /**< metric to compare route to the same destination */
};


/** \brief routing table
 * \struct routing_table
 */
struct routing_table{
    struct route* table; /**< pointer to the array of route */
    short top; /**< top of the routing_table */
    short cursor; /**< current cursor to add a new route */
    short max_length; /**< max length of the routing table */
};

/** \brief message send and receive for routing
 */
struct message {
    uint8_t flags; /**< type of the message : add a route, remove a route, ... */
    unsigned short source; /**< source of the message */
    unsigned short route; /**< destination of the route */
    unsigned short hop; /**< number of hop to reach the route */
};

extern void print_route(struct route r);

extern void print_routing_table(struct routing_table* routing_table);

extern struct routing_table * create_routing_table(int initial_capacity);

extern void delete_routing_table(struct routing_table* table);

void add_route(struct routing_table* table, struct route r);

void change_route(struct routing_table* table, struct route r, unsigned short dest);

int remove_route(struct routing_table* routing_table, unsigned short dest);

extern struct route find_route(struct routing_table* table, unsigned short dest);

extern int update_message (struct routing_table* table, struct message msg);
#endif // ROUTING_H
