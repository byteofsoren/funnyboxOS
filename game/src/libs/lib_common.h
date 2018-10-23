/*
 * lib_common.h
 *
 * Created: 2018-09-28 15:50:50
 *  Author: Magnus Sörensen
 */ 


#ifndef LIB_COMMON_H_
#define LIB_COMMON_H_


typedef unsigned int usec_t;
typedef void (*on_time_t)(usec_t);
typedef void (*onStatePtr)(int);


#endif /* LIB_COMMON_H_ */