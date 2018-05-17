#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

typedef struct Foo {
    int x;
    int y;
    struct list_head list;
} Foo;



struct list_head foo_list;


void foo_dump(struct list_head * list){
    struct list_head * pos;
    printf("--------dump--------\r\n");
    list_for_each(pos, list){
        /**
         * get pointer to object according to 
         * pointer to list_head in the object
         */
        const Foo * pfoo = list_entry(pos, Foo, list);
        printf("%d,%d\r\n", pfoo->x, pfoo->y);
    }
    printf("list is %s\r\n", list_empty(list) ? "EMPTY" : "NOT EMPTY");
    printf("--------------------\r\n");
}

struct list_head * make_another_list(int val){
    struct list_head * plist = malloc(sizeof(struct list_head));
    Foo * p;
    
    INIT_LIST_HEAD(plist);
    p = malloc(sizeof(Foo));
    p->x = p->y = val;
    list_add_tail(&p->list, plist);
    p = malloc(sizeof(Foo));
    p->x = p->y = val;
    list_add_tail(&p->list, plist);
    p = malloc(sizeof(Foo));
    p->x = p->y = val;
    list_add_tail(&p->list, plist);

    //printf("another list:\r\n");
    //foo_dump(plist);

    return plist;
}

int main(int argc, char **argv){
    /**
     * initialize the list
     */
    INIT_LIST_HEAD(&foo_list);
    for(;;){
        char line[1024];
        int x,y,val;
        Foo * ptr;
        struct list_head * pos;
        fgets(line, sizeof line, stdin);
        if( sscanf(line, "add %d,%d", &x, &y) == 2 ){
            ptr = malloc(sizeof(Foo));
            ptr->x = x;
            ptr->y = y;
            /**
             * add to tail 
             */
            list_add_tail(&ptr->list, &foo_list);
        }
        else if( sscanf(line, "adds %d,%d", &x, &y) == 2 ){
            ptr = malloc(sizeof(Foo));
            ptr->x = x;
            ptr->y = y;
            list_for_each(pos, &foo_list){
                Foo * pfoo = list_entry(pos, Foo, list);
                if(x < pfoo->x && y < pfoo->y){
                    /**
                     * add to specified node 
                     */
                    list_add(&ptr->list, pos->prev);
                    break;
                }
            }
        }
        else if( sscanf(line, "del %d,%d", &x, &y) ==2 ){
            list_for_each(pos, &foo_list){
                Foo * pfoo = list_entry(pos, Foo, list);
                if(pfoo->x == x && pfoo->y == y){
                    /**
                     * delete a node( &pfoo->list == pos here )
                     */
                    //list_del(&pfoo->list);
                    list_del(pos);
                    free(pfoo);
                }
            }
        }
        else if(sscanf(line, "splice %d", &val) == 1){
            /**
             * splice two lists
             */
            list_splice(make_another_list(val), &foo_list);
        }
        foo_dump(&foo_list);
    }
    return 0;
}

