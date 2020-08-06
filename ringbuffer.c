#include <string.h>
#include "ringbuffer.h"

#define DEBUG_LOG	0
#if 	DEBUG_LOG	
#include <stdio.h>
#endif

/**@brief     ring buffer初始化
 *
 * @details   对ring buffer的结构体初始化，传入buffer的地址，以及buffer的大小
 *
 * @param[in] rb:    ring buffer结构体
 * @param[in] buf:   buffer
 * @param[in] size:  buffer的大小
 *
 * @return    -1：参数错误；
 *			   0：正常
 */
int8_t ringbuffer_init(ringbuffer_t* rb, uint8_t* buf, uint16_t size)
{
	if(rb == NULL || buf == NULL || size == 0)
	{
		return -1;
	}
	memset(rb, 0, sizeof(ringbuffer_t));
	rb->buffer = buf;
	rb->buffer_size = size;
	// rb->write_index = 16;
	// rb->read_index = 16;
	memset(buf, 0, sizeof(uint8_t)*size);
	return 0;
}

/**@brief     ring buffer的空闲容量
 *
 *
 * @param[in] rb: ring buffer结构体
 *
 * @return    空闲空间大小 byte
 *
 */
ring_t ringbuffer_free_space(ringbuffer_t* rb)
{
	if(rb->_bMirror == 0)
	{
		return rb->buffer_size - (rb->write_index - rb->read_index);
	}
	else
	{
		return rb->read_index - rb->write_index;
	}
}

/**@brief     ring buffer的已用容量
 *
 *
 * @param[in] rb: ring buffer结构体
 *
 * @return    已用空间大小 byte
 *
 */
ring_t ringbuffer_use_space(ringbuffer_t* rb)
{
	if(rb->_bMirror == 0)
	{
		return rb->write_index - rb->read_index;
	}
	else
	{
		return rb->buffer_size + rb->write_index - rb->read_index;
	}
}

/**@brief     数据放入ring buffer
 *
 *
 * @param[in] rb:    ring buffer结构体
 * @param[in] pdata: 数据地址
 * @param[in] len:   数据的长度 (byte)
 *
 * @return    -1：失败
 *			   0：成功
 */
#if 1
int8_t ringbuffer_put(ringbuffer_t *rb, uint8_t *pdata, ring_t len)
{
	ring_t i;
	ring_t free_capacity;
	free_capacity = ringbuffer_free_space(rb);
	#if 	DEBUG_LOG
	printf("free:%u\n", free_capacity);
	#endif
	if(free_capacity >= len)
	{
		//数据较少时,使用这种方法
		for(i = 0; i < len; i++)
		{
			rb->buffer[rb->write_index] = pdata[i];
			if(rb->write_index == rb->buffer_size - 1)
			{
				rb->_bMirror = !rb->_bMirror;
				rb->write_index = 0;
			}
			else
				rb->write_index ++;
		}
		#if 	DEBUG_LOG
		printf("write_index:%u\n", rb->write_index);
		#endif
		return 0;
	}
	else
	{
		return -1;
	}
}
#else
int8_t ringbuffer_put(ringbuffer_t* rb, uint8_t *pdata, ring_t len)
{
	ring_t i;
	ring_t free_capacity;
	free_capacity = ringbuffer_free_space(rb);
	#if 	DEBUG_LOG
	printf("free:%u\n", free_capacity);
	#endif
	if(free_capacity >= len)
	{
		//数据>10000时用
		ring_t pos_size = rb->buffer_size - rb->write_index;
		if(pos_size < len)
		{
			ring_t neg_size = len - pos_size;
			memcpy(&rb->buffer[rb->write_index], pdata, pos_size);
			memcpy(&rb->buffer[0], &pdata[pos_size], neg_size);
			rb->_bMirror = !rb->_bMirror;
			rb->write_index = neg_size;
		}
		else if(pos_size == len)
		{
			memcpy(&rb->buffer[rb->write_index], pdata, len);
			rb->_bMirror = !rb->_bMirror;
			rb->write_index = 0;
		}
		else
		{
			memcpy(&rb->buffer[rb->write_index], pdata, len);
			rb->write_index = rb->write_index + len;
		}
		#if 	DEBUG_LOG
		printf("write_index:%u\n", rb->write_index);
		#endif
		return 0;
	}
	else
	{
		return -1;
	}
}
#endif

/**@brief     从ring buffer中取出数据
 *
 *
 * @param[in] rb:    ring buffer结构体
 * @param[in] pdata: 存放数据的地址
 * @param[in] len:   数据的长度 (byte)
 *
 * @return    -1：失败
 *			   0：成功
 */
#if 1
int8_t ringbuffer_get(ringbuffer_t* rb, uint8_t *pdata, ring_t len)
{
	ring_t i;
	ring_t use_capacity;
	use_capacity = ringbuffer_use_space(rb);
	#if 	DEBUG_LOG
	printf("use:%u\n", use_capacity);
	#endif
	if(use_capacity >= len)
	{
		//数据较少时,使用这种方法
		for(i = 0; i < len; i++)
		{
			pdata[i] = rb->buffer[rb->read_index];

			if(rb->read_index == rb->buffer_size - 1)
			{
				rb->_bMirror = !rb->_bMirror;
				rb->read_index = 0;
			}
			else
				rb->read_index ++;
		}
		#if 	DEBUG_LOG
		printf("read_index:%u\n", rb->read_index);
		#endif
		return 0;
	}
	else
		return -1;
}
#else
int8_t ringbuffer_get(ringbuffer_t* rb, uint8_t *pdata, ring_t len)
{
	ring_t i;
	ring_t use_capacity;
	use_capacity = ringbuffer_use_space(rb);
	#if 	DEBUG_LOG
	printf("use:%u\n", use_capacity);
	#endif
	if(use_capacity >= len)
	{
		//数据>10000时用
		ring_t pos_size = rb->buffer_size - rb->read_index;
		if(pos_size < len)
		{
			ring_t neg_size = len - pos_size;
			memcpy(pdata, &rb->buffer[rb->read_index], pos_size);
			memcpy(&pdata[pos_size], &rb->buffer[0], neg_size);
			rb->_bMirror = !rb->_bMirror;
			rb->read_index = neg_size;
		}
		else if(pos_size == len)
		{
			memcpy(pdata, &rb->buffer[rb->read_index], len);
			rb->_bMirror = !rb->_bMirror;
			rb->read_index = 0;
		}
		else
		{
			memcpy(pdata, &rb->buffer[rb->read_index], len);
			rb->read_index = rb->read_index + len;
		}
		#if 	DEBUG_LOG
		printf("read_index:%u\n", rb->read_index);
		#endif
		return 0;
	}
	else
		return -1;
}
#endif

/**@brief     清除ring buffer
 *
 *
 * @param[in] rb:    ring buffer结构体
 *
 */
void ringbuffer_all_clear(ringbuffer_t* rb)
{
	memset(rb->buffer, 0, sizeof(uint8_t)*rb->buffer_size);
	rb->write_index = 0;
	rb->read_index = 0;
	rb->_bMirror = 0;
}
