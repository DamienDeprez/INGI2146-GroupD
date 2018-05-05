#include "routing.h"

int main()
{
    printf("route struct size : %d bytes - message size : %d bytes\n", sizeof(struct route), sizeof(struct message));
    printf("taille max de la table de routage : %f kB\n",(sizeof(struct route)*255+4)/1024.0);
    struct routing_table* table = NULL;
    struct message msg1,msg2,msg3,msg4;
    msg1.route=1;
    msg1.flags=1;
    msg1.source=2;
    msg1.hop=2;

    // update the route to 1
    msg2.route=1;
    msg2.flags=1;
    msg2.source=3;
    msg2.hop=1;

    // add the route to 4
    msg3.route=4;
    msg3.source=3;
    msg3.flags=1;
    msg3.hop=1;

    // remove the route to 4
    msg4.route=4;
    msg4.source=0;
    msg4.flags=2;
    msg4.hop=0;

    table = create_routing_table(5);

    update_message(table,msg1);
    update_message(table,msg2);
    //update_message(table,msg3);
    update_message(table,msg4);
    print_routing_table(table);



    /*add_route(table, r1);
    add_route(table, r2);
    add_route(table, r3);
    add_route(table, r4);*/
    delete_routing_table(table);
    return 0;
}
