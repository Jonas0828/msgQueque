#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__
#include <stdint.h>
#include <stdbool.h> 
typedef bool _BOOL;			//8051 MCU 可以用bit
typedef uint16_t ring_t;	//8bit MCU 用uint8_t

typedef struct 
{
	uint8_t* buffer;
	ring_t write_index;
	ring_t read_index;
	ring_t buffer_size;
	_BOOL	_bMirror;
}ringbuffer_t;

//ring buffer初始化
int8_t ringbuffer_init(ringbuffer_t* rb, uint8_t* buf, ring_t size);
//ring buffer剩余空间
ring_t ringbuffer_free_space(ringbuffer_t* rb);
//ring buffer已用空间
ring_t ringbuffer_use_space(ringbuffer_t* rb);
//数据放入ring buffer
int8_t ringbuffer_put(ringbuffer_t* rb, uint8_t *pdata, ring_t len);
//从ring buffer中取出数据
int8_t ringbuffer_get(ringbuffer_t* rb, uint8_t *pdata, ring_t len);
//清除ring buffer数据
void ringbuffer_all_clear(ringbuffer_t* rb);
#endif
