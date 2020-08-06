#include <stdio.h>
#include <stdlib.h>
#include "msgQueque.h"

#define MAIN_QUEQUE_SIZE                        20
#define MAIN_MSG_POOL_SIZE                      512
static msgQueque_t main_msg;
static queque_data_info_t main_queque[MAIN_QUEQUE_SIZE];
static uint8_t main_msg_pool[20];

int main()
{
    printf("msgQueqeu test!\r\n");
    msgQueque_init(&main_msg, main_queque, MAIN_QUEQUE_SIZE, main_msg_pool, MAIN_MSG_POOL_SIZE);
    msgQueque_put(&main_msg, 1, "123", 4);
    msgQueque_put(&main_msg, 2, "abcd", 5);
    msgQueque_put(&main_msg, 3, NULL, 0);
    msgQueque_put(&main_msg, 4, "1234567890", 11);


    while (1)
    {
        queque_data_info_t get_que;
        uint8_t get_buf[128];

        if(msgQueque_get(&main_msg, &get_que, get_buf) == 0)
        {
            printf("--------------------\r\n");
            printf("get main msg\r\n");
            printf("id: %u\r\n", get_que.event);
            printf("len: %u\r\n", get_que.buf_size);
            printf("data: ");
            for(int i = 0; i < get_que.buf_size; i++)
                printf("%c", get_buf[i]);
            printf("\r\n");
        }
        else
        {
            return 1;
        }
        
    }
    
    return 0;
}

