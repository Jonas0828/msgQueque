/***************************************************************************
*
* Copyright (c) 2020, Xinkerr
*
* This file is part of msgQueque.
*
* msgQueque is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* msgQueque is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with msgQueque.  If not, see <https://www.gnu.org/licenses/>.
*	
* LICENSE: LGPL V2.1
* see: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
*
* Date:    2020/8/6
* Author:  鄭訫
* Version: 2.0
* Github:  https://github.com/Xinkerr/msgQueque
* Mail:    634326056@qq.com
*
* Disclaimer:
* AUTHOR MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* AUTHOR SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
****************************************************************************/

#ifndef __MSGQUEQUE_H__
#define __MSGQUEQUE_H__
#include <stdint.h>
#include <stdbool.h>
#include "ringbuffer.h"

typedef uint16_t msgbuf_szie_t;

typedef enum
{
	msgQUEQUE_EMPTY = 0,			
	msgQUEQUE_FULL,
	msgQUEQUE_NORMAL,
	msgQUEQUE_ERROR
}queque_state_m;

//列队信息
typedef struct
{
	uint16_t event;						//消息事件（指消息的来源或是标志）
	msgbuf_szie_t buf_size;				//数据大小
}queque_data_info_t;

//消息队列
typedef struct 
{
	queque_data_info_t*	pdata; 				//消息体成员的指针

	uint16_t write_index;							
	uint16_t read_index;
	bool _bMirror;
	uint16_t queque_size;

	ringbuffer_t ringbuf_handle;
}msgQueque_t;


//消息队列初始化
int msgQueque_init(msgQueque_t* msg, queque_data_info_t* queque, uint16_t queque_size, uint8_t* msg_pool, uint16_t pool_size);
//消息队列的状态
queque_state_m msgQueque_status(msgQueque_t* msg);
//加入消息
int msgQueque_put(msgQueque_t* msg, uint16_t event, uint8_t *pdata, uint16_t size);
//取出消息
int msgQueque_get(msgQueque_t* msg, queque_data_info_t* queque_info, uint8_t* msg_content);
#endif
